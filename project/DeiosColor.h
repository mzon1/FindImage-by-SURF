#pragma once
#include <cv.h>
#include <cxcore.h>
class CDeiosColor
{
public:
	CDeiosColor(void);
	~CDeiosColor(void);
	void DSplitRGB(IplImage * color_image, IplImage ** red, IplImage ** green, IplImage ** blue);
};

