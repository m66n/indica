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


#ifndef CONSTANTS_H__6D1ECBFE_FC6E_4F00_98C7_CED20F0BCF27
#define CONSTANTS_H__6D1ECBFE_FC6E_4F00_98C7_CED20F0BCF27


#include <boost\noncopyable.hpp>


class Constants : boost::noncopyable
{
public:

   static LPCWSTR ApplicationName() { return L"indica"; }

   static LPCWSTR CapsLockOff() { return L"CapsLock: Off";  }
   static LPCWSTR CapsLockOn() { return L"CapsLock: On";  }

   static LPCWSTR NumLockOff() { return L"NumLock: Off";  }
   static LPCWSTR NumLockOn() { return L"NumLock: On";  }

   static LPCWSTR ScrollLockOff() { return L"ScrLk: Off";  }
   static LPCWSTR ScrollLockOn() { return L"ScrLk: On";  }

   static LPCWSTR ShortcutName() { return L"indica"; }

   static LPCWSTR Description() { return L"Notification area indicators for Num Lock, Caps Lock, & Scroll Lock"; }

   static LPCWSTR ConfigFilename() { return L"indica.config"; }

private:

   Constants();
   ~Constants();
};


#endif // CONSTANTS_H__6D1ECBFE_FC6E_4F00_98C7_CED20F0BCF27
