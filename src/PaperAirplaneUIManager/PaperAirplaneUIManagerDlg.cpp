#include "stdafx.h"
#include "PaperAirplaneUIManager.h"
#include "PaperAirplaneUIManagerDlg.h"
#include "afxdialogex.h"
#include <winsock2.h>
#include <ws2tcpip.h>

#pragma comment(lib, "ws2_32.lib")

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

#define TIMER_UPDATE_MAPPINGS 1
#define UPDATE_INTERVAL 1000

// CAboutDlg dialog used for App About

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_ABOUTBOX };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

// Implementation
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialogEx(IDD_ABOUTBOX)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()


// CPaperAirplaneUIManagerDlg dialog

CPaperAirplaneUIManagerDlg::CPaperAirplaneUIManagerDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_PAPERAIRPLANEUIMANAGER_DIALOG, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
	m_bConnected = FALSE;
}

void CPaperAirplaneUIManagerDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST_SOCKET_MAPPINGS, m_listSocketMappings);
	DDX_Control(pDX, IDC_LIST_FILTER_PORTS, m_listFilterPorts);
	DDX_Control(pDX, IDC_LIST_FILTER_ADDRESSES, m_listFilterAddresses);
	DDX_Control(pDX, IDC_LIST_FILTER_HOSTNAMES, m_listFilterHostNames);
	DDX_Control(pDX, IDC_LIST_BLACKLIST_PROCESSES, m_listBlacklistProcesses);
	DDX_Control(pDX, IDC_LIST_WHITELIST_PROCESSES, m_listWhitelistProcesses);
	DDX_Control(pDX, IDC_EDIT_LOG, m_editLog);
	DDX_Control(pDX, IDC_CHECK_ENABLE_PROXY, m_checkEnableProxy);
	DDX_Control(pDX, IDC_CHECK_RESOLVE_DNS_REMOTE, m_checkResolveDNSRemote);
	DDX_Control(pDX, IDC_EDIT_PROXY_HOST, m_editProxyHost);
	DDX_Control(pDX, IDC_EDIT_PROXY_PORT, m_editProxyPort);
	DDX_Control(pDX, IDC_STATIC_CONNECTION_STATUS, m_staticConnectionStatus);
}

