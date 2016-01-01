
// MFC_OpenCVView.cpp : CMFC_OpenCVView 클래스의 구현
//

#include "stdafx.h"
// SHARED_HANDLERS는 미리 보기, 축소판 그림 및 검색 필터 처리기를 구현하는 ATL 프로젝트에서 정의할 수 있으며
// 해당 프로젝트와 문서 코드를 공유하도록 해 줍니다.
#ifndef SHARED_HANDLERS
#include "MFC_OpenCV.h"
#endif

#include "MFC_OpenCVDoc.h"
#include "MFC_OpenCVView.h"
#include "FilterDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CMFC_OpenCVView

IMPLEMENT_DYNCREATE(CMFC_OpenCVView, CScrollView)

BEGIN_MESSAGE_MAP(CMFC_OpenCVView, CScrollView)
	// 표준 인쇄 명령입니다.
	ON_COMMAND(ID_FILE_PRINT, &CScrollView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, &CScrollView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, &CMFC_OpenCVView::OnFilePrintPreview)
	ON_WM_CONTEXTMENU()
	ON_WM_RBUTTONUP()
	ON_COMMAND(ID_FILE_SAVE_AS, &CMFC_OpenCVView::OnFileSaveAs)
	ON_COMMAND(ID_EDIT_COPY, &CMFC_OpenCVView::OnEditCopy)
//	ON_COMMAND(ID_COLOR_SPLIT_RGB, &CMFC_OpenCVView::OnColorSplitRgb)
ON_COMMAND(ID_FILTER2D, &CMFC_OpenCVView::OnFilter2d)
END_MESSAGE_MAP()

// CMFC_OpenCVView 생성/소멸

CMFC_OpenCVView::CMFC_OpenCVView()
{
	// TODO: 여기에 생성 코드를 추가합니다.

}

CMFC_OpenCVView::~CMFC_OpenCVView()
{
}

BOOL CMFC_OpenCVView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: CREATESTRUCT cs를 수정하여 여기에서
	//  Window 클래스 또는 스타일을 수정합니다.

	return CScrollView::PreCreateWindow(cs);
}

// CMFC_OpenCVView 그리기

void CMFC_OpenCVView::OnDraw(CDC* pDC)
{
	CMFC_OpenCVDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;

	// TODO: 여기에 원시 데이터에 대한 그리기 코드를 추가합니다.
	if(pDoc->m_CvvImage.GetImage()){
		int height = pDoc->m_CvvImage.Height();
		int width = pDoc->m_CvvImage.Width();
		CRect rect = CRect(0,0, width, height);
		pDoc->m_CvvImage.DrawToHDC(pDC->GetSafeHdc(), &rect);
	}

		ResizeParentToFit(TRUE);
}

void CMFC_OpenCVView::OnInitialUpdate()
{
	CMFC_OpenCVDoc *pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	CScrollView::OnInitialUpdate();

	CSize sizeTotal;
	// TODO: 이 뷰의 전체 크기를 계산합니다.
	if(pDoc->m_CvvImage.GetImage()){
		int height = pDoc->m_CvvImage.Height();
		int width = pDoc->m_CvvImage.Width();
		sizeTotal = CSize(width, height);
	}else{
		sizeTotal.cx = sizeTotal.cy = 100;
	}
	SetScrollSizes(MM_TEXT, sizeTotal);
	ResizeParentToFit(TRUE);
}


// CMFC_OpenCVView 인쇄


void CMFC_OpenCVView::OnFilePrintPreview()
{
#ifndef SHARED_HANDLERS
	AFXPrintPreview(this);
#endif
}

BOOL CMFC_OpenCVView::OnPreparePrinting(CPrintInfo* pInfo)
{
	// 기본적인 준비
	return DoPreparePrinting(pInfo);
}

void CMFC_OpenCVView::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: 인쇄하기 전에 추가 초기화 작업을 추가합니다.
}

void CMFC_OpenCVView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: 인쇄 후 정리 작업을 추가합니다.
}

void CMFC_OpenCVView::OnRButtonUp(UINT /* nFlags */, CPoint point)
{
	ClientToScreen(&point);
	OnContextMenu(this, point);
}

void CMFC_OpenCVView::OnContextMenu(CWnd* /* pWnd */, CPoint point)
{
#ifndef SHARED_HANDLERS
	theApp.GetContextMenuManager()->ShowPopupMenu(IDR_POPUP_EDIT, point.x, point.y, this, TRUE);
#endif
}


// CMFC_OpenCVView 진단

#ifdef _DEBUG
void CMFC_OpenCVView::AssertValid() const
{
	CScrollView::AssertValid();
}

void CMFC_OpenCVView::Dump(CDumpContext& dc) const
{
	CScrollView::Dump(dc);
}

CMFC_OpenCVDoc* CMFC_OpenCVView::GetDocument() const // 디버그되지 않은 버전은 인라인으로 지정됩니다.
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CMFC_OpenCVDoc)));
	return (CMFC_OpenCVDoc*)m_pDocument;
}
#endif //_DEBUG


// CMFC_OpenCVView 메시지 처리기


void CMFC_OpenCVView::OnFileSaveAs()
{
	// TODO: 여기에 명령 처리기 코드를 추가합니다.
	CMFC_OpenCVDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	
	CString strFilter = TEXT("JPEG files(*.jpg, *.jpeg, *.jpe) |  *.jpg; *.jpeg; *.jpe |\
							Portable Network Graphics file(*.png) | *.png |\
							Windows Bitmap files(*.bmp, *.dib) | *.bmp; *.dib |\
							Sun resters(*.sr, *.ras) | *.sr; *.ras |\
							TIFF files(*.tiff, *.tif) | *.tiff; *.tif |\
							지원하는 모든 영상 파일 | *.jpg; *.jpeg; *.jpe; *.png; *.bmp; *.dib; *.sr; *.ras; *.tiff; *.tif || ");
	CFileDialog dlgFile(FALSE, TEXT("jpg"), NULL, OFN_OVERWRITEPROMPT, strFilter, this);

	if(IDOK == dlgFile.DoModal()) {
		pDoc->OnSaveDocument(dlgFile.GetPathName());
	}
}


void CMFC_OpenCVView::OnEditCopy()
{
	// TODO: 여기에 명령 처리기 코드를 추가합니다.
	CMFC_OpenCVDoc* pDoc = GetDocument();   
	ASSERT_VALID(pDoc);
	pDoc->CopyClipBoard( pDoc->m_CvvImage.GetImage() );  
}


void CMFC_OpenCVView::OnFilter2d()
{
	// TODO: 여기에 명령 처리기 코드를 추가합니다.
	CFilterDlg dlg;
	dlg.DoModal();

	Invalidate(FALSE);
}
