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


#ifndef CONFIG_H__B28B9F0E_4468_49A4_8932_B554925B7A68
#define CONFIG_H__B28B9F0E_4468_49A4_8932_B554925B7A68


#include "utility.h"
#include <boost\noncopyable.hpp>


class Config : boost::noncopyable
{
public:

   Config() : numLockActive_( true ), capsLockActive_( true ),
      scrollLockActive_( false ), automatic_( true ) {}

   explicit Config( BYTE byte )
   {
      Set( byte );
   }

   void Set( BYTE byte )
   {
      bool a;
      bool b;
      bool c;
      bool d;

      Utility::Byte2Bool( byte, a, b, c, d );

      SetNumLockActive( a );
      SetCapsLockActive( b );
      SetScrollLockActive( c );
      SetAutomatic( d );
   }

   void SetNumLockActive( bool active ) { numLockActive_ = active; }
   void SetCapsLockActive( bool active ) { capsLockActive_ = active; }
   void SetScrollLockActive( bool active ) { scrollLockActive_ = active; }
   void SetAutomatic( bool automatic ) { automatic_ = automatic; }

   bool IsNumLockActive() const { return numLockActive_; }
   bool IsCapsLockActive() const { return capsLockActive_; }
   bool IsScrollLockActive() const { return scrollLockActive_; }
   bool IsAutomatic() const { return automatic_; }

   operator BYTE()
   {
      return Utility::Bool2Byte( IsNumLockActive(), IsCapsLockActive(),
      IsScrollLockActive(), IsAutomatic() );
   }

private:

   bool numLockActive_;
   bool capsLockActive_;
   bool scrollLockActive_;
   bool automatic_;
};


#endif // CONFIG_H__B28B9F0E_4468_49A4_8932_B554925B7A68
