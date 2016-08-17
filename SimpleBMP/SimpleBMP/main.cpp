#include <stdio.h>
#include <memory.h>

#include "raytracing/raytrace_scene.h"
#include "raytracing/raytrace_model.h"

#include <d3dx9math.h>
#include <d3dx9.h>
#pragma comment(lib, "d3dx9.lib")

#include <iostream>
#include <cmath>
#include <cstdlib>
#define DM1 (DIM-1)
#define _sq(x) ((x)*(x)) // square
#define _cb(x) abs((x)*(x)*(x)) // absolute value of cube
#define _cr(x) (unsigned char)(pow((x),1.0/3.0)) // cube root

FILE *fp;
int main(){
	RayTrace::Scene scene;
	RayTrace::ICollideLight* light1 = new RayTrace::CCollideLightPoint(D3DXVECTOR3(0, 9, 0), D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f), 30);
	scene.AddLight( light1 );
	RayTrace::ICollideModel* model1 = new RayTrace::CCollildeBall( D3DXVECTOR3(0, 0, 0), D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f), 2 );
	scene.AddModel( model1 );
	scene.GenerateRayTrace();
	const D3DXCOLOR* imgData = scene.GetDataPtr();

	//构件图像数据
	int width = scene.GetWidth();
	int height = scene.GetHeight();

	//int width = 298;
	//int height = 199;
	//D3DXCOLOR* imgData = new D3DXCOLOR[width * height];
	//for( int w = 0; w < width; ++w )
	//	for( int h = 0; h < height; ++h )
	//	{
	//		if( w + h <= 150 )
	//		{
	//			*(imgData + w * height + h) = D3DXCOLOR(1.0f, 0.0f, 0.0f, 0.0f);
	//		}
	//		else
	//		{
	//			*(imgData + w * height + h) = D3DXCOLOR(0.0f, 0.0f, 0.0f, 0.0f);
	//		}
	//	}

	fopen_s(&fp, "MathPic.ppm","wb");
	fprintf(fp, "P6\n%d %d\n255\n", height, width);
	for(int w=0;w<width;w++)
		for(int h=0;h<height;h++)
		{
			static unsigned char color[3];
			color[0] = int( (imgData + w * height + h)->r * 255) &255;
			color[1] = int( (imgData + w * height + h)->g * 255) &255;
			color[2] = int( (imgData + w * height + h)->b * 255) &255;
			fwrite(color, 1, 3, fp);
		}
	fclose(fp);

	return 0;
}