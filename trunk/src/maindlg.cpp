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


#include "stdafx.h"
#include "resource.h"
#include "constants.h"
#include "file.h"
#include "config.h"
#include "trayicon.h"
#include "..\indica-lib\inc\indica-lib.h"
#include "messages.h"
#include "smartptr.h"
#include "maindlg.h"

#include <comdef.h>


CMainDlg* CMainDlg::kludge_ = NULL;


BOOL CMainDlg::PreTranslateMessage( MSG* pMsg )
{
   return CWindow::IsDialogMessage( pMsg );
}


BOOL CMainDlg::OnIdle()
{
   return FALSE;
}


LRESULT CMainDlg::OnInitDialog( UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/ )
{
   CenterWindow();

   HICON hIcon = AtlLoadIconImage( IDR_MAINFRAME, LR_DEFAULTCOLOR,
      ::GetSystemMetrics( SM_CXICON ), ::GetSystemMetrics( SM_CYICON ) );
   SetIcon( hIcon, TRUE );

   HICON hIconSmall = AtlLoadIconImage( IDR_MAINFRAME, LR_DEFAULTCOLOR,
      ::GetSystemMetrics( SM_CXSMICON ), ::GetSystemMetrics( SM_CYSMICON ) );
   SetIcon( hIconSmall, FALSE );

   CMessageLoop* pLoop = _Module.GetMessageLoop();
   ATLASSERT( NULL != pLoop );
   pLoop->AddMessageFilter( this );
   pLoop->AddIdleHandler( this );

   UIAddChildWindowContainer( m_hWnd );

   kludge_ = this;

   DoDataExchange( FALSE );

   numLockIcon_.LoadIcon( IDI_NUMOFF, ::GetSystemMetrics( SM_CXSMICON ),
      ::GetSystemMetrics( SM_CYSMICON ) );
   capsLockIcon_.LoadIcon( IDI_CAPSOFF, ::GetSystemMetrics( SM_CXSMICON ),
      ::GetSystemMetrics( SM_CYSMICON ) );
   scrLockIcon_.LoadIcon( IDI_SCROFF, ::GetSystemMetrics( SM_CXSMICON ),
      ::GetSystemMetrics( SM_CYSMICON ) );

   c_numLock.SetIcon( numLockIcon_ );
   c_capsLock.SetIcon( capsLockIcon_ );
   c_scrLock.SetIcon( scrLockIcon_ );

   LoadConfig();

   Hook( m_hWnd );

   ShowWindow( NoneSelected() ? SW_SHOWDEFAULT : SW_HIDE );

   return TRUE;
}


LRESULT CMainDlg::OnDestroy( UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/ )
{
   Unhook();

   CMessageLoop* pLoop = _Module.GetMessageLoop();
   ATLASSERT( NULL != pLoop );
   pLoop->RemoveMessageFilter( this );
   pLoop->RemoveIdleHandler( this );

   return 0;
}


void CMainDlg::CloseDialog( int nVal )
{
   DestroyWindow();
   ::PostQuitMessage( nVal );
}


LRESULT CMainDlg::OnCancel( WORD /*wNotifyCode*/, WORD wID, HWND /*hWndCtl*/, BOOL& /*bHandled*/ )
{
   CloseDialog( wID );
   return 0;
}


void CMainDlg::LoadConfig()
{
   File file( GetConfigPath().c_str() );

   BYTE value = 0;

   if ( !file.ReadByte( value, config_ ) )
   {
      file.Reset();
      file.WriteByte( value );
   }
   else
   {
      config_.Set( value );
   }

   ApplyConfig();
}


void CMainDlg::ApplyConfig()
{
   RestoreIndicators();

   c_numLock.SetCheck( config_.IsNumLockActive() ? BST_CHECKED : BST_UNCHECKED );
   c_capsLock.SetCheck( config_.IsCapsLockActive() ? BST_CHECKED : BST_UNCHECKED );
   c_scrLock.SetCheck( config_.IsScrollLockActive() ? BST_CHECKED : BST_UNCHECKED );

   SetAutomatic( config_.IsAutomatic() );

   c_startup.SetCheck( config_.IsAutomatic() ? BST_CHECKED : BST_UNCHECKED );
}


