#pragma once
#include <cv.h>
#include <cxcore.h>
class CDeiosImgUtil
{
public:
	CDeiosImgUtil(void);
	~CDeiosImgUtil(void);
	IplImage *Resize4Bitmap(int resize_height, int resize_width, IplImage * image);
	HBITMAP IplImage2Bitmap(IplImage * image);
};

