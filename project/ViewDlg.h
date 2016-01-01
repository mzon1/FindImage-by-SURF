#pragma once
#include "highgui.h"
#include "BlobLabeling.h"


// CViewDlg 대화 상자입니다.

class CViewDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CViewDlg)

public:
	CViewDlg(CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~CViewDlg();

// 대화 상자 데이터입니다.
	enum { IDD = IDD_CamView };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()
public:
	CvvImage m_CvvImage;
	int m_SurfPoint[IMAGENUM];
	int m_MaxSurfPoint;
	int m_Number;
private:
	CvCapture *capture;
	IplImage *image;
	bool viewState;
public:
	CButton m_btnOK;
	CStatic m_sView;
	virtual BOOL OnInitDialog();
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	virtual void OnOK();
	afx_msg void OnDestroy();
//	afx_msg void OnBnClickedButton1();
	afx_msg void OnBnClickedbtncapture();
	afx_msg void OnBnClickedLab();
private:
	bool m_bBinarize;					
private:
	bool m_SURF;
public:
	CButton m_Surf_able;
};