std::wstring CMainDlg::GetAppDataPath()
{
   WCHAR szPath[MAX_PATH];

   HRESULT hr = SHGetFolderPathW( NULL, CSIDL_APPDATA, NULL,
      SHGFP_TYPE_CURRENT, szPath );

   if ( SUCCEEDED( hr ) )
   {
      PathAppendW( szPath, Constants::ApplicationName() );
      SHCreateDirectoryExW( NULL, szPath, NULL );
   }

   return szPath;
}


std::wstring CMainDlg::GetConfigPath()
{
   WCHAR szPath[MAX_PATH];

   size_t numChars = GetAppDataPath().copy( szPath, MAX_PATH );

   szPath[numChars] = L'\0';

   PathAppendW( szPath, Constants::ConfigFilename() );

   return szPath;
}


void CMainDlg::AddIndicator( UINT vkId )
{
   switch ( vkId )
   {
   case VK_NUMLOCK:
      SetupTrayIcon( numLockTray_, IDI_NUMOFF, IDI_NUMON,
         Constants::NumLockOff(), Constants::NumLockOn() );
      numLockTray_->Add( m_hWnd, VK_NUMLOCK, IsKeyOn( VK_NUMLOCK ) );
      config_.SetNumLockActive( true );
      break;

   case VK_CAPITAL:
      SetupTrayIcon( capsLockTray_, IDI_CAPSOFF, IDI_CAPSON,
         Constants::CapsLockOff(), Constants::CapsLockOn() );
      capsLockTray_->Add( m_hWnd, VK_CAPITAL, IsKeyOn( VK_CAPITAL ) );
      config_.SetCapsLockActive( true );
      break;

   case VK_SCROLL:
      SetupTrayIcon( scrLockTray_, IDI_SCROFF, IDI_SCRON,
         Constants::ScrollLockOff(), Constants::ScrollLockOn() );
      scrLockTray_->Add( m_hWnd, VK_SCROLL, IsKeyOn( VK_SCROLL ) );
      config_.SetScrollLockActive( true );
      break;
   }

   SaveConfig();
}


void CMainDlg::RemoveIndicator( UINT vkId )
{
   switch ( vkId )
   {
   case VK_NUMLOCK:
      config_.SetNumLockActive( false );
      numLockTray_.reset();
      break;
   case VK_CAPITAL:
      config_.SetCapsLockActive( false );
      capsLockTray_.reset();
      break;
   case VK_SCROLL:
      config_.SetScrollLockActive( false );
      scrLockTray_.reset();
      break;
   }

   SaveConfig();
}


void CMainDlg::SetAutomatic( bool automatic )
{
   if ( automatic )
   {
      std::wstring path( GetAppPath() );

      WCHAR szDirectory[MAX_PATH];
      GetCurrentDirectoryW( MAX_PATH, szDirectory );

      std::wstring shortcutPath( GetShortcutPath() );

      CreateShortcut( path.c_str(), szDirectory,
         Constants::Description(), shortcutPath.c_str() );
   }
   else
   {
      RemoveShortcut( GetShortcutPath().c_str() );
   }

   config_.SetAutomatic( IsAutomaticActive() );

   SaveConfig();
}


bool CMainDlg::CreateShortcut( LPCWSTR filePath, LPCWSTR workingDirectory,
                    LPCWSTR description, LPCWSTR shortcutPath )
{
   SmartShellLinkPtr shellPtr;

   CheckHR( CoCreateInstance( CLSID_ShellLink, NULL, CLSCTX_INPROC_SERVER,
      IID_IShellLink, reinterpret_cast< LPVOID* >( shellPtr.GetInterfacePointer() ) ), true );

   SmartPersistFilePtr filePtr;

   CheckHR( shellPtr->QueryInterface( IID_IPersistFile,
      reinterpret_cast< LPVOID* >( filePtr.GetInterfacePointer() ) ), true );
   CheckHR( shellPtr->SetPath( filePath ), true );
   CheckHR( shellPtr->SetWorkingDirectory( workingDirectory ), true );
   CheckHR( shellPtr->SetDescription( description ), true );

   CheckHR( filePtr->Save( shortcutPath, TRUE ), true );

   return true;
}


void CMainDlg::CheckHR( HRESULT hr, bool throwOnFailure )
{
   if ( SUCCEEDED( hr ) )
   {
      return;
   }

   if ( throwOnFailure )
   {
      throw _com_error( hr );
   }
}


