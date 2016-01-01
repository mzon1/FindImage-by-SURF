
// MFC_OpenCVDoc.cpp : CMFC_OpenCVDoc Ŭ������ ����
//

#include "stdafx.h"
// SHARED_HANDLERS�� �̸� ����, ����� �׸� �� �˻� ���� ó���⸦ �����ϴ� ATL ������Ʈ���� ������ �� ������
// �ش� ������Ʈ�� ���� �ڵ带 �����ϵ��� �� �ݴϴ�.
#ifndef SHARED_HANDLERS
#include "MFC_OpenCV.h"
#endif

#include "MFC_OpenCVDoc.h"

#include "DeiosColor.h"

#include <propkey.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

// CMFC_OpenCVDoc

IMPLEMENT_DYNCREATE(CMFC_OpenCVDoc, CDocument)

BEGIN_MESSAGE_MAP(CMFC_OpenCVDoc, CDocument)
//	ON_COMMAND(ID_FILE_OPEN, &CMFC_OpenCVDoc::OnFileOpen)
	ON_COMMAND(ID_COLOR_SPLIT_RGB, &CMFC_OpenCVDoc::OnColorSplitRgb)
END_MESSAGE_MAP()


// CMFC_OpenCVDoc ����/�Ҹ�

CMFC_OpenCVDoc::CMFC_OpenCVDoc()
{
	// TODO: ���⿡ ��ȸ�� ���� �ڵ带 �߰��մϴ�.

}

CMFC_OpenCVDoc::~CMFC_OpenCVDoc()
{
}

BOOL CMFC_OpenCVDoc::OnNewDocument()
{
	if (!CDocument::OnNewDocument())
		return FALSE;

	// TODO: ���⿡ ���ʱ�ȭ �ڵ带 �߰��մϴ�.
	// SDI ������ �� ������ �ٽ� ����մϴ�.

	return TRUE;
}




// CMFC_OpenCVDoc serialization

void CMFC_OpenCVDoc::Serialize(CArchive& ar)
{
	if (ar.IsStoring())
	{
		// TODO: ���⿡ ���� �ڵ带 �߰��մϴ�.
	}
	else
	{
		// TODO: ���⿡ �ε� �ڵ带 �߰��մϴ�.
	}
}

#ifdef SHARED_HANDLERS

// ����� �׸��� �����մϴ�.
void CMFC_OpenCVDoc::OnDrawThumbnail(CDC& dc, LPRECT lprcBounds)
{
	// ������ �����͸� �׸����� �� �ڵ带 �����Ͻʽÿ�.
	dc.FillSolidRect(lprcBounds, RGB(255, 255, 255));

	CString strText = _T("TODO: implement thumbnail drawing here");
	LOGFONT lf;

	CFont* pDefaultGUIFont = CFont::FromHandle((HFONT) GetStockObject(DEFAULT_GUI_FONT));
	pDefaultGUIFont->GetLogFont(&lf);
	lf.lfHeight = 36;

	CFont fontDraw;
	fontDraw.CreateFontIndirect(&lf);

	CFont* pOldFont = dc.SelectObject(&fontDraw);
	dc.DrawText(strText, lprcBounds, DT_CENTER | DT_WORDBREAK);
	dc.SelectObject(pOldFont);
}

// �˻� ó���⸦ �����մϴ�.
void CMFC_OpenCVDoc::InitializeSearchContent()
{
	CString strSearchContent;
	// ������ �����Ϳ��� �˻� �������� �����մϴ�.
	// ������ �κ��� ";"�� ���еǾ�� �մϴ�.

	// ��: strSearchContent = _T("point;rectangle;circle;ole object;");
	SetSearchContent(strSearchContent);
}

void CMFC_OpenCVDoc::SetSearchContent(const CString& value)
{
	if (value.IsEmpty())
	{
		RemoveChunk(PKEY_Search_Contents.fmtid, PKEY_Search_Contents.pid);
	}
	else
	{
		CMFCFilterChunkValueImpl *pChunk = NULL;
		ATLTRY(pChunk = new CMFCFilterChunkValueImpl);
		if (pChunk != NULL)
		{
			pChunk->SetTextValue(PKEY_Search_Contents, value, CHUNK_TEXT);
			SetChunkValue(pChunk);
		}
	}
}

#endif // SHARED_HANDLERS

// CMFC_OpenCVDoc ����

#ifdef _DEBUG
void CMFC_OpenCVDoc::AssertValid() const
{
	CDocument::AssertValid();
}

void CMFC_OpenCVDoc::Dump(CDumpContext& dc) const
{
	CDocument::Dump(dc);
}
#endif //_DEBUG


// CMFC_OpenCVDoc ���


BOOL CMFC_OpenCVDoc::OnOpenDocument(LPCTSTR lpszPathName)
{
	if (!CDocument::OnOpenDocument(lpszPathName))
		return FALSE;

	// TODO:  ���⿡ Ư��ȭ�� �ۼ� �ڵ带 �߰��մϴ�.
	
	USES_CONVERSION;
	return m_CvvImage.Load(W2A(lpszPathName));
}


