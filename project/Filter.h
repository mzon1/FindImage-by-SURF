#pragma once
#include <cv.h>
#include <cxcore.h>
class CFilter
{
public:
	CFilter(void);
	~CFilter(void);
	IplImage * ApplyFilter2D(IplImage * src, float *kernel, int kHeight, int kWidth);
	IplImage * ApplyFilter2D(IplImage * src, float **kernel, int kHeight, int kWidth);
	IplImage * ApplyFilter2D(IplImage * src, CvMat kernel);
};

