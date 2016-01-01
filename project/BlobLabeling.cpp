// BlobLabeling.cpp : ���� �����Դϴ�.
//

#include "stdafx.h"
#include "MFC_OpenCV.h"
#include "BlobLabeling.h"

#define _DEF_MAX_BLOBS		10000
#define _DEF_MAX_LABEL		  100

#define DIM_VECTOR			128
#define THRESHOLD			0.3


// CBlobLabeling

IMPLEMENT_DYNAMIC(CBlobLabeling, CWnd)

CBlobLabeling::CBlobLabeling(void)
{
	m_nThreshold	= 0;
	m_nBlobs		= _DEF_MAX_BLOBS;
	m_Image			= NULL;
	m_recBlobs		= NULL;
	for (int i = 0; i < IMAGENUM; i++)
	{
		m_Search_Surf[i] = FALSE;
	}
	
	vec_filename.push_back("capture/anemone.tif");
	vec_filename.push_back("capture/balloons.tif");
	vec_filename.push_back("capture/smile_wizmars.jpg");
	vec_filename.push_back("capture/2010-06-13 18;00;16.jpg");
	vec_filename.push_back("capture/Mandrill.bmp");
	vec_filename.push_back("capture/face.tif");
	vec_filename.push_back("capture/clock1.tif");
	vec_filename.push_back("capture/duck.tif");
	vec_filename.push_back("capture/fruits.jpg");
}

CBlobLabeling::~CBlobLabeling(void)
{
	if( m_Image != NULL )	cvReleaseImage( &m_Image );	

	if( m_recBlobs != NULL )
	{
		delete m_recBlobs;
		m_recBlobs = NULL;
	}
}

void CBlobLabeling::SetParam(IplImage* image, int nThreshold)
{
	if( m_recBlobs != NULL )
	{
		delete m_recBlobs;

		m_recBlobs	= NULL;
		m_nBlobs	= _DEF_MAX_BLOBS;
	}

	if( m_Image != NULL )	cvReleaseImage( &m_Image );

	m_Image			= cvCloneImage( image );

	m_nThreshold	= nThreshold;
}

void CBlobLabeling::DoLabeling()
{
	m_nBlobs = Labeling(m_Image, m_nThreshold);
}

int CBlobLabeling::Labeling(IplImage* image, int nThreshold)
{
	if( image->nChannels != 1 ) 	return 0;

	int nNumber;

	int nWidth	= image->width;
	int nHeight = image->height;

	unsigned char* tmpBuf = new unsigned char [nWidth * nHeight];

	int i,j;

	for(j=0;j<nHeight;j++)
		for(i=0;i<nWidth ;i++)
			tmpBuf[j*nWidth+i] = (unsigned char)image->imageData[j*image->widthStep+i];

	// ���̺��� ���� ����Ʈ �ʱ�ȭ
	InitvPoint(nWidth, nHeight);

	// ���̺�
	nNumber = _Labeling(tmpBuf, nWidth, nHeight, nThreshold);

	// ����Ʈ �޸� ����
	DeletevPoint();

	if( nNumber != _DEF_MAX_BLOBS )		m_recBlobs = new CvRect [nNumber];

	if( nNumber != 0 )	DetectLabelingRegion(nNumber, tmpBuf, nWidth, nHeight);

	for(j=0;j<nHeight;j++)
		for(i=0;i<nWidth ;i++)
			image->imageData[j*image->widthStep+i] = tmpBuf[j*nWidth+i];

	delete tmpBuf;
	return nNumber;
}

// m_vPoint �ʱ�ȭ �Լ�
void CBlobLabeling::InitvPoint(int nWidth, int nHeight)
{
	int nX, nY;

	m_vPoint = new Visited [nWidth * nHeight];

	for(nY = 0; nY < nHeight; nY++)
	{
		for(nX = 0; nX < nWidth; nX++)
		{
			m_vPoint[nY * nWidth + nX].bVisitedFlag		= FALSE;
			m_vPoint[nY * nWidth + nX].ptReturnPoint.x	= nX;
			m_vPoint[nY * nWidth + nX].ptReturnPoint.y	= nY;
		}
	}
}

void CBlobLabeling::DeletevPoint()
{
	delete m_vPoint;
}

