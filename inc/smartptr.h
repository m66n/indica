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


#ifndef SMARTPTR_H__53934DCF_F053_434C_AAB4_3B2496B335FE
#define SMARTPTR_H__53934DCF_F053_434C_AAB4_3B2496B335FE


template < class T >
class SmartRefPointer
{
private:

   T* m_interface;

   void Reset()
   {
      if ( m_interface )
      {
         m_interface->Release();
      }

      m_interface = NULL;
   }

   void AddReference()
   {
      if ( m_interface )
      {
          m_interface->AddRef();
      }
   }

public:

   SmartRefPointer()
   {
      m_interface = NULL;
   }

   SmartRefPointer( T* rhs )
   {
      m_interface = rhs;
      AddReference();
   }

   SmartRefPointer( const SmartRefPointer& rhs )
   {
      m_interface = rhs.m_interface;
      AddReference();
   }

   ~SmartRefPointer()
   {
      Reset();
   }

   T* Get() const
   {
      return m_interface;
   }

   T* Release()
   {
      T* retval = m_interface;
      m_interface = NULL;
      return retval;
   }

   void Clear()
   {
      Reset();
   }

   T** GetInterfacePointer()
   {
      Reset();
      return &m_interface;
   }

   SmartRefPointer& operator =( const SmartRefPointer& rhs )
   {
      Reset();
      m_interface = rhs.m_interface;
      AddReference();
      return *this;
   }

   T* operator ->() const
   {
      return m_interface;
   }

   operator const T*() const
   {
      return m_interface;
   }
};


#endif // SMARTPTR_H__53934DCF_F053_434C_AAB4_3B2496B335FE
