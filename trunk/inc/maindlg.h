// indica -- Notification area indicators for Num Lock, Caps Lock, & Scroll Lock
// Copyright 2010  http://indica.googlecode.com/

// This program is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.

// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.

// You should have received a copy of the GNU General Public License
// along with this program.  If not, see <http://www.gnu.org/licenses/>.


#ifndef MAINDLG_H__3EF87EE5_2FF8_4AAE_95D3_49018892147F
#define MAINDLG_H__3EF87EE5_2FF8_4AAE_95D3_49018892147F


class CMainDlg : public CDialogImpl< CMainDlg >, public CUpdateUI< CMainDlg >,
      public CMessageFilter, public CIdleHandler, public CWinDataExchange< CMainDlg >
{
public:

   enum { IDD = IDD_MAINDLG };

   virtual BOOL PreTranslateMessage( MSG* pMsg );
   virtual BOOL OnIdle();

   BEGIN_UPDATE_UI_MAP( CMainDlg )
   END_UPDATE_UI_MAP()

   BEGIN_MSG_MAP( CMainDlg )
      MESSAGE_HANDLER( WM_INITDIALOG, OnInitDialog )
      MESSAGE_HANDLER( WM_DESTROY, OnDestroy )
      MESSAGE_HANDLER( WM_SYSCOMMAND, OnSysCommand )
      MESSAGE_HANDLER( RWM_LOCKKEYPRESSED, OnLockKeyPressed )
      MESSAGE_HANDLER( RWM_IDENTITY, OnIdentity )
      MESSAGE_HANDLER( RWM_ACTIVATE, OnActivate )
      MESSAGE_HANDLER( RWM_TRAYICON, OnTrayIcon )
      MESSAGE_HANDLER( RWM_TASKBARCREATED, OnTaskbarCreated )
      COMMAND_HANDLER( IDC_NUMLOCK, BN_CLICKED, OnNumLock )
      COMMAND_HANDLER( IDC_CAPSLOCK, BN_CLICKED, OnCapsLock )
      COMMAND_HANDLER( IDC_SCRLOCK, BN_CLICKED, OnScrLock )
      COMMAND_HANDLER( IDC_STARTUP, BN_CLICKED, OnStartup )
      COMMAND_ID_HANDLER( IDCANCEL, OnCancel )
      COMMAND_ID_HANDLER( ID__EXIT, OnCancel )
      COMMAND_ID_HANDLER( ID__PREFERENCES, OnPreferences )
   END_MSG_MAP()

   BEGIN_DDX_MAP( CMainDlg )
      DDX_CONTROL_HANDLE( IDC_NUMLOCK, c_numLock )
      DDX_CONTROL_HANDLE( IDC_CAPSLOCK, c_capsLock )
      DDX_CONTROL_HANDLE( IDC_SCRLOCK, c_scrLock )
      DDX_CONTROL_HANDLE( IDC_STARTUP, c_startup )
   END_DDX_MAP()

   bool NoneSelected() const;

private:

   enum { TIMER_ID_CLICK = 1,
      TIMER_ID_RESTORE = 2,
      TIMER_DELAY_RESTORE = 7000 };

   CButton c_numLock;
   CButton c_capsLock;
   CButton c_scrLock;
   CButton c_startup;

   CIcon numLockIcon_;
   CIcon capsLockIcon_;
   CIcon scrLockIcon_;

   typedef boost::scoped_ptr< TrayIcon > TrayIconPtr;

   TrayIconPtr numLockTray_;
   TrayIconPtr capsLockTray_;
   TrayIconPtr scrLockTray_;

   typedef SmartRefPointer< IShellLinkW > SmartShellLinkPtr;
   typedef SmartRefPointer< IPersistFile > SmartPersistFilePtr;

   Config config_;

   void LoadConfig();
   void ApplyConfig();
   void SaveConfig();

   std::wstring GetAppDataPath();
   std::wstring GetConfigPath();
   std::wstring GetAppPath();
   std::wstring GetShortcutPath();

   void AddIndicator( UINT id );
   void RemoveIndicator( UINT id );
   void SetupTrayIcon( TrayIconPtr& ptr, UINT offId, UINT onId, LPCWSTR offText, LPCWSTR onText );
   void RestoreIndicators();

   void SetAutomatic( bool automatic );

   bool IsKeyOn( UINT vkId );
   bool IsKeyActive( UINT vkId );
   bool IsAutomaticActive();

   bool CreateShortcut( LPCWSTR filePath, LPCWSTR workingDirectory,
      LPCWSTR description, LPCWSTR shortcutPath );
   void RemoveShortcut( LPCWSTR szShortcutPath );

   void ShowTrayIconMenu();

   void CheckHR( HRESULT hr, bool throwOnFailure );

   static CMainDlg* kludge_;
   static void CALLBACK DelayedSingleClick( HWND, UINT, UINT_PTR, DWORD );
   static void CALLBACK DelayedTrayIconRestore(HWND, UINT, UINT_PTR, DWORD);

   void ToggleWindow();

// Handler prototypes (uncomment arguments if needed):
//	LRESULT MessageHandler( UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/ )
//	LRESULT CommandHandler( WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/ )
//	LRESULT NotifyHandler( int /*idCtrl*/, LPNMHDR /*pnmh*/, BOOL& /*bHandled*/ )

   LRESULT OnInitDialog( UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/ );
   LRESULT OnDestroy( UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/ );
   LRESULT OnSysCommand( UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/ );
   LRESULT OnLockKeyPressed( UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/ );
   LRESULT OnIdentity( UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/ );
   LRESULT OnActivate( UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/ );
   LRESULT OnTrayIcon( UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/ );
   LRESULT OnTaskbarCreated( UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/ );
   LRESULT OnNumLock( WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/ );
   LRESULT OnCapsLock( WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/ );
   LRESULT OnScrLock( WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/ );
   LRESULT OnStartup( WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/ );
   LRESULT OnCancel( WORD /*wNotifyCode*/, WORD wID, HWND /*hWndCtl*/, BOOL& /*bHandled*/ );
   LRESULT OnPreferences( WORD /*wNotifyCode*/, WORD wID, HWND /*hWndCtl*/, BOOL& /*bHandled*/ );

   void CloseDialog( int nVal );
};


#endif // MAINDLG_H__3EF87EE5_2FF8_4AAE_95D3_49018892147F