// Size�� nWidth�̰� nHeight�� DataBuf���� 
// nThreshold���� ���� ������ ������ �������� blob���� ȹ��
int CBlobLabeling::_Labeling(unsigned char *DataBuf, int nWidth, int nHeight, int nThreshold)
{
	int Index = 0, num = 0;
	int nX, nY, k, l;
	int StartX , StartY, EndX , EndY;

	// Find connected components
	for(nY = 0; nY < nHeight; nY++)
	{
		for(nX = 0; nX < nWidth; nX++)
		{
			if(DataBuf[nY * nWidth + nX] == 255)		// Is this a new component?, 255 == Object
			{
				num++;

				DataBuf[nY * nWidth + nX] = num;

				StartX = nX, StartY = nY, EndX = nX, EndY= nY;

				__NRFIndNeighbor(DataBuf, nWidth, nHeight, nX, nY, &StartX, &StartY, &EndX, &EndY);

				if(__Area(DataBuf, StartX, StartY, EndX, EndY, nWidth, num) < nThreshold)
				{
					for(k = StartY; k <= EndY; k++)
					{
						for(l = StartX; l <= EndX; l++)
						{
							if(DataBuf[k * nWidth + l] == num)
								DataBuf[k * nWidth + l] = 0;
						}
					}
					--num;

					if(num > 250)
						return  0;
				}
			}
		}
	}

	return num;	
}

// Blob labeling�ؼ� ����� ����� rec�� �� 
void CBlobLabeling::DetectLabelingRegion(int nLabelNumber, unsigned char *DataBuf, int nWidth, int nHeight)
{
	int nX, nY;
	int nLabelIndex ;

	BOOL bFirstFlag[255] = {FALSE,};

	for(nY = 1; nY < nHeight - 1; nY++)
	{
		for(nX = 1; nX < nWidth - 1; nX++)
		{
			nLabelIndex = DataBuf[nY * nWidth + nX];

			if(nLabelIndex != 0)	// Is this a new component?, 255 == Object
			{
				if(bFirstFlag[nLabelIndex] == FALSE)
				{
					m_recBlobs[nLabelIndex-1].x			= nX;
					m_recBlobs[nLabelIndex-1].y			= nY;
					m_recBlobs[nLabelIndex-1].width		= 0;
					m_recBlobs[nLabelIndex-1].height	= 0;

					bFirstFlag[nLabelIndex] = TRUE;
				}
				else
				{
					int left	= m_recBlobs[nLabelIndex-1].x;
					int right	= left + m_recBlobs[nLabelIndex-1].width;
					int top		= m_recBlobs[nLabelIndex-1].y;
					int bottom	= top + m_recBlobs[nLabelIndex-1].height;

					if( left   >= nX )	left	= nX;
					if( right  <= nX )	right	= nX;
					if( top    >= nY )	top		= nY;
					if( bottom <= nY )	bottom	= nY;

					m_recBlobs[nLabelIndex-1].x			= left;
					m_recBlobs[nLabelIndex-1].y			= top;
					m_recBlobs[nLabelIndex-1].width		= right - left;
					m_recBlobs[nLabelIndex-1].height	= bottom - top;

				}
			}

		}
	}

}

