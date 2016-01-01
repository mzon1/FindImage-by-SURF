// FilterDlg.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "MFC_OpenCV.h"
#include "FilterDlg.h"
#include "afxdialogex.h"


// CFilterDlg 대화 상자입니다.

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


// CFilterDlg 메시지 처리기입니다.


// 정성환 외 1명/컴퓨터 비전 실무 프로그래밍/홍릉과학출판사/2007/317p~320p
BOOL CFilterDlg::OnInitDialog(){
	CDialogEx::OnInitDialog();

	// TODO:  여기에 추가 초기화 작업을 추가합니다.

	// STEP 1 : Picture 컨트롤 크기 초기화  
	picture_height = 228; // Picture 컨트롤의 높이 기준  
	picture_width = 328; // Picture 컨트롤의 너비 기준(4의 배수) 

	// STEP 2 : 필터 마스크 초기화 

	// STEP 2-1 : Soften filter - 부드러운 효과를 냄. 
	float _mask1[] = {	1.0f/18.0f,  1.0f/18.0f, 1.0f/18.0f, 
						1.0f/18.0f, 10.0f/18.0f, 1.0f/18.0f, 
						1.0f/18.0f,  1.0f/18.0f, 1.0f/18.0f }; 
	memcpy( soften_mask, _mask1, sizeof(float)*9 ); 

	// STEP 2-2 : Enhance focus filter - 물체를 둘러싼 것을 초점을 맞춘 효과를 냄. 
	float _mask2[] = {	-1.0f/3.0f,  0.0f/3.0f, -1.0f/3.0f, 
						0.0f/3.0f,  7.0f/3.0f,  0.0f/3.0f, 
						-1.0f/3.0f,  0.0f/3.0f, -1.0f/3.0f }; 
	memcpy( ef_mask, _mask2, sizeof(float)*9 ); 

	// STEP 2-3 : Enhance detail filter - 물체를 둘러싼 것을 상세히 드러내는 효과를 냄. 
	float _mask3[] = {	0.0f/5.0f, -1.0f/5.0f,  0.0f/5.0f, 
						-1.0f/5.0f,  9.0f/5.0f, -1.0f/5.0f, 
						0.0f/5.0f, -1.0f/5.0f,  0.0f/5.0f }; 
	memcpy( ed_mask, _mask3, sizeof(float)*9 ); 

	// STEP 2-4 : Blur light filter - 블러링 단점인 잡음 미제거 보완 효과를 냄. 
	float _mask4[] = {	1.0f/14.0f, 1.0f/14.0f, 1.0f/14.0f, 
						2.0f/14.0f, 2.0f/14.0f, 2.0f/14.0f, 
						1.0f/14.0f, 1.0f/14.0f, 1.0f/14.0f }; 
	memcpy( bl_mask, _mask4, sizeof(float)*9 ); 

	// STEP 2-5 : Blur blending filter - 블러링 단점인 잡음 미제거 보완 효과를 냄. 
	float _mask5[] = {	1.0f/16.0f, 2.0f/16.0f, 1.0f/16.0f, 
						2.0f/16.0f, 4.0f/16.0f, 2.0f/16.0f, 
						1.0f/16.0f, 2.0f/16.0f, 1.0f/16.0f }; 
	memcpy( bb_mask, _mask5, sizeof(float)*9 ); 

	// STEP 3 : 콤보 박스에 항목 추가  
	CString strFilterList[] = { _T( "Soften filter" ), _T( "Enhance focus filter" ), _T( "Enhance detail filter" ), _T( "Blur light filter" ),_T( "Blur blending filter" )}; 

	int length = sizeof(strFilterList) / sizeof(CString); 
	for(int i = 0; i< length ; i++)
		m_combo_filter.AddString(strFilterList[i]);

	CMainFrame *pFrame = (CMainFrame *)AfxGetMainWnd();
	CChildFrame *pChild = (CChildFrame *)pFrame->GetActiveFrame();
	CMFC_OpenCVDoc *pDoc = (CMFC_OpenCVDoc *)pChild->GetActiveDocument(); 

	// STEP 5 : 원 영상을 복사 
	d_org_cvvImage.CopyOf( pDoc->m_CvvImage.GetImage(), pDoc->m_CvvImage.GetImage()->nChannels*8 ); 
	d_cvvImage.CopyOf( pDoc->m_CvvImage.GetImage(), pDoc->m_CvvImage.GetImage()->nChannels*8 ); 

	// STEP 6 : 원 영상을 비트맵으로 변환하기 위한 영상 크기 재조절 
	CDeiosImgUtil cUtil; 

	// (주의!) BITMAP의 특성 : 가로의 길이가 4의 배수가 아니면 찌그러짐 
	IplImage *image = d_org_cvvImage.GetImage(); 
	IplImage *resize_image = cUtil.Resize4Bitmap( picture_height, picture_width, image ); 

	HBITMAP hBitmap = cUtil.IplImage2Bitmap( resize_image ); 
	cvReleaseImage( &resize_image ); 

	// STEP 7 : 변환한 비트맵을 m_preview 즉, picture 컨트롤에 뿌려준다. 
	m_preview_image.SetBitmap( hBitmap ); 

	// STEP 8 : 화면 갱신 
	UpdateData( FALSE );  

	return TRUE;  // return TRUE unless you set the focus to a control
	// 예외: OCX 속성 페이지는 FALSE를 반환해야 합니다.
}