BEGIN_MESSAGE_MAP(CPaperAirplaneUIManagerDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_WM_DESTROY()
	ON_WM_TIMER()
	ON_BN_CLICKED(IDC_BUTTON_APPLY_CONFIG, &CPaperAirplaneUIManagerDlg::OnBnClickedButtonApplyConfig)
	ON_BN_CLICKED(IDC_BUTTON_EXPORT_CONFIG, &CPaperAirplaneUIManagerDlg::OnBnClickedButtonExportConfig)
	ON_BN_CLICKED(IDC_BUTTON_IMPORT_CONFIG, &CPaperAirplaneUIManagerDlg::OnBnClickedButtonImportConfig)
	ON_BN_CLICKED(IDC_BUTTON_ADD_FILTER_PORT, &CPaperAirplaneUIManagerDlg::OnBnClickedButtonAddFilterPort)
	ON_BN_CLICKED(IDC_BUTTON_REMOVE_FILTER_PORT, &CPaperAirplaneUIManagerDlg::OnBnClickedButtonRemoveFilterPort)
	ON_BN_CLICKED(IDC_BUTTON_ADD_FILTER_ADDRESS, &CPaperAirplaneUIManagerDlg::OnBnClickedButtonAddFilterAddress)
	ON_BN_CLICKED(IDC_BUTTON_REMOVE_FILTER_ADDRESS, &CPaperAirplaneUIManagerDlg::OnBnClickedButtonRemoveFilterAddress)
	ON_BN_CLICKED(IDC_BUTTON_ADD_FILTER_HOSTNAME, &CPaperAirplaneUIManagerDlg::OnBnClickedButtonAddFilterHostname)
	ON_BN_CLICKED(IDC_BUTTON_REMOVE_FILTER_HOSTNAME, &CPaperAirplaneUIManagerDlg::OnBnClickedButtonRemoveFilterHostname)
	ON_BN_CLICKED(IDC_BUTTON_ADD_BLACKLIST_PROCESS, &CPaperAirplaneUIManagerDlg::OnBnClickedButtonAddBlacklistProcess)
	ON_BN_CLICKED(IDC_BUTTON_REMOVE_BLACKLIST_PROCESS, &CPaperAirplaneUIManagerDlg::OnBnClickedButtonRemoveBlacklistProcess)
	ON_BN_CLICKED(IDC_BUTTON_ADD_WHITELIST_PROCESS, &CPaperAirplaneUIManagerDlg::OnBnClickedButtonAddWhitelistProcess)
	ON_BN_CLICKED(IDC_BUTTON_REMOVE_WHITELIST_PROCESS, &CPaperAirplaneUIManagerDlg::OnBnClickedButtonRemoveWhitelistProcess)
	ON_BN_CLICKED(IDC_BUTTON_CLEAR_LOG, &CPaperAirplaneUIManagerDlg::OnBnClickedButtonClearLog)
	ON_MESSAGE(PAPERAIRPLANE_PROXECONNECT_MESSAGE, &CPaperAirplaneUIManagerDlg::OnProxyConnect)
	ON_MESSAGE(PAPERAIRPLANE_PROXEBINDCONN_MESSAGE, &CPaperAirplaneUIManagerDlg::OnProxyBindConn)
	ON_MESSAGE(PAPERAIRPLANE_PROXECLOSECONN_MESSAGE, &CPaperAirplaneUIManagerDlg::OnProxyCloseConn)
END_MESSAGE_MAP()


// CPaperAirplaneUIManagerDlg message handlers

BOOL CPaperAirplaneUIManagerDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// Add "About..." menu item to system menu.

	// IDM_ABOUTBOX must be in the system command range.
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		BOOL bNameValid;
		CString strAboutMenu;
		bNameValid = strAboutMenu.LoadString(IDS_ABOUTBOX);
		ASSERT(bNameValid);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon

	// Initialize controls
	InitializeControls();
	InitializeListColumns();

	// Initialize configuration manager
	if (m_configManager.Initialize())
	{
		UpdateConnectionStatus(TRUE);
		AddLogMessage(_T("Configuration manager initialized successfully"));
	}
	else
	{
		UpdateConnectionStatus(FALSE);
		AddLogMessage(_T("Failed to initialize configuration manager"));
	}

	// Load configuration
	LoadConfigurationToUI();

	// Start update timer
	SetTimer(TIMER_UPDATE_MAPPINGS, UPDATE_INTERVAL, NULL);

	return TRUE;  // return TRUE  unless you set the focus to a control
}

void CPaperAirplaneUIManagerDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialogEx::OnSysCommand(nID, lParam);
	}
}

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void CPaperAirplaneUIManagerDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // device context for painting

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// Center icon in client rectangle
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// Draw the icon
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

// The system calls this function to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR CPaperAirplaneUIManagerDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

void CPaperAirplaneUIManagerDlg::OnDestroy()
{
	KillTimer(TIMER_UPDATE_MAPPINGS);
	m_configManager.Cleanup();
	CDialogEx::OnDestroy();
}

void CPaperAirplaneUIManagerDlg::OnTimer(UINT_PTR nIDEvent)
{
	if (nIDEvent == TIMER_UPDATE_MAPPINGS)
	{
		UpdateSocketMappingsList();
	}
	CDialogEx::OnTimer(nIDEvent);
}

void CPaperAirplaneUIManagerDlg::InitializeControls()
{
	// Set extended styles for list controls
	DWORD dwStyle = LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES;
	m_listSocketMappings.SetExtendedStyle(dwStyle);
	m_listFilterPorts.SetExtendedStyle(dwStyle);
	m_listFilterAddresses.SetExtendedStyle(dwStyle);
	m_listFilterHostNames.SetExtendedStyle(dwStyle);
	m_listBlacklistProcesses.SetExtendedStyle(dwStyle);
	m_listWhitelistProcesses.SetExtendedStyle(dwStyle);
}

