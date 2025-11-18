#include "stdafx.h"
#include "ConfigurationManager.h"
#include <shlwapi.h>
#include <fstream>

#pragma comment(lib, "shlwapi.lib")

CConfigurationManager::CConfigurationManager()
{
	m_hMapFile = NULL;
	m_hEvent = NULL;
	m_pMapView = NULL;
	m_msgId = 0;
	
	memset(&m_config, 0, sizeof(m_config));
	m_config.EnableProxyClient = FALSE;
	m_config.ResolveDNSRemote = FALSE;
	m_config.ProxyNotifyHWnd = 0;
	m_config.ProxyHostPort = 1080;
	
	InitializeCriticalSection(&m_cs);
}

CConfigurationManager::~CConfigurationManager()
{
	Cleanup();
	DeleteCriticalSection(&m_cs);
}

BOOL CConfigurationManager::Initialize()
{
	return CreateSharedMemory();
}

void CConfigurationManager::Cleanup()
{
	Enter();
	
	if (m_pMapView != NULL)
	{
		UnmapViewOfFile(m_pMapView);
		m_pMapView = NULL;
	}
	
	if (m_hMapFile != NULL)
	{
		CloseHandle(m_hMapFile);
		m_hMapFile = NULL;
	}
	
	if (m_hEvent != NULL)
	{
		CloseHandle(m_hEvent);
		m_hEvent = NULL;
	}
	
	Leave();
}

BOOL CConfigurationManager::CreateSharedMemory()
{
	// Create file mapping
	m_hMapFile = CreateFileMappingA(
		INVALID_HANDLE_VALUE,
		NULL,
		PAGE_READWRITE,
		0,
		64 * 1024, // 64KB should be enough
		PAPERAIRPLANE_CONFIGURATION_MAP);
	
	if (m_hMapFile == NULL)
	{
		return FALSE;
	}
	
	// Map view of file
	m_pMapView = MapViewOfFile(
		m_hMapFile,
		FILE_MAP_ALL_ACCESS,
		0,
		0,
		0);
	
	if (m_pMapView == NULL)
	{
		CloseHandle(m_hMapFile);
		m_hMapFile = NULL;
		return FALSE;
	}
	
	// Create event for notification
	m_hEvent = CreateEventA(
		NULL,
		FALSE, // Auto-reset
		FALSE, // Initially non-signaled
		PAPERAIRPLANE_CONFIGURATION_EVENT);
	
	if (m_hEvent == NULL)
	{
		UnmapViewOfFile(m_pMapView);
		CloseHandle(m_hMapFile);
		m_pMapView = NULL;
		m_hMapFile = NULL;
		return FALSE;
	}
	
	return TRUE;
}

BOOL CConfigurationManager::OpenSharedMemory()
{
	m_hMapFile = OpenFileMappingA(
		FILE_MAP_ALL_ACCESS,
		FALSE,
		PAPERAIRPLANE_CONFIGURATION_MAP);
	
	if (m_hMapFile == NULL)
	{
		return FALSE;
	}
	
	m_pMapView = MapViewOfFile(
		m_hMapFile,
		FILE_MAP_ALL_ACCESS,
		0,
		0,
		0);
	
	if (m_pMapView == NULL)
	{
		CloseHandle(m_hMapFile);
		m_hMapFile = NULL;
		return FALSE;
	}
	
	m_hEvent = OpenEventA(
		EVENT_ALL_ACCESS,
		FALSE,
		PAPERAIRPLANE_CONFIGURATION_EVENT);
	
	if (m_hEvent == NULL)
	{
		UnmapViewOfFile(m_pMapView);
		CloseHandle(m_hMapFile);
		m_pMapView = NULL;
		m_hMapFile = NULL;
		return FALSE;
	}
	
	return TRUE;
}

BOOL CConfigurationManager::ApplyConfiguration()
{
	if (!WriteConfigurationToMemory())
	{
		return FALSE;
	}
	
	// Signal the event to notify relay server
	if (m_hEvent != NULL)
	{
		SetEvent(m_hEvent);
	}
	
	return TRUE;
}

