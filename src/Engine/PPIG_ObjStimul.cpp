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

#pragma hdrstop
#include "PPIG_Mission.h"
#include "PPIG_ObjStimul.h"

/*---------------------------------------------------------------------------
 description: constructor
---------------------------------------------------------------------------*/
CIGObjectStimul::CIGObjectStimul()
{
   mValue        = 0;
   mType         = -1;
   mSourceObject = 0;
   mOriginPoint  = CPPoint(-1, -1);
}
//---------------------------------------------------------------------------

/*---------------------------------------------------------------------------
 description: custom constructor
---------------------------------------------------------------------------*/
CIGObjectStimul::CIGObjectStimul(int type, int value, CIGBaseObject *sourceObject, CPPoint originPoint)
{
   mValue        = value;
   mType         = type;
   mSourceObject = sourceObject;
   mOriginPoint  = originPoint;
}
//---------------------------------------------------------------------------

/*---------------------------------------------------------------------------
 description: destructor
---------------------------------------------------------------------------*/
CIGObjectStimul::~CIGObjectStimul()
{
}
//---------------------------------------------------------------------------


//---------------------------------------------------------------------------
//
// Class serialize/deserialize section
//
//---------------------------------------------------------------------------

void CIGObjectStimul::Serialize(PP::Stream& _a)
{
  int lSourceObjectId = -1;
  CSerPoint        lOriginPoint(mOriginPoint);

  // setup
  if (mSourceObject)
  {
    lSourceObjectId = mSourceObject->GetId();
  }

  // serialize
  _a.writeInt(&mType);
  _a.writeInt(&mValue);
  _a.writeInt(&lSourceObjectId);
  lOriginPoint.Serialize(_a);
}
//---------------------------------------------------------------------------

unsigned __int32 CIGObjectStimul::DataSize()
{
  return 3 * sizeof(int) + CSerPoint().DataSize();
}
//---------------------------------------------------------------------------

void CIGObjectStimul::DeSerialize(PP::Stream& _a)
{
  int       lSourceObjectId;
  CSerPoint lOriginPoint;

  // deserialize
  _a.readInt(&mType         );
  _a.readInt(&mValue        );
  _a.readInt(&lSourceObjectId);
  lOriginPoint.DeSerialize(_a);

  // setup
  mSourceObject = GetMissionInstance()->GetObjectById(lSourceObjectId);
  mOriginPoint  = lOriginPoint.Get();
}
//---------------------------------------------------------------------------

#ifdef __BORLANDC__
#pragma package(smart_init)
#endif