LRESULT CMainDlg::OnLockKeyPressed( UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM lParam, BOOL& /*bHandled*/ )
{
   UINT vkId = static_cast< UINT >( lParam );

   switch ( vkId )
   {
   case VK_NUMLOCK:
      if ( numLockTray_ )
      {
         numLockTray_->SetState( IsKeyOn( VK_NUMLOCK ) );
      }
      break;
   case VK_CAPITAL:
      if ( capsLockTray_ )
      {
         capsLockTray_->SetState( IsKeyOn( VK_CAPITAL ) );
      }
      break;
   case VK_SCROLL:
      if ( scrLockTray_ )
      {
         scrLockTray_->SetState( IsKeyOn( VK_SCROLL ) );
      }
      break;
   }

   return 0;
}


void CMainDlg::SetupTrayIcon( TrayIconPtr& ptr, UINT offId, UINT onId, LPCWSTR offText, LPCWSTR onText )
{
   if ( !ptr )
   {
      TrayIconInfo info;
      info.offId = offId;
      info.onId = onId;
      info.offText = offText;
      info.onText = onText;

      ptr.reset( new TrayIcon( info ) );
   }
}


bool CMainDlg::IsKeyOn( UINT vkId )
{
   SHORT result = GetKeyState( vkId );
   return ( ( result & 0x01 ) == 0x01 );
}


void CMainDlg::SaveConfig()
{
   File file( GetConfigPath().c_str() );
   file.WriteByte( config_ );
}


LRESULT CMainDlg::OnNumLock( WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/ )
{
   if ( c_numLock.GetCheck() == BST_CHECKED )
   {
      AddIndicator( VK_NUMLOCK );
   }
   else
   {
      RemoveIndicator( VK_NUMLOCK );
   }

   return 0;
}


LRESULT CMainDlg::OnCapsLock( WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/ )
{
   if ( c_capsLock.GetCheck() == BST_CHECKED )
   {
      AddIndicator( VK_CAPITAL );
   }
   else
   {
      RemoveIndicator( VK_CAPITAL );
   }

   return 0;
}


LRESULT CMainDlg::OnScrLock( WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/ )
{
   if ( c_scrLock.GetCheck() == BST_CHECKED )
   {
      AddIndicator( VK_SCROLL );
   }
   else
   {
      RemoveIndicator( VK_SCROLL );
   }

   return 0;
}


LRESULT CMainDlg::OnStartup( WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/ )
{
   bool checked = ( c_startup.GetCheck() == BST_CHECKED );

   SetAutomatic( checked );

   c_startup.SetCheck( IsAutomaticActive() ? BST_CHECKED : BST_UNCHECKED );

   return 0;
}


LRESULT CMainDlg::OnIdentity( UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/ )
{
   return RWM_IDENTITY;
}


LRESULT CMainDlg::OnActivate( UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/ )
{
   return 0;
}


bool CMainDlg::IsKeyActive( UINT vkId )
{
   switch ( vkId )
   {
   case VK_NUMLOCK:
      return numLockTray_;

   case VK_CAPITAL:
      return capsLockTray_;

   case VK_SCROLL:
      return scrLockTray_;
   }

   return false;
}


std::wstring CMainDlg::GetAppPath()
{
   WCHAR szPath[MAX_PATH];

   GetModuleFileNameW( NULL, szPath, MAX_PATH );
   szPath[MAX_PATH-1] = L'\0';

   return szPath;
}


void CMainDlg::RemoveShortcut( LPCWSTR szShortcutPath )
{
   if ( !DeleteFileW( szShortcutPath ) )
   {
      DWORD dwError = GetLastError();
      if ( ERROR_PATH_NOT_FOUND != dwError &&
           ERROR_FILE_NOT_FOUND != dwError )
      {
         CheckHR( HRESULT_FROM_WIN32( dwError ), false );
      }
   }
}


std::wstring CMainDlg::GetShortcutPath()
{
   WCHAR szShortcutPath[MAX_PATH];

   CheckHR( SHGetFolderPathW( NULL, CSIDL_STARTUP, NULL,
      SHGFP_TYPE_CURRENT, szShortcutPath ), true );

   wcscat_s( szShortcutPath, _countof( szShortcutPath ), L"\\" );
   wcscat_s( szShortcutPath, _countof( szShortcutPath ), Constants::ShortcutName() );
   wcscat_s( szShortcutPath, _countof( szShortcutPath ), L".lnk" );

   return szShortcutPath;
}


bool CMainDlg::IsAutomaticActive()
{
   return Utility::FileExists( GetShortcutPath().c_str() );
}


