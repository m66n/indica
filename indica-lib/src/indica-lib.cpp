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
#include "indica-lib.h"


#pragma data_seg( ".indica-lib" )
HWND g_hwnd = NULL;
HHOOK g_hhk = NULL;
#pragma data_seg()

#pragma comment( linker, "/SECTION:.indica-lib,RWS" )


HINSTANCE g_hInstance = NULL;


LRESULT CALLBACK HookProc( int, WPARAM, LPARAM );


BOOL APIENTRY DllMain( HMODULE hModule,
                       DWORD ul_reason_for_call,
                       LPVOID /*lpReserved*/ )
{
   if ( DLL_PROCESS_ATTACH == ul_reason_for_call )
   {
      g_hInstance = reinterpret_cast< HINSTANCE >( hModule );
   }

   return TRUE;
}


INDICALIB_API bool Hook( HWND hwndCaller )
{
   g_hwnd = hwndCaller;

   g_hhk = SetWindowsHookEx( WH_KEYBOARD_LL, reinterpret_cast< HOOKPROC >( HookProc ),
      g_hInstance, 0 );

   return ( NULL != g_hhk );
}


INDICALIB_API bool Unhook()
{
   return ( UnhookWindowsHookEx( g_hhk ) != 0 );
}


LRESULT CALLBACK HookProc( int code, WPARAM wParam, LPARAM lParam )
{
   if ( code >= 0 )
   {
      LPKBDLLHOOKSTRUCT pKbDllHookStruct = reinterpret_cast< LPKBDLLHOOKSTRUCT >( lParam );

      UINT vkId = pKbDllHookStruct->vkCode;

      bool numLock = ( VK_NUMLOCK == vkId );
      bool capsLock = ( VK_CAPITAL == vkId );
      bool scrollLock = ( VK_SCROLL  == vkId );

      bool keyUp = ( ( pKbDllHookStruct->flags & LLKHF_UP ) == LLKHF_UP );

      if ( ( numLock || capsLock || scrollLock ) && keyUp )
      {
         PostMessage( g_hwnd, RWM_LOCKKEYPRESSED, NULL, vkId );
      }
   }

   return CallNextHookEx( g_hhk, code, wParam, lParam );
}
