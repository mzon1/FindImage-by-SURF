#pragma once
#include "MainFrm.h"
#include "ChildFrm.h"
#include "MFC_OpenCVDoc.h"
#include "Filter.h"
#include "DeiosImgUtil.h"


// CFilterDlg 대화 상자입니다.

class CFilterDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CFilterDlg)

public:
	CFilterDlg(CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~CFilterDlg();

	CvvImage d_org_cvvImage; // 원 영상 
	CvvImage d_cvvImage; // 필터링 결과 후의 영상 

	int picture_height; // Picture 컨트롤의 높이 기준  
	int picture_width; // Picture 컨트롤의 너비 기준(4의 배수) 

	float soften_mask[9]; // For Soften filtering 
	float ef_mask[9]; // For Enhance focus filtering 
	float ed_mask[9]; // For Enhance detail filtering 
	float bl_mask[9]; // For Blur light filtering 
	float bb_mask[9]; // For Blur blending filtering 

// 대화 상자 데이터입니다.
	enum { IDD = IDD_FILTER_DIALOG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()
public:
	CComboBox m_combo_filter;
	CStatic m_preview_image;
	virtual BOOL OnInitDialog();
	afx_msg void OnDestroy();
	afx_msg void OnBnClickedPreviewButton();
	afx_msg void OnBnClickedInitialButton();
	afx_msg void OnBnClickedOk();
};
