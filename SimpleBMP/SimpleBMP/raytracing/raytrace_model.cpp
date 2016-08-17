
#include "raytrace_model.h"

const float min_error = 0.0001f;

namespace RayTrace
{
	/************************************************************************/
	/* 光线                                                                     */
	/************************************************************************/
	
	void CCollideRay::Set( const D3DXVECTOR3& start, const D3DXVECTOR3& normal ){
	m_start = start;
	m_normal = normal;
	D3DXVec3Dot(&m_normal, &m_normal);
	D3DXVec3Normalize(&m_normal, &m_normal);
	}

	/************************************************************************/
	/* 模型                                                                     */
	/************************************************************************/
	bool CCollildeBall::IsCollide(const ICollideRay& line, D3DXVECTOR3* out_collidePoint, float* out_collideDist, D3DXVECTOR3* out_collideNormal) const
	{
		float proj_length, proj_half;
		if( CalLineCross(line, NULL, &proj_length, &proj_half) )
		{
			float result_length = 0;
			if( proj_length - proj_half >= 0 )
			{
				result_length = proj_length - proj_half;
			}
			else if( proj_length + proj_half >= 0 )
			{
				result_length = proj_length + proj_half;
			}
			else
			{
				return false;
			}

			if( result_length < min_error )
				return false;

			if( out_collidePoint )
				*out_collidePoint = line.GetStartPoint() + line.GetNormal() * result_length;
			if( out_collideDist )
				*out_collideDist = result_length;
			if( out_collideNormal )
			{
				D3DXVECTOR3 normal = line.GetStartPoint() + line.GetNormal() * result_length - GetCenter();
				D3DXVec3Normalize(&normal, &normal);
				*out_collideNormal = normal;/*( line.GetStartPoint() + line.GetNormal() * result_length - GetCenter() ) / m_radius*/;
			}
			return true;
		}
		else
		{
			return false;
		}
	}
	
	bool CCollildeBall::CalLineCross(const ICollideRay& line, D3DXVECTOR3* out_projPoint, float* out_projLengthProj, float* out_projLengthHalf) const
	{
		float proj_factor = D3DXVec3Dot( &(m_center - line.GetStartPoint()), &line.GetNormal() );
		float o_to_line = D3DXVec3LengthSq(&(m_center - line.GetStartPoint())) - proj_factor * proj_factor;
		if( o_to_line < 0 )
			return false;

		if( out_projPoint )
			*out_projPoint = line.GetStartPoint() + line.GetNormal() * proj_factor;
		if( out_projLengthProj )
			*out_projLengthProj = proj_factor;
		if( out_projLengthHalf )
			*out_projLengthHalf = sqrt( m_radius * m_radius - o_to_line );

		return true;
	}

	bool CCollideRectangle::IsCollide(const ICollideRay& line, D3DXVECTOR3* out_collidePoint, float* out_collideDist, D3DXVECTOR3* out_collideNormal) const
	{
		D3DXVECTOR3 collide_point;
		if( CalLineCross(line, &collide_point) )
		{
			float res_length = 0;
			float collide_width = D3DXVec3Dot( &(collide_point - m_center), &GetWidthNormal() );
			float collide_height = D3DXVec3Dot( &(collide_point - m_center), &GetWidthNormal() );
			if( abs(collide_width) >= m_width && abs(collide_height) >= m_height )
			{
				return false;
			}

			if( D3DXVec3LengthSq(&(collide_point - line.GetStartPoint())) < min_error )
				return false;
			
			if( out_collidePoint )
				*out_collidePoint = collide_point;
			if( out_collideDist )
				*out_collideDist = D3DXVec3Length(&(collide_point - line.GetStartPoint()));
			if( out_collideNormal )
				*out_collideNormal = GetNormal();

			return true;
		}
		else
		{
			return false;
		}
	}

	bool CCollideRectangle::CalLineCross(const ICollideRay& line, D3DXVECTOR3* out_collidePoint) const
	{
		float proj_factor = D3DXVec3Dot( &(m_center - line.GetStartPoint()), &line.GetNormal() );
		if( out_collidePoint )
			*out_collidePoint = line.GetStartPoint() + line.GetNormal() * proj_factor;

		return true;
	}

	/************************************************************************/
	/* 光源                                                                     */
	/************************************************************************/
}