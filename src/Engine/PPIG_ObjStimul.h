//-----------------------------------------------------------------------------
// This file is part of PPTactical Engine.                                   //
//                                                                           //
// PPTactical Engine - engine for tactical/strategy games                    // 
// Copyright (C) 1998, 1999, 2000, 2001 Stefan Dicu & Tudor Girba            //
//                                                                           //
// PPTactical Engine is free software; you can redistribute it               //
// and/or modify it under the terms of the GNU Lesser General Public License as     // 
// published by the Free Software Foundation; either version 2.1 of the        //
// License, or (at your option) any later version.                           //
//                                                                           //  
// PPTactical Engine is distributed in the hope that it will be useful, but  //
// WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY// 
// or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU Lesser General Public License  //
// for more details.                                                         //
//                                                                           // 
// You should have received a copy of the GNU Lesser General Public License         //
// along with PPTactical Engine; if not, write to the Free Software          //
// Foundation Inc. 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA   // 
//-----------------------------------------------------------------------------
/*---------------------------------------------------------------------------
 application: Pure Power In Game

 description: Class In Game Stimul
 last modify: 14 05 00
        by grabX
---------------------------------------------------------------------------*/
//---------------------------------------------------------------------------
#ifndef PPIG_ObjStimulH
#define PPIG_ObjStimulH

#include "basic2d.h"
#include "Stream.h"

class CIGBaseObject;

class CIGObjectStimul
{
   public:
      CIGObjectStimul();
      CIGObjectStimul(int type, int value, CIGBaseObject *sourceObject, CPPoint originPoint);
      ~CIGObjectStimul();

  public:
    //## Get and Set Operations for Class Attributes (generated)
      virtual int GetType ();
      virtual void SetType (int value);

      virtual int GetValue ();
      virtual void SetValue (int value);

      virtual CIGBaseObject * GetSourceObject ();
      virtual void SetSourceObject (CIGBaseObject * value);

      virtual CPPoint GetOriginPoint();
      virtual void SetOriginPoint(CPPoint _v);

  public: // serialization - deserialization
    virtual void Serialize(PP::Stream& _a);
    virtual unsigned __int32 DataSize();
    virtual void DeSerialize(PP::Stream& _a);

  private: //## implementation
    // Data Members for Class Attributes
      int            mType;
      int            mValue;
      CIGBaseObject *mSourceObject;
      CPPoint        mOriginPoint;
};

/////////////////////////////////////////////////////////////////////////////
//GET-SET (INLINE) SECTION
/////////////////////////////////////////////////////////////////////////////
/*---------------------------------------------------------------------------
 description: Type
---------------------------------------------------------------------------*/
inline int CIGObjectStimul::GetType ()
{
  return mType;
}

inline void CIGObjectStimul::SetType (int value)
{
  mType = value;
}

/*---------------------------------------------------------------------------
 description: Value
---------------------------------------------------------------------------*/
inline int CIGObjectStimul::GetValue ()
{
  return mValue;
}

inline void CIGObjectStimul::SetValue (int value)
{
  mValue = value;
}

/*---------------------------------------------------------------------------
 description: SourceObject
---------------------------------------------------------------------------*/
inline CIGBaseObject * CIGObjectStimul::GetSourceObject ()
{
  return mSourceObject;
}

inline void CIGObjectStimul::SetSourceObject (CIGBaseObject * value)
{
  mSourceObject = value;
}

/*---------------------------------------------------------------------------
 description: origin point
---------------------------------------------------------------------------*/
inline CPPoint CIGObjectStimul::GetOriginPoint()
{
  return mOriginPoint;
}
//---------------------------------------------------------------------------

inline void CIGObjectStimul::SetOriginPoint(CPPoint _v)
{
  mOriginPoint = _v;
}
//---------------------------------------------------------------------------

#endif