void CPaperAirplaneUIManagerDlg::InitializeListColumns()
{
	// Socket mappings columns
	m_listSocketMappings.InsertColumn(0, _T("Socket"), LVCFMT_LEFT, 70);
	m_listSocketMappings.InsertColumn(1, _T("Process ID"), LVCFMT_LEFT, 80);
	m_listSocketMappings.InsertColumn(2, _T("Process Name"), LVCFMT_LEFT, 120);
	m_listSocketMappings.InsertColumn(3, _T("Local Address"), LVCFMT_LEFT, 130);
	m_listSocketMappings.InsertColumn(4, _T("Remote Address"), LVCFMT_LEFT, 130);
	m_listSocketMappings.InsertColumn(5, _T("Status"), LVCFMT_LEFT, 100);

	// Filter ports column
	m_listFilterPorts.InsertColumn(0, _T("Port Number"), LVCFMT_LEFT, 150);

	// Filter addresses column
	m_listFilterAddresses.InsertColumn(0, _T("IP Address"), LVCFMT_LEFT, 150);

	// Filter hostnames column
	m_listFilterHostNames.InsertColumn(0, _T("Hostname"), LVCFMT_LEFT, 200);

	// Blacklist processes column
	m_listBlacklistProcesses.InsertColumn(0, _T("Process Path"), LVCFMT_LEFT, 300);

	// Whitelist processes column
	m_listWhitelistProcesses.InsertColumn(0, _T("Process Path"), LVCFMT_LEFT, 300);
}

void CPaperAirplaneUIManagerDlg::LoadConfigurationToUI()
{
	PaperAirplaneConfiguration* config = m_configManager.GetConfiguration();
	
	// Load checkbox states
	m_checkEnableProxy.SetCheck(config->EnableProxyClient ? BST_CHECKED : BST_UNCHECKED);
	m_checkResolveDNSRemote.SetCheck(config->ResolveDNSRemote ? BST_CHECKED : BST_UNCHECKED);

	// Load proxy settings
	m_editProxyHost.SetWindowText(config->ProxyHostAddress);
	CString portStr;
	portStr.Format(_T("%d"), config->ProxyHostPort);
	m_editProxyPort.SetWindowText(portStr);

	// Update filters list
	UpdateFiltersList();
}

void CPaperAirplaneUIManagerDlg::SaveConfigurationFromUI()
{
	PaperAirplaneConfiguration* config = m_configManager.GetConfiguration();

	// Save checkbox states
	config->EnableProxyClient = (m_checkEnableProxy.GetCheck() == BST_CHECKED) ? TRUE : FALSE;
	config->ResolveDNSRemote = (m_checkResolveDNSRemote.GetCheck() == BST_CHECKED) ? TRUE : FALSE;

	// Save proxy settings
	CString proxyHost, proxyPort;
	m_editProxyHost.GetWindowText(proxyHost);
	m_editProxyPort.GetWindowText(proxyPort);
	
	USHORT port = (USHORT)_ttoi(proxyPort);
	if (port == 0)
		port = 1080;
	
	m_configManager.SetProxyHost(proxyHost, port);
	
	// Set the window handle for notifications
	config->ProxyNotifyHWnd = (INT32)(LONG_PTR)GetSafeHwnd();
}

void CPaperAirplaneUIManagerDlg::UpdateSocketMappingsList()
{
	// This would be populated by actual socket mapping data from relay server
	// For now, it's a placeholder for the structure
}

void CPaperAirplaneUIManagerDlg::UpdateFiltersList()
{
	PaperAirplaneConfiguration* config = m_configManager.GetConfiguration();

	// Update filter ports list
	m_listFilterPorts.DeleteAllItems();
	int index = 0;
	for (hash_set<USHORT>::iterator it = config->FilterPortNumber.begin();
		 it != config->FilterPortNumber.end(); ++it)
	{
		CString portStr;
		portStr.Format(_T("%d"), *it);
		m_listFilterPorts.InsertItem(index, portStr);
		index++;
	}

	// Update filter addresses list
	m_listFilterAddresses.DeleteAllItems();
	index = 0;
	for (hash_set<ULONG>::iterator it = config->FilterHostAddress.begin();
		 it != config->FilterHostAddress.end(); ++it)
	{
		m_listFilterAddresses.InsertItem(index, FormatIPAddress(*it));
		index++;
	}

	// Update filter hostnames list
	m_listFilterHostNames.DeleteAllItems();
	index = 0;
	for (hash_set<string>::iterator it = config->FilterHostName.begin();
		 it != config->FilterHostName.end(); ++it)
	{
		m_listFilterHostNames.InsertItem(index, CString(it->c_str()));
		index++;
	}

	// Update blacklist processes list
	m_listBlacklistProcesses.DeleteAllItems();
	index = 0;
	for (size_t i = 0; i < config->BlacklistProcesses.size(); i++)
	{
		m_listBlacklistProcesses.InsertItem(index, CString(config->BlacklistProcesses[i].c_str()));
		index++;
	}

	// Update whitelist processes list (not currently used in shared memory, but available for UI)
	m_listWhitelistProcesses.DeleteAllItems();
	index = 0;
	for (size_t i = 0; i < config->WhitelistProcesses.size(); i++)
	{
		m_listWhitelistProcesses.InsertItem(index, CString(config->WhitelistProcesses[i].c_str()));
		index++;
	}
}

