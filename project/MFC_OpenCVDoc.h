
// MFC_OpenCVDoc.h : CMFC_OpenCVDoc Ŭ������ �������̽�
//


#pragma once
#include <cv.h>
#include <cxcore.h>
#include <highgui.h>

class CMFC_OpenCVDoc : public CDocument
{
protected: // serialization������ ��������ϴ�.
	CMFC_OpenCVDoc();
	DECLARE_DYNCREATE(CMFC_OpenCVDoc)

// Ư���Դϴ�.
public:

// �۾��Դϴ�.
public:

// �������Դϴ�.
public:
	virtual BOOL OnNewDocument();
	virtual void Serialize(CArchive& ar);
#ifdef SHARED_HANDLERS
	virtual void InitializeSearchContent();
	virtual void OnDrawThumbnail(CDC& dc, LPRECT lprcBounds);
#endif // SHARED_HANDLERS

// �����Դϴ�.
public:
	virtual ~CMFC_OpenCVDoc();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// ������ �޽��� �� �Լ�
protected:
	DECLARE_MESSAGE_MAP()

#ifdef SHARED_HANDLERS
	// �˻� ó���⿡ ���� �˻� �������� �����ϴ� ����� �Լ�
	void SetSearchContent(const CString& value);
#endif // SHARED_HANDLERS
public:
	virtual BOOL OnOpenDocument(LPCTSTR lpszPathName);
	CvvImage m_CvvImage;
	virtual BOOL OnSaveDocument(LPCTSTR lpszPathName);
//	afx_msg void OnFileOpen();
	virtual void DeleteContents();
	void CopyClipBoard(IplImage * m_pCopyImage);
	HANDLE CopyToHandle(IplImage * image);
	afx_msg void OnColorSplitRgb();
	virtual void OnCloseDocument();
};
