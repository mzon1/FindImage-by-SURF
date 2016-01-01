
// MFC_OpenCV.cpp : ���� ���α׷��� ���� Ŭ���� ������ �����մϴ�.
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
	// ǥ�� ������ ���ʷ� �ϴ� ���� ����Դϴ�.
	ON_COMMAND(ID_FILE_NEW, &CWinAppEx::OnFileNew)
//	ON_COMMAND(ID_FILE_OPEN, &CWinAppEx::OnFileOpen)
	// ǥ�� �μ� ���� ����Դϴ�.
	ON_COMMAND(ID_FILE_PRINT_SETUP, &CWinAppEx::OnFilePrintSetup)
	ON_COMMAND(ID_FILE_OPEN, &CMFC_OpenCVApp::OnFileOpen)
	ON_COMMAND(ID_FILE_CLOSE_ALL, &CMFC_OpenCVApp::OnFileCloseAll)
	ON_COMMAND(ID_EDIT_PASTE, &CMFC_OpenCVApp::OnEditPaste)
	ON_UPDATE_COMMAND_UI(ID_EDIT_PASTE, &CMFC_OpenCVApp::OnUpdateEditPaste)
	ON_COMMAND(ID_CAM_VIEW, &CMFC_OpenCVApp::OnCamView)
END_MESSAGE_MAP()


// CMFC_OpenCVApp ����

CMFC_OpenCVApp::CMFC_OpenCVApp()
{
	m_bHiColorIcons = TRUE;

	// �ٽ� ���� ������ ����
	m_dwRestartManagerSupportFlags = AFX_RESTART_MANAGER_SUPPORT_ALL_ASPECTS;
#ifdef _MANAGED
	// ���� ���α׷��� ���� ��� ��Ÿ�� ������ ����Ͽ� ������ ���(/clr):
	//     1) �� �߰� ������ �ٽ� ���� ������ ������ ����� �۵��ϴ� �� �ʿ��մϴ�.
	//     2) ������Ʈ���� �����Ϸ��� System.Windows.Forms�� ���� ������ �߰��ؾ� �մϴ�.
	System::Windows::Forms::Application::SetUnhandledExceptionMode(System::Windows::Forms::UnhandledExceptionMode::ThrowException);
#endif

	// TODO: �Ʒ� ���� ���α׷� ID ���ڿ��� ���� ID ���ڿ��� �ٲٽʽÿ�(����).
	// ���ڿ��� ���� ����: CompanyName.ProductName.SubProduct.VersionInformation
	SetAppID(_T("MFC_OpenCV.AppID.NoVersion"));

	// TODO: ���⿡ ���� �ڵ带 �߰��մϴ�.
	// InitInstance�� ��� �߿��� �ʱ�ȭ �۾��� ��ġ�մϴ�.
}

// ������ CMFC_OpenCVApp ��ü�Դϴ�.

CMFC_OpenCVApp theApp;


// CMFC_OpenCVApp �ʱ�ȭ

