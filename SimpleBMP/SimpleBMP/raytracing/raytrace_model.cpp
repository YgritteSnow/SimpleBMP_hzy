
#include "raytrace_model.h"

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
			float near_length = proj_length - proj_half;
			if( near_length >= 0 )
			{
				if( out_collidePoint) 
					*out_collidePoint = line.GetStartPoint() + line.GetNormal() * near_length;
				return true;
			}

			float far_length = proj_length + proj_half;
			if( far_length >= 0 )
			{
				if( out_collidePoint ) 
					*out_collidePoint = line.GetStartPoint() + line.GetNormal() * far_length;
				return true;
			}

			return false;
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
		if( out_projLengthHalf )
			*out_projLengthHalf = sqrt( o_to_line );

		return true;
	}

	bool CCollideRectangle::IsCollide(const ICollideRay& line, D3DXVECTOR3* out_collidePoint, float* out_collideDist, D3DXVECTOR3* out_collideNormal) const
	{
		D3DXVECTOR3 collide_point;
		if( CalLineCross(line, &collide_point) )
		{
			float collide_width = D3DXVec3Dot( &(collide_point - m_center), &GetWidthNormal() );
			if( abs(collide_width) <= m_width )
			{
				if( out_collidePoint )
					*out_collidePoint = collide_point;
				return true;
			}
			
			float collide_height = D3DXVec3Dot( &(collide_point - m_center), &GetWidthNormal() );
			if( abs(collide_height) <= m_height )
			{
				if( out_collidePoint )
					*out_collidePoint = collide_point;
				return true;
			}

			return false;
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