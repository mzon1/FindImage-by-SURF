// FilterDlg.cpp : ���� �����Դϴ�.
//

#include "stdafx.h"
#include "MFC_OpenCV.h"
#include "FilterDlg.h"
#include "afxdialogex.h"


// CFilterDlg ��ȭ �����Դϴ�.

IMPLEMENT_DYNAMIC(CFilterDlg, CDialogEx)

CFilterDlg::CFilterDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CFilterDlg::IDD, pParent)
{

}

CFilterDlg::~CFilterDlg()
{
}

void CFilterDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_COMBO_FILTER_TYPE, m_combo_filter);
	DDX_Control(pDX, IDC_PREVIEW_IMAGE, m_preview_image);
}


BEGIN_MESSAGE_MAP(CFilterDlg, CDialogEx)
	ON_WM_DESTROY()
	ON_BN_CLICKED(IDC_PREVIEW_BUTTON, &CFilterDlg::OnBnClickedPreviewButton)
	ON_BN_CLICKED(IDC_INITIAL_BUTTON, &CFilterDlg::OnBnClickedInitialButton)
	ON_BN_CLICKED(IDOK, &CFilterDlg::OnBnClickedOk)
END_MESSAGE_MAP()


// CFilterDlg �޽��� ó�����Դϴ�.


// ����ȯ �� 1��/��ǻ�� ���� �ǹ� ���α׷���/ȫ���������ǻ�/2007/317p~320p
BOOL CFilterDlg::OnInitDialog(){
	CDialogEx::OnInitDialog();

	// TODO:  ���⿡ �߰� �ʱ�ȭ �۾��� �߰��մϴ�.

	// STEP 1 : Picture ��Ʈ�� ũ�� �ʱ�ȭ  
	picture_height = 228; // Picture ��Ʈ���� ���� ����  
	picture_width = 328; // Picture ��Ʈ���� �ʺ� ����(4�� ���) 

	// STEP 2 : ���� ����ũ �ʱ�ȭ 

	// STEP 2-1 : Soften filter - �ε巯�� ȿ���� ��. 
	float _mask1[] = {	1.0f/18.0f,  1.0f/18.0f, 1.0f/18.0f, 
						1.0f/18.0f, 10.0f/18.0f, 1.0f/18.0f, 
						1.0f/18.0f,  1.0f/18.0f, 1.0f/18.0f }; 
	memcpy( soften_mask, _mask1, sizeof(float)*9 ); 

	// STEP 2-2 : Enhance focus filter - ��ü�� �ѷ��� ���� ������ ���� ȿ���� ��. 
	float _mask2[] = {	-1.0f/3.0f,  0.0f/3.0f, -1.0f/3.0f, 
						0.0f/3.0f,  7.0f/3.0f,  0.0f/3.0f, 
						-1.0f/3.0f,  0.0f/3.0f, -1.0f/3.0f }; 
	memcpy( ef_mask, _mask2, sizeof(float)*9 ); 

	// STEP 2-3 : Enhance detail filter - ��ü�� �ѷ��� ���� ���� �巯���� ȿ���� ��. 
	float _mask3[] = {	0.0f/5.0f, -1.0f/5.0f,  0.0f/5.0f, 
						-1.0f/5.0f,  9.0f/5.0f, -1.0f/5.0f, 
						0.0f/5.0f, -1.0f/5.0f,  0.0f/5.0f }; 
	memcpy( ed_mask, _mask3, sizeof(float)*9 ); 

	// STEP 2-4 : Blur light filter - ���� ������ ���� ������ ���� ȿ���� ��. 
	float _mask4[] = {	1.0f/14.0f, 1.0f/14.0f, 1.0f/14.0f, 
						2.0f/14.0f, 2.0f/14.0f, 2.0f/14.0f, 
						1.0f/14.0f, 1.0f/14.0f, 1.0f/14.0f }; 
	memcpy( bl_mask, _mask4, sizeof(float)*9 ); 

	// STEP 2-5 : Blur blending filter - ���� ������ ���� ������ ���� ȿ���� ��. 
	float _mask5[] = {	1.0f/16.0f, 2.0f/16.0f, 1.0f/16.0f, 
						2.0f/16.0f, 4.0f/16.0f, 2.0f/16.0f, 
						1.0f/16.0f, 2.0f/16.0f, 1.0f/16.0f }; 
	memcpy( bb_mask, _mask5, sizeof(float)*9 ); 

	// STEP 3 : �޺� �ڽ��� �׸� �߰�  
	CString strFilterList[] = { _T( "Soften filter" ), _T( "Enhance focus filter" ), _T( "Enhance detail filter" ), _T( "Blur light filter" ),_T( "Blur blending filter" )}; 

	int length = sizeof(strFilterList) / sizeof(CString); 
	for(int i = 0; i< length ; i++)
		m_combo_filter.AddString(strFilterList[i]);

	CMainFrame *pFrame = (CMainFrame *)AfxGetMainWnd();
	CChildFrame *pChild = (CChildFrame *)pFrame->GetActiveFrame();
	CMFC_OpenCVDoc *pDoc = (CMFC_OpenCVDoc *)pChild->GetActiveDocument(); 

	// STEP 5 : �� ������ ���� 
	d_org_cvvImage.CopyOf( pDoc->m_CvvImage.GetImage(), pDoc->m_CvvImage.GetImage()->nChannels*8 ); 
	d_cvvImage.CopyOf( pDoc->m_CvvImage.GetImage(), pDoc->m_CvvImage.GetImage()->nChannels*8 ); 

	// STEP 6 : �� ������ ��Ʈ������ ��ȯ�ϱ� ���� ���� ũ�� ������ 
	CDeiosImgUtil cUtil; 

	// (����!) BITMAP�� Ư�� : ������ ���̰� 4�� ����� �ƴϸ� ��׷��� 
	IplImage *image = d_org_cvvImage.GetImage(); 
	IplImage *resize_image = cUtil.Resize4Bitmap( picture_height, picture_width, image ); 

	HBITMAP hBitmap = cUtil.IplImage2Bitmap( resize_image ); 
	cvReleaseImage( &resize_image ); 

	// STEP 7 : ��ȯ�� ��Ʈ���� m_preview ��, picture ��Ʈ�ѿ� �ѷ��ش�. 
	m_preview_image.SetBitmap( hBitmap ); 

	// STEP 8 : ȭ�� ���� 
	UpdateData( FALSE );  

	return TRUE;  // return TRUE unless you set the focus to a control
	// ����: OCX �Ӽ� �������� FALSE�� ��ȯ�ؾ� �մϴ�.
}


