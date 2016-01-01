
// MFC_OpenCV.cpp : 응용 프로그램에 대한 클래스 동작을 정의합니다.
//

#include "stdafx.h"
#include "afxwinappex.h"
#include "afxdialogex.h"
#include "MFC_OpenCV.h"
#include "MainFrm.h"

#include "ChildFrm.h"
#include "MFC_OpenCVDoc.h"
#include "MFC_OpenCVView.h"
#include "ViewDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CMFC_OpenCVApp

BEGIN_MESSAGE_MAP(CMFC_OpenCVApp, CWinAppEx)
	ON_COMMAND(ID_APP_ABOUT, &CMFC_OpenCVApp::OnAppAbout)
	// 표준 파일을 기초로 하는 문서 명령입니다.
	ON_COMMAND(ID_FILE_NEW, &CWinAppEx::OnFileNew)
//	ON_COMMAND(ID_FILE_OPEN, &CWinAppEx::OnFileOpen)
	// 표준 인쇄 설정 명령입니다.
	ON_COMMAND(ID_FILE_PRINT_SETUP, &CWinAppEx::OnFilePrintSetup)
	ON_COMMAND(ID_FILE_OPEN, &CMFC_OpenCVApp::OnFileOpen)
	ON_COMMAND(ID_FILE_CLOSE_ALL, &CMFC_OpenCVApp::OnFileCloseAll)
	ON_COMMAND(ID_EDIT_PASTE, &CMFC_OpenCVApp::OnEditPaste)
	ON_UPDATE_COMMAND_UI(ID_EDIT_PASTE, &CMFC_OpenCVApp::OnUpdateEditPaste)
	ON_COMMAND(ID_CAM_VIEW, &CMFC_OpenCVApp::OnCamView)
END_MESSAGE_MAP()


// CMFC_OpenCVApp 생성

CMFC_OpenCVApp::CMFC_OpenCVApp()
{
	m_bHiColorIcons = TRUE;

	// 다시 시작 관리자 지원
	m_dwRestartManagerSupportFlags = AFX_RESTART_MANAGER_SUPPORT_ALL_ASPECTS;
#ifdef _MANAGED
	// 응용 프로그램을 공용 언어 런타임 지원을 사용하여 빌드한 경우(/clr):
	//     1) 이 추가 설정은 다시 시작 관리자 지원이 제대로 작동하는 데 필요합니다.
	//     2) 프로젝트에서 빌드하려면 System.Windows.Forms에 대한 참조를 추가해야 합니다.
	System::Windows::Forms::Application::SetUnhandledExceptionMode(System::Windows::Forms::UnhandledExceptionMode::ThrowException);
#endif

	// TODO: 아래 응용 프로그램 ID 문자열을 고유 ID 문자열로 바꾸십시오(권장).
	// 문자열에 대한 서식: CompanyName.ProductName.SubProduct.VersionInformation
	SetAppID(_T("MFC_OpenCV.AppID.NoVersion"));

	// TODO: 여기에 생성 코드를 추가합니다.
	// InitInstance에 모든 중요한 초기화 작업을 배치합니다.
}

// 유일한 CMFC_OpenCVApp 개체입니다.

CMFC_OpenCVApp theApp;


// CMFC_OpenCVApp 초기화