BOOL CMFC_OpenCVApp::InitInstance()
{
	// ���� ���α׷� �Ŵ��佺Ʈ�� ComCtl32.dll ���� 6 �̻��� ����Ͽ� ���־� ��Ÿ����
	// ����ϵ��� �����ϴ� ���, Windows XP �󿡼� �ݵ�� InitCommonControlsEx()�� �ʿ��մϴ�. 
	// InitCommonControlsEx()�� ������� ������ â�� ���� �� �����ϴ�.
	INITCOMMONCONTROLSEX InitCtrls;
	InitCtrls.dwSize = sizeof(InitCtrls);
	// ���� ���α׷����� ����� ��� ���� ��Ʈ�� Ŭ������ �����ϵ���
	// �� �׸��� �����Ͻʽÿ�.
	InitCtrls.dwICC = ICC_WIN95_CLASSES;
	InitCommonControlsEx(&InitCtrls);

	CWinAppEx::InitInstance();


	EnableTaskbarInteraction(FALSE);

	// RichEdit ��Ʈ���� ����Ϸ���  AfxInitRichEdit2()�� �־�� �մϴ�.	
	// AfxInitRichEdit2();

	// ǥ�� �ʱ�ȭ
	// �̵� ����� ������� �ʰ� ���� ���� ������ ũ�⸦ ���̷���
	// �Ʒ����� �ʿ� ���� Ư�� �ʱ�ȭ
	// ��ƾ�� �����ؾ� �մϴ�.
	// �ش� ������ ����� ������Ʈ�� Ű�� �����Ͻʽÿ�.
	// TODO: �� ���ڿ��� ȸ�� �Ǵ� ������ �̸��� ����
	// ������ �������� �����ؾ� �մϴ�.
	SetRegistryKey(_T("���� ���� ���α׷� �����翡�� ������ ���� ���α׷�"));
	LoadStdProfileSettings(4);  // MRU�� �����Ͽ� ǥ�� INI ���� �ɼ��� �ε��մϴ�.


	InitContextMenuManager();

	InitKeyboardManager();

	InitTooltipManager();
	CMFCToolTipInfo ttParams;
	ttParams.m_bVislManagerTheme = TRUE;
	theApp.GetTooltipManager()->SetTooltipParams(AFX_TOOLTIP_TYPE_ALL,
		RUNTIME_CLASS(CMFCToolTipCtrl), &ttParams);

	// ���� ���α׷��� ���� ���ø��� ����մϴ�. ���� ���ø���
	//  ����, ������ â �� �� ������ ���� ������ �մϴ�.
	CMultiDocTemplate* pDocTemplate;
	pDocTemplate = new CMultiDocTemplate(IDR_MFC_OpenCVTYPE,
		RUNTIME_CLASS(CMFC_OpenCVDoc),
		RUNTIME_CLASS(CChildFrame), // ����� ���� MDI �ڽ� �������Դϴ�.
		RUNTIME_CLASS(CMFC_OpenCVView));
	if (!pDocTemplate)
		return FALSE;
	AddDocTemplate(pDocTemplate);

	// �� MDI ������ â�� ����ϴ�.
	CMainFrame* pMainFrame = new CMainFrame;
	if (!pMainFrame || !pMainFrame->LoadFrame(IDR_MAINFRAME))
	{
		delete pMainFrame;
		return FALSE;
	}
	m_pMainWnd = pMainFrame;
	// ���̻簡 ���� ��쿡�� DragAcceptFiles�� ȣ���մϴ�.
	//  MDI ���� ���α׷������� m_pMainWnd�� ������ �� �ٷ� �̷��� ȣ���� �߻��ؾ� �մϴ�.

	// ǥ�� �� ���, DDE, ���� ���⿡ ���� ������� ���� �м��մϴ�.
	CCommandLineInfo cmdInfo;
	ParseCommandLine(cmdInfo);

	if(cmdInfo.m_nShellCommand == CCommandLineInfo::FileNew){
		cmdInfo.m_nShellCommand = CCommandLineInfo::FileNothing;
	}

	// ����ٿ� ������ ����� ����ġ�մϴ�.
	// ���� ���α׷��� /RegServer, /Register, /Unregserver �Ǵ� /Unregister�� ���۵� ��� FALSE�� ��ȯ�մϴ�.
	if (!ProcessShellCommand(cmdInfo))
		return FALSE;

	pMainFrame->DragAcceptFiles();
	// �� â�� �ʱ�ȭ�Ǿ����Ƿ� �̸� ǥ���ϰ� ������Ʈ�մϴ�.
	pMainFrame->ShowWindow(m_nCmdShow);
	pMainFrame->UpdateWindow();

	return TRUE;
}

int CMFC_OpenCVApp::ExitInstance()
{
	//TODO: �߰��� �߰� ���ҽ��� ó���մϴ�.
	return CWinAppEx::ExitInstance();
}

// CMFC_OpenCVApp �޽��� ó����


// ���� ���α׷� ������ ���Ǵ� CAboutDlg ��ȭ �����Դϴ�.

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// ��ȭ ���� �������Դϴ�.
	enum { IDD = IDD_ABOUTBOX };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �����Դϴ�.

// �����Դϴ�.
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

