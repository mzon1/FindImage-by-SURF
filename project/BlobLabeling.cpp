// BlobLabeling.cpp : 구현 파일입니다.
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

	// 레이블링을 위한 포인트 초기화
	InitvPoint(nWidth, nHeight);

	// 레이블링
	nNumber = _Labeling(tmpBuf, nWidth, nHeight, nThreshold);

	// 포인트 메모리 해제
	DeletevPoint();

	if( nNumber != _DEF_MAX_BLOBS )		m_recBlobs = new CvRect [nNumber];

	if( nNumber != 0 )	DetectLabelingRegion(nNumber, tmpBuf, nWidth, nHeight);

	for(j=0;j<nHeight;j++)
		for(i=0;i<nWidth ;i++)
			image->imageData[j*image->widthStep+i] = tmpBuf[j*nWidth+i];

	delete tmpBuf;
	return nNumber;
}

// m_vPoint 초기화 함수
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

// Size가 nWidth이고 nHeight인 DataBuf에서 
// nThreshold보다 작은 영역을 제외한 나머지를 blob으로 획득
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

// Blob labeling해서 얻어진 결과의 rec을 얻어냄 
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

// Blob Labeling을 실제 행하는 function
// 2000년 정보처리학회에 실린 논문 참조
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
		if( (CurrentPoint.x != 0) && (DataBuf[CurrentPoint.y * nWidth + CurrentPoint.x - 1] == 255) )   // -X 방향
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

		if( (CurrentPoint.x != nWidth - 1) && (DataBuf[CurrentPoint.y * nWidth + CurrentPoint.x + 1] == 255) )   // -X 방향
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

		if( (CurrentPoint.y != 0) && (DataBuf[(CurrentPoint.y - 1) * nWidth + CurrentPoint.x] == 255) )   // -X 방향
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

		if( (CurrentPoint.y != nHeight - 1) && (DataBuf[(CurrentPoint.y + 1) * nWidth + CurrentPoint.x] == 255) )   // -X 방향
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

// 영역중 실제 blob의 칼라를 가진 영역의 크기를 획득
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

// nWidth와 nHeight보다 작은 rec을 모두 삭제
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

// nWidth와 nHeight보다 큰 rec을 모두 삭제
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
* 2개 벡터의 유클리드거리의 계산해서 반환
*
* @param[in] vec     벡터1의 배열
* @param[in] mvec   벡터2의 배열
* @param[in] length  벡터의 길이
*
* @return 유클리드 거리
*/

double CBlobLabeling::euclidDistance(float* vec1, float* vec2, int length) {
	double sum = 0.0;
	for (int i = 0; i < length; i++) {
		sum += (vec1[i] - vec2[i]) * (vec1[i] - vec2[i]);
	}
	return sqrt(sum);
}

/**
* 최근접점 탐색
*
* @param[in]   vec             특징 벡터
* @param[in]   laplacian       라플라시안
* @param[in]   keypoints       키포인트의 집합（집합의 중심으로부터 최근접점의 탐색)
* @param[in]   descriptors     특징 벡터의 집합
*
* @return 최근접점의 인덱스（발견되지 않을 때는 -1）
*/

int CBlobLabeling::nearestNeighbor(float* vec, int laplacian, CvSeq* keypoints, CvSeq* descriptors) {
	int neighbor = -1;
	double minDist = 1e6;

	for (int i = 0; i < descriptors->total; i++) {
		CvSURFPoint* pt = (CvSURFPoint*)cvGetSeqElem(keypoints, i);
		// 라플라시안이 다른 키포인트는 무시
		if (laplacian != pt->laplacian) continue;
		float* v = (float*)cvGetSeqElem(descriptors, i);
		double d = euclidDistance(vec, v, DIM_VECTOR);
		// 보다 가까운 점이 있으면 옮겨 놓기
		if (d < minDist) {
			minDist = d;
			neighbor = i;
		}
	}

	// 최근접점에서도 거리가 임계값 이상이라면 무시한다.
	if (minDist < THRESHOLD) {
		return neighbor;
	}

	// 최근접점이 없는 경우
	return -1;
}

/**
* 영상1의 키포인트와 가까운 화상 2의 키포인트를 짝(pair)으로 해 반환
*
* @param[in]  keypoints1       영상1의 키포인트
* @param[in]  descriptors1     영상1의 특징벡터
* @param[in]  keypoints2       영상2의 키포인트
* @param[in]  descriptors2     영상2의 특징벡터


* @param[out] ptpairs          유사 키포인트 인덱스의 열(2개당 1짝)
*
* @return 없음
*/
void CBlobLabeling::findPairs(CvSeq* keypoints1, CvSeq* descriptors1, CvSeq* keypoints2, CvSeq* descriptors2,vector<int>& ptpairs) {
	ptpairs.clear();
	// 영상 1의 각 키포인트에 관해서 최근접점을 검색
	for (int i = 0; i < descriptors1->total; i++) {
		CvSURFPoint* pt1 = (CvSURFPoint*)cvGetSeqElem(keypoints1, i);
		float* desc1 = (float*)cvGetSeqElem(descriptors1, i);
		// 최근접점의 검색
		int nn = nearestNeighbor(desc1, pt1->laplacian, keypoints2, descriptors2);
		// 최근접점이 있을 경우 영상 1의 인덱스와 영상 2의 인덱스를 차례로 등록
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
	cvInitFont(&font, CV_FONT_HERSHEY_SIMPLEX|CV_FONT_ITALIC, 1.0, 1.0, 0, 2);  //이런 저런 설정.
	cvPutText(src, s_output_result, pos, &font, cvScalar(0 ,0, 255));   //cvPoint로 글자 시작 위치 설정(uv)
	//void cvInitFont(CvFont* font, int font_face, double hscale, double vscale, double italic_scale, int thickness)
}



int CBlobLabeling::DoSurf(IplImage* input, int n)
{
	// 영상은 그레이스케일로 로드
	IplImage* grayImage1 = cvCreateImage(cvGetSize(input), IPL_DEPTH_8U, 1);
	cvCvtColor(input, grayImage1, CV_BGR2GRAY);
	//IplImage* grayImage2 = cvLoadImage("capture/anemone.tif", CV_LOAD_IMAGE_GRAYSCALE);
	IplImage* grayImage2 = cvLoadImage(vec_filename[n], CV_LOAD_IMAGE_GRAYSCALE);

	CvSeq *keypoints1 = 0, *descriptors1 = 0;
	CvSeq *keypoints2 = 0, *descriptors2 = 0;
	CvMemStorage* storage = cvCreateMemStorage(0);
	CvSURFParams params = cvSURFParams(500, 1);

	// SURF의 추출
	cvExtractSURF(grayImage1, 0, &keypoints1, &descriptors1, storage, params);
	cvExtractSURF(grayImage2, 0, &keypoints2, &descriptors2, storage, params);

	// 특징 벡터의 유사도가 높은 키포인트들을 선으로 연결
	vector<int> ptpairs;  // keypoints의 인덱스가 2개씩 짝이 되도록 저장
	findPairs(keypoints1, descriptors1, keypoints2, descriptors2, ptpairs);

	// 2개씩 짝을 맞추어 직선을 긋는다.
	int size = (int)ptpairs.size();

	//textImage(resultImage, cvPoint(ns.x+(ne.x-ns.x)/2, ns.y+(ne.y-ns.y)/2), n); // 최종 결과위한 누적용
	// 후처리 - 메모리 해제 등
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



// CBlobLabeling 메시지 처리기입니다.