BOOL CMFC_OpenCVApp::InitInstance()
{
	// 응용 프로그램 매니페스트가 ComCtl32.dll 버전 6 이상을 사용하여 비주얼 스타일을
	// 사용하도록 지정하는 경우, Windows XP 상에서 반드시 InitCommonControlsEx()가 필요합니다. 
	// InitCommonControlsEx()를 사용하지 않으면 창을 만들 수 없습니다.
	INITCOMMONCONTROLSEX InitCtrls;
	InitCtrls.dwSize = sizeof(InitCtrls);
	// 응용 프로그램에서 사용할 모든 공용 컨트롤 클래스를 포함하도록
	// 이 항목을 설정하십시오.
	InitCtrls.dwICC = ICC_WIN95_CLASSES;
	InitCommonControlsEx(&InitCtrls);

	CWinAppEx::InitInstance();


	EnableTaskbarInteraction(FALSE);

	// RichEdit 컨트롤을 사용하려면  AfxInitRichEdit2()가 있어야 합니다.	
	// AfxInitRichEdit2();

	// 표준 초기화
	// 이들 기능을 사용하지 않고 최종 실행 파일의 크기를 줄이려면
	// 아래에서 필요 없는 특정 초기화
	// 루틴을 제거해야 합니다.
	// 해당 설정이 저장된 레지스트리 키를 변경하십시오.
	// TODO: 이 문자열을 회사 또는 조직의 이름과 같은
	// 적절한 내용으로 수정해야 합니다.
	SetRegistryKey(_T("로컬 응용 프로그램 마법사에서 생성된 응용 프로그램"));
	LoadStdProfileSettings(4);  // MRU를 포함하여 표준 INI 파일 옵션을 로드합니다.


	InitContextMenuManager();

	InitKeyboardManager();

	InitTooltipManager();
	CMFCToolTipInfo ttParams;
	ttParams.m_bVislManagerTheme = TRUE;
	theApp.GetTooltipManager()->SetTooltipParams(AFX_TOOLTIP_TYPE_ALL,
		RUNTIME_CLASS(CMFCToolTipCtrl), &ttParams);

	// 응용 프로그램의 문서 템플릿을 등록합니다. 문서 템플릿은
	//  문서, 프레임 창 및 뷰 사이의 연결 역할을 합니다.
	CMultiDocTemplate* pDocTemplate;
	pDocTemplate = new CMultiDocTemplate(IDR_MFC_OpenCVTYPE,
		RUNTIME_CLASS(CMFC_OpenCVDoc),
		RUNTIME_CLASS(CChildFrame), // 사용자 지정 MDI 자식 프레임입니다.
		RUNTIME_CLASS(CMFC_OpenCVView));
	if (!pDocTemplate)
		return FALSE;
	AddDocTemplate(pDocTemplate);

	// 주 MDI 프레임 창을 만듭니다.
	CMainFrame* pMainFrame = new CMainFrame;
	if (!pMainFrame || !pMainFrame->LoadFrame(IDR_MAINFRAME))
	{
		delete pMainFrame;
		return FALSE;
	}
	m_pMainWnd = pMainFrame;
	// 접미사가 있을 경우에만 DragAcceptFiles를 호출합니다.
	//  MDI 응용 프로그램에서는 m_pMainWnd를 설정한 후 바로 이러한 호출이 발생해야 합니다.

	// 표준 셸 명령, DDE, 파일 열기에 대한 명령줄을 구문 분석합니다.
	CCommandLineInfo cmdInfo;
	ParseCommandLine(cmdInfo);

	if(cmdInfo.m_nShellCommand == CCommandLineInfo::FileNew){
		cmdInfo.m_nShellCommand = CCommandLineInfo::FileNothing;
	}

	// 명령줄에 지정된 명령을 디스패치합니다.
	// 응용 프로그램이 /RegServer, /Register, /Unregserver 또는 /Unregister로 시작된 경우 FALSE를 반환합니다.
	if (!ProcessShellCommand(cmdInfo))
		return FALSE;

	pMainFrame->DragAcceptFiles();
	// 주 창이 초기화되었으므로 이를 표시하고 업데이트합니다.
	pMainFrame->ShowWindow(m_nCmdShow);
	pMainFrame->UpdateWindow();

	return TRUE;
}

int CMFC_OpenCVApp::ExitInstance()
{
	//TODO: 추가한 추가 리소스를 처리합니다.
	return CWinAppEx::ExitInstance();
}

// CMFC_OpenCVApp 메시지 처리기


// 응용 프로그램 정보에 사용되는 CAboutDlg 대화 상자입니다.

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// 대화 상자 데이터입니다.
	enum { IDD = IDD_ABOUTBOX };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