// Blob Labeling�� ���� ���ϴ� function
// 2000�� ����ó����ȸ�� �Ǹ� �� ����
int CBlobLabeling::__NRFIndNeighbor(unsigned char *DataBuf, int nWidth, int nHeight, int nPosX, int nPosY, int *StartX, int *StartY, int *EndX, int *EndY )
{
	CvPoint CurrentPoint;

	CurrentPoint.x = nPosX;
	CurrentPoint.y = nPosY;

	m_vPoint[CurrentPoint.y * nWidth +  CurrentPoint.x].bVisitedFlag    = TRUE;
	m_vPoint[CurrentPoint.y * nWidth +  CurrentPoint.x].ptReturnPoint.x = nPosX;
	m_vPoint[CurrentPoint.y * nWidth +  CurrentPoint.x].ptReturnPoint.y = nPosY;

	while(1)
	{
		if( (CurrentPoint.x != 0) && (DataBuf[CurrentPoint.y * nWidth + CurrentPoint.x - 1] == 255) )   // -X ����
		{
			if( m_vPoint[CurrentPoint.y * nWidth +  CurrentPoint.x - 1].bVisitedFlag == FALSE )
			{
				DataBuf[CurrentPoint.y  * nWidth + CurrentPoint.x  - 1]					= DataBuf[CurrentPoint.y * nWidth + CurrentPoint.x];	// If so, mark it
				m_vPoint[CurrentPoint.y * nWidth +  CurrentPoint.x - 1].bVisitedFlag	= TRUE;
				m_vPoint[CurrentPoint.y * nWidth +  CurrentPoint.x - 1].ptReturnPoint	= CurrentPoint;
				CurrentPoint.x--;

				if(CurrentPoint.x <= 0)
					CurrentPoint.x = 0;

				if(*StartX >= CurrentPoint.x)
					*StartX = CurrentPoint.x;

				continue;
			}
		}

		if( (CurrentPoint.x != nWidth - 1) && (DataBuf[CurrentPoint.y * nWidth + CurrentPoint.x + 1] == 255) )   // -X ����
		{
			if( m_vPoint[CurrentPoint.y * nWidth +  CurrentPoint.x + 1].bVisitedFlag == FALSE )
			{
				DataBuf[CurrentPoint.y * nWidth + CurrentPoint.x + 1]					= DataBuf[CurrentPoint.y * nWidth + CurrentPoint.x];	// If so, mark it
				m_vPoint[CurrentPoint.y * nWidth +  CurrentPoint.x + 1].bVisitedFlag	= TRUE;
				m_vPoint[CurrentPoint.y * nWidth +  CurrentPoint.x + 1].ptReturnPoint	= CurrentPoint;
				CurrentPoint.x++;

				if(CurrentPoint.x >= nWidth - 1)
					CurrentPoint.x = nWidth - 1;

				if(*EndX <= CurrentPoint.x)
					*EndX = CurrentPoint.x;

				continue;
			}
		}

		if( (CurrentPoint.y != 0) && (DataBuf[(CurrentPoint.y - 1) * nWidth + CurrentPoint.x] == 255) )   // -X ����
		{
			if( m_vPoint[(CurrentPoint.y - 1) * nWidth +  CurrentPoint.x].bVisitedFlag == FALSE )
			{
				DataBuf[(CurrentPoint.y - 1) * nWidth + CurrentPoint.x]					= DataBuf[CurrentPoint.y * nWidth + CurrentPoint.x];	// If so, mark it
				m_vPoint[(CurrentPoint.y - 1) * nWidth +  CurrentPoint.x].bVisitedFlag	= TRUE;
				m_vPoint[(CurrentPoint.y - 1) * nWidth +  CurrentPoint.x].ptReturnPoint = CurrentPoint;
				CurrentPoint.y--;

				if(CurrentPoint.y <= 0)
					CurrentPoint.y = 0;

				if(*StartY >= CurrentPoint.y)
					*StartY = CurrentPoint.y;

				continue;
			}
		}

		if( (CurrentPoint.y != nHeight - 1) && (DataBuf[(CurrentPoint.y + 1) * nWidth + CurrentPoint.x] == 255) )   // -X ����
		{
			if( m_vPoint[(CurrentPoint.y + 1) * nWidth +  CurrentPoint.x].bVisitedFlag == FALSE )
			{
				DataBuf[(CurrentPoint.y + 1) * nWidth + CurrentPoint.x]					= DataBuf[CurrentPoint.y * nWidth + CurrentPoint.x];	// If so, mark it
				m_vPoint[(CurrentPoint.y + 1) * nWidth +  CurrentPoint.x].bVisitedFlag	= TRUE;
				m_vPoint[(CurrentPoint.y + 1) * nWidth +  CurrentPoint.x].ptReturnPoint = CurrentPoint;
				CurrentPoint.y++;

				if(CurrentPoint.y >= nHeight - 1)
					CurrentPoint.y = nHeight - 1;

				if(*EndY <= CurrentPoint.y)
					*EndY = CurrentPoint.y;

				continue;
			}
		}

		if(		(CurrentPoint.x == m_vPoint[CurrentPoint.y * nWidth + CurrentPoint.x].ptReturnPoint.x) 
			&&	(CurrentPoint.y == m_vPoint[CurrentPoint.y * nWidth + CurrentPoint.x].ptReturnPoint.y) )
		{
			break;
		}
		else
		{
			CurrentPoint = m_vPoint[CurrentPoint.y * nWidth + CurrentPoint.x].ptReturnPoint;
		}
	}

	return 0;
}

// ������ ���� blob�� Į�� ���� ������ ũ�⸦ ȹ��
int CBlobLabeling::__Area(unsigned char *DataBuf, int StartX, int StartY, int EndX, int EndY, int nWidth, int nLevel)
{
	int nArea = 0;
	int nX, nY;

	for (nY = StartY; nY < EndY; nY++)
		for (nX = StartX; nX < EndX; nX++)
			if (DataBuf[nY * nWidth + nX] == nLevel)
				++nArea;

	return nArea;
}

/******************************************************************************************************/

// nWidth�� nHeight���� ���� rec�� ��� ����
void CBlobLabeling::BlobSmallSizeConstraint(int nWidth, int nHeight)
{
	_BlobSmallSizeConstraint(nWidth, nHeight, m_recBlobs, &m_nBlobs);
}

