#ifndef __RAYTRACING_RAYTRACE_SCENE_H__
#define __RAYTRACING_RAYTRACE_SCENE_H__

#include <vector>

#include <d3dx9math.h>
#include <d3dx9.h>
#pragma comment(lib, "d3dx9.lib")

#include "raytrace_model.h"
#include "raytrace_config.h"

namespace RayTrace
{

	class Scene
	{
	public:
		Scene(){m_imgData = new D3DXCOLOR[c_screen_width * c_screen_height];}
		~Scene(){delete [] m_imgData;}

	public:
		void GenerateRayTrace();
		void AddLight(ICollideLight* light){m_vec_light.push_back(light);}
		void AddModel(ICollideModel* model){m_vec_model.push_back(model);}
		const D3DXCOLOR* GetDataPtr(){return m_imgData;}
		const int GetWidth(){return c_screen_width;}
		const int GetHeight(){return c_screen_height;}

	private:
		void SetImgBit(int x, int y, const D3DXCOLOR& color){*(m_imgData + x*c_screen_height + y) = color;}
		bool CalPixel_byRay( const ICollideRay& ray, D3DXCOLOR* out_color, int recurDepth, float reflectFactor ) const ;
		D3DXCOLOR CalLights( const D3DXVECTOR3& pos, const float dist, const D3DXVECTOR3& normal, const D3DXVECTOR3& cameraDirect, const ICollideModel* curModel ) const ;
		
		void CalReflectRay( const D3DXVECTOR3& collidePoint, const D3DXVECTOR3& collideNormal, const D3DXVECTOR3& collideRay, ICollideRay* out_reflectRay) const ;
		void CalRefractRay( const D3DXVECTOR3& collidePoint, const D3DXVECTOR3& collideNormal, const D3DXVECTOR3& collideRay, ICollideRay* out_reflectRay) const ;

		CCollideRay CalCameraRay( int screen_x, int screen_y ) const ;

	private:
		D3DXCOLOR* m_imgData/*[c_screen_width][c_screen_height]*/;

		std::vector<ICollideLight*> m_vec_light;
		std::vector<ICollideModel*> m_vec_model;
	};

}


#endif 