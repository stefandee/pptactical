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

 description: Class In Game Object Event Manager
 last modify: 09 07 00
        by grabX
---------------------------------------------------------------------------*/

//---------------------------------------------------------------------------
#pragma hdrstop

#include "PPIG_ObjEventManager.h"

/*---------------------------------------------------------------------------
 description: constructor
---------------------------------------------------------------------------*/
CIGObjEventManager::CIGObjEventManager()
{
   //mEvents = new tList;
   mEvents = new CPList<CIGObjectEvent>(32);
}
//---------------------------------------------------------------------------

/*---------------------------------------------------------------------------
 description: destructor
---------------------------------------------------------------------------*/
CIGObjEventManager::~CIGObjEventManager()
{
   delete mEvents;
}
//---------------------------------------------------------------------------

/*---------------------------------------------------------------------------
 description: updates the events
              sends the events to all the
 others     :
---------------------------------------------------------------------------*/
void CIGObjEventManager::Update()
{

}
//---------------------------------------------------------------------------

/*---------------------------------------------------------------------------
 description: adds an event to the list
---------------------------------------------------------------------------*/
void CIGObjEventManager::NoteEvent(CIGObjectEvent *Event)
{
   mEvents->Add(Event);
}
//---------------------------------------------------------------------------

/////////////////////////////////////////////////////////////////////////////
//PROTECTED SECTION
/////////////////////////////////////////////////////////////////////////////
/*---------------------------------------------------------------------------
 description: deletes all events and frees the memory
---------------------------------------------------------------------------*/
void CIGObjEventManager::FreeAll()
{
   for(int i = 0; i < mEvents->Count(); i++)
   {
     mEvents->Delete(i);
   }  
}
//---------------------------------------------------------------------------

#ifdef __BORLANDC__
#pragma package(smart_init)
#endif
