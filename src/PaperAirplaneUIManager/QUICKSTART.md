# PaperAirplane UI Manager - Quick Start Guide

## 5-Minute Quick Start

### Prerequisites Check
- [ ] Windows 7 or later
- [ ] Visual Studio 2015, 2017, or 2019 installed
- [ ] MFC libraries installed (comes with Visual Studio C++ workload)
- [ ] PaperAirplane relay server installed

### Step 1: Build the Application (2 minutes)

1. **Open the solution**
   ```
   Navigate to: src/PaperAirplaneUIManager/
   Double-click: PaperAirplaneUIManager.sln
   ```

2. **Select configuration**
   - Configuration: `Release` (or `Debug` for development)
   - Platform: `Win32` or `x64` (match your system)

3. **Build**
   - Press `Ctrl+Shift+B` or click Build > Build Solution
   - Wait for compilation to complete
   - Executable location: `Release/PaperAirplaneUIManager.exe` (or `Debug/`)

### Step 2: First Run (1 minute)

1. **Launch the application**
   ```
   Double-click: Release/PaperAirplaneUIManager.exe
   ```

2. **Check connection status**
   - Top of window shows: "Status: Connected" (green) or "Status: Disconnected" (red)
   - If disconnected, ensure PaperAirplane relay server DLL is installed

3. **Verify log output**
   - Log window at bottom should show: "Configuration manager initialized successfully"

### Step 3: Basic Configuration (2 minutes)

1. **Enable proxy**
   - Check ☑ "Enable Proxy Client"
   - Check ☑ "Resolve DNS Remote" (optional)

2. **Set proxy server**
   - Proxy Host: `127.0.0.1` (or your SOCKS5 proxy address)
   - Port: `1080` (or your proxy port)

3. **Apply configuration**
   - Click `[Apply Configuration]` button
   - Wait for confirmation: "Configuration applied successfully"
   - Check log for success message

### Step 4: Add Filter Rules (Optional)

#### Add a port filter:
1. Enter port number in "Filter Ports" input box (e.g., `443`)
2. Click `[Add]` button under "Filter Ports"
3. Port appears in the list

#### Add an address filter:
1. Enter IP address in "Filter Addresses" input box (e.g., `192.168.1.1`)
2. Click `[Add]` button under "Filter Addresses"
3. Address appears in the list

#### Add a hostname filter:
1. Enter hostname in "Filter Hostnames" input box (e.g., `example.com`)
2. Click `[Add]` button under "Filter Hostnames"
3. Hostname appears in the list

### Step 5: Save Configuration

1. **Export configuration**
   - Click `[Export Configuration]` button
   - Choose save location
   - Enter filename (e.g., `MyConfig.pap`)
   - Click Save

2. **Import configuration** (when needed)
   - Click `[Import Configuration]` button
   - Select your `.pap` file
   - Configuration loads automatically

## Common Tasks

### Task: Block a specific application
1. Click `[Add]` button under "Blacklist"
2. Browse to application executable (e.g., `C:\Program Files\App\app.exe`)
3. Click Open
4. Application appears in blacklist
5. Click `[Apply Configuration]`

### Task: Change proxy server
1. Update "Proxy Host" field
2. Update "Port" field
3. Click `[Apply Configuration]`
4. New proxy takes effect immediately (no restart needed!)

### Task: View active connections
- Look at "Socket Mappings" list
- Shows all active socket connections
- Updates every second automatically

### Task: Clear all filters
1. Select items in filter lists
2. Click `[Remove]` button for each
3. Or manually clear by removing all items
4. Click `[Apply Configuration]`

### Task: View application logs
- Check the "Log" window at bottom
- Shows all events with timestamps
- Click `[Clear]` to reset log

## Troubleshooting

### Problem: Status shows "Disconnected"
**Solution**: 
- Ensure PaperAirplane relay server DLL is installed
- Check if shared memory objects exist (use Process Explorer)
- Restart the application

### Problem: Configuration not applying
**Solution**:
- Check log window for error messages
- Verify input values (valid IP addresses, port numbers)
- Try running as Administrator
- Click `[Apply Configuration]` after making changes

### Problem: Filters not working
**Solution**:
- Ensure "Enable Proxy Client" is checked
- Verify filter syntax (IP addresses in correct format)
- Click `[Apply Configuration]` after adding filters
- Test with different applications

### Problem: Application won't start
**Solution**:
- Check MFC140.dll is present (or install Visual C++ Redistributable)
- Verify Windows version (Windows 7+)
- Try rebuilding in Debug mode for more details

## Tips and Tricks

### Tip 1: Export before major changes
Always export your working configuration before making significant changes. This allows easy rollback.

### Tip 2: Use descriptive filenames
Name exported configurations descriptively:
- `Work-Proxy.pap`
- `Home-NoFilters.pap`
- `Gaming-HighPort.pap`

### Tip 3: Monitor the log
Keep an eye on the log window to understand what's happening and catch errors early.

### Tip 4: Test incrementally
Add filters one at a time and test to identify which rules work best for your needs.

### Tip 5: Use blacklist wisely
Only blacklist applications that have issues with the proxy. Most applications work fine through the proxy.

## Next Steps

After basic setup, explore these features:

1. **Socket Monitoring**
   - Watch active connections in real-time
   - Identify which applications are using the proxy

2. **Advanced Filtering**
   - Combine port, address, and hostname filters
   - Create complex filtering rules

3. **Configuration Profiles**
   - Create multiple .pap files for different scenarios
   - Quickly switch between configurations

4. **Process Management**
   - Fine-tune which applications use the proxy
   - Build comprehensive whitelist/blacklist

## Getting Help

1. **Documentation**
   - Read `README.md` for comprehensive guide
   - Check `用户指南.md` for Chinese instructions
   - Review `DEVELOPER_GUIDE.md` for technical details

2. **Log Analysis**
   - Log window shows detailed error messages
   - Timestamps help identify when issues occur

3. **Community**
   - Check GitHub issues
   - Review existing configurations
   - Share your experience

## Success Checklist

- [ ] Application builds without errors
- [ ] Application starts and shows UI
- [ ] Status shows "Connected"
- [ ] Can check "Enable Proxy Client"
- [ ] Can enter proxy address and port
- [ ] "Apply Configuration" works
- [ ] Can add/remove filters
- [ ] Can export configuration
- [ ] Can import configuration
- [ ] Log shows messages
- [ ] Filters take effect in applications

If all items are checked, you're ready to use PaperAirplane UI Manager!

---

**Note**: This is a quick start guide. For detailed information, please refer to the complete documentation in README.md and other guide files.

**Estimated Time**: 5-10 minutes for first-time setup  
**Difficulty Level**: Beginner-friendly  
**Support**: See documentation files for detailed help
