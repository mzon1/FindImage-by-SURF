// ViewDlg.cpp : ���� �����Դϴ�.
//

#include "stdafx.h"
#include "MFC_OpenCV.h"
#include "ViewDlg.h"
#include "afxdialogex.h"




// CViewDlg ��ȭ �����Դϴ�.

IMPLEMENT_DYNAMIC(CViewDlg, CDialogEx)

CViewDlg::CViewDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CViewDlg::IDD, pParent)
	, viewState(false)
	, m_bBinarize(false)
	, m_SURF(false)
	, m_MaxSurfPoint(0)
	, m_Number(0)
{

}

CViewDlg::~CViewDlg()
{
}

void CViewDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	//  DDX_Control(pDX, IDC_STATIC_VIEW, m_StaticView);
	DDX_Control(pDX, IDOK, m_btnOK);
	DDX_Control(pDX, IDC_STATIC_VIEW, m_sView);
	DDX_Control(pDX, IDLAB, m_Surf_able);
}


BEGIN_MESSAGE_MAP(CViewDlg, CDialogEx)
	ON_WM_TIMER()
	ON_WM_DESTROY()
//	ON_BN_CLICKED(IDC_BUTTON1, &CViewDlg::OnBnClickedButton1)
ON_BN_CLICKED(IDC_btnCapture, &CViewDlg::OnBnClickedbtncapture)
ON_BN_CLICKED(IDLAB, &CViewDlg::OnBnClickedLab)
END_MESSAGE_MAP()


// CViewDlg �޽��� ó�����Դϴ�.


BOOL CViewDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  ���⿡ �߰� �ʱ�ȭ �۾��� �߰��մϴ�.
	capture = cvCaptureFromCAM(0);
	image = 0;
	viewState = FALSE;
	for (int i = 0; i < IMAGENUM; i++)
	{
		m_SurfPoint[i] = 0;
	}

	return TRUE;  // return TRUE unless you set the focus to a control
	// ����: OCX �Ӽ� �������� FALSE�� ��ȯ�ؾ� �մϴ�.
}


void CViewDlg::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: ���⿡ �޽��� ó���� �ڵ带 �߰� ��/�Ǵ� �⺻���� ȣ���մϴ�.
	if(1 == nIDEvent){
		cvGrabFrame( capture );
		image = cvQueryFrame( capture );
		m_CvvImage.CopyOf(image);
		
		CRect viewSize;
		m_sView.GetClientRect(&viewSize);
		m_CvvImage.DrawToHDC( m_sView.GetDC()->GetSafeHdc() ,viewSize);

		//Invalidate(FALSE);
	}
	CDialogEx::OnTimer(nIDEvent);
}


void CViewDlg::OnOK()
{
	// TODO: ���⿡ Ư��ȭ�� �ڵ带 �߰� ��/�Ǵ� �⺻ Ŭ������ ȣ���մϴ�.
	if(viewState){
		KillTimer(1);
		viewState = FALSE;
		m_btnOK.SetWindowText(TEXT("START"));

	}else{
		SetTimer(1, (UINT)((1/(double)12)*1000),NULL);
		viewState = TRUE;
		m_btnOK.SetWindowText(TEXT("STOP"));
	}
}


void CViewDlg::OnDestroy()
{
	CDialogEx::OnDestroy();

	// TODO: ���⿡ �޽��� ó���� �ڵ带 �߰��մϴ�.
	if(NULL != capture) cvReleaseCapture(&capture);
	m_CvvImage.~CvvImage();
}


//void CViewDlg::OnBnClickedButton1()
//{
//	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
//}


void CViewDlg::OnBnClickedbtncapture()
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
	OpenByNewDocument(NULL, image);
}


void CViewDlg::OnBnClickedLab()
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
	if (m_bBinarize)
	{	
		m_Surf_able.SetWindowText(TEXT("SURF_ON"));
		IplImage* gray = cvCreateImage( cvSize(image->width, image->height), 8, 1 );
		cvCvtColor( image, gray, CV_BGR2GRAY );
		cvThreshold( gray, gray, 0.0, 255.0, CV_THRESH_BINARY | CV_THRESH_OTSU);

		// ���̺��� ���(255)�� ��ü�� �����Ƿ� ����ȭ�� ������ ������Ų��.
		cvThreshold( gray, gray, 1, 255, CV_THRESH_BINARY_INV );

		// ���̺� ��� ǥ�� ���� �̹��� ����
		//IplImage* labeled = cvCreateImage( cvSize( gray->width, gray->height ), 8, 3 );
		//cvCvtColor( gray, labeled, CV_GRAY2BGR );

		//SURF ��� �̹��� ����

		IplImage* surfimg = cvCreateImage( cvSize( gray->width, gray->height ), 8, 3 );
		cvCopy(image, surfimg);

		// ���̺�
		CBlobLabeling blob;
		blob.SetParam( gray, 100 );	// ���̺� �� �̹����� �ּ� �ȼ��� ���� ����

		blob.DoLabeling();

			
		int nMaxWidth	= gray->width  * 7 / 10;	// ���� ���� ��ü ũ���� 50% �̻��� ���̺��� ����
		int nMaxHeight	= gray->height * 7 / 10;	// ���� ���� ��ü ũ���� 50% �̻��� ���̺��� ����

		blob.BlobSmallSizeConstraint( 100, 100 );
		blob.BlobBigSizeConstraint( nMaxWidth, nMaxHeight );	

		for( int i=0; i < blob.m_nBlobs; i++ )
		{
			CvPoint	pt1 = cvPoint(	blob.m_recBlobs[i].x,				
				blob.m_recBlobs[i].y );
			CvPoint pt2 = cvPoint(	pt1.x + blob.m_recBlobs[i].width,	
				pt1.y + blob.m_recBlobs[i].height );

			cvSetImageROI(image, cvRect(pt1.x, pt1.y, blob.m_recBlobs[i].width,blob.m_recBlobs[i].height));

			// �� ���̺� ǥ��
			CvScalar color	= cvScalar( 0, 0, 255 );

			
			for (int j = 0; j < IMAGENUM; j++)
			{
				if (blob.m_Search_Surf[j] == FALSE)
				{
					m_SurfPoint[j] = blob.DoSurf(image, j);
				}
				else
				{
					m_SurfPoint[j] = 0;
				}
			}
			

			m_Number = 0;
			m_MaxSurfPoint = m_SurfPoint[0];
			for (int k = 0; k<IMAGENUM-1; k++)
			{
				if(m_MaxSurfPoint <m_SurfPoint[k+1])
				{
					m_MaxSurfPoint = m_SurfPoint[k+1];
					m_Number = k+1;
				}
			}

			blob.m_Search_Surf[m_Number] = TRUE;
			blob.textImage(surfimg, cvPoint(pt1.x+(blob.m_recBlobs[i].width)/2 ,pt1.y+(blob.m_recBlobs[i].height)/2), m_Number);

			cvDrawRect( surfimg, pt1, pt2, color, 2 );
		}
		cvResetImageROI(image);

		cvShowImage( "SURF", surfimg );
		//cvShowImage("GRAY", gray);
		//OpenByNewDocument(NULL, labeled);

		//cvReleaseImage( &labeled );
		cvReleaseImage( &surfimg );
		cvReleaseImage( &gray );
		m_bBinarize = false;
	}
	else
	{
		m_Surf_able.SetWindowText(TEXT("SURF_OFF"));
		for (int i = 0; i < IMAGENUM; i++)
		{
			m_SurfPoint[i] = 0;
		}
		m_MaxSurfPoint = 0;
		m_bBinarize = true;
		cvDestroyAllWindows();
	}	
}