BOOL CMFC_OpenCVDoc::OnSaveDocument(LPCTSTR lpszPathName)
{
	// TODO: ���⿡ Ư��ȭ�� �ڵ带 �߰� ��/�Ǵ� �⺻ Ŭ������ ȣ���մϴ�.
	USES_CONVERSION;
	return m_CvvImage.Save(W2A(lpszPathName));
}


//void CMFC_OpenCVDoc::OnFileOpen()
//{
//	// TODO: ���⿡ ��� ó���� �ڵ带 �߰��մϴ�.
//}


void CMFC_OpenCVDoc::DeleteContents()
{
	// TODO: ���⿡ Ư��ȭ�� �ڵ带 �߰� ��/�Ǵ� �⺻ Ŭ������ ȣ���մϴ�.
	if(NULL != m_CvvImage.GetImage()){
		m_CvvImage.~CvvImage();
	}
	CDocument::DeleteContents();
}


void CMFC_OpenCVDoc::CopyClipBoard(IplImage * m_pCopyImage)
{
	HANDLE hDIB = CopyToHandle(m_pCopyImage);
	if(::OpenClipboard(AfxGetMainWnd()->GetSafeHwnd())){
		if(::EmptyClipboard()){
			if(NULL == ::SetClipboardData(CF_DIB, hDIB)){
				AfxMessageBox(TEXT("Ŭ�����忡 ������ �����ϴ� ���� ������ �߻��߽��ϴ�."));
			}
		}
	}
	::CloseClipboard();
}


HANDLE CMFC_OpenCVDoc::CopyToHandle(IplImage * image)
{
	int nChannels = image->nChannels;   
	int bpp = 8*nChannels;   
	IplImage *flip_image = cvCloneImage( image );   
	cvFlip( image, flip_image, 0 );    
	int bmpDataSize = flip_image->imageSize;   
	// ��Ʈ�� ��� ����    
	BITMAPINFOHEADER bmpHeader;   
	bmpHeader.biSize = sizeof(BITMAPINFOHEADER); // �� ����ü�� ũ��   
	bmpHeader.biHeight = image->height; // ������ ����   
	bmpHeader.biWidth = image->width;   // ������ �ʺ�   
	bmpHeader.biPlanes = 1; // ��Ʈ �÷��� �� (�׻� 1��)   
	bmpHeader.biBitCount = bpp; // �� ȭ�Ҵ� ��Ʈ ����    
	bmpHeader.biCompression = BI_RGB; // BI_RGB : �������� ����   
	bmpHeader.biSizeImage = bmpDataSize; // ������ ũ��   
	if(8 == bpp) bmpHeader.biClrUsed = bmpHeader.biClrImportant  = 256;    
	else if(24 == bpp) bmpHeader.biClrUsed = bmpHeader.biClrImportant  = 0;
	int bmpAllSize = sizeof(BITMAPINFOHEADER) + sizeof(RGBQUAD)*256 + bmpDataSize*sizeof(char);   
	HGLOBAL hDIB = (HGLOBAL)::GlobalAlloc(GMEM_MOVEABLE| GMEM_ZEROINIT,   
										  bmpAllSize );   
	if(NULL == hDIB) return NULL;   
	LPSTR pDIB = (LPSTR)::GlobalLock((HGLOBAL)hDIB);   
	memcpy( pDIB, &bmpHeader, sizeof(BITMAPINFOHEADER) );   
  
	if(8 == bpp){   
		RGBQUAD palette[256];   
		memcpy( pDIB+sizeof(BITMAPINFOHEADER), palette, sizeof(RGBQUAD)*256 );   
		memcpy( pDIB+sizeof(BITMAPINFOHEADER)+sizeof(RGBQUAD)*256,    
				flip_image->imageData, bmpDataSize );   
	}    
	else if(24 == bpp){   
		memcpy( pDIB+sizeof(BITMAPINFOHEADER),    
				flip_image->imageData, bmpDataSize );   
	}  
	::GlobalUnlock((HGLOBAL)hDIB);   
	cvReleaseImage( &flip_image );   
    
	return hDIB;
}


void CMFC_OpenCVDoc::OnColorSplitRgb()
{
	// TODO: ���⿡ ��� ó���� �ڵ带 �߰��մϴ�.
	if( m_CvvImage.GetImage()->nChannels != 3 ){   
		AfxMessageBox( TEXT("�÷� ������ �ƴմϴ�."), MB_ICONSTOP );   
		return;   
	}

	IplImage *red, *green, *blue;    
	CDeiosColor cColor;   
	cColor.DSplitRGB( m_CvvImage.GetImage(), &red, &green, &blue );   

	OpenByNewDocument(TEXT("[SplitRGB()] ��ȯ ��� - ���� ä��"), red);
	OpenByNewDocument(TEXT("[SplitRGB()] ��ȯ ��� - �ʷ� ä��"), green);
	OpenByNewDocument(TEXT("[SplitRGB()] ��ȯ ��� - �Ķ� ä��"), blue);

	cvReleaseImage( &red );   
	cvReleaseImage( &green );   
	cvReleaseImage( &blue );   
}


void CMFC_OpenCVDoc::OnCloseDocument()
{
	// TODO: ���⿡ Ư��ȭ�� �ڵ带 �߰� ��/�Ǵ� �⺻ Ŭ������ ȣ���մϴ�.
	m_CvvImage.Destroy();
	CDocument::OnCloseDocument();
}