void CFilterDlg::OnDestroy()
{
	CDialogEx::OnDestroy();

	// TODO: ���⿡ �޽��� ó���� �ڵ带 �߰��մϴ�.
}


// ����ȯ �� 1��/��ǻ�� ���� �ǹ� ���α׷���/ȫ���������ǻ�/2007/328p~330p
void CFilterDlg::OnBnClickedPreviewButton()
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
	float kernel[9] = {0., }; 
	int kernel_height = 3; 
	int kernel_width = 3; 
	int kernel_length = kernel_height*kernel_width; 

	// STEP 1 : �޺� �ڽ����� ������ �׸��� �������� 
	CString str; 
	int nIndex = m_combo_filter.GetCurSel(); 
	if( nIndex == CB_ERR ) return; 
	m_combo_filter.GetLBText( nIndex, str ); 

	// STEP 2 : �޺� �ڽ����� ������ ���͸� ���� �ϱ� ���� Ŀ�� ���� 
	if( str == "Soften filter" ) 
		memcpy( kernel, soften_mask, sizeof(float)*kernel_length ); 
	else if( str == "Enhance focus filter" ) 
		memcpy( kernel, ef_mask, sizeof(float)*kernel_length ); 
	else if( str == "Enhance detail filter" ) 
		memcpy( kernel, ed_mask, sizeof(float)*kernel_length ); 
	else if( str == "Blur light filter" ) 
		memcpy( kernel, bl_mask, sizeof(float)*kernel_length ); 
	else if( str == "Blur blending filter" ) 
		memcpy( kernel, bb_mask, sizeof(float)*kernel_length ); 

	// STEP 3 : �� ������ ��Ʈ������ ��ȯ�ϱ� ���� ���� ũ�� ������ 
	CDeiosImgUtil cUtil; 
	IplImage *image = d_cvvImage.GetImage(); 
	IplImage *resize_image = cUtil.Resize4Bitmap( picture_height, picture_width, image ); 

	// STEP 4 : ���� ���� ���͸� ���� 
	CFilter cFilter; 
	IplImage *filtered_image = cFilter.ApplyFilter2D( resize_image, kernel, kernel_height, kernel_width ); 
	IplImage *filtered_image2 =  cFilter.ApplyFilter2D( d_cvvImage.GetImage(), kernel, kernel_height, kernel_width ); 

	d_cvvImage.CopyOf(filtered_image2);

	// STEP 5 : ������ ����� ��Ʈ������ ��ȯ. 
	HBITMAP hBitmap = cUtil.IplImage2Bitmap( filtered_image ); 
	//d_cvvImage.CopyOf( filtered_image, filtered_image->nChannels*8 ); 

	// STEP 6 : ��ȯ�� ��Ʈ���� m_preview ��, picture ��Ʈ�ѿ� �ѷ���. 
	m_preview_image.SetBitmap( hBitmap ); 
	m_preview_image.Invalidate(); 

	// STEP 7 : �޸� ���� 
	cvReleaseImage( &resize_image ); 
	cvReleaseImage( &filtered_image ); 
	cvReleaseImage( &filtered_image2 );

	// STEP 8 : ȭ�� ���� 
	UpdateData( FALSE ); 	
}


// ����ȯ �� 1��/��ǻ�� ���� �ǹ� ���α׷���/ȫ���������ǻ�/2007/332p
void CFilterDlg::OnBnClickedInitialButton()
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
	// STEP 1 : �� ������ ������. 
	CDeiosImgUtil cUtil; 
	IplImage *image = d_org_cvvImage.GetImage(); 
	IplImage *resize_image = cUtil.Resize4Bitmap( picture_height, 
		picture_width, 
		image ); 

	// STEP 2 : ��Ʈ������ ��ȯ. 
	HBITMAP hBitmap = cUtil.IplImage2Bitmap( resize_image ); 
	d_cvvImage.CopyOf( resize_image); 

	// STEP 3 : ��ȯ�� ��Ʈ���� m_preview ��, picture ��Ʈ�ѿ� �ѷ���. 
	m_preview_image.SetBitmap( hBitmap ); 
	m_preview_image.Invalidate(); 

	// STEP 4 : �޸� ���� 
	cvReleaseImage( &resize_image ); 

	// STEP 5 : ȭ�� ���� 
	UpdateData( FALSE );  
}


void CFilterDlg::OnBnClickedOk()
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
	CMainFrame *pFrame = (CMainFrame *)AfxGetMainWnd(); 
	CChildFrame *pChild = (CChildFrame *)pFrame->GetActiveFrame(); 
	CMFC_OpenCVDoc *pDoc = (CMFC_OpenCVDoc *)pChild->GetActiveDocument(); 

	pDoc->m_CvvImage.CopyOf( d_cvvImage ); 
	UpdateData(TRUE);
	CDialogEx::OnOK();
}
