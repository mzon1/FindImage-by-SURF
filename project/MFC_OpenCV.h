
// MFC_OpenCV.h : MFC_OpenCV ���� ���α׷��� ���� �� ��� ����
//
#pragma once

#ifndef __AFXWIN_H__
	#error "PCH�� ���� �� ������ �����ϱ� ���� 'stdafx.h'�� �����մϴ�."
#endif

#include "resource.h"       // �� ��ȣ�Դϴ�.
#include <cv.h>


// CMFC_OpenCVApp:
// �� Ŭ������ ������ ���ؼ��� MFC_OpenCV.cpp�� �����Ͻʽÿ�.
//

class CMFC_OpenCVApp : public CWinAppEx
{
public:
	CMFC_OpenCVApp();


// �������Դϴ�.
public:
	virtual BOOL InitInstance();
	virtual int ExitInstance();

// �����Դϴ�.
	UINT  m_nAppLook;
	BOOL  m_bHiColorIcons;

	virtual void PreLoadState();
	virtual void LoadCustomState();
	virtual void SaveCustomState();

	afx_msg void OnAppAbout();
	DECLARE_MESSAGE_MAP()
	afx_msg void OnFileOpen();
	afx_msg void OnFileCloseAll();
	afx_msg void OnEditPaste();
	IplImage * CreateFromHandle(HANDLE hDIB);
	afx_msg void OnUpdateEditPaste(CCmdUI *pCmdUI);

	afx_msg void OnCamView();
};

extern CMFC_OpenCVApp theApp;
void OpenByNewDocument(LPTSTR szTitle, IplImage * image);