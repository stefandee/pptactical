//-----------------------------------------------------------------------------
// This file is part of PPTactical Engine.                                   //
//                                                                           //
// PPTactical Engine - engine for tactical/strategy games                    //
// Copyright (C) 1998-2004 Stefan Dicu                                       //
//                                                                           //
// PPTactical Engine is free software; you can redistribute it               //
// and/or modify it under the terms of the GNU Lesser General Public License //
// as published by the Free Software Foundation; either version 2.1 of the   //
// License, or (at your option) any later version.                           //
//                                                                           //
// PPTactical Engine is distributed in the hope that it will be useful, but  //
// WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY//
// or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU Lesser General Public   //
// License for more details.                                                 //
//                                                                           //
// You should have received a copy of the GNU Lesser General Public License  //
// along with PPTactical Engine; if not, write to the Free Software          //
// Foundation Inc. 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA   //
//-----------------------------------------------------------------------------

#pragma hdrstop

#include "PPIG_SerObjList.h"
#include "PPIG_ObjList.h"
#include "PPIG_ObjectCreator.h"
//---------------------------------------------------------------------------

void CIGSerBaseObjectList::Serialize(PP::Stream& _a)
{
  CSerInt         lObjCount(mBaseObjectList->Count());
  CIGBaseObject   *lObject;

  lObjCount.Serialize(_a);

  VLOG(5) << ".Saving objects... (" << lObjCount.Get() << ")";

  // serialization of objects is done in two steps:
  // 1. write objects type and id
  // 2. actual serialization (writing of data)
  // it is done this way because of the cross references that may exist
  // between objects (an unit holds a pointer to another unit)
  // i think that this should solve circular references
  for(int i = 0; i < lObjCount.Get(); i++)
  {
   lObject = mBaseObjectList->GetObject(i);

   CSerInt lObjType(CIGObjectCreator().ObjectClassNameToId(lObject->ClassName()));

   // write the object type
   lObjType.Serialize(_a);

   // write the object id
   CSerInt lId(lObject->GetId());
   lId.Serialize(_a);
  }

  // now follows the actual serialization
  for(int i = 0; i < mBaseObjectList->Count(); i++)
  {
   lObject = mBaseObjectList->GetObject(i);
   lObject->Serialize(_a);
   VLOG(5) << "..Object written";
  }

  VLOG(5) << ".Objects written succesfully";
  //Objects - over
}
//---------------------------------------------------------------------------

unsigned __int32 CIGSerBaseObjectList::DataSize()
{
  return 0;
}
//---------------------------------------------------------------------------

void CIGSerBaseObjectList::DeSerialize(PP::Stream& _a)
{
  CSerInt         lObjCount, lObjType, lId;
  CIGBaseObject   *lObject;

  lObjCount.DeSerialize(_a);

  VLOG(5) << ".Loading objects... (" << lObjCount.Get() << ")";

  // deserialization of objects is done in two steps:
  // 1. * read objects type and id
  //    * create the object instance depending on the type and set id
  //    * add the unit to unit list
  // 2. actual deserialization
  // it is done this way because of the cross references that exist
  // between objects (an unit holds a pointer to another unit)
  // at serialization, an unit writes the referenced unit id, while at
  // deserialization it asks the Mission to return a pointer to that object
  for(int i = 0; i < lObjCount.Get(); i++)
  {
    lObjType.DeSerialize(_a);

    try
    {
      lObject = CIGObjectCreator().CreateObjectFromId(lObjType.Get());
    }
    catch(...)
    {
      // simply assert, the map is fucked-up :D
    }

    lId.DeSerialize(_a);
    lObject->SetId(lId.Get());

    mBaseObjectList->Add(lObject);
  }

  // now follows the deserialization
  for(int i = 0; i < mBaseObjectList->Count(); i++)
  {
    lObject = mBaseObjectList->GetObject(i);

    // just to make sure we didn't screw something up somewhere
    if (!lObject)
    {
     continue;
    }

    VLOG(5) << lObject->ClassName();

    lObject->DeSerialize(_a);

    VLOG(5) << "..Object loaded";
  }
}
//---------------------------------------------------------------------------

#ifdef PPT_COMPILER_BORLAND
#pragma package(smart_init)
#endif
