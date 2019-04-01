#pragma once
#include "afxext.h"
class CProgStatusBar :
	public CStatusBar
{
public:
	CProgStatusBar();
	~CProgStatusBar();
	CProgressCtrl& GetProgressCtrl() {
		return m_wndProgBar;
	}
	void OnProgress(UINT pct);
	void setRange(UINT range);

	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);

	afx_msg void OnSize(UINT nType, int cx, int cy);

	CProgressCtrl	m_wndProgBar;
	DECLARE_MESSAGE_MAP()
	DECLARE_DYNAMIC(CProgStatusBar)
};

