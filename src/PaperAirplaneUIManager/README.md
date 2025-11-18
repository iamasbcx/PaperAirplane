# PaperAirplane UI Manager

## Overview

PaperAirplane UI Manager is an MFC-based graphical user interface for managing the PaperAirplane relay server configuration. It provides a comprehensive interface for configuring proxy settings, managing filter rules, and monitoring socket connections.

## Features

### 1. Socket Mapping Display
- Real-time display of active socket connections
- Shows socket handle, process ID, process name, local/remote addresses, and connection status
- Statistics for bytes sent/received (extensible)

### 2. Configuration Management
- **Enable Proxy Client**: Toggle proxy functionality on/off
- **Resolve DNS Remote**: Enable/disable remote DNS resolution
- **Proxy Settings**: Configure proxy host address and port (default: 127.0.0.1:1080)

### 3. Filter Rules
- **Port Filtering**: Add/remove specific port numbers to filter
- **Address Filtering**: Add/remove IP addresses to filter
- **Hostname Filtering**: Add/remove hostnames to filter

### 4. Process Management
- **Blacklist**: Add processes that should NOT use the proxy (process paths)
- **Whitelist**: Add processes that CAN use the proxy (for future use)

### 5. Configuration Import/Export
- Export current configuration to a .pap file
- Import configuration from a .pap file
- Configuration files use binary format for efficiency

### 6. Hot Configuration Reload
- Changes are applied immediately via shared memory
- Uses Windows named shared memory: `PAPERAIRPLANE_CONFIGURATION_MAP`
- Signals relay server via event: `PAPERAIRPLANE_CONFIGURATION_EVENT`
- No need to restart applications or services

### 7. Logging and Status
- Real-time connection status display (Connected/Disconnected)
- Event logging for all operations
- Proxy connection/bind/close callbacks from relay server
- Timestamped log entries

## Building the Project

### Requirements
- Visual Studio 2015 or later (2017 recommended)
- Windows SDK 10.0 or later
- MFC libraries (installed with Visual Studio)

### Build Steps
1. Open `PaperAirplaneUIManager.vcxproj` in Visual Studio
2. Select your desired configuration (Debug/Release) and platform (Win32/x64)
3. Build the solution (Ctrl+Shift+B)

The compiled executable will be in the Debug or Release folder depending on your configuration.

## Usage

### Initial Setup
1. Run PaperAirplaneUIManager.exe
2. The application will automatically create shared memory and event objects
3. If the relay server is loaded (via LSP/NSP), status will show "Connected"

### Configuring Proxy Settings
1. Check "Enable Proxy Client" to activate proxy functionality
2. Enter proxy host address (e.g., 127.0.0.1)
3. Enter proxy port (e.g., 1080 for SOCKS5)
4. Optionally check "Resolve DNS Remote" for remote DNS resolution
5. Click "Apply Configuration" to hot-reload settings

### Managing Filters
- **Port Filters**: Enter a port number in the input box and click "Add". Select and click "Remove" to delete.
- **Address Filters**: Enter an IP address (e.g., 192.168.1.1) and click "Add"
- **Hostname Filters**: Enter a hostname (e.g., example.com) and click "Add"

### Managing Process Lists
- **Blacklist**: Click "Add" to browse for an executable. Selected processes will NOT use the proxy.
- **Whitelist**: Similar to blacklist but for allowing specific processes (future use)

### Import/Export
- **Export**: Click "Export Configuration" and choose a location to save the .pap file
- **Import**: Click "Import Configuration" and select a .pap file to load

## Architecture

### Shared Memory Protocol
The UI Manager communicates with the relay server DLL through shared memory:

```
Message Format:
[4 bytes] Message ID (incremented for each update)
[1 byte]  EnableProxyClient
[1 byte]  ResolveDNSRemote
[4 bytes] ProxyNotifyHWnd (window handle for callbacks)
[2 bytes] FilterPortNumber count
  [N * 2 bytes] Port numbers
[2 bytes] FilterHostAddress count
  [N * 4 bytes] IP addresses (network byte order)
[2 bytes] FilterHostName count
  [N entries] [2 bytes length][string] Hostnames
[2 bytes] Blacklist process count
  [N entries] [2 bytes length][string] Process paths
```

### Window Messages
The UI Manager receives notifications from the relay server via Windows messages:
- `PAPERAIRPLANE_PROXECONNECT_MESSAGE (0x1311)`: Proxy connection request
- `PAPERAIRPLANE_PROXEBINDCONN_MESSAGE (0x1312)`: Proxy bind request
- `PAPERAIRPLANE_PROXECLOSECONN_MESSAGE (0x1313)`: Proxy close notification

## File Structure

```
PaperAirplaneUIManager/
├── PaperAirplaneUIManager.cpp         # Application entry point
├── PaperAirplaneUIManager.h           # Application header
├── PaperAirplaneUIManagerDlg.cpp      # Main dialog implementation
├── PaperAirplaneUIManagerDlg.h        # Main dialog header
├── ConfigurationManager.cpp            # Configuration management logic
├── ConfigurationManager.h              # Configuration manager header
├── stdafx.cpp                          # Precompiled header source
├── stdafx.h                            # Precompiled header
├── targetver.h                         # Windows version targeting
├── resource.h                          # Resource IDs
├── PaperAirplaneUIManager.rc          # Resource script
├── PaperAirplaneUIManager.vcxproj     # Visual Studio project
├── PaperAirplaneUIManager.vcxproj.filters # Project filters
└── res/
    ├── PaperAirplaneUIManager.ico     # Application icon
    └── PaperAirplaneUIManager.rc2     # Additional resources
```

## Configuration File Format

Configuration files (.pap) use a binary format:
1. Version number (4 bytes)
2. EnableProxyClient (1 byte)
3. ResolveDNSRemote (1 byte)
4. ProxyHostAddress (4 bytes length + string)
5. ProxyHostPort (2 bytes)
6. Filter arrays (each with count + entries)
7. Process lists (each with count + entries)

## Technical Notes

### Thread Safety
- The ConfigurationManager uses critical sections for thread-safe access
- The shared memory is synchronized using a named event

### Memory Management
- Uses STL containers (hash_set, vector) for efficient filter management
- Automatic cleanup in destructors

### Compatibility
- Compatible with Windows 7 and later
- Supports both 32-bit and 64-bit builds
- Uses Unicode character set

## Future Enhancements

Potential improvements for future versions:
1. Real-time socket mapping updates from relay server
2. Bandwidth monitoring and statistics
3. Traffic visualization graphs
4. Process auto-discovery and management
5. Multiple proxy server configurations
6. Rule-based routing (per-application proxy settings)
7. Import/export in JSON or XML format
8. Tray icon with quick settings
9. Connection quality indicators
10. Proxy authentication support

## Troubleshooting

### Status shows "Disconnected"
- Ensure the PaperAirplane relay server DLL is installed and loaded
- Check that no other application is using the shared memory names
- Verify Windows permissions allow creating named objects

### Configuration not applying
- Click "Apply Configuration" after making changes
- Check the log for error messages
- Ensure you have administrator privileges if needed

### Filters not working
- Verify filter syntax (IP addresses, port numbers)
- Check that EnableProxyClient is enabled
- Review the relay server logs for filter processing

## License

This project is part of the PaperAirplane project. Refer to the main project LICENSE file for details.

## Contributing

Contributions are welcome! Please follow the existing code style and test your changes thoroughly.
