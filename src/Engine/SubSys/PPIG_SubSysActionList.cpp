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

 description: Class In Game Action List
              Class In Game Object Action
                  this is created by the object that give the order
                  the handle is always set by the object
 last modify: 09 06 00
        by grabX
---------------------------------------------------------------------------*/
//---------------------------------------------------------------------------
#pragma hdrstop

#include "PPIG_SubSysActionList.h"

/*---------------------------------------------------------------------------
 description: constructor
---------------------------------------------------------------------------*/
//CIGSubSysActionList::CIGSubSysActionList() : TList()
CIGSubSysActionList::CIGSubSysActionList()
{
  mList = new CPList<CIGSubSysAction>(4);
}

/*---------------------------------------------------------------------------
 description: adds an action to the list
 parameters : Action - the action
---------------------------------------------------------------------------*/
void CIGSubSysActionList::Add(CIGSubSysAction* Action)
{
   //TList::Add(Action);
   mList->Add(Action);
}

/*---------------------------------------------------------------------------
 description: delete an action to the list
 parameters : index - index of action
---------------------------------------------------------------------------*/
/*
void CIGSubSysActionList::Delete(int index)
{
  mList->Delete(index);
}
*/

/*---------------------------------------------------------------------------
 description: delete an action to the list
 parameters : index - index of action
---------------------------------------------------------------------------*/
void CIGSubSysActionList::Remove(int index)
{
  mList->Remove(index);
}

/*---------------------------------------------------------------------------
 description: frees all the actions in the list
---------------------------------------------------------------------------*/
void CIGSubSysActionList::FreeAll()
{
   //Clear();
   //Capacity = 0;
   for(int i = 0; i < mList->Count(); i++)
   {
     mList->Delete(0);
   }

   //mList->Clear();
}

/*
int CIGSubSysActionList::Count()
{
  return mList->Count();
}
*/

/*---------------------------------------------------------------------------
 description: returns true if the list is empty
---------------------------------------------------------------------------*/
bool CIGSubSysActionList::IsEmpty()
{
   //return (Count <= 0);
   return (mList->Count() <= 0);
}

/*---------------------------------------------------------------------------
 description: gets the Index Action
 parameters : Index - no of the wanted action
---------------------------------------------------------------------------*/
CIGSubSysAction* CIGSubSysActionList::GetAction(int Index)
{
   //return ((CIGSubSysAction*)Items[Index]);
   return mList->Get(Index);
}

/*---------------------------------------------------------------------------
 description: gets the Index Action
 parameters : Index - no of the wanted action
---------------------------------------------------------------------------*/
CIGSubSysAction* CIGSubSysActionList::operator[](int Index)
{
   //return ((CIGSubSysAction*)Items[Index]);
   return (mList->Get(Index));
}
//---------------------------------------------------------------------------

CIGSubSysActionList::~CIGSubSysActionList()
{
  delete mList;
}
//---------------------------------------------------------------------------

#ifdef __BORLANDC__
#pragma package(smart_init)
#endif

