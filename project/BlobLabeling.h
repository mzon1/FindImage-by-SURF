#pragma once
#include <cv.h>
#include <highgui.h>
#include <vector>
using std::vector;
using namespace std;

#define IMAGENUM	9



typedef struct 
{
	BOOL	bVisitedFlag;
	CvPoint ptReturnPoint;
} Visited;
// CBlobLabeling

class CBlobLabeling : public CWnd
{
	DECLARE_DYNAMIC(CBlobLabeling)

public:
	CBlobLabeling();
	virtual ~CBlobLabeling();
public:
	IplImage*	m_Image;				// ���̺��� ���� �̹���
	int			m_nThreshold;			// ���̺� ������Ȧ�� ��
	Visited*	m_vPoint;				// ���̺��� �湮����
	int			m_nBlobs;				// ���̺��� ����
	CvRect*		m_recBlobs;				// �� ���̺� ����
	bool m_Search_Surf[IMAGENUM];
	vector<const char*> vec_filename;


public:
	// ���̺� �̹��� ����
	void		SetParam(IplImage* image, int nThreshold);

	// ���̺�(����)
	void		DoLabeling();

	int		DoSurf(IplImage* input, int n);

	void		textImage(IplImage* src, CvPoint pos, int index);

	void findPairs(CvSeq* keypoints1, CvSeq* descriptors1, 
		CvSeq* keypoints2, CvSeq* descriptors2,vector<int>& ptpairs);

	int nearestNeighbor(float* vec, int laplacian, 
		CvSeq* keypoints, CvSeq* descriptors);

	double euclidDistance(float* vec1, float* vec2, int length);

private:
	// ���̺�(����)
	int		 Labeling(IplImage* image, int nThreshold);

	// ����Ʈ �ʱ�ȭ
	void	 InitvPoint(int nWidth, int nHeight);
	void	 DeletevPoint();

	// ���̺� ��� ���
	void	 DetectLabelingRegion(int nLabelNumber, unsigned char *DataBuf, int nWidth, int nHeight);

	// ���̺�(���� �˰���)
	int		_Labeling(unsigned char *DataBuf, int nWidth, int nHeight, int nThreshold);

	// _Labling ���� ��� �Լ�
	int		__NRFIndNeighbor(unsigned char *DataBuf, int nWidth, int nHeight, int nPosX, int nPosY, int *StartX, int *StartY, int *EndX, int *EndY );
	int		__Area(unsigned char *DataBuf, int StartX, int StartY, int EndX, int EndY, int nWidth, int nLevel);

	/******************************************************************/

	// ����, ���� ũ�Ⱑ �񱳰� ���� ���� ���̺� ����
public:
	void	BlobSmallSizeConstraint(int nWidth, int nHeight);
private:
	void	_BlobSmallSizeConstraint(int nWidth, int nHeight, CvRect* rect, int* nRecNumber);

	// ����, ���� ũ�Ⱑ �񱳰� ���� ū ���̺� ����
public:
	void	BlobBigSizeConstraint(int nWidth, int nHeight);
private:
	void	_BlobBigSizeConstraint(int nWidth, int nHeight, CvRect* rect, int* nRecNumber);

protected:
	DECLARE_MESSAGE_MAP()

};


