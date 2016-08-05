/*
BMP文件格式如下：
阅读方式从上到下，表示文件从起始，依次顺序排列

-----------------
字节数
-----------------
14 bytes bmp文件头
-----------------
40 bytes 图像信息
-----------------
 6 bytes 可选的调色盘，但是我们用的24位（r8g8b8）没有。所以这个结构 = 0bytes
-----------------
linesize * height bytes
         RGB数据。一个二维表。
-----------------

值得注意的是，RGB一般按照行存储。每一行需要 4bytes对齐，因此 linesize 不一定等于  width*bits;
用下面提供的函数来计算。
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
	uint32	DataOffset;	// in bytes. rgb数据偏移值。
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
	uint8 B;	//windows bgr的原因，高位R 低位B。
	uint8 G;
	uint8 R;
} Color;

#pragma pack(pop)


inline int32 LineSize(int32 width, int32 bits) { return (width * bits + 31) / 32 * 4; }