void CBlobLabeling::_BlobSmallSizeConstraint(int nWidth, int nHeight, CvRect* rect, int *nRecNumber)
{
	if(*nRecNumber == 0)	return;

	int nX;
	int ntempRec = 0;

	CvRect* temp = new CvRect[*nRecNumber];

	for(nX = 0; nX < *nRecNumber; nX++)
	{
		temp[nX] = rect[nX];
	}

	for(nX = 0; nX < *nRecNumber; nX++)
	{
		if( (rect[nX].width > nWidth) && (rect[nX].height > nHeight) )
		{
			temp[ntempRec] = rect[nX];
			ntempRec++;
		}
	}

	*nRecNumber = ntempRec;

	for(nX = 0; nX < *nRecNumber; nX++)
		rect[nX] = temp[nX];

	delete temp;
}

// nWidth�� nHeight���� ū rec�� ��� ����
void CBlobLabeling::BlobBigSizeConstraint(int nWidth, int nHeight)
{
	_BlobBigSizeConstraint(nWidth, nHeight, m_recBlobs, &m_nBlobs);
}

void CBlobLabeling::_BlobBigSizeConstraint(int nWidth, int nHeight, CvRect* rect, int* nRecNumber)
{
	if(*nRecNumber == 0)	return;

	int nX;
	int ntempRec = 0;

	CvRect* temp = new CvRect [*nRecNumber];

	for(nX = 0; nX < *nRecNumber; nX++)
	{
		temp[nX] = rect[nX];
	}

	for(nX = 0; nX < *nRecNumber; nX++)
	{
		if( (rect[nX].width < nWidth) && (rect[nX].height < nHeight) )
		{
			temp[ntempRec] = rect[nX];
			ntempRec++;
		}
	}

	*nRecNumber = ntempRec;

	for(nX = 0; nX < *nRecNumber; nX++)
		rect[nX] = temp[nX];

	delete temp;
}

/**
* 2�� ������ ��Ŭ����Ÿ��� ����ؼ� ��ȯ
*
* @param[in] vec     ����1�� �迭
* @param[in] mvec   ����2�� �迭
* @param[in] length  ������ ����
*
* @return ��Ŭ���� �Ÿ�
*/

double CBlobLabeling::euclidDistance(float* vec1, float* vec2, int length) {
	double sum = 0.0;
	for (int i = 0; i < length; i++) {
		sum += (vec1[i] - vec2[i]) * (vec1[i] - vec2[i]);
	}
	return sqrt(sum);
}

/**
* �ֱ����� Ž��
*
* @param[in]   vec             Ư¡ ����
* @param[in]   laplacian       ���ö�þ�
* @param[in]   keypoints       Ű����Ʈ�� ���գ������� �߽����κ��� �ֱ������� Ž��)
* @param[in]   descriptors     Ư¡ ������ ����
*
* @return �ֱ������� �ε������߰ߵ��� ���� ���� -1��
*/

int CBlobLabeling::nearestNeighbor(float* vec, int laplacian, CvSeq* keypoints, CvSeq* descriptors) {
	int neighbor = -1;
	double minDist = 1e6;

	for (int i = 0; i < descriptors->total; i++) {
		CvSURFPoint* pt = (CvSURFPoint*)cvGetSeqElem(keypoints, i);
		// ���ö�þ��� �ٸ� Ű����Ʈ�� ����
		if (laplacian != pt->laplacian) continue;
		float* v = (float*)cvGetSeqElem(descriptors, i);
		double d = euclidDistance(vec, v, DIM_VECTOR);
		// ���� ����� ���� ������ �Ű� ����
		if (d < minDist) {
			minDist = d;
			neighbor = i;
		}
	}

	// �ֱ����������� �Ÿ��� �Ӱ谪 �̻��̶�� �����Ѵ�.
	if (minDist < THRESHOLD) {
		return neighbor;
	}

	// �ֱ������� ���� ���
	return -1;
}

