# Developer Quick Reference

## Project Structure

```
PaperAirplaneUIManager/
├── Main Application
│   ├── PaperAirplaneUIManager.cpp/h      - Application entry point
│   ├── PaperAirplaneUIManagerDlg.cpp/h   - Main dialog window
│   └── ConfigurationManager.cpp/h         - Configuration management
│
├── Build Files
│   ├── PaperAirplaneUIManager.sln         - Visual Studio solution
│   ├── PaperAirplaneUIManager.vcxproj     - Project file
│   └── PaperAirplaneUIManager.vcxproj.filters
│
├── Resources
│   ├── PaperAirplaneUIManager.rc          - Resource script
│   ├── resource.h                          - Resource IDs
│   └── res/
│       ├── PaperAirplaneUIManager.ico     - Application icon
│       └── PaperAirplaneUIManager.rc2     - Additional resources
│
├── Precompiled Headers
│   ├── stdafx.cpp/h                       - PCH files
│   └── targetver.h                        - Windows version targeting
│
└── Documentation
    ├── README.md                           - English documentation
    ├── 用户指南.md                         - Chinese user guide
    └── UI_DESIGN.md                       - UI design description
```

## Key Classes

### CPaperAirplaneUIManagerApp
- **Location**: PaperAirplaneUIManager.cpp/h
- **Purpose**: Application entry point and initialization
- **Key Methods**:
  - `InitInstance()`: Initialize MFC and show main dialog

### CPaperAirplaneUIManagerDlg
- **Location**: PaperAirplaneUIManagerDlg.cpp/h
- **Purpose**: Main dialog window with all UI controls
- **Key Methods**:
  - `OnInitDialog()`: Initialize controls and configuration manager
  - `OnBnClickedButtonApplyConfig()`: Apply configuration to relay server
  - `OnBnClickedButtonExportConfig()`: Export configuration to file
  - `OnBnClickedButtonImportConfig()`: Import configuration from file
  - `OnProxyConnect/Bind/Close()`: Handle relay server callbacks
  - `UpdateFiltersList()`: Refresh all filter list controls
  - `AddLogMessage()`: Add timestamped log entry

### CConfigurationManager
- **Location**: ConfigurationManager.cpp/h
- **Purpose**: Manage configuration and shared memory communication
- **Key Methods**:
  - `Initialize()`: Create shared memory and event objects
  - `ApplyConfiguration()`: Write config to shared memory and signal event
  - `ExportToFile()`: Serialize configuration to binary file
  - `ImportFromFile()`: Deserialize configuration from binary file
  - `Add/Remove/ClearFilter*()`: Manage filter collections

## Configuration Structure

```cpp
typedef struct PaperAirplaneConfiguration
{
    BYTE EnableProxyClient;           // Enable/disable proxy
    BYTE ResolveDNSRemote;            // Remote DNS resolution
    INT32 ProxyNotifyHWnd;            // Window handle for callbacks
    hash_set<USHORT> FilterPortNumber;     // Filtered ports
    hash_set<ULONG> FilterHostAddress;     // Filtered IP addresses
    hash_set<string> FilterHostName;       // Filtered hostnames
    CString ProxyHostAddress;         // Proxy server address
    USHORT ProxyHostPort;             // Proxy server port
    vector<string> WhitelistProcesses;// Allowed processes
    vector<string> BlacklistProcesses;// Blocked processes
} PaperAirplaneConfiguration;
```

## Shared Memory Protocol

### Memory Map Name
```cpp
#define PAPERAIRPLANE_CONFIGURATION_MAP "PAPERAIRPLANE_CONFIGURATION_MAP"
```

### Event Name
```cpp
#define PAPERAIRPLANE_CONFIGURATION_EVENT "PAPERAIRPLANE_CONFIGURATION_EVENT"
```

### Message Format
```
Offset | Size | Description
-------|------|------------
0      | 4    | Message ID (increments on each update)
4      | 1    | EnableProxyClient flag
5      | 1    | ResolveDNSRemote flag
6      | 4    | ProxyNotifyHWnd (window handle)
10     | 2    | FilterPortNumber count
12     | N*2  | Port numbers (USHORT each)
       | 2    | FilterHostAddress count
       | N*4  | IP addresses (ULONG each, network byte order)
       | 2    | FilterHostName count
       | N*X  | Hostnames (each: 2 bytes length + string)
       | 2    | Blacklist process count
       | N*X  | Process paths (each: 2 bytes length + string)
```

## Window Messages

### Proxy Connect Request
```cpp
#define PAPERAIRPLANE_PROXECONNECT_MESSAGE 0x1311
// wParam: Process ID
// lParam: Socket handle
// Return: Port number (or 0 if failed)
```

### Proxy Bind Request
```cpp
#define PAPERAIRPLANE_PROXEBINDCONN_MESSAGE 0x1312
// wParam: Process ID
// lParam: Socket handle
// Return: Port number (or 0 if failed)
```

