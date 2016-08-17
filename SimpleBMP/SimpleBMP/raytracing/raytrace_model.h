#ifndef __RAYTRACING_RAYTRACE_MODEL_H__
#define __RAYTRACING_RAYTRACE_MODEL_H__

#include <d3dx9.h>
#include <d3dx9math.h>
#pragma comment(lib, "d3dx9.lib")

namespace RayTrace
{
	/************************************************************************/
	/* 光线                                                                     */
	/************************************************************************/
	class ICollideRay
	{
	public:
		virtual D3DXVECTOR3 GetNormal() const = 0;
		virtual D3DXVECTOR3 GetStartPoint() const = 0;
		virtual void Set( const D3DXVECTOR3& start, const D3DXVECTOR3& normal ) = 0;
	};

	class CCollideRay : public ICollideRay
	{
	public:
		CCollideRay(){}
		CCollideRay(const D3DXVECTOR3& start, const D3DXVECTOR3& end):m_start(start), m_normal(end - start){
			D3DXVec3Normalize(&m_normal, &m_normal);}

		virtual D3DXVECTOR3 GetNormal() const {
			return m_normal;};
		virtual D3DXVECTOR3 GetStartPoint() const {
			return m_start;};

		virtual void Set( const D3DXVECTOR3& start, const D3DXVECTOR3& normal );
			/*m_start = start;
			m_normal = normal;
			D3DXVec3Normalize(&m_normal, &m_normal);
		}*/
		
	private:
		D3DXVECTOR3 m_start;
		D3DXVECTOR3 m_normal;
	};

	/************************************************************************/
	/* 模型                                                                     */
	/************************************************************************/
	class ICollideModel
	{
	public:
		/* 是否碰撞
		__in line: 射线
		__out out_collidePoint: 碰撞位置
		__out out_collideDist: 到碰撞点的距离
		__out out_collideNormal: 碰撞点的法线
		*/
		virtual bool IsCollide(const ICollideRay& line, D3DXVECTOR3* out_collidePoint, float* out_collideDist, D3DXVECTOR3* out_collideNormal) const = 0;
		virtual D3DXVECTOR3 GetCenter() const = 0;
		virtual D3DXCOLOR GetColor(const D3DXVECTOR3& pos) const = 0;
	};

	class CCollildeBall : public ICollideModel
	{
	public:
		CCollildeBall( const D3DXVECTOR3& center, const D3DXCOLOR& color, float radius ):m_center(center), m_color(color), m_radius(radius){}

	public:
		virtual bool IsCollide(const ICollideRay& line, D3DXVECTOR3* out_collidePoint, float* out_collideDist, D3DXVECTOR3* out_collideNormal) const;
		virtual D3DXVECTOR3 GetCenter() const {
			return m_center;};
		virtual D3DXCOLOR GetColor(const D3DXVECTOR3& pos) const {
			return m_color;}

	protected:
		bool CalLineCross(const ICollideRay& line, D3DXVECTOR3* out_collidePoint, float* out_collideLengthProj, float* out_collideLengthHalf) const;

	private:
		D3DXVECTOR3 m_center;
		D3DXCOLOR m_color;
		float m_radius;
	};

	class CCollideRectangle : public ICollideModel
	{
	public:
		virtual bool IsCollide(const ICollideRay& line, D3DXVECTOR3* out_collidePoint, float* out_collideDist, D3DXVECTOR3* out_collideNormal) const;
		virtual D3DXVECTOR3 GetCenter() const {
			return m_center;};
		virtual D3DXCOLOR GetColor(const D3DXVECTOR3& pos) const {
			return m_color;}

	protected:
		virtual const D3DXVECTOR3& GetNormal() const;
		virtual D3DXVECTOR3 GetWidthNormal() const;
		virtual D3DXVECTOR3 GetHeightNormal() const;

		bool CalLineCross(const ICollideRay& line, D3DXVECTOR3* out_collidePoint) const;

	private:
		D3DXVECTOR3 m_normal;
		D3DXVECTOR3 m_widthnormal;
		D3DXVECTOR3 m_center;
		float m_width;
		float m_height;
		D3DXCOLOR m_color;
	};

	/************************************************************************/
	/* 光源                                                                     */
	/************************************************************************/
	class ICollideLight
	{
	public:
		virtual const D3DXVECTOR3& GetPosition() const = 0;
		virtual const D3DXCOLOR& GetColor() const = 0;
	};

	class CCollideLightPoint : public ICollideLight
	{
	public:
		CCollideLightPoint(const D3DXVECTOR3& center, const D3DXCOLOR& color, float factorA):m_position(center), m_color(color), m_factorA(factorA){}

	public:
		virtual const D3DXVECTOR3& GetPosition() const {
			return m_position;}
		virtual const D3DXCOLOR& GetColor() const {
			return m_color;}
	private:
		D3DXVECTOR3 m_position;
		D3DXCOLOR m_color;
		float m_factorA;
	};

}

#endif 