void CPaperAirplaneUIManagerDlg::UpdateConnectionStatus(BOOL connected)
{
	m_bConnected = connected;
	if (connected)
	{
		m_staticConnectionStatus.SetWindowText(_T("Status: Connected"));
		AddLogMessage(_T("Connected to relay server"));
	}
	else
	{
		m_staticConnectionStatus.SetWindowText(_T("Status: Disconnected"));
		AddLogMessage(_T("Disconnected from relay server"));
	}
}

void CPaperAirplaneUIManagerDlg::AddLogMessage(const CString& message)
{
	CString currentText;
	m_editLog.GetWindowText(currentText);
	
	// Get current time
	SYSTEMTIME st;
	GetLocalTime(&st);
	CString timeStr;
	timeStr.Format(_T("[%02d:%02d:%02d] "), st.wHour, st.wMinute, st.wSecond);
	
	currentText += timeStr + message + _T("\r\n");
	
	// Limit log size to prevent memory issues
	if (currentText.GetLength() > 32000)
	{
		currentText = currentText.Right(16000);
	}
	
	m_editLog.SetWindowText(currentText);
	
	// Scroll to bottom
	int len = m_editLog.GetWindowTextLength();
	m_editLog.SetSel(len, len);
	m_editLog.LineScroll(m_editLog.GetLineCount());
}

CString CPaperAirplaneUIManagerDlg::FormatIPAddress(ULONG address)
{
	struct in_addr addr;
	addr.S_un.S_addr = address;
	char buffer[INET_ADDRSTRLEN];
	inet_ntop(AF_INET, &addr, buffer, INET_ADDRSTRLEN);
	return CString(buffer);
}

ULONG CPaperAirplaneUIManagerDlg::ParseIPAddress(const CString& address)
{
	struct in_addr addr;
	CT2A asciiStr(address);
	if (inet_pton(AF_INET, asciiStr, &addr) == 1)
	{
		return addr.S_un.S_addr;
	}
	return 0;
}

// Button handlers

void CPaperAirplaneUIManagerDlg::OnBnClickedButtonApplyConfig()
{
	SaveConfigurationFromUI();
	
	if (m_configManager.ApplyConfiguration())
	{
		AddLogMessage(_T("Configuration applied successfully"));
		MessageBox(_T("Configuration applied and hot-reloaded to relay server"), _T("Success"), MB_OK | MB_ICONINFORMATION);
	}
	else
	{
		AddLogMessage(_T("Failed to apply configuration"));
		MessageBox(_T("Failed to apply configuration"), _T("Error"), MB_OK | MB_ICONERROR);
	}
}

void CPaperAirplaneUIManagerDlg::OnBnClickedButtonExportConfig()
{
	CFileDialog dlg(FALSE, _T("pap"), _T("PaperAirplaneConfig.pap"),
		OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT,
		_T("PaperAirplane Config (*.pap)|*.pap|All Files (*.*)|*.*||"));

	if (dlg.DoModal() == IDOK)
	{
		SaveConfigurationFromUI();
		
		if (m_configManager.ExportToFile(dlg.GetPathName()))
		{
			AddLogMessage(_T("Configuration exported to: ") + dlg.GetPathName());
			MessageBox(_T("Configuration exported successfully"), _T("Success"), MB_OK | MB_ICONINFORMATION);
		}
		else
		{
			AddLogMessage(_T("Failed to export configuration"));
			MessageBox(_T("Failed to export configuration"), _T("Error"), MB_OK | MB_ICONERROR);
		}
	}
}

