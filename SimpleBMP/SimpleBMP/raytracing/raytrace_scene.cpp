#include "raytrace_scene.h"
#include "raytrace_config.h"

namespace RayTrace
{
	bool Scene::CalPixel_byRay(const ICollideRay& ray, D3DXCOLOR* out_color, int recurDepth) const
	{
		if( IsMaxRecurDepth( recurDepth ) )
			return false;

		bool is_collide = false;
		float min_z = c_max_dist;
		D3DXVECTOR3 min_collideNormal;
		D3DXVECTOR3 min_collidePoint;
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
				}
			}
		}

		if( is_collide )
		{
			// 计算当前点被光照的影响
			out_color += CalLights( &min_collidePoint );

			// 计算当前点的反射的影响
			CCollideRay out_reflectRay;
			CalReflectRay( min_collidePoint, min_collideNormal, ray.GetNormal(), &out_reflectRay );
			CalPixel_byRay( out_reflectRay, out_color, recurDepth+1 );

			// 计算当前点的折射的影响
			CCollideRay out_refractRay;
			CalRefractRay( min_collidePoint, min_collideNormal, ray.GetNormal(), &out_refractRay );
			CalPixel_byRay( out_refractRay, out_color, recurDepth+1 );

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
				if( CalPixel_byRay( CalCameraRay( screen_x, screen_y ), &out_color, 0 ) )
				{
					SetImgBit(screen_x, screen_y, out_color);
				}
				else
				{
					SetImgBit(screen_x, screen_y, D3DXCOLOR());
				}
			}
	}

	D3DXCOLOR Scene::CalLights(D3DXVECTOR3* pos) const 
	{
		bool is_collide = false;
		for( auto it_model = m_vec_model.begin(); it_model != m_vec_model.end(); ++it_model )
		{
			CCollideRay shadowRay(*pos, (*it_model)->GetCenter());

			D3DXVECTOR3 collidePoint, collideNormal;
			float collideDist;
			if( (*it_model)->IsCollide(shadowRay, &collidePoint, &collideDist, &collideNormal) )
			{
				is_collide = true;
				break;
			}
		}

		if( !is_collide )
		{
			return D3DXCOLOR(1.0, 1.0, 0.0, 1.0);
		}
		else
		{
			return D3DXCOLOR(0.0, 1.0, 1.0, 1.0);
		}
	}
}