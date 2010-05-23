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


#ifndef FILE_H__023C64FE_FCAF_43CF_B0B1_B3BECD5DA3A4
#define FILE_H__023C64FE_FCAF_43CF_B0B1_B3BECD5DA3A4


#include <boost\noncopyable.hpp>


class File : boost::noncopyable
{
public:

   explicit File( LPCWSTR szPath ) : handle_( CreateFileW( szPath, GENERIC_READ | GENERIC_WRITE,
      0, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL ) )
   {
      if ( INVALID_HANDLE_VALUE == handle_ )
      {
         CreateDefaultFile( szPath );
      }
   }

   ~File()
   {
      CloseHandle( handle_ );
   }

   bool ReadByte( BYTE& value, BYTE default )
   {
      DWORD bytesRead = 0;

      ReadFile( handle_, &value, 1, &bytesRead, NULL );

      if ( 1 != bytesRead )
      {
         value = default;
         return false;
      }

      return true;
   }

   bool WriteByte( BYTE value )
   {
      DWORD bytesWritten = 0;
      WriteFile( handle_, &value, 1, &bytesWritten, NULL );
      return ( 1 == bytesWritten );
   }

   void Reset()
   {
      SetFilePointer( handle_, 0, 0, FILE_BEGIN );
   }

private:

   HANDLE handle_;

   void CreateDefaultFile( LPCWSTR szPath )
   {
      handle_ = CreateFileW( szPath, GENERIC_READ | GENERIC_WRITE,
         0, NULL, OPEN_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL );
   }
};


#endif // FILE_H__023C64FE_FCAF_43CF_B0B1_B3BECD5DA3A4
