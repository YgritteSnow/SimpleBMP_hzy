#include "raytrace_scene.h"
#include "raytrace_config.h"

const float min_reflect_factor = 0.001f;
const float temp_reflect_decay = 0.2f;

namespace RayTrace
{
	bool Scene::CalPixel_byRay(const ICollideRay& ray, D3DXCOLOR* out_color, int recurDepth, float reflectFactor ) const
	{
		if( recurDepth == 0 )
			*out_color = D3DXCOLOR(0.0f, 0.0f, 0.0f, 0.0f);

		if( reflectFactor < min_reflect_factor )
			return false;

		if( IsMaxRecurDepth( recurDepth ) )
			return false;

		bool is_collide = false;
		float min_z = c_max_dist;
		D3DXVECTOR3 min_collideNormal;
		D3DXVECTOR3 min_collidePoint;
		ICollideModel* min_collideModel = NULL;
		for( auto it_model = m_vec_model.begin(); it_model != m_vec_model.end(); ++it_model)
		{
			
			D3DXVECTOR3 collidePoint, collideNormal;
			float collideDist;
			if( (*it_model)->IsCollide(ray, &collidePoint, &collideDist, &collideNormal) )
			{
				if( min_z > collideDist )
				{
					is_collide = true;
					min_z = collideDist;
					min_collidePoint = collidePoint;
					min_collideNormal = collideNormal;
					min_collideModel = *it_model;
				}
			}
		}

		if( is_collide )
		{
			// 计算当前点被光照的影响
			*out_color += CalLights( min_collidePoint, min_z, min_collideNormal, ray.GetNormal(), min_collideModel ) * reflectFactor;

			// 计算当前点的反射的影响
			CCollideRay out_reflectRay;
			CalReflectRay( min_collidePoint, min_collideNormal, ray.GetNormal(), &out_reflectRay );
			CalPixel_byRay( out_reflectRay, out_color, recurDepth+1, temp_reflect_decay * reflectFactor );

			// 计算当前点的折射的影响
			CCollideRay out_refractRay;
			CalRefractRay( min_collidePoint, min_collideNormal, ray.GetNormal(), &out_refractRay );
			CalPixel_byRay( out_refractRay, out_color, recurDepth+1, temp_reflect_decay * reflectFactor );

			return true;
		}
		else
		{
			return false;
		}
	}

	void Scene::CalReflectRay( const D3DXVECTOR3& collidePoint, const D3DXVECTOR3& collideNormal, const D3DXVECTOR3& collideRay, ICollideRay* out_reflectRay ) const
	{
		float t = D3DXVec3Dot( &collideNormal, &collideRay );
		out_reflectRay->Set( collidePoint, collideRay + 2*t* collideNormal );
	}

	void Scene::CalRefractRay( const D3DXVECTOR3& collidePoint, const D3DXVECTOR3& collideNormal, const D3DXVECTOR3& collideRay, ICollideRay* out_reflectRay ) const
	{
		out_reflectRay->Set( collidePoint, collideNormal );
	}

	CCollideRay Scene::CalCameraRay( int screen_x, int screen_y ) const 
	{
		// todo 
		D3DXVECTOR3 screenpoint(screen_x/100.f, screen_y/100.f, 0.f);
		return CCollideRay( D3DXVECTOR3(0, 0, -5), screenpoint );
	}

	void Scene::GenerateRayTrace()
	{
		for( int screen_x = 0; screen_x < c_screen_width; ++screen_x )
			for( int screen_y = 0; screen_y < c_screen_height; ++screen_y )
			{
				D3DXCOLOR out_color;
				if( CalPixel_byRay( CalCameraRay( screen_x, screen_y ), &out_color, 0, 1.f ) )
				{
					SetImgBit(screen_x, screen_y, out_color);
				}
				else
				{
					SetImgBit(screen_x, screen_y, D3DXCOLOR(0.0f, 0.0f, 0.0f, 0.0f));
				}
			}
	}

	D3DXCOLOR Scene::CalLights(const D3DXVECTOR3& pos, const float dist, const D3DXVECTOR3& normal, const D3DXVECTOR3& cameraDirect, const ICollideModel* curModel) const 
	{
		D3DXCOLOR res_color = D3DXCOLOR(0.0f, 0.0f, 0.0f, 0.0f);
		
		// 遍历每一个光源
		for( auto it_light = m_vec_light.begin(); it_light != m_vec_light.end(); ++it_light )
		{
			// 先检查是否和物体表面法线反向
			if( D3DXVec3Dot(&normal, &((*it_light)->GetPosition() - pos)) <= 0 )
				continue;

			// 再遍历每一个物体，检查是否被相交，如果相交则无视这条射线
			CCollideRay shadowRay(pos, (*it_light)->GetPosition());

			bool is_collide = false;
			for( auto it_model = m_vec_model.begin(); it_model != m_vec_model.end(); ++it_model )
			{
				if( *it_model == curModel )
					continue;

				if( (*it_model)->IsCollide(shadowRay, NULL, NULL, NULL) )
				{
					is_collide = true;
					break;
				}
			}

			if( !is_collide )
			{
				D3DXCOLOR origin_color;
				D3DXColorModulate(&origin_color, &(*it_light)->GetColor(), &curModel->GetColor(pos));
				// diffuse light
				float diffuse_factor = D3DXVec3Dot(&normal, &shadowRay.GetNormal());
				res_color += origin_color * diffuse_factor;

				// specular light
				D3DXVECTOR3 half_angle = -cameraDirect + shadowRay.GetNormal();
				D3DXVec3Normalize(&half_angle, &half_angle);
				float specular_factor = D3DXVec3Dot(&normal, &half_angle);
				res_color += origin_color * (float)pow((double)specular_factor, 2);
			}
		}

		return res_color;
	}
}