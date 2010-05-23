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
#include "trayicon.h"


SafeIcon::SafeIcon( UINT id ) :
   hIcon_( reinterpret_cast< HICON >( LoadImage( GetModuleHandle( NULL ),
      MAKEINTRESOURCE( id ), IMAGE_ICON, GetSystemMetrics( SM_CXSMICON ),
      GetSystemMetrics( SM_CYSMICON ), LR_LOADTRANSPARENT ) ) )
{
}


SafeIcon::~SafeIcon()
{
   DestroyIcon( hIcon_ );
}



TrayIcon::TrayIcon( const TrayIconInfo& info ) : offIcon_( info.offId ),
   onIcon_( info.onId ), offText_( info.offText ), onText_( info.onText )
{
   ZeroMemory( &nid_, sizeof( nid_ ) );
   nid_.cbSize = sizeof( nid_ );
}


TrayIcon::~TrayIcon()
{
   Remove();
}


void TrayIcon::Add( HWND hwnd, UINT id, bool on )
{
   nid_.hWnd = hwnd;
   nid_.hIcon = ( on ? onIcon_ : offIcon_ );
   nid_.uID = id;
   nid_.uFlags = NIF_MESSAGE | NIF_ICON | NIF_TIP;
   nid_.uCallbackMessage = RWM_TRAYICON;

   wcscpy_s( nid_.szTip, _countof( nid_.szTip ),
      on ? onText_.c_str() : offText_.c_str() );

   Shell_NotifyIconW( NIM_ADD, &nid_ );
}


void TrayIcon::Remove()
{
   Shell_NotifyIconW( NIM_DELETE, &nid_ );
}


void TrayIcon::SetState( bool on )
{
   nid_.hIcon = ( on ? onIcon_ : offIcon_ );
   nid_.uFlags = NIF_ICON | NIF_TIP;

   wcscpy_s( nid_.szTip, _countof( nid_.szTip ),
      on ? onText_.c_str() : offText_.c_str() );

   Shell_NotifyIconW( NIM_MODIFY, &nid_ );
}
