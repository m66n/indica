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


#ifndef INDICA_LIB_H__575EF9D6_28AD_4855_AB40_019D8C7FCB50
#define INDICA_LIB_H__575EF9D6_28AD_4855_AB40_019D8C7FCB50


#ifdef INDICALIB_EXPORTS
#define INDICALIB_API __declspec(dllexport)
#else
#define INDICALIB_API __declspec(dllimport)
#endif


const UINT RWM_LOCKKEYPRESSED = RegisterWindowMessageW( L"RWM_LOCKKEYPRESSED__655FD8EB_63F1_4C06_8E44_D6A7FA5DC5C9" );


INDICALIB_API bool Hook( HWND );
INDICALIB_API bool Unhook();


#endif // INDICA_LIB_H__575EF9D6_28AD_4855_AB40_019D8C7FCB50
