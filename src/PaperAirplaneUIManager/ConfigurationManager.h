#pragma once

#include <hash_set>
#include <vector>
#include <string>

using namespace std;
using namespace stdext;

#define PAPERAIRPLANE_CONFIGURATION_MAP "PAPERAIRPLANE_CONFIGURATION_MAP"
#define PAPERAIRPLANE_CONFIGURATION_EVENT "PAPERAIRPLANE_CONFIGURATION_EVENT"

#define PAPERAIRPLANE_PROXECONNECT_MESSAGE 0x1311
#define PAPERAIRPLANE_PROXEBINDCONN_MESSAGE 0x1312
#define PAPERAIRPLANE_PROXECLOSECONN_MESSAGE 0x1313

// Configuration structure matching the relay server
typedef struct PaperAirplaneConfiguration
{
	BYTE EnableProxyClient;
	BYTE ResolveDNSRemote;
	INT32 ProxyNotifyHWnd;
	hash_set<USHORT> FilterPortNumber;
	hash_set<ULONG> FilterHostAddress;
	hash_set<string> FilterHostName;
	
	// Additional fields for UI management
	CString ProxyHostAddress;
	USHORT ProxyHostPort;
	vector<string> WhitelistProcesses;
	vector<string> BlacklistProcesses;
} PaperAirplaneConfiguration;

// Socket mapping information
typedef struct SocketMappingInfo
{
	SOCKET SocketHandle;
	DWORD ProcessId;
	CString ProcessName;
	CString LocalAddress;
	USHORT LocalPort;
	CString RemoteAddress;
	USHORT RemotePort;
	CString Status;
	DWORD BytesSent;
	DWORD BytesReceived;
} SocketMappingInfo;

// Configuration Manager class
class CConfigurationManager
{
private:
	HANDLE m_hMapFile;
	HANDLE m_hEvent;
	LPVOID m_pMapView;
	int m_msgId;
	PaperAirplaneConfiguration m_config;
	CRITICAL_SECTION m_cs;
	vector<SocketMappingInfo> m_socketMappings;

public:
	CConfigurationManager();
	~CConfigurationManager();

	// Initialize/cleanup
	BOOL Initialize();
	void Cleanup();

	// Configuration operations
	BOOL LoadConfiguration();
	BOOL SaveConfiguration();
	BOOL ApplyConfiguration();
	
	// Export/import
	BOOL ExportToFile(const CString& filename);
	BOOL ImportFromFile(const CString& filename);

	// Getters
	PaperAirplaneConfiguration* GetConfiguration() { return &m_config; }
	const vector<SocketMappingInfo>& GetSocketMappings() const { return m_socketMappings; }

	// Setters
	void SetEnableProxyClient(BOOL enable) { m_config.EnableProxyClient = (BYTE)enable; }
	void SetResolveDNSRemote(BOOL enable) { m_config.ResolveDNSRemote = (BYTE)enable; }
	void SetProxyHost(const CString& host, USHORT port);
	
	// Filter management
	void AddFilterPort(USHORT port);
	void RemoveFilterPort(USHORT port);
	void ClearFilterPorts();
	
	void AddFilterAddress(ULONG address);
	void RemoveFilterAddress(ULONG address);
	void ClearFilterAddresses();
	
	void AddFilterHostName(const CString& hostname);
	void RemoveFilterHostName(const CString& hostname);
	void ClearFilterHostNames();

	// Process whitelist/blacklist
	void AddWhitelistProcess(const CString& process);
	void RemoveWhitelistProcess(const CString& process);
	void ClearWhitelistProcesses();
	
	void AddBlacklistProcess(const CString& process);
	void RemoveBlacklistProcess(const CString& process);
	void ClearBlacklistProcesses();

private:
	BOOL CreateSharedMemory();
	BOOL OpenSharedMemory();
	BOOL WriteConfigurationToMemory();
	void Enter() { EnterCriticalSection(&m_cs); }
	void Leave() { LeaveCriticalSection(&m_cs); }
};
