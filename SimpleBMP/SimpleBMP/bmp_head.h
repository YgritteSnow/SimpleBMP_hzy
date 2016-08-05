/*
BMP�ļ���ʽ���£�
�Ķ���ʽ���ϵ��£���ʾ�ļ�����ʼ������˳������

-----------------
�ֽ���
-----------------
14 bytes bmp�ļ�ͷ
-----------------
40 bytes ͼ����Ϣ
-----------------
 6 bytes ��ѡ�ĵ�ɫ�̣����������õ�24λ��r8g8b8��û�С���������ṹ = 0bytes
-----------------
linesize * height bytes
         RGB���ݡ�һ����ά��
-----------------

ֵ��ע����ǣ�RGBһ�㰴���д洢��ÿһ����Ҫ 4bytes���룬��� linesize ��һ������  width*bits;
�������ṩ�ĺ��������㡣
*/

#pragma once

#pragma pack(push)
#pragma pack(1)

typedef char int8;
typedef short int16;
typedef int int32;
typedef unsigned char uint8;
typedef unsigned short uint16;
typedef unsigned int uint32;


typedef struct tagBITMAPFILEHEADER {
	uint16	MagicBM;	//0x4D42
	uint32	FileSize;	// whole file size.
	uint16	Reserved1;
	uint16	Reserved2;
	uint32	DataOffset;	// in bytes. rgb����ƫ��ֵ��
} BitmapFileHead;

typedef struct tagBITMAPINFOHEADER {
	uint32	InfoSize;	// =sizeof(BitmapInfoHead) =sizeof(this)
	int32	Width;
	int32	Height;
	uint16	Plane;		// always = 1, compare to other format like yuv, i420, etc.
	uint16	DataBits;	// normally use 24or32, 1, 4 is special used for compressed.
	uint32	Format;		// must be 0 for uncompressed here.
	uint32	DataSize;	// raw data size.
	int32	PPMX;
	int32	PPMY;
	uint32	ClrUsed;
	uint32	ClrImportant;
} BitmapInfoHead;

typedef struct tagColor
{
	uint8 B;	//windows bgr��ԭ�򣬸�λR ��λB��
	uint8 G;
	uint8 R;
} Color;

#pragma pack(pop)


inline int32 LineSize(int32 width, int32 bits) { return (width * bits + 31) / 32 * 4; }