void CPaperAirplaneUIManagerDlg::OnBnClickedButtonImportConfig()
{
	CFileDialog dlg(TRUE, _T("pap"), NULL,
		OFN_HIDEREADONLY | OFN_FILEMUSTEXIST,
		_T("PaperAirplane Config (*.pap)|*.pap|All Files (*.*)|*.*||"));

	if (dlg.DoModal() == IDOK)
	{
		if (m_configManager.ImportFromFile(dlg.GetPathName()))
		{
			AddLogMessage(_T("Configuration imported from: ") + dlg.GetPathName());
			LoadConfigurationToUI();
			MessageBox(_T("Configuration imported successfully"), _T("Success"), MB_OK | MB_ICONINFORMATION);
		}
		else
		{
			AddLogMessage(_T("Failed to import configuration"));
			MessageBox(_T("Failed to import configuration"), _T("Error"), MB_OK | MB_ICONERROR);
		}
	}
}

void CPaperAirplaneUIManagerDlg::OnBnClickedButtonAddFilterPort()
{
	CString portStr;
	if (AfxMessageBox(_T("Enter port number (1-65535):"), MB_OKCANCEL) == IDOK)
	{
		// Simple input - in production, use a custom dialog
		TCHAR buffer[256];
		if (GetDlgItemText(IDC_EDIT_INPUT_PORT, buffer, 256) > 0)
		{
			USHORT port = (USHORT)_ttoi(buffer);
			if (port > 0 && port <= 65535)
			{
				m_configManager.AddFilterPort(port);
				UpdateFiltersList();
				AddLogMessage(_T("Added filter port: ") + CString(buffer));
			}
		}
	}
}

void CPaperAirplaneUIManagerDlg::OnBnClickedButtonRemoveFilterPort()
{
	int selectedIndex = m_listFilterPorts.GetNextItem(-1, LVNI_SELECTED);
	if (selectedIndex >= 0)
	{
		CString portStr = m_listFilterPorts.GetItemText(selectedIndex, 0);
		USHORT port = (USHORT)_ttoi(portStr);
		m_configManager.RemoveFilterPort(port);
		UpdateFiltersList();
		AddLogMessage(_T("Removed filter port: ") + portStr);
	}
}

void CPaperAirplaneUIManagerDlg::OnBnClickedButtonAddFilterAddress()
{
	// Simple implementation - in production, use a custom dialog
	TCHAR buffer[256];
	if (GetDlgItemText(IDC_EDIT_INPUT_ADDRESS, buffer, 256) > 0)
	{
		ULONG address = ParseIPAddress(buffer);
		if (address != 0)
		{
			m_configManager.AddFilterAddress(address);
			UpdateFiltersList();
			AddLogMessage(_T("Added filter address: ") + CString(buffer));
		}
	}
}

void CPaperAirplaneUIManagerDlg::OnBnClickedButtonRemoveFilterAddress()
{
	int selectedIndex = m_listFilterAddresses.GetNextItem(-1, LVNI_SELECTED);
	if (selectedIndex >= 0)
	{
		CString addressStr = m_listFilterAddresses.GetItemText(selectedIndex, 0);
		ULONG address = ParseIPAddress(addressStr);
		m_configManager.RemoveFilterAddress(address);
		UpdateFiltersList();
		AddLogMessage(_T("Removed filter address: ") + addressStr);
	}
}

void CPaperAirplaneUIManagerDlg::OnBnClickedButtonAddFilterHostname()
{
	TCHAR buffer[256];
	if (GetDlgItemText(IDC_EDIT_INPUT_HOSTNAME, buffer, 256) > 0)
	{
		m_configManager.AddFilterHostName(buffer);
		UpdateFiltersList();
		AddLogMessage(_T("Added filter hostname: ") + CString(buffer));
	}
}