LRESULT CMainDlg::OnTrayIcon( UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM lParam, BOOL& /*bHandled*/ )
{
   switch ( LOWORD( lParam ) )
   {
   case WM_RBUTTONUP:
      ShowTrayIconMenu();
      break;
      
   case WM_LBUTTONDOWN:

      SetTimer( TIMER_ID_CLICK, GetDoubleClickTime(), DelayedSingleClick );
      break;

   case WM_LBUTTONDBLCLK:
      KillTimer( TIMER_ID_CLICK );
      CloseDialog( 0 );
      break;
   }

   return 0;
}


void CMainDlg::ShowTrayIconMenu()
{
   CMenu menu;
   menu.LoadMenu( IDR_TRAYMENU );

   if ( menu.IsNull() )
   {
      return;
   }

   SetForegroundWindow( m_hWnd );

   CMenuHandle popup = menu.GetSubMenu( 0 );

   if ( !popup.IsNull() )
   {
      CPoint ptCursor;
      GetCursorPos( &ptCursor );

      popup.SetMenuDefaultItem( ID__EXIT, FALSE );

      CMenuItemInfo mii;
      popup.GetMenuItemInfo( ID__PREFERENCES, FALSE, &mii );

      mii.fMask = MIIM_STATE;
      mii.fState = ( IsWindowVisible() ? MFS_DISABLED : MFS_ENABLED );

      popup.SetMenuItemInfo( ID__PREFERENCES, FALSE, &mii );

      popup.TrackPopupMenu( TPM_LEFTALIGN, ptCursor.x, ptCursor.y, m_hWnd, NULL );

      SendMessage( WM_NULL, 0, 0 );
   }
}


void CALLBACK CMainDlg::DelayedSingleClick( HWND, UINT, UINT_PTR id, DWORD )
{
   if ( kludge_ )
   {
      kludge_->KillTimer( id );
      kludge_->ToggleWindow();
   }
}


bool CMainDlg::NoneSelected() const
{
   return ( ( c_numLock.GetCheck() != BST_CHECKED ) &&
            ( c_capsLock.GetCheck() != BST_CHECKED ) &&
            ( c_scrLock.GetCheck() != BST_CHECKED ) );
}


LRESULT CMainDlg::OnPreferences( WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/ )
{
   ToggleWindow();
   return 0;
}


LRESULT CMainDlg::OnTaskbarCreated( UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/ )
{
   RestoreIndicators();
   return 0;
}


void CMainDlg::RestoreIndicators()
{
   // Sometimes the tray icons are not visible at startup. Maybe a short delay will help.
   //
   SetTimer(TIMER_ID_RESTORE, TIMER_DELAY_RESTORE, DelayedTrayIconRestore);

   //config_.IsScrollLockActive() ? AddIndicator( VK_SCROLL ) : RemoveIndicator( VK_SCROLL );
   //config_.IsCapsLockActive() ? AddIndicator( VK_CAPITAL ) : RemoveIndicator( VK_CAPITAL );
   //config_.IsNumLockActive() ? AddIndicator( VK_NUMLOCK ) : RemoveIndicator( VK_NUMLOCK );
}


void CMainDlg::ToggleWindow()
{
   int showCmd = ( IsWindowVisible() ? SW_HIDE : SW_SHOW );
   ShowWindow( showCmd );
}


LRESULT CMainDlg::OnSysCommand( UINT /*uMsg*/, WPARAM wParam, LPARAM /*lParam*/, BOOL& bHandled )
{
   bHandled = FALSE;

   if ( SC_MINIMIZE == wParam )
   {
      if ( !NoneSelected() )
      {
         ShowWindow( SW_HIDE );
         bHandled = TRUE;
      }
   }

   return 0;
}


void CALLBACK CMainDlg::DelayedTrayIconRestore(HWND, UINT, UINT_PTR, DWORD)
{
   if ( kludge_ )
   {
      kludge_->config_.IsScrollLockActive() ? kludge_->AddIndicator( VK_SCROLL ) : kludge_->RemoveIndicator( VK_SCROLL );
      kludge_->config_.IsCapsLockActive() ? kludge_->AddIndicator( VK_CAPITAL ) : kludge_->RemoveIndicator( VK_CAPITAL );
      kludge_->config_.IsNumLockActive() ? kludge_->AddIndicator( VK_NUMLOCK ) : kludge_->RemoveIndicator( VK_NUMLOCK );
   }
}
