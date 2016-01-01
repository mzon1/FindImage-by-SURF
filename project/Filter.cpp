#include "StdAfx.h"
#include "Filter.h"


CFilter::CFilter(void)
{
}


CFilter::~CFilter(void)
{
}

// 정성환 외 1명/컴퓨터 비전 실무 프로그래밍/홍릉과학출판사/2007/295p
IplImage * CFilter::ApplyFilter2D(IplImage * src, float *kernel, int kHeight, int kWidth){
	CvMat mat_kernel = cvMat( kWidth, kHeight, CV_32FC1, kernel );   

	IplImage *dst = ApplyFilter2D( src, mat_kernel );   

	cvReleaseData( &mat_kernel );   

	return dst;   
}
// 정성환 외 1명/컴퓨터 비전 실무 프로그래밍/홍릉과학출판사/2007/295p~296p
IplImage * CFilter::ApplyFilter2D(IplImage * src, float **kernel, int kHeight, int kWidth){
	int cnt = -1;   

	float *single_kernel = (float *)calloc( kHeight*kWidth, sizeof(float) );   

	for(int i = 0; i < kHeight ; i++)
		for(int j = 0; j<kWidth ; j++)
			single_kernel[++cnt] = kernel[i][j];

	IplImage *dst = ApplyFilter2D( src, single_kernel, kHeight, kWidth );  

	free(single_kernel);

	return dst;  
}
// 정성환 외 1명/컴퓨터 비전 실무 프로그래밍/홍릉과학출판사/2007/296p
IplImage * CFilter::ApplyFilter2D(IplImage * src, CvMat kernel){
	IplImage *dst = cvCreateImage(cvGetSize(src), IPL_DEPTH_8U, src->nChannels);

	cvFilter2D(src, dst, &kernel, cvPoint(-1, -1));

	return dst;
}