BOOL CConfigurationManager::WriteConfigurationToMemory()
{
	if (m_pMapView == NULL)
	{
		return FALSE;
	}
	
	Enter();
	
	char* buf = (char*)m_pMapView;
	
	// Write message ID
	m_msgId++;
	*(INT*)buf = m_msgId;
	buf += 4;
	
	// Write EnableProxyClient
	*buf++ = m_config.EnableProxyClient;
	
	if (m_config.EnableProxyClient)
	{
		// Write ResolveDNSRemote
		*buf++ = m_config.ResolveDNSRemote;
		
		// Write ProxyNotifyHWnd
		*(INT32*)buf = m_config.ProxyNotifyHWnd;
		buf += 4;
		
		// Write FilterPortNumber
		*(USHORT*)buf = (USHORT)m_config.FilterPortNumber.size();
		buf += 2;
		for (hash_set<USHORT>::iterator it = m_config.FilterPortNumber.begin();
			 it != m_config.FilterPortNumber.end(); ++it)
		{
			*(USHORT*)buf = *it;
			buf += 2;
		}
		
		// Write FilterHostAddress
		*(USHORT*)buf = (USHORT)m_config.FilterHostAddress.size();
		buf += 2;
		for (hash_set<ULONG>::iterator it = m_config.FilterHostAddress.begin();
			 it != m_config.FilterHostAddress.end(); ++it)
		{
			*(ULONG*)buf = *it;
			buf += 4;
		}
		
		// Write FilterHostName
		*(USHORT*)buf = (USHORT)m_config.FilterHostName.size();
		buf += 2;
		for (hash_set<string>::iterator it = m_config.FilterHostName.begin();
			 it != m_config.FilterHostName.end(); ++it)
		{
			INT cb = (INT)it->length() + 1;
			*(USHORT*)buf = (USHORT)cb;
			buf += 2;
			strcpy_s(buf, cb, it->c_str());
			buf += cb;
		}
		
		// Write blacklist processes (process paths)
		*(USHORT*)buf = (USHORT)m_config.BlacklistProcesses.size();
		buf += 2;
		for (size_t i = 0; i < m_config.BlacklistProcesses.size(); i++)
		{
			INT cb = (INT)m_config.BlacklistProcesses[i].length() + 1;
			*(USHORT*)buf = (USHORT)cb;
			buf += 2;
			strcpy_s(buf, cb, m_config.BlacklistProcesses[i].c_str());
			buf += cb;
		}
	}
	
	Leave();
	
	return TRUE;
}

BOOL CConfigurationManager::LoadConfiguration()
{
	// TODO: Load from registry or configuration file
	return TRUE;
}

BOOL CConfigurationManager::SaveConfiguration()
{
	// TODO: Save to registry or configuration file
	return TRUE;
}

