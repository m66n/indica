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


CAppModule _Module;


int Run( LPTSTR /*lpstrCmdLine*/ = NULL, int nCmdShow = SW_SHOWDEFAULT )
{
   CMessageLoop theLoop;
   _Module.AddMessageLoop( &theLoop );

   CMainDlg dlgMain;

   if ( dlgMain.Create( NULL ) == NULL )
   {
      ATLTRACE( _T("Main dialog creation failed!\n") );
      return 0;
   }

   int nRet = theLoop.Run();

   _Module.RemoveMessageLoop();

   return nRet;
}


BOOL CALLBACK EnumWindowsProc( HWND hwnd, LPARAM lParam )
{
   DWORD_PTR result = NULL;

   BOOL success = static_cast< BOOL >( SendMessageTimeout( hwnd, RWM_IDENTITY, 0, 0,
      SMTO_BLOCK | SMTO_ABORTIFHUNG, 100, &result ) );

   if ( success && ( RWM_IDENTITY == result ) )
   {
      HWND* pTarget = reinterpret_cast< HWND* >( lParam );
      *pTarget = hwnd;
      return FALSE;
   }

   return TRUE;
}


int WINAPI _tWinMain( HINSTANCE hInstance, HINSTANCE /*hPrevInstance*/, LPTSTR lpstrCmdLine, int nCmdShow )
{
   HANDLE hMutexSingleInstance = CreateMutexW( NULL, FALSE,
      L"INDICA__136CA0E0_9ACD_45D2_BA2B_01BFA009D3F0" );

   if ( GetLastError() == ERROR_ALREADY_EXISTS ||
        GetLastError() == ERROR_ACCESS_DENIED )
   {
      HWND hwndOther = NULL;

      EnumWindows( EnumWindowsProc, reinterpret_cast< LPARAM >( &hwndOther ) );

      if ( NULL != hwndOther )
      {
         PostMessage( hwndOther, RWM_ACTIVATE, 0, 0 );
      }

      return -1;
   }

   HRESULT hRes = ::CoInitialize( NULL );
   ATLASSERT( SUCCEEDED( hRes ) );

   AtlInitCommonControls( ICC_BAR_CLASSES );  // add flags to support other controls

   hRes = _Module.Init( NULL, hInstance );
   ATLASSERT( SUCCEEDED( hRes ) );

   int nRet = Run( lpstrCmdLine, nCmdShow );

   _Module.Term();

   ::CoUninitialize();

   return nRet;
}
