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
	BitmapFileHead fileHeader;
	BitmapInfoHead infoHeader;
	memset(&fileHeader, 0, sizeof(fileHeader));
	memset(&infoHeader, 0, sizeof(infoHeader));

	//先写文件头
	fileHeader.MagicBM = 0x4D42;
	fileHeader.DataOffset = sizeof(fileHeader) + sizeof(infoHeader);
	fileHeader.FileSize = fileHeader.DataOffset + pitch * height;

	//再写信息头
	infoHeader.InfoSize = sizeof(infoHeader);
	infoHeader.Plane = 1;
	infoHeader.Width = width;
	infoHeader.Height = height;
	infoHeader.DataBits = BITS24;
	infoHeader.Format = 0;
	infoHeader.DataSize = fileHeader.FileSize - fileHeader.DataOffset;

	fwrite(&fileHeader, sizeof(fileHeader), 1, file);
	fwrite(&infoHeader, sizeof(infoHeader), 1, file);

	//开始写数据
	uint8* buffer = reinterpret_cast<uint8*>(colors);
	if (pitch == width * 3)
	{
		//原点在左下角，因此反着写buffer
		for (int i = height - 1; i >= 0; i--)
		{
			int offset = pitch * i;
			uint8* lineData = buffer + offset;
			fwrite(lineData, pitch, 1, file);
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
	int i=1;
	RayTrace::Scene scene;
	RayTrace::ICollideLight* light1 = new RayTrace::CCollideLightPoint(D3DXVECTOR3(0, 1, 0), 30);
	scene.AddLight( light1 );
	RayTrace::ICollideModel* model1 = new RayTrace::CCollildeBall( D3DXVECTOR3(0, 0, 0), 10 );
	scene.AddModel( model1 );
	scene.GenerateRayTrace();
	const D3DXCOLOR* imgData = scene.GetDataPtr();

	//构件图像数据
	int width = scene.GetWidth();
	int height = scene.GetHeight();
	const int PIXEL_COUNT = width * height;
	Color* colors = new Color[PIXEL_COUNT];

	//设置背景色
	//for(auto& c : colors) for  c++11
	for (int i = 0; i < PIXEL_COUNT; i++)
	{
		Color& color = colors[i];
		color.R = imgData[i].r * 255;
		color.G = imgData[i].g * 255;
		color.B = imgData[i].b * 255;
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