// 구현입니다.
protected:
	DECLARE_MESSAGE_MAP()
public:
//	afx_msg void OnCamView();
};

CAboutDlg::CAboutDlg() : CDialogEx(CAboutDlg::IDD)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
//	ON_COMMAND(ID_CAM_VIEW, &CAboutDlg::OnCamView)
END_MESSAGE_MAP()

// 대화 상자를 실행하기 위한 응용 프로그램 명령입니다.
void CMFC_OpenCVApp::OnAppAbout()
{
	CAboutDlg aboutDlg;
	aboutDlg.DoModal();
}

// CMFC_OpenCVApp 사용자 지정 로드/저장 메서드

void CMFC_OpenCVApp::PreLoadState()
{
	BOOL bNameValid;
	CString strName;
	bNameValid = strName.LoadString(IDS_EDIT_MENU);
	ASSERT(bNameValid);
	GetContextMenuManager()->AddMenu(strName, IDR_POPUP_EDIT);
}

void CMFC_OpenCVApp::LoadCustomState()
{
}

void CMFC_OpenCVApp::SaveCustomState()
{
}

// CMFC_OpenCVApp 메시지 처리기


void CMFC_OpenCVApp::OnFileOpen()
{
	// TODO: 여기에 명령 처리기 코드를 추가합니다.
	CString strFilter = TEXT("JPEG files(*.jpg, *.jpeg, *.jpe) |  *.jpg; *.jpeg; *.jpe |\
							Portable Network Graphics file(*.png) | *.png |\
							Windows Bitmap files(*.bmp, *.dib) | *.bmp; *.dib |\
							Sun resters(*.sr, *.ras) | *.sr; *.ras |\
							TIFF files(*.tiff, *.tif) | *.tiff; *.tif |\
							지원하는 모든 영상 파일 | *.jpg; *.jpeg; *.jpe; *.png; *.bmp; *.dib; *.sr; *.ras; *.tiff; *.tif || ");
	CFileDialog dlgFile(TRUE, TEXT("jpg"), TEXT("*.jpg"), OFN_FILEMUSTEXIST | OFN_HIDEREADONLY | OFN_ALLOWMULTISELECT, strFilter);
	if(IDOK == dlgFile.DoModal()) {
		POSITION pos = dlgFile.GetStartPosition();
		while(pos)	OpenDocumentFile(dlgFile.GetNextPathName(pos));
	}
}


void CMFC_OpenCVApp::OnFileCloseAll()
{
	// TODO: 여기에 명령 처리기 코드를 추가합니다.
	POSITION pos;
	CDocTemplate* pTemp;

	pos = GetFirstDocTemplatePosition();
	while(NULL != pos){
		pTemp = GetNextDocTemplate(pos);
		pTemp->CloseAllDocuments(FALSE);
	}
}


void CMFC_OpenCVApp::OnEditPaste()
{
	// TODO: 여기에 명령 처리기 코드를 추가합니다.
	POSITION pos = GetFirstDocTemplatePosition();   
	CDocTemplate *pTemplate = GetNextDocTemplate(pos);   
	CMFC_OpenCVDoc *pDoc = (CMFC_OpenCVDoc* )pTemplate->OpenDocumentFile(NULL);    
   
	if( pDoc ){   
		HANDLE hDIB = NULL;   
		if (::OpenClipboard(AfxGetMainWnd()->GetSafeHwnd())) hDIB = ::GetClipboardData(CF_DIB);   
   
		if( hDIB ){   
			IplImage *image = CreateFromHandle( hDIB );   
		   
			pDoc->m_CvvImage.CopyOf( image, image->nChannels*8 );   
   
			POSITION pos = pDoc->GetFirstViewPosition();   
			CMFC_OpenCVView *pView = (CMFC_OpenCVView *)pDoc->GetNextView(pos);   
   
			CSize sizeTotal = CSize(pDoc->m_CvvImage.Width(), pDoc->m_CvvImage.Height());   
   
			pView->SetScrollSizes(MM_TEXT, sizeTotal);   
			pView->ResizeParentToFit(FALSE);   
		}   
   
		::CloseClipboard();   
	} 
}


