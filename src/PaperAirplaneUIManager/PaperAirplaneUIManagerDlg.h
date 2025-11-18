#pragma once

#include "ConfigurationManager.h"

// CPaperAirplaneUIManagerDlg dialog
class CPaperAirplaneUIManagerDlg : public CDialogEx
{
// Construction
public:
	CPaperAirplaneUIManagerDlg(CWnd* pParent = NULL);	// standard constructor

// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_PAPERAIRPLANEUIMANAGER_DIALOG };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support


// Implementation
protected:
	HICON m_hIcon;
	CConfigurationManager m_configManager;
	BOOL m_bConnected;
	
	// Controls
	CListCtrl m_listSocketMappings;
	CListCtrl m_listFilterPorts;
	CListCtrl m_listFilterAddresses;
	CListCtrl m_listFilterHostNames;
	CListCtrl m_listBlacklistProcesses;
	CListCtrl m_listWhitelistProcesses;
	CEdit m_editLog;
	
	// Configuration controls
	CButton m_checkEnableProxy;
	CButton m_checkResolveDNSRemote;
	CEdit m_editProxyHost;
	CEdit m_editProxyPort;
	CStatic m_staticConnectionStatus;

	// Generated message map functions
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	afx_msg void OnDestroy();
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	
	// Button handlers
	afx_msg void OnBnClickedButtonApplyConfig();
	afx_msg void OnBnClickedButtonExportConfig();
	afx_msg void OnBnClickedButtonImportConfig();
	afx_msg void OnBnClickedButtonAddFilterPort();
	afx_msg void OnBnClickedButtonRemoveFilterPort();
	afx_msg void OnBnClickedButtonAddFilterAddress();
	afx_msg void OnBnClickedButtonRemoveFilterAddress();
	afx_msg void OnBnClickedButtonAddFilterHostname();
	afx_msg void OnBnClickedButtonRemoveFilterHostname();
	afx_msg void OnBnClickedButtonAddBlacklistProcess();
	afx_msg void OnBnClickedButtonRemoveBlacklistProcess();
	afx_msg void OnBnClickedButtonAddWhitelistProcess();
	afx_msg void OnBnClickedButtonRemoveWhitelistProcess();
	afx_msg void OnBnClickedButtonClearLog();
	
	// Relay server message handlers
	afx_msg LRESULT OnProxyConnect(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnProxyBindConn(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnProxyCloseConn(WPARAM wParam, LPARAM lParam);

	DECLARE_MESSAGE_MAP()

private:
	void InitializeControls();
	void InitializeListColumns();
	void LoadConfigurationToUI();
	void SaveConfigurationFromUI();
	void UpdateSocketMappingsList();
	void UpdateFiltersList();
	void UpdateConnectionStatus(BOOL connected);
	void AddLogMessage(const CString& message);
	CString FormatIPAddress(ULONG address);
	ULONG ParseIPAddress(const CString& address);
};