/**
* ����1�� Ű����Ʈ�� ����� ȭ�� 2�� Ű����Ʈ�� ¦(pair)���� �� ��ȯ
*
* @param[in]  keypoints1       ����1�� Ű����Ʈ
* @param[in]  descriptors1     ����1�� Ư¡����
* @param[in]  keypoints2       ����2�� Ű����Ʈ
* @param[in]  descriptors2     ����2�� Ư¡����


* @param[out] ptpairs          ���� Ű����Ʈ �ε����� ��(2���� 1¦)
*
* @return ����
*/
void CBlobLabeling::findPairs(CvSeq* keypoints1, CvSeq* descriptors1, CvSeq* keypoints2, CvSeq* descriptors2,vector<int>& ptpairs) {
	ptpairs.clear();
	// ���� 1�� �� Ű����Ʈ�� ���ؼ� �ֱ������� �˻�
	for (int i = 0; i < descriptors1->total; i++) {
		CvSURFPoint* pt1 = (CvSURFPoint*)cvGetSeqElem(keypoints1, i);
		float* desc1 = (float*)cvGetSeqElem(descriptors1, i);
		// �ֱ������� �˻�
		int nn = nearestNeighbor(desc1, pt1->laplacian, keypoints2, descriptors2);
		// �ֱ������� ���� ��� ���� 1�� �ε����� ���� 2�� �ε����� ���ʷ� ���
		if (nn >= 0) {
			ptpairs.push_back(i);
			ptpairs.push_back(nn);
		}
	}
}

void CBlobLabeling::textImage(IplImage* src, CvPoint pos, int index)
{
	char s_output_result[50];
	CvFont font;
	switch (index)
	{
	case 0:
		sprintf(s_output_result, "anemone");
		break;
	case 1:
		sprintf(s_output_result, "balloons");
		break;
	case 2:
		sprintf(s_output_result, "smile_wizmars");
		break;
	case 3:
		sprintf(s_output_result, "augustus");
		break;
	case 4:
		sprintf(s_output_result, "Mandrill");
		break;
	case 5:
		sprintf(s_output_result, "face");
		break;
	case 6:
		sprintf(s_output_result, "clock");
		break;
	case 7:
		sprintf(s_output_result, "duck");
		break;
	case 8:
		sprintf(s_output_result, "fruits");
		break;
	default:
		sprintf(s_output_result, "null");
		break;
	}
	cvInitFont(&font, CV_FONT_HERSHEY_SIMPLEX|CV_FONT_ITALIC, 1.0, 1.0, 0, 2);  //�̷� ���� ����.
	cvPutText(src, s_output_result, pos, &font, cvScalar(0 ,0, 255));   //cvPoint�� ���� ���� ��ġ ����(uv)
	//void cvInitFont(CvFont* font, int font_face, double hscale, double vscale, double italic_scale, int thickness)
}



int CBlobLabeling::DoSurf(IplImage* input, int n)
{
	// ������ �׷��̽����Ϸ� �ε�
	IplImage* grayImage1 = cvCreateImage(cvGetSize(input), IPL_DEPTH_8U, 1);
	cvCvtColor(input, grayImage1, CV_BGR2GRAY);
	//IplImage* grayImage2 = cvLoadImage("capture/anemone.tif", CV_LOAD_IMAGE_GRAYSCALE);
	IplImage* grayImage2 = cvLoadImage(vec_filename[n], CV_LOAD_IMAGE_GRAYSCALE);

	CvSeq *keypoints1 = 0, *descriptors1 = 0;
	CvSeq *keypoints2 = 0, *descriptors2 = 0;
	CvMemStorage* storage = cvCreateMemStorage(0);
	CvSURFParams params = cvSURFParams(500, 1);

	// SURF�� ����
	cvExtractSURF(grayImage1, 0, &keypoints1, &descriptors1, storage, params);
	cvExtractSURF(grayImage2, 0, &keypoints2, &descriptors2, storage, params);

	// Ư¡ ������ ���絵�� ���� Ű����Ʈ���� ������ ����
	vector<int> ptpairs;  // keypoints�� �ε����� 2���� ¦�� �ǵ��� ����
	findPairs(keypoints1, descriptors1, keypoints2, descriptors2, ptpairs);

	// 2���� ¦�� ���߾� ������ �ߴ´�.
	int size = (int)ptpairs.size();

	//textImage(resultImage, cvPoint(ns.x+(ne.x-ns.x)/2, ns.y+(ne.y-ns.y)/2), n); // ���� ������� ������
	// ��ó�� - �޸� ���� ��
	cvReleaseImage(&grayImage1);
	cvReleaseImage(&grayImage2);
	cvClearSeq(keypoints1);
	cvClearSeq(descriptors1);
	cvClearSeq(keypoints2);
	cvClearSeq(descriptors2);
	cvReleaseMemStorage(&storage);

	return size;
}


BEGIN_MESSAGE_MAP(CBlobLabeling, CWnd)
END_MESSAGE_MAP()



// CBlobLabeling �޽��� ó�����Դϴ�.