IplImage * CMFC_OpenCVApp::CreateFromHandle(HANDLE hDIB)
{
	LPSTR pDIB = (LPSTR)::GlobalLock((HGLOBAL)hDIB);     
	BITMAPINFOHEADER bmpHeader;    
	memcpy( &bmpHeader, pDIB, sizeof(BITMAPINFOHEADER) );     
	   
	int height = bmpHeader.biHeight;   
	int width = bmpHeader.biWidth;   
	int nChannels = bmpHeader.biBitCount/8;   
	int bmpDataSize = bmpHeader.biSizeImage;   
	char *imageData = (char *)calloc( bmpDataSize, sizeof(char) );   
   
	if(8 == bmpHeader.biBitCount) 
		memcpy( imageData, pDIB+sizeof(BITMAPINFOHEADER)+sizeof(RGBQUAD)*256,    
				bmpDataSize );
	else if(24 == bmpHeader.biBitCount) 
		memcpy( imageData, pDIB+sizeof(BITMAPINFOHEADER), bmpDataSize );   

	IplImage *flip_image = cvCreateImage(cvSize(width,height), IPL_DEPTH_8U, nChannels);   
	memcpy( flip_image->imageData, imageData, bmpDataSize );   
  
	IplImage *recover_image = cvCloneImage( flip_image );   
	cvFlip( flip_image, recover_image, 0 );   
   
	::GlobalUnlock((HGLOBAL)hDIB);     

	free(imageData);   
	cvReleaseImage( &flip_image );   
   
	return recover_image;   
}


void CMFC_OpenCVApp::OnUpdateEditPaste(CCmdUI *pCmdUI)
{
	// TODO: 여기에 명령 업데이트 UI 처리기 코드를 추가합니다.
	if( !IsClipboardFormatAvailable(CF_DIB) ) pCmdUI->Enable( FALSE ); 
}

/*	RGB 컬러 공간 분리 - 메뉴 기반 구현2
	문서의 제목과 이미지를 받아 새 창으로 열어주는 함수. */
void OpenByNewDocument(LPTSTR szTitle, IplImage * image)
{
	// 새 창에서 열 이미지의 크기를 계산합니다.
	CSize sizeTotal = CSize(image->width, image->height);

	// 새 도큐먼트를 생성하는 코드입니다.
	POSITION pos = ((CMFC_OpenCVApp *)AfxGetApp())->GetFirstDocTemplatePosition();    
	CDocTemplate *pTemplate = ((CMFC_OpenCVApp *)AfxGetApp())->GetNextDocTemplate(pos);

	CMFC_OpenCVDoc *pDoc = (CMFC_OpenCVDoc* )pTemplate->OpenDocumentFile(NULL);
	pos = pDoc->GetFirstViewPosition();
	CMFC_OpenCVView *pView = (CMFC_OpenCVView *)pDoc->GetNextView(pos);
	pView->SetScrollSizes(MM_TEXT, sizeTotal);
	pView->ResizeParentToFit(FALSE);

	//  두 번째 인자로 받아온 이미지를 새로 만들어진 문서에 적용합니다.
	pDoc->m_CvvImage.CopyOf(image,-1);

	if(NULL != szTitle){
		pDoc->SetTitle(szTitle);   
	}
}


//void CAboutDlg::OnCamView()
//{
//	// TODO: 여기에 명령 처리기 코드를 추가합니다.
//	CViewDlg dlg;
//	dlg.DoModal();
//}


void CMFC_OpenCVApp::OnCamView()
{	// TODO: 여기에 명령 처리기 코드를 추가합니다.
	static CViewDlg dlg;
	if( NULL == dlg.GetSafeHwnd()){
		dlg.Create(IDD_CamView);
	}
	dlg.ShowWindow(SW_SHOW);
}
