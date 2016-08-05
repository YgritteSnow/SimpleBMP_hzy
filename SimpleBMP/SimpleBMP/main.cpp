#include "bmp_head.h"
#include <stdio.h>
#include <memory.h>

void SaveToFile_RGB24(FILE* file, Color* colors, int width, int height)
{
	//����bmp��ʽд�ļ�
	const int BITS24 = 24;
	int pitch = LineSize(width, BITS24);
	BitmapFileHead fileHeader;
	BitmapInfoHead infoHeader;
	memset(&fileHeader, 0, sizeof(fileHeader));
	memset(&infoHeader, 0, sizeof(infoHeader));

	//��д�ļ�ͷ
	fileHeader.MagicBM = 0x4D42;
	fileHeader.DataOffset = sizeof(fileHeader) + sizeof(infoHeader);
	fileHeader.FileSize = fileHeader.DataOffset + pitch * height;

	//��д��Ϣͷ
	infoHeader.InfoSize = sizeof(infoHeader);
	infoHeader.Plane = 1;
	infoHeader.Width = width;
	infoHeader.Height = height;
	infoHeader.DataBits = BITS24;
	infoHeader.Format = 0;
	infoHeader.DataSize = fileHeader.FileSize - fileHeader.DataOffset;

	fwrite(&fileHeader, sizeof(fileHeader), 1, file);
	fwrite(&infoHeader, sizeof(infoHeader), 1, file);

	//��ʼд����
	uint8* buffer = reinterpret_cast<uint8*>(colors);
	if (pitch == width * 3)
	{
		//ԭ�������½ǣ���˷���дbuffer
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
		dummySize = dummySize - pitch;	//ÿ�к�ͷ�и�Сβ�ͣ���������ÿ�г��ȣ�4�ֽڵı�����
		uint8* dummyBuffer = new uint8[dummySize];

		//ԭ�������½ǣ���˷���дbuffer
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
	//����ͼ������
	int width = 256;
	int height = 256;
	const int PIXEL_COUNT = width * height;
	Color* colors = new Color[PIXEL_COUNT];

	//���ñ���ɫ
	//for(auto& c : colors) for  c++11
	for (int i = 0; i < PIXEL_COUNT; i++)
	{
		Color& color = colors[i];
		color.R = i&255;
		color.G = 0;
		color.B = 0;
	}

	//����ͼ��
	//����ط���ʼ����Ҫ��������
	//Render(colors, width, height);

	//������
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