BOOL CConfigurationManager::ExportToFile(const CString& filename)
{
	std::ofstream file;
	file.open(filename, std::ios::out | std::ios::binary);
	
	if (!file.is_open())
	{
		return FALSE;
	}
	
	Enter();
	
	// Write configuration version
	int version = 1;
	file.write((char*)&version, sizeof(int));
	
	// Write EnableProxyClient
	file.write((char*)&m_config.EnableProxyClient, sizeof(BYTE));
	
	// Write ResolveDNSRemote
	file.write((char*)&m_config.ResolveDNSRemote, sizeof(BYTE));
	
	// Write ProxyHostAddress
	int len = m_config.ProxyHostAddress.GetLength();
	file.write((char*)&len, sizeof(int));
	if (len > 0)
	{
		CT2A asciiStr(m_config.ProxyHostAddress);
		file.write(asciiStr, len);
	}
	
	// Write ProxyHostPort
	file.write((char*)&m_config.ProxyHostPort, sizeof(USHORT));
	
	// Write FilterPortNumber
	int count = (int)m_config.FilterPortNumber.size();
	file.write((char*)&count, sizeof(int));
	for (hash_set<USHORT>::iterator it = m_config.FilterPortNumber.begin();
		 it != m_config.FilterPortNumber.end(); ++it)
	{
		file.write((char*)&(*it), sizeof(USHORT));
	}
	
	// Write FilterHostAddress
	count = (int)m_config.FilterHostAddress.size();
	file.write((char*)&count, sizeof(int));
	for (hash_set<ULONG>::iterator it = m_config.FilterHostAddress.begin();
		 it != m_config.FilterHostAddress.end(); ++it)
	{
		file.write((char*)&(*it), sizeof(ULONG));
	}
	
	// Write FilterHostName
	count = (int)m_config.FilterHostName.size();
	file.write((char*)&count, sizeof(int));
	for (hash_set<string>::iterator it = m_config.FilterHostName.begin();
		 it != m_config.FilterHostName.end(); ++it)
	{
		len = (int)it->length();
		file.write((char*)&len, sizeof(int));
		file.write(it->c_str(), len);
	}
	
	// Write WhitelistProcesses
	count = (int)m_config.WhitelistProcesses.size();
	file.write((char*)&count, sizeof(int));
	for (size_t i = 0; i < m_config.WhitelistProcesses.size(); i++)
	{
		len = (int)m_config.WhitelistProcesses[i].length();
		file.write((char*)&len, sizeof(int));
		file.write(m_config.WhitelistProcesses[i].c_str(), len);
	}
	
	// Write BlacklistProcesses
	count = (int)m_config.BlacklistProcesses.size();
	file.write((char*)&count, sizeof(int));
	for (size_t i = 0; i < m_config.BlacklistProcesses.size(); i++)
	{
		len = (int)m_config.BlacklistProcesses[i].length();
		file.write((char*)&len, sizeof(int));
		file.write(m_config.BlacklistProcesses[i].c_str(), len);
	}
	
	Leave();
	
	file.close();
	return TRUE;
}

BOOL CConfigurationManager::ImportFromFile(const CString& filename)
{
	std::ifstream file;
	file.open(filename, std::ios::in | std::ios::binary);
	
	if (!file.is_open())
	{
		return FALSE;
	}
	
	Enter();
	
	// Read configuration version
	int version = 0;
	file.read((char*)&version, sizeof(int));
	
	if (version != 1)
	{
		Leave();
		file.close();
		return FALSE;
	}
	
	// Read EnableProxyClient
	file.read((char*)&m_config.EnableProxyClient, sizeof(BYTE));
	
	// Read ResolveDNSRemote
	file.read((char*)&m_config.ResolveDNSRemote, sizeof(BYTE));
	
	// Read ProxyHostAddress
	int len = 0;
	file.read((char*)&len, sizeof(int));
	if (len > 0)
	{
		char* buffer = new char[len + 1];
		file.read(buffer, len);
		buffer[len] = '\0';
		m_config.ProxyHostAddress = CString(buffer);
		delete[] buffer;
	}
	
	// Read ProxyHostPort
	file.read((char*)&m_config.ProxyHostPort, sizeof(USHORT));
	
	// Read FilterPortNumber
	int count = 0;
	file.read((char*)&count, sizeof(int));
	m_config.FilterPortNumber.clear();
	for (int i = 0; i < count; i++)
	{
		USHORT port;
		file.read((char*)&port, sizeof(USHORT));
		m_config.FilterPortNumber.insert(port);
	}
	
	// Read FilterHostAddress
	file.read((char*)&count, sizeof(int));
	m_config.FilterHostAddress.clear();
	for (int i = 0; i < count; i++)
	{
		ULONG addr;
		file.read((char*)&addr, sizeof(ULONG));
		m_config.FilterHostAddress.insert(addr);
	}
	
	// Read FilterHostName
	file.read((char*)&count, sizeof(int));
	m_config.FilterHostName.clear();
	for (int i = 0; i < count; i++)
	{
		file.read((char*)&len, sizeof(int));
		char* buffer = new char[len + 1];
		file.read(buffer, len);
		buffer[len] = '\0';
		m_config.FilterHostName.insert(string(buffer));
		delete[] buffer;
	}
	
	// Read WhitelistProcesses
	file.read((char*)&count, sizeof(int));
	m_config.WhitelistProcesses.clear();
	for (int i = 0; i < count; i++)
	{
		file.read((char*)&len, sizeof(int));
		char* buffer = new char[len + 1];
		file.read(buffer, len);
		buffer[len] = '\0';
		m_config.WhitelistProcesses.push_back(string(buffer));
		delete[] buffer;
	}
	
	// Read BlacklistProcesses
	file.read((char*)&count, sizeof(int));
	m_config.BlacklistProcesses.clear();
	for (int i = 0; i < count; i++)
	{
		file.read((char*)&len, sizeof(int));
		char* buffer = new char[len + 1];
		file.read(buffer, len);
		buffer[len] = '\0';
		m_config.BlacklistProcesses.push_back(string(buffer));
		delete[] buffer;
	}
	
	Leave();
	
	file.close();
	return TRUE;
}

