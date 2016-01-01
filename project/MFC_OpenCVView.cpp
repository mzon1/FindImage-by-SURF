
// MFC_OpenCVView.cpp : CMFC_OpenCVView Ŭ������ ����
//

#include "stdafx.h"
// SHARED_HANDLERS�� �̸� ����, ����� �׸� �� �˻� ���� ó���⸦ �����ϴ� ATL ������Ʈ���� ������ �� ������
// �ش� ������Ʈ�� ���� �ڵ带 �����ϵ��� �� �ݴϴ�.
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
	// ǥ�� �μ� ����Դϴ�.
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

// CMFC_OpenCVView ����/�Ҹ�

CMFC_OpenCVView::CMFC_OpenCVView()
{
	// TODO: ���⿡ ���� �ڵ带 �߰��մϴ�.

}

CMFC_OpenCVView::~CMFC_OpenCVView()
{
}

BOOL CMFC_OpenCVView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: CREATESTRUCT cs�� �����Ͽ� ���⿡��
	//  Window Ŭ���� �Ǵ� ��Ÿ���� �����մϴ�.

	return CScrollView::PreCreateWindow(cs);
}

// CMFC_OpenCVView �׸���

void CMFC_OpenCVView::OnDraw(CDC* pDC)
{
	CMFC_OpenCVDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;

	// TODO: ���⿡ ���� �����Ϳ� ���� �׸��� �ڵ带 �߰��մϴ�.
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
	// TODO: �� ���� ��ü ũ�⸦ ����մϴ�.
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


// CMFC_OpenCVView �μ�


void CMFC_OpenCVView::OnFilePrintPreview()
{
#ifndef SHARED_HANDLERS
	AFXPrintPreview(this);
#endif
}

BOOL CMFC_OpenCVView::OnPreparePrinting(CPrintInfo* pInfo)
{
	// �⺻���� �غ�
	return DoPreparePrinting(pInfo);
}

void CMFC_OpenCVView::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: �μ��ϱ� ���� �߰� �ʱ�ȭ �۾��� �߰��մϴ�.
}

void CMFC_OpenCVView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: �μ� �� ���� �۾��� �߰��մϴ�.
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


// CMFC_OpenCVView ����

#ifdef _DEBUG
void CMFC_OpenCVView::AssertValid() const
{
	CScrollView::AssertValid();
}

void CMFC_OpenCVView::Dump(CDumpContext& dc) const
{
	CScrollView::Dump(dc);
}

CMFC_OpenCVDoc* CMFC_OpenCVView::GetDocument() const // ����׵��� ���� ������ �ζ������� �����˴ϴ�.
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CMFC_OpenCVDoc)));
	return (CMFC_OpenCVDoc*)m_pDocument;
}
#endif //_DEBUG


// CMFC_OpenCVView �޽��� ó����


void CMFC_OpenCVView::OnFileSaveAs()
{
	// TODO: ���⿡ ��� ó���� �ڵ带 �߰��մϴ�.
	CMFC_OpenCVDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	
	CString strFilter = TEXT("JPEG files(*.jpg, *.jpeg, *.jpe) |  *.jpg; *.jpeg; *.jpe |\
							Portable Network Graphics file(*.png) | *.png |\
							Windows Bitmap files(*.bmp, *.dib) | *.bmp; *.dib |\
							Sun resters(*.sr, *.ras) | *.sr; *.ras |\
							TIFF files(*.tiff, *.tif) | *.tiff; *.tif |\
							�����ϴ� ��� ���� ���� | *.jpg; *.jpeg; *.jpe; *.png; *.bmp; *.dib; *.sr; *.ras; *.tiff; *.tif || ");
	CFileDialog dlgFile(FALSE, TEXT("jpg"), NULL, OFN_OVERWRITEPROMPT, strFilter, this);

	if(IDOK == dlgFile.DoModal()) {
		pDoc->OnSaveDocument(dlgFile.GetPathName());
	}
}


void CMFC_OpenCVView::OnEditCopy()
{
	// TODO: ���⿡ ��� ó���� �ڵ带 �߰��մϴ�.
	CMFC_OpenCVDoc* pDoc = GetDocument();   
	ASSERT_VALID(pDoc);
	pDoc->CopyClipBoard( pDoc->m_CvvImage.GetImage() );  
}


void CMFC_OpenCVView::OnFilter2d()
{
	// TODO: ���⿡ ��� ó���� �ڵ带 �߰��մϴ�.
	CFilterDlg dlg;
	dlg.DoModal();

	Invalidate(FALSE);
}
