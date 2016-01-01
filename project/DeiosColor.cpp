#include "StdAfx.h"
#include "DeiosColor.h"


CDeiosColor::CDeiosColor(void)
{
}


CDeiosColor::~CDeiosColor(void)
{
}


void CDeiosColor::DSplitRGB(IplImage * color_image, IplImage ** red, IplImage ** green, IplImage ** blue)
{
	*red = cvCreateImage(cvGetSize(color_image), IPL_DEPTH_8U,1);
	*green = cvCreateImage(cvGetSize(color_image), IPL_DEPTH_8U,1);
	*blue = cvCreateImage(cvGetSize(color_image), IPL_DEPTH_8U,1);

	cvSplit(color_image, *red, *green, *blue, NULL);
}
