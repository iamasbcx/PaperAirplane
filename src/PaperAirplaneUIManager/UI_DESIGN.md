# PaperAirplane UI Manager - Visual Design Description

## Main Window Layout

The PaperAirplane UI Manager is a comprehensive dialog-based MFC application with the following visual layout:

### Window Title
```
PaperAirplane UI Manager
```

### Top Section - Connection Status (7, 7, 606, 30)
```
┌─ Connection Status ─────────────────────────────────────┐
│ Status: Connected                                         │
└──────────────────────────────────────────────────────────┘
```
- Displays real-time connection status to the relay server
- Green text for "Connected", Red text for "Disconnected"

### Configuration Section (Split Layout)

#### Left Panel - Proxy Configuration (7, 42, 300, 70)
```
┌─ Proxy Configuration ───────────────────────────────────┐
│ ☑ Enable Proxy Client                                    │
│ ☑ Resolve DNS Remote                                     │
│                                                           │
│ Proxy Host: [127.0.0.1________________]                  │
│ Port: [1080]                                             │
└──────────────────────────────────────────────────────────┘
```
- Checkboxes for enabling features
- Text input for proxy host address
- Number input for proxy port

#### Right Panel - Actions (320, 42, 293, 70)
```
┌─ Actions ───────────────────────────────────────────────┐
│  [ Apply Configuration ]  [ Import Configuration ]       │
│  [ Export Configuration ]                                │
└──────────────────────────────────────────────────────────┘
```
- Large buttons for configuration management

### Socket Mappings Section (7, 117, 606, 120)
```
┌─ Socket Mappings ──────────────────────────────────────────────────────────────┐
│ Socket │ Process ID │ Process Name │ Local Address    │ Remote Address   │ Status │
├────────┼────────────┼──────────────┼──────────────────┼──────────────────┼────────┤
│ 1234   │ 5678       │ chrome.exe   │ 192.168.1.2:4567 │ 8.8.8.8:443     │ Active │
│ 1235   │ 5678       │ chrome.exe   │ 192.168.1.2:4568 │ 1.1.1.1:443     │ Active │
└────────────────────────────────────────────────────────────────────────────────┘
```
- List control showing all active socket connections
- Columns: Socket Handle, Process ID, Process Name, Local Address, Remote Address, Status
- Full row selection with grid lines

### Filter Rules Section (7, 242, 606, 120)
```
┌─ Filter Rules ───────────────────────────────────────────────────────────────────┐
│                                                                                   │
│ Filter Ports:      │ Filter Addresses:  │ Filter Hostnames:                     │
│ ┌──────────────┐   │ ┌──────────────┐   │ ┌─────────────────────┐               │
│ │ 80           │   │ │ 192.168.1.1  │   │ │ example.com         │               │
│ │ 443          │   │ │ 10.0.0.1     │   │ │ localhost           │               │
│ │ 8080         │   │ │              │   │ │                     │               │
│ └──────────────┘   │ └──────────────┘   │ └─────────────────────┘               │
│ [Add] [Remove]     │ [Add] [Remove]     │ [Add] [Remove]                        │
│ [____Port____]     │ [___Address___]    │ [______Hostname______]                │
└───────────────────────────────────────────────────────────────────────────────────┘
```
- Three side-by-side list controls for different filter types
- Input boxes below each list
- Add/Remove buttons for management

### Process Lists Section (7, 367, 606, 65)
```
┌─ Process Lists ──────────────────────────────────────────────────────────────────┐
│                                                                                   │
│ Blacklist:                              │ Whitelist:                             │
│ ┌───────────────────────────────────┐   │ ┌───────────────────────────────────┐ │
│ │ C:\Program Files\App1\app.exe     │   │ │ C:\Program Files\App2\app.exe     │ │
│ │ C:\Program Files\App3\app.exe     │   │ │                                   │ │
│ └───────────────────────────────────┘   │ └───────────────────────────────────┘ │
│       [Add]  [Remove]                   │       [Add]  [Remove]                 │
└───────────────────────────────────────────────────────────────────────────────────┘
```
- Two list controls side by side
- Blacklist: processes that won't use proxy
- Whitelist: processes that can use proxy (future feature)

### Log Section (7, 437, 606, 36)
```
┌─ Log ────────────────────────────────────────────────────────────────────────────┐
│ [12:34:56] Configuration manager initialized successfully                        │
│ [12:35:01] Configuration applied successfully                                    │
│ [12:35:10] Proxy connect request - Process ID: 1234, Socket: 5678              │
│                                                                          [Clear] │
└───────────────────────────────────────────────────────────────────────────────────┘
```
- Multi-line read-only edit control
- Timestamped log messages
- Auto-scroll to bottom
- Clear button to reset log

## Color Scheme
- Standard Windows dialog background (light gray)
- Group boxes with beveled borders
- List controls with white background and grid lines
- Active connection status in green
- Disconnected status in red
- Standard button colors with hover effects

## Dialog Dimensions
- Width: 620 pixels
- Height: 480 pixels
- Fixed size (not resizable)
- Centered on screen at startup

## Font
- MS Shell Dlg, 8pt (standard Windows dialog font)
- Unicode character set support

## Icons
- Main window icon: Paper airplane symbol (placeholder provided)
- System menu with About dialog

## Visual Features
1. **List Controls**: Full row selection, grid lines, sortable columns
2. **Buttons**: Standard 3D appearance with hover effects
3. **Edit Controls**: Single-line and multi-line with borders
4. **Checkboxes**: Standard Windows checkboxes with labels
5. **Group Boxes**: Beveled frames with titles
6. **Status Display**: Static text with dynamic color changes

## User Experience
- Intuitive layout with logical grouping
- Clear visual hierarchy
- Responsive to user actions
- Immediate feedback through log messages
- Tooltips on hover (can be added for buttons)
- Tab order follows visual flow

## Accessibility
- Keyboard navigation support (Tab key)
- Accelerator keys for buttons (Alt+key)
- High contrast mode compatible
- Screen reader friendly labels

## Notes
When the application is built and run on Windows:
1. The window will appear centered on screen
2. All controls will be properly sized and aligned
3. The connection status will update in real-time
4. List controls will be empty initially and populate with data
5. The log will show initialization messages

To see the actual UI, build the project in Visual Studio and run the executable on a Windows system with the PaperAirplane relay server installed.
