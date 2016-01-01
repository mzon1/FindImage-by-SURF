#pragma once
#include "MainFrm.h"
#include "ChildFrm.h"
#include "MFC_OpenCVDoc.h"
#include "Filter.h"
#include "DeiosImgUtil.h"


// CFilterDlg ��ȭ �����Դϴ�.

class CFilterDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CFilterDlg)

public:
	CFilterDlg(CWnd* pParent = NULL);   // ǥ�� �������Դϴ�.
	virtual ~CFilterDlg();

	CvvImage d_org_cvvImage; // �� ���� 
	CvvImage d_cvvImage; // ���͸� ��� ���� ���� 

	int picture_height; // Picture ��Ʈ���� ���� ����  
	int picture_width; // Picture ��Ʈ���� �ʺ� ����(4�� ���) 

	float soften_mask[9]; // For Soften filtering 
	float ef_mask[9]; // For Enhance focus filtering 
	float ed_mask[9]; // For Enhance detail filtering 
	float bl_mask[9]; // For Blur light filtering 
	float bb_mask[9]; // For Blur blending filtering 

// ��ȭ ���� �������Դϴ�.
	enum { IDD = IDD_FILTER_DIALOG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �����Դϴ�.

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
