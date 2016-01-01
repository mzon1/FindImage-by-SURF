#include "StdAfx.h"
#include "DeiosImgUtil.h"


CDeiosImgUtil::CDeiosImgUtil(void)
{
}


CDeiosImgUtil::~CDeiosImgUtil(void)
{
}

// ����ȯ �� 1��/��ǻ�� ���� �ǹ� ���α׷���/ȫ���������ǻ�/2007/324p~325p
IplImage *CDeiosImgUtil::Resize4Bitmap(int resize_height, int resize_width, IplImage * image)
{
	int height = image->height;
	int width = image->width - (image->width%4);

	int sHeight = height;
	int sWidth = width;

	if( (height > resize_height) && (width > resize_width) ){
		sHeight = resize_width;
		width = (int) (width*(resize_height/(double)height));
		sWidth = width - (width%4);
	}else if( height > resize_height){
		sHeight = resize_height;
	}else if( width > resize_width){
		sWidth = resize_width - (resize_width%4);
	}

	IplImage *resize_image = cvCreateImage(cvSize(sWidth, sHeight), IPL_DEPTH_8U, image->nChannels);

	cvResize(image, resize_image, CV_INTER_CUBIC);

	return resize_image;
}

// ����ȯ �� 1��/��ǻ�� ���� �ǹ� ���α׷���/ȫ���������ǻ�/2007/325p~326p
HBITMAP CDeiosImgUtil::IplImage2Bitmap(IplImage *image )

{
	HDC hDC=::CreateCompatibleDC(0);
	BYTE tmp[sizeof(BITMAPINFO)+255*4 ];
	BITMAPINFO *bmi = (BITMAPINFO *) tmp;
	HBITMAP hBmp;

	int height = image->height;
	int width = image->width;
	int widthStep = width +(width % 4);
	int nChannels = image->nChannels;
	int bpp = 8*nChannels;
	int bmpDataSize = height*widthStep*nChannels;

	memset( bmi, 0, sizeof(BITMAPINFO) );
	bmi->bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
	bmi->bmiHeader.biHeight = height; // ������ ����
	bmi->bmiHeader.biWidth = width; //������ �ʺ�
	bmi->bmiHeader.biPlanes = 1; // ��Ʈ �÷��� �� (�׻� 1��)
	bmi->bmiHeader.biBitCount = bpp; // �� ȭ�Ҵ� ��Ʈ ����
	bmi->bmiHeader.biCompression = BI_RGB; // BI_RGB: �������� ����.
	bmi->bmiHeader.biSizeImage = bmpDataSize; // ������ ũ��
	bmi->bmiHeader.biClrUsed = 0;

	if( bpp == 8 ){
		for( int i=0; i<256; i++ ){
			bmi->bmiColors[i].rgbBlue = i;
			bmi->bmiColors[i].rgbGreen = i;
			bmi->bmiColors[i].rgbRed = i;
		}
	}

	hBmp = ::CreateDIBSection( hDC, bmi, DIB_RGB_COLORS, NULL, 0, 0 );
	::DeleteDC(hDC);

	::SetBitmapBits( hBmp, image->imageSize, image->imageData );

	return hBmp;
}