### Proxy Close Notification
```cpp
#define PAPERAIRPLANE_PROXECLOSECONN_MESSAGE 0x1313
// wParam: Process ID
// lParam: Socket handle
// Return: TRUE if successful
```

## Resource IDs

### Dialog IDs
```cpp
IDD_PAPERAIRPLANEUIMANAGER_DIALOG  102
IDD_ABOUTBOX                        100
```

### Control IDs
```cpp
// List Controls
IDC_LIST_SOCKET_MAPPINGS          1000
IDC_LIST_FILTER_PORTS             1001
IDC_LIST_FILTER_ADDRESSES         1002
IDC_LIST_FILTER_HOSTNAMES         1003
IDC_LIST_BLACKLIST_PROCESSES      1004
IDC_LIST_WHITELIST_PROCESSES      1005

// Configuration Controls
IDC_CHECK_ENABLE_PROXY            1007
IDC_CHECK_RESOLVE_DNS_REMOTE      1008
IDC_EDIT_PROXY_HOST               1009
IDC_EDIT_PROXY_PORT               1010

// Action Buttons
IDC_BUTTON_APPLY_CONFIG           1100
IDC_BUTTON_EXPORT_CONFIG          1101
IDC_BUTTON_IMPORT_CONFIG          1102
```

## Common Tasks

### Adding a New Filter Type
1. Add member to `PaperAirplaneConfiguration` struct
2. Add list control to dialog resource
3. Implement Add/Remove/Clear methods in `CConfigurationManager`
4. Update `WriteConfigurationToMemory()` to serialize
5. Update `ExportToFile()` and `ImportFromFile()`
6. Add UI handlers in `CPaperAirplaneUIManagerDlg`

### Handling a New Callback
1. Define message ID in header
2. Add message handler declaration to dialog class
3. Implement handler method
4. Add to message map with `ON_MESSAGE()`

### Modifying Shared Memory Protocol
1. Update `WriteConfigurationToMemory()` in ConfigurationManager
2. Ensure relay server's `HandleQueryConfiguration()` is compatible
3. Increment configuration version in export/import
4. Test thoroughly with relay server

## Build Configurations

### Debug Build
- Full debugging symbols
- No optimization
- Runtime checks enabled
- MFC Debug libraries

### Release Build
- Optimizations enabled
- No debugging symbols
- Smaller executable size
- MFC Release libraries

### Platform Targets
- **Win32**: 32-bit Windows
- **x64**: 64-bit Windows

## Dependencies

### System Libraries
- ws2_32.lib (Winsock 2)
- shlwapi.lib (Shell Lightweight API)

### MFC Requirements
- Dynamic MFC linking (MFC140.dll)
- Unicode character set
- Windows SDK 10.0+

## Coding Guidelines

### Naming Conventions
- Classes: `CClassName`
- Member variables: `m_variableName`
- Functions: `PascalCase()`
- Constants: `UPPER_CASE`
- Hungarian notation for types (p=pointer, h=handle, etc.)

### Error Handling
- Check return values from Windows APIs
- Use try/catch for file I/O
- Log errors to the log window
- Show message boxes for critical errors

### Thread Safety
- Use critical sections for shared data
- Enter/Leave around configuration access
- Avoid blocking UI thread

## Testing Checklist

- [ ] Configuration applies without errors
- [ ] Export saves all settings correctly
- [ ] Import loads all settings correctly
- [ ] Filters display in lists properly
- [ ] Add/Remove operations work for all filters
- [ ] Process blacklist/whitelist management works
- [ ] Log messages appear with timestamps
- [ ] Connection status updates correctly
- [ ] Relay server callbacks are received
- [ ] Hot reload updates relay server config
- [ ] Multiple rapid updates don't cause issues
- [ ] Large filter lists perform well
- [ ] Application closes cleanly

## Debugging Tips

### Check Shared Memory
```cpp
// Use Process Explorer or similar tool to view:
// - Named shared memory objects
// - Named event objects
// - Verify they exist and have correct names
```

### Monitor Messages
```cpp
// Add logging to message handlers to verify callbacks
AddLogMessage(_T("Received message: ") + message);
```

### Verify Configuration
```cpp
// Export configuration and inspect binary file
// Use hex editor to verify format
```

## Performance Considerations

1. **List Controls**: Use virtual mode for large datasets
2. **Filter Operations**: Use hash_set for O(1) lookups
3. **Logging**: Limit log size to prevent memory issues
4. **Shared Memory**: Write only when configuration changes
5. **Event Signaling**: Use auto-reset events

## Future Enhancements

1. Real-time socket mapping updates from relay server
2. Bandwidth monitoring and statistics graphs
3. Process auto-discovery with icons
4. Rule-based routing configuration
5. Multiple proxy profile management
6. Tray icon with quick settings menu
7. Import/export in JSON/XML formats
8. Localization support (multi-language)
9. Themes and custom visual styles
10. Advanced filtering with regex support
