
// MFC_OpenCVView.h : CMFC_OpenCVView Ŭ������ �������̽�
//

#pragma once

class CMFC_OpenCVView : public CScrollView
{
protected: // serialization������ ��������ϴ�.
	CMFC_OpenCVView();
	DECLARE_DYNCREATE(CMFC_OpenCVView)

// Ư���Դϴ�.
public:
	CMFC_OpenCVDoc* GetDocument() const;

// �۾��Դϴ�.
public:

// �������Դϴ�.
public:
	virtual void OnDraw(CDC* pDC);  // �� �並 �׸��� ���� �����ǵǾ����ϴ�.
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
protected:
	virtual void OnInitialUpdate(); // ���� �� ó�� ȣ��Ǿ����ϴ�.
	virtual BOOL OnPreparePrinting(CPrintInfo* pInfo);
	virtual void OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnEndPrinting(CDC* pDC, CPrintInfo* pInfo);

// �����Դϴ�.
public:
	virtual ~CMFC_OpenCVView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// ������ �޽��� �� �Լ�
protected:
	afx_msg void OnFilePrintPreview();
	afx_msg void OnRButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnContextMenu(CWnd* pWnd, CPoint point);
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnFileSaveAs();
	afx_msg void OnEditCopy();
//	afx_msg void OnColorSplitRgb();
	afx_msg void OnFilter2d();
};

#ifndef _DEBUG  // MFC_OpenCVView.cpp�� ����� ����
inline CMFC_OpenCVDoc* CMFC_OpenCVView::GetDocument() const
   { return reinterpret_cast<CMFC_OpenCVDoc*>(m_pDocument); }
#endif