void CFilterDlg::OnDestroy()
{
	CDialogEx::OnDestroy();

	// TODO: 여기에 메시지 처리기 코드를 추가합니다.
}


// 정성환 외 1명/컴퓨터 비전 실무 프로그래밍/홍릉과학출판사/2007/328p~330p
void CFilterDlg::OnBnClickedPreviewButton()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	float kernel[9] = {0., }; 
	int kernel_height = 3; 
	int kernel_width = 3; 
	int kernel_length = kernel_height*kernel_width; 

	// STEP 1 : 콤보 박스에서 선택한 항목을 가져오기 
	CString str; 
	int nIndex = m_combo_filter.GetCurSel(); 
	if( nIndex == CB_ERR ) return; 
	m_combo_filter.GetLBText( nIndex, str ); 

	// STEP 2 : 콤보 박스에서 선택한 필터링 수행 하기 위한 커널 복사 
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

	// STEP 3 : 원 영상을 비트맵으로 변환하기 위한 영상 크기 재조절 
	CDeiosImgUtil cUtil; 
	IplImage *image = d_cvvImage.GetImage(); 
	IplImage *resize_image = cUtil.Resize4Bitmap( picture_height, picture_width, image ); 

	// STEP 4 : 공간 영역 필터링 수행 
	CFilter cFilter; 
	IplImage *filtered_image = cFilter.ApplyFilter2D( resize_image, kernel, kernel_height, kernel_width ); 
	IplImage *filtered_image2 =  cFilter.ApplyFilter2D( d_cvvImage.GetImage(), kernel, kernel_height, kernel_width ); 

	d_cvvImage.CopyOf(filtered_image2);

	// STEP 5 : 수행한 결과를 비트맵으로 변환. 
	HBITMAP hBitmap = cUtil.IplImage2Bitmap( filtered_image ); 
	//d_cvvImage.CopyOf( filtered_image, filtered_image->nChannels*8 ); 

	// STEP 6 : 변환한 비트맵을 m_preview 즉, picture 컨트롤에 뿌려줌. 
	m_preview_image.SetBitmap( hBitmap ); 
	m_preview_image.Invalidate(); 

	// STEP 7 : 메모리 해제 
	cvReleaseImage( &resize_image ); 
	cvReleaseImage( &filtered_image ); 
	cvReleaseImage( &filtered_image2 );

	// STEP 8 : 화면 갱신 
	UpdateData( FALSE ); 	
}


// 정성환 외 1명/컴퓨터 비전 실무 프로그래밍/홍릉과학출판사/2007/332p
void CFilterDlg::OnBnClickedInitialButton()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	// STEP 1 : 원 영상을 가져옴. 
	CDeiosImgUtil cUtil; 
	IplImage *image = d_org_cvvImage.GetImage(); 
	IplImage *resize_image = cUtil.Resize4Bitmap( picture_height, 
		picture_width, 
		image ); 

	// STEP 2 : 비트맵으로 변환. 
	HBITMAP hBitmap = cUtil.IplImage2Bitmap( resize_image ); 
	d_cvvImage.CopyOf( resize_image); 

	// STEP 3 : 변환한 비트맵을 m_preview 즉, picture 컨트롤에 뿌려줌. 
	m_preview_image.SetBitmap( hBitmap ); 
	m_preview_image.Invalidate(); 

	// STEP 4 : 메모리 해제 
	cvReleaseImage( &resize_image ); 

	// STEP 5 : 화면 갱신 
	UpdateData( FALSE );  
}


void CFilterDlg::OnBnClickedOk()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	CMainFrame *pFrame = (CMainFrame *)AfxGetMainWnd(); 
	CChildFrame *pChild = (CChildFrame *)pFrame->GetActiveFrame(); 
	CMFC_OpenCVDoc *pDoc = (CMFC_OpenCVDoc *)pChild->GetActiveDocument(); 

	pDoc->m_CvvImage.CopyOf( d_cvvImage ); 
	UpdateData(TRUE);
	CDialogEx::OnOK();
}
