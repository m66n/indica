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


#ifndef UTILITY_H__C555AE47_14AE_4A06_9E32_8C0CE6240683
#define UTILITY_H__C555AE47_14AE_4A06_9E32_8C0CE6240683


#include <windef.h>
#include <boost\noncopyable.hpp>


class Utility : boost::noncopyable
{
public:

   static BYTE Bool2Byte( bool a, bool b, bool c, bool d )
   {
      BYTE value = 0;

      value |= ( a ? BIT0 : 0 );
      value |= ( b ? BIT1 : 0 );
      value |= ( c ? BIT2 : 0 );
      value |= ( d ? BIT3 : 0 );

      return value;
   }

   static void Byte2Bool( BYTE byte, bool& a, bool& b, bool& c, bool& d )
   {
      a = ( ( byte & BIT0 ) == BIT0 );
      b = ( ( byte & BIT1 ) == BIT1 );
      c = ( ( byte & BIT2 ) == BIT2 );
      d = ( ( byte & BIT3 ) == BIT3 );
   }

   static bool FileExists( LPCWSTR fileName )
   {
      return ( GetFileAttributesW( fileName ) != INVALID_FILE_ATTRIBUTES );
   }

private:

   Utility();
   ~Utility();

   enum
   {
      BIT0 = 1,
      BIT1 = 1 << 1,
      BIT2 = 1 << 2,
      BIT3 = 1 << 3,
      BIT4 = 1 << 4,
      BIT5 = 1 << 5,
      BIT6 = 1 << 6,
      BIT7 = 1 << 7
   };
};


#endif // UTILITY_H__C555AE47_14AE_4A06_9E32_8C0CE6240683
