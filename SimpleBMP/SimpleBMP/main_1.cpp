#include "bmp_head.h"
#include <stdio.h>
#include <memory.h>

#include "raytracing/raytrace_scene.h"
#include "raytracing/raytrace_model.h"

#include <d3dx9math.h>
#include <d3dx9.h>
#pragma comment(lib, "d3dx9.lib")

void SaveToFile_RGB24(FILE* file, Color* colors, int width, int height)
{
	//根据bmp格式写文件
	const int BITS24 = 24;
	int pitch = LineSize(width, BITS24);
	BITMAPFILEHEADER fileHeader;
	BITMAPINFOHEADER infoHeader;
	memset(&fileHeader, 0, sizeof(fileHeader));
	memset(&infoHeader, 0, sizeof(infoHeader));

	//先写文件头
	fileHeader.bfType = 0x4D42;
	fileHeader.bfOffBits = sizeof(fileHeader) + sizeof(infoHeader);
	fileHeader.bfSize = fileHeader.bfOffBits + pitch * height;

	//再写信息头
	infoHeader.biSize = sizeof(infoHeader);
	infoHeader.biPlanes = 1;
	infoHeader.biWidth = width;
	infoHeader.biHeight = height;
	infoHeader.biBitCount = BITS24;
	infoHeader.biCompression = 0;
	infoHeader.biSizeImage = fileHeader.bfSize - fileHeader.bfOffBits;

	fwrite(&fileHeader, sizeof(fileHeader), 1, file);
	fwrite(&infoHeader, sizeof(infoHeader), 1, file);

	//开始写数据
	uint8* buffer = reinterpret_cast<uint8*>(colors);
	if (pitch == width * 3)
	{
		//原点在左下角，因此反着写buffer
		for (int i = 0; i < height; ++i )//height - 1; i >= 0; i--)
		{
			int offset = pitch * i;
			uint8* lineData = buffer + offset;
			//uint8* lineData = reinterpret_cast<uint8*>(colors+width*i);
			fwrite(lineData, pitch, 1, file);
			//fwrite(lineData, 1, 1, file);
		}
	}
	else
	{
		int dummySize = pitch;
		pitch = width * (BITS24 / 8);
		dummySize = dummySize - pitch;	//每行后头有个小尾巴，用来补齐每行长度（4字节的倍数）
		uint8* dummyBuffer = new uint8[dummySize];

		//原点在左下角，因此反着写buffer
		for (int i = height - 1; i >= 0; i--)
		{
			int offset = pitch * i;
			uint8* lineData = buffer + offset;
			fwrite(lineData, pitch, 1, file);
			fwrite(dummyBuffer, dummySize, 1, file);
		}
		delete[] dummyBuffer;
	}

}

int main()
{
	//RayTrace::Scene scene;
	//RayTrace::ICollideLight* light1 = new RayTrace::CCollideLightPoint(D3DXVECTOR3(0, 1, 0), 30);
	//scene.AddLight( light1 );
	//RayTrace::ICollideModel* model1 = new RayTrace::CCollildeBall( D3DXVECTOR3(0, 0, 0), 10 );
	//scene.AddModel( model1 );
	//scene.GenerateRayTrace();
	//const D3DXCOLOR* imgData = scene.GetDataPtr();

	////构件图像数据
	//int width = scene.GetWidth();
	//int height = scene.GetHeight();

	int width = 124;
	int height = 124;
	const int PIXEL_COUNT = width * height;
	Color* colors = new Color[PIXEL_COUNT];

	//设置背景色
	//for(auto& c : colors) for  c++11
	for (int idx = 0; idx < PIXEL_COUNT; idx++)
	{
		Color& color = colors[idx];
		int i = idx / width;
		int j = idx % width;
		color.R = idx&255;
		color.G = 0;//GR(i,j)&255;
		color.B = 0;//BL(i,j)&255;
	}

	//绘制图像
	//这个地方开始你想要做的事情
	//Render(colors, width, height);

	//保存结果
	const char* filePath = "f:\\a.bmp";
	FILE* bmpFile;
	fopen_s(&bmpFile, filePath, "w");
	if (bmpFile)
	{
		SaveToFile_RGB24(bmpFile, colors, width, height);
		fclose(bmpFile);
	}
	delete[] colors;
	return 0;
}