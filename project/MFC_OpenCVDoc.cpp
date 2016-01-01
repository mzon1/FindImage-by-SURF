
// MFC_OpenCVDoc.cpp : CMFC_OpenCVDoc 클래스의 구현
//

#include "stdafx.h"
// SHARED_HANDLERS는 미리 보기, 축소판 그림 및 검색 필터 처리기를 구현하는 ATL 프로젝트에서 정의할 수 있으며
// 해당 프로젝트와 문서 코드를 공유하도록 해 줍니다.
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


// CMFC_OpenCVDoc 생성/소멸

CMFC_OpenCVDoc::CMFC_OpenCVDoc()
{
	// TODO: 여기에 일회성 생성 코드를 추가합니다.

}

CMFC_OpenCVDoc::~CMFC_OpenCVDoc()
{
}

BOOL CMFC_OpenCVDoc::OnNewDocument()
{
	if (!CDocument::OnNewDocument())
		return FALSE;

	// TODO: 여기에 재초기화 코드를 추가합니다.
	// SDI 문서는 이 문서를 다시 사용합니다.

	return TRUE;
}




// CMFC_OpenCVDoc serialization

void CMFC_OpenCVDoc::Serialize(CArchive& ar)
{
	if (ar.IsStoring())
	{
		// TODO: 여기에 저장 코드를 추가합니다.
	}
	else
	{
		// TODO: 여기에 로딩 코드를 추가합니다.
	}
}

#ifdef SHARED_HANDLERS

// 축소판 그림을 지원합니다.
void CMFC_OpenCVDoc::OnDrawThumbnail(CDC& dc, LPRECT lprcBounds)
{
	// 문서의 데이터를 그리려면 이 코드를 수정하십시오.
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

// 검색 처리기를 지원합니다.
void CMFC_OpenCVDoc::InitializeSearchContent()
{
	CString strSearchContent;
	// 문서의 데이터에서 검색 콘텐츠를 설정합니다.
	// 콘텐츠 부분은 ";"로 구분되어야 합니다.

	// 예: strSearchContent = _T("point;rectangle;circle;ole object;");
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

// CMFC_OpenCVDoc 진단

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


// CMFC_OpenCVDoc 명령


BOOL CMFC_OpenCVDoc::OnOpenDocument(LPCTSTR lpszPathName)
{
	if (!CDocument::OnOpenDocument(lpszPathName))
		return FALSE;

	// TODO:  여기에 특수화된 작성 코드를 추가합니다.
	
	USES_CONVERSION;
	return m_CvvImage.Load(W2A(lpszPathName));
}


BOOL CMFC_OpenCVDoc::OnSaveDocument(LPCTSTR lpszPathName)
{
	// TODO: 여기에 특수화된 코드를 추가 및/또는 기본 클래스를 호출합니다.
	USES_CONVERSION;
	return m_CvvImage.Save(W2A(lpszPathName));
}


//void CMFC_OpenCVDoc::OnFileOpen()
//{
//	// TODO: 여기에 명령 처리기 코드를 추가합니다.
//}


void CMFC_OpenCVDoc::DeleteContents()
{
	// TODO: 여기에 특수화된 코드를 추가 및/또는 기본 클래스를 호출합니다.
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
				AfxMessageBox(TEXT("클립보드에 영상을 저장하는 도중 오류가 발생했습니다."));
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
	// 비트맵 헤더 구성    
	BITMAPINFOHEADER bmpHeader;   
	bmpHeader.biSize = sizeof(BITMAPINFOHEADER); // 이 구조체의 크기   
	bmpHeader.biHeight = image->height; // 영상의 높이   
	bmpHeader.biWidth = image->width;   // 영상의 너비   
	bmpHeader.biPlanes = 1; // 비트 플레인 수 (항상 1임)   
	bmpHeader.biBitCount = bpp; // 한 화소당 비트 개수    
	bmpHeader.biCompression = BI_RGB; // BI_RGB : 압축하지 않음   
	bmpHeader.biSizeImage = bmpDataSize; // 영상의 크기   
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
	// TODO: 여기에 명령 처리기 코드를 추가합니다.
	if( m_CvvImage.GetImage()->nChannels != 3 ){   
		AfxMessageBox( TEXT("컬러 영상이 아닙니다."), MB_ICONSTOP );   
		return;   
	}

	IplImage *red, *green, *blue;    
	CDeiosColor cColor;   
	cColor.DSplitRGB( m_CvvImage.GetImage(), &red, &green, &blue );   

	OpenByNewDocument(TEXT("[SplitRGB()] 변환 결과 - 빨강 채널"), red);
	OpenByNewDocument(TEXT("[SplitRGB()] 변환 결과 - 초록 채널"), green);
	OpenByNewDocument(TEXT("[SplitRGB()] 변환 결과 - 파랑 채널"), blue);

	cvReleaseImage( &red );   
	cvReleaseImage( &green );   
	cvReleaseImage( &blue );   
}


void CMFC_OpenCVDoc::OnCloseDocument()
{
	// TODO: 여기에 특수화된 코드를 추가 및/또는 기본 클래스를 호출합니다.
	m_CvvImage.Destroy();
	CDocument::OnCloseDocument();
}