// ��ȭ ���ڸ� �����ϱ� ���� ���� ���α׷� ����Դϴ�.
void CMFC_OpenCVApp::OnAppAbout()
{
	CAboutDlg aboutDlg;
	aboutDlg.DoModal();
}

// CMFC_OpenCVApp ����� ���� �ε�/���� �޼���

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

// CMFC_OpenCVApp �޽��� ó����


void CMFC_OpenCVApp::OnFileOpen()
{
	// TODO: ���⿡ ��� ó���� �ڵ带 �߰��մϴ�.
	CString strFilter = TEXT("JPEG files(*.jpg, *.jpeg, *.jpe) |  *.jpg; *.jpeg; *.jpe |\
							Portable Network Graphics file(*.png) | *.png |\
							Windows Bitmap files(*.bmp, *.dib) | *.bmp; *.dib |\
							Sun resters(*.sr, *.ras) | *.sr; *.ras |\
							TIFF files(*.tiff, *.tif) | *.tiff; *.tif |\
							�����ϴ� ��� ���� ���� | *.jpg; *.jpeg; *.jpe; *.png; *.bmp; *.dib; *.sr; *.ras; *.tiff; *.tif || ");
	CFileDialog dlgFile(TRUE, TEXT("jpg"), TEXT("*.jpg"), OFN_FILEMUSTEXIST | OFN_HIDEREADONLY | OFN_ALLOWMULTISELECT, strFilter);
	if(IDOK == dlgFile.DoModal()) {
		POSITION pos = dlgFile.GetStartPosition();
		while(pos)	OpenDocumentFile(dlgFile.GetNextPathName(pos));
	}
}


void CMFC_OpenCVApp::OnFileCloseAll()
{
	// TODO: ���⿡ ��� ó���� �ڵ带 �߰��մϴ�.
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
	// TODO: ���⿡ ��� ó���� �ڵ带 �߰��մϴ�.
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
	// TODO: ���⿡ ��� ������Ʈ UI ó���� �ڵ带 �߰��մϴ�.
	if( !IsClipboardFormatAvailable(CF_DIB) ) pCmdUI->Enable( FALSE ); 
}

/*	RGB �÷� ���� �и� - �޴� ��� ����2
	������ ����� �̹����� �޾� �� â���� �����ִ� �Լ�. */
void OpenByNewDocument(LPTSTR szTitle, IplImage * image)
{
	// �� â���� �� �̹����� ũ�⸦ ����մϴ�.
	CSize sizeTotal = CSize(image->width, image->height);

	// �� ��ť��Ʈ�� �����ϴ� �ڵ��Դϴ�.
	POSITION pos = ((CMFC_OpenCVApp *)AfxGetApp())->GetFirstDocTemplatePosition();    
	CDocTemplate *pTemplate = ((CMFC_OpenCVApp *)AfxGetApp())->GetNextDocTemplate(pos);

	CMFC_OpenCVDoc *pDoc = (CMFC_OpenCVDoc* )pTemplate->OpenDocumentFile(NULL);
	pos = pDoc->GetFirstViewPosition();
	CMFC_OpenCVView *pView = (CMFC_OpenCVView *)pDoc->GetNextView(pos);
	pView->SetScrollSizes(MM_TEXT, sizeTotal);
	pView->ResizeParentToFit(FALSE);

	//  �� ��° ���ڷ� �޾ƿ� �̹����� ���� ������� ������ �����մϴ�.
	pDoc->m_CvvImage.CopyOf(image,-1);

	if(NULL != szTitle){
		pDoc->SetTitle(szTitle);   
	}
}


//void CAboutDlg::OnCamView()
//{
//	// TODO: ���⿡ ��� ó���� �ڵ带 �߰��մϴ�.
//	CViewDlg dlg;
//	dlg.DoModal();
//}


void CMFC_OpenCVApp::OnCamView()
{	// TODO: ���⿡ ��� ó���� �ڵ带 �߰��մϴ�.
	static CViewDlg dlg;
	if( NULL == dlg.GetSafeHwnd()){
		dlg.Create(IDD_CamView);
	}
	dlg.ShowWindow(SW_SHOW);
}
