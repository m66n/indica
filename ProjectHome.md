Notification Area Icons:
![http://indica.googlecode.com/svn-history/r4/images/tray-icons.png](http://indica.googlecode.com/svn-history/r4/images/tray-icons.png)

Preferences:
![http://indica.googlecode.com/svn-history/r4/images/control-panel.png](http://indica.googlecode.com/svn-history/r4/images/control-panel.png)

### Installation ###
  1. Download ZIP file for your architecture and extract to a directory of your choosing (i.e. _C:\Program Files\indica_).
  1. Launch program by double-clicking on _indica.exe_.
  1. Choose which indicators you want in the notification area and if you want to create a shortcut in the Startup folder.
  1. Click the minimize button to dismiss the configuration dialog. Clicking the close button will exit the application and remove the icons from the notification area.
  * Click on an icon in the notification area to show the configuration dialog.
  * Double-click on an icon to exit the application and remove the icons from the notification area.
  * Right-click on an icon to show a menu of options.

Note: You may need the Visual Studio 2010 SP1 Redistributable Package ([64-bit](http://www.microsoft.com/en-us/download/details.aspx?id=13523) or [32-bit](http://www.microsoft.com/en-us/download/details.aspx?id=8328))

Note: Make sure that "Notification Area Icons" settings in Windows allow for "Show icons and notifications" for each indica icon.

Note: There is a seven second delay in showing the notification area icons after the program is launched. This addresses icons not showing in the notification area at Windows startup or after Windows Explorer crashes.

Note: Configuration data for indica is stored in _%APPDATA%\indica_.

Note: Windows startup link is stored in _%APPDATA%\Microsoft\Windows\Start Menu\Programs\Startup_.

### Build Environment ###
  * Visual Studio 2010 SP1
  * Windows Template Library (WTL)
  * Boost 1.51 (header files only; no compilation necessary)

### Attribution ###
  * Gorilla icon courtesy of [Jakub Steiner](http://art.gnome.org/themes/icon/1297).  Used under [Creative Commons Attribution-Share Alike 2.0 Generic license](http://creativecommons.org/licenses/by-sa/2.0/).