void CPaperAirplaneUIManagerDlg::OnBnClickedButtonRemoveFilterHostname()
{
	int selectedIndex = m_listFilterHostNames.GetNextItem(-1, LVNI_SELECTED);
	if (selectedIndex >= 0)
	{
		CString hostname = m_listFilterHostNames.GetItemText(selectedIndex, 0);
		m_configManager.RemoveFilterHostName(hostname);
		UpdateFiltersList();
		AddLogMessage(_T("Removed filter hostname: ") + hostname);
	}
}

void CPaperAirplaneUIManagerDlg::OnBnClickedButtonAddBlacklistProcess()
{
	CFileDialog dlg(TRUE, NULL, NULL,
		OFN_HIDEREADONLY | OFN_FILEMUSTEXIST,
		_T("Executable Files (*.exe)|*.exe|All Files (*.*)|*.*||"));

	if (dlg.DoModal() == IDOK)
	{
		m_configManager.AddBlacklistProcess(dlg.GetPathName());
		UpdateFiltersList();
		AddLogMessage(_T("Added blacklist process: ") + dlg.GetPathName());
	}
}

void CPaperAirplaneUIManagerDlg::OnBnClickedButtonRemoveBlacklistProcess()
{
	int selectedIndex = m_listBlacklistProcesses.GetNextItem(-1, LVNI_SELECTED);
	if (selectedIndex >= 0)
	{
		CString process = m_listBlacklistProcesses.GetItemText(selectedIndex, 0);
		m_configManager.RemoveBlacklistProcess(process);
		UpdateFiltersList();
		AddLogMessage(_T("Removed blacklist process: ") + process);
	}
}

void CPaperAirplaneUIManagerDlg::OnBnClickedButtonAddWhitelistProcess()
{
	CFileDialog dlg(TRUE, NULL, NULL,
		OFN_HIDEREADONLY | OFN_FILEMUSTEXIST,
		_T("Executable Files (*.exe)|*.exe|All Files (*.*)|*.*||"));

	if (dlg.DoModal() == IDOK)
	{
		m_configManager.AddWhitelistProcess(dlg.GetPathName());
		UpdateFiltersList();
		AddLogMessage(_T("Added whitelist process: ") + dlg.GetPathName());
	}
}

void CPaperAirplaneUIManagerDlg::OnBnClickedButtonRemoveWhitelistProcess()
{
	int selectedIndex = m_listWhitelistProcesses.GetNextItem(-1, LVNI_SELECTED);
	if (selectedIndex >= 0)
	{
		CString process = m_listWhitelistProcesses.GetItemText(selectedIndex, 0);
		m_configManager.RemoveWhitelistProcess(process);
		UpdateFiltersList();
		AddLogMessage(_T("Removed whitelist process: ") + process);
	}
}

void CPaperAirplaneUIManagerDlg::OnBnClickedButtonClearLog()
{
	m_editLog.SetWindowText(_T(""));
	AddLogMessage(_T("Log cleared"));
}

// Relay server message handlers

LRESULT CPaperAirplaneUIManagerDlg::OnProxyConnect(WPARAM wParam, LPARAM lParam)
{
	DWORD processId = (DWORD)wParam;
	SOCKET socket = (SOCKET)lParam;
	
	CString message;
	message.Format(_T("Proxy connect request - Process ID: %d, Socket: %d"), processId, socket);
	AddLogMessage(message);
	
	// Return a port number (example: 1080)
	// In production, this would handle actual proxy connection logic
	return 1080;
}

LRESULT CPaperAirplaneUIManagerDlg::OnProxyBindConn(WPARAM wParam, LPARAM lParam)
{
	DWORD processId = (DWORD)wParam;
	SOCKET socket = (SOCKET)lParam;
	
	CString message;
	message.Format(_T("Proxy bind request - Process ID: %d, Socket: %d"), processId, socket);
	AddLogMessage(message);
	
	// Return a port number
	return 0;
}

LRESULT CPaperAirplaneUIManagerDlg::OnProxyCloseConn(WPARAM wParam, LPARAM lParam)
{
	DWORD processId = (DWORD)wParam;
	SOCKET socket = (SOCKET)lParam;
	
	CString message;
	message.Format(_T("Proxy close request - Process ID: %d, Socket: %d"), processId, socket);
	AddLogMessage(message);
	
	return TRUE;
}