void CConfigurationManager::SetProxyHost(const CString& host, USHORT port)
{
	Enter();
	m_config.ProxyHostAddress = host;
	m_config.ProxyHostPort = port;
	Leave();
}

void CConfigurationManager::AddFilterPort(USHORT port)
{
	Enter();
	m_config.FilterPortNumber.insert(port);
	Leave();
}

void CConfigurationManager::RemoveFilterPort(USHORT port)
{
	Enter();
	m_config.FilterPortNumber.erase(port);
	Leave();
}

void CConfigurationManager::ClearFilterPorts()
{
	Enter();
	m_config.FilterPortNumber.clear();
	Leave();
}

void CConfigurationManager::AddFilterAddress(ULONG address)
{
	Enter();
	m_config.FilterHostAddress.insert(address);
	Leave();
}

void CConfigurationManager::RemoveFilterAddress(ULONG address)
{
	Enter();
	m_config.FilterHostAddress.erase(address);
	Leave();
}

void CConfigurationManager::ClearFilterAddresses()
{
	Enter();
	m_config.FilterHostAddress.clear();
	Leave();
}

void CConfigurationManager::AddFilterHostName(const CString& hostname)
{
	Enter();
	CT2A asciiStr(hostname);
	string str(asciiStr);
	// Convert to lowercase
	std::transform(str.begin(), str.end(), str.begin(), ::tolower);
	m_config.FilterHostName.insert(str);
	Leave();
}

void CConfigurationManager::RemoveFilterHostName(const CString& hostname)
{
	Enter();
	CT2A asciiStr(hostname);
	string str(asciiStr);
	std::transform(str.begin(), str.end(), str.begin(), ::tolower);
	m_config.FilterHostName.erase(str);
	Leave();
}

void CConfigurationManager::ClearFilterHostNames()
{
	Enter();
	m_config.FilterHostName.clear();
	Leave();
}

void CConfigurationManager::AddWhitelistProcess(const CString& process)
{
	Enter();
	CT2A asciiStr(process);
	m_config.WhitelistProcesses.push_back(string(asciiStr));
	Leave();
}

void CConfigurationManager::RemoveWhitelistProcess(const CString& process)
{
	Enter();
	CT2A asciiStr(process);
	string str(asciiStr);
	for (vector<string>::iterator it = m_config.WhitelistProcesses.begin();
		 it != m_config.WhitelistProcesses.end(); ++it)
	{
		if (*it == str)
		{
			m_config.WhitelistProcesses.erase(it);
			break;
		}
	}
	Leave();
}

void CConfigurationManager::ClearWhitelistProcesses()
{
	Enter();
	m_config.WhitelistProcesses.clear();
	Leave();
}

void CConfigurationManager::AddBlacklistProcess(const CString& process)
{
	Enter();
	CT2A asciiStr(process);
	m_config.BlacklistProcesses.push_back(string(asciiStr));
	Leave();
}

void CConfigurationManager::RemoveBlacklistProcess(const CString& process)
{
	Enter();
	CT2A asciiStr(process);
	string str(asciiStr);
	for (vector<string>::iterator it = m_config.BlacklistProcesses.begin();
		 it != m_config.BlacklistProcesses.end(); ++it)
	{
		if (*it == str)
		{
			m_config.BlacklistProcesses.erase(it);
			break;
		}
	}
	Leave();
}

void CConfigurationManager::ClearBlacklistProcesses()
{
	Enter();
	m_config.BlacklistProcesses.clear();
	Leave();
}
