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


#ifndef TRAYICON_H__036583B1_2D5B_4F1C_9A73_514006342E08
#define TRAYICON_H__036583B1_2D5B_4F1C_9A73_514006342E08


#include <shellapi.h>
#include <string>
#include <boost\noncopyable.hpp>


const UINT RWM_TRAYICON = RegisterWindowMessageW( L"RWM_TRAYICON__8A97930C_B6C6_4386_A1EF_ABCAFACE35E6" );


class SafeIcon : boost::noncopyable
{
public:

   explicit SafeIcon( UINT id );
   ~SafeIcon();

   inline operator HICON() { return hIcon_; }

private:

   HICON hIcon_;
};


struct TrayIconInfo
{
   UINT offId;
   UINT onId;
   std::wstring offText;
   std::wstring onText;
};


class TrayIcon
{
public:

   explicit TrayIcon( const TrayIconInfo& info );
   ~TrayIcon();

   void Add( HWND hwnd, UINT id, bool on );
   void Remove();
   void SetState( bool on );

private:

   SafeIcon offIcon_;
   SafeIcon onIcon_;
   std::wstring offText_;
   std::wstring onText_;

   NOTIFYICONDATAW nid_;
};


#endif // TRAYICON_H__036583B1_2D5B_4F1C_9A73_514006342E08
