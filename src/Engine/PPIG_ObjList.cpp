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
 application: Pure Power - in game

 description: Class In Game Object List
 last modify: 08 05 00
        by grabX
---------------------------------------------------------------------------*/
#pragma hdrstop

#include "PPIG_ObjList.h"

CIGBaseObjectList::CIGBaseObjectList()
{
    mList = new CPList<CIGBaseObject>(4);
}
//---------------------------------------------------------------------------

/*---------------------------------------------------------------------------
 description: handles the tick event
---------------------------------------------------------------------------*/
void CIGBaseObjectList::Update()
{
    VLOG(9) << "CIGBaseObjectList::Update - BEGIN";
    for (int i=0; i < mList->Count(); i++)
        (mList->Get(i))->Update();
    VLOG(9) << "CIGBaseObjectList::Update - OVER";
}
//---------------------------------------------------------------------------

/*---------------------------------------------------------------------------
 description: paints the objects
 parameters : ptViewport     - the zone of the screen where the paint is wanted
              rcWindow       - the visible portion of the map
              zoomLevel      - is 1 by default
              paintedObjects - pointer to a list with already painted objects
---------------------------------------------------------------------------*/
void CIGBaseObjectList::Paint(CPPoint ptViewport, CPPoint ptWindow, int Width, int Height, int zoomLevel, CIGBaseObjectList *paintedObjects)
{
//   for (int i=0; i<Count; i++)
    for (int i= mList->Count()-1; i>=0; i--)
//      if (paintedObjects->Contains(((CIGBaseObject *)Items[i])) == false)
    {
        (mList->Get(i))->Paint(ptViewport, ptWindow, Width, Height, zoomLevel);
//         paintedObjects->Add((CIGBaseObject *)Items[i]);
    }
}
//---------------------------------------------------------------------------

/*---------------------------------------------------------------------------
 description: paints the objects selection
 parameters : ptViewport     - the zone of the screen where the paint is wanted
              rcWindow       - the visible portion of the map
              zoomLevel      - is 1 by default
              paintedObjects - pointer to a list with already painted objects
---------------------------------------------------------------------------*/
void CIGBaseObjectList::PaintSelection(CPPoint ptViewport, CPPoint ptWindow, int Width, int Height, int zoomLevel, CIGBaseObjectList *paintedObjects)
{
    if (paintedObjects != NULL)
    {
        for (int i=0; i < mList->Count(); i++)
        {
            if (paintedObjects->Contains(mList->Get(i)) == false)
            {
                (mList->Get(i))->PaintSelection(ptViewport, ptWindow, Width, Height, zoomLevel);
                paintedObjects->Add(mList->Get(i));
            }
        }
    }
    else
        for (int i = 0; i < mList->Count(); i++)
        {
            (mList->Get(i))->PaintSelection(ptViewport, ptWindow, Width, Height, zoomLevel);
        }
}
//---------------------------------------------------------------------------

/*---------------------------------------------------------------------------
 description: paints the objects health bar
 parameters : ptViewport     - the zone of the screen where the paint is wanted
              rcWindow       - the visible portion of the map
              zoomLevel      - is 1 by default
              paintedObjects - pointer to a list with already painted objects
---------------------------------------------------------------------------*/
void CIGBaseObjectList::PaintHealthBar(CPPoint ptViewport, CPPoint ptWindow, int Width, int Height, int zoomLevel, CIGBaseObjectList *paintedObjects)
{
    /*
       if (paintedObjects != NULL)
       {
          for (int i=0; i<Count; i++)
             if (paintedObjects->Contains(((CIGBaseObject *)Items[i])) == false)
             {
                ((CIGBaseObject *)Items[i])->PaintHealthBar(ptViewport, ptWindow, Width, Height, zoomLevel);
                paintedObjects->Add((CIGBaseObject *)Items[i]);
             }
       }
       else
          for (int i=0; i<Count; i++)
             ((CIGBaseObject *)Items[i])->PaintHealthBar(ptViewport, ptWindow, Width, Height, zoomLevel);

    */
}
//---------------------------------------------------------------------------

/*---------------------------------------------------------------------------
 description: paints the objects on the radar
 parameters : ptViewport     - the zone of the screen where the paint is wanted
              rcWindow       - the visible portion of the map
              zoomLevel      - is 1 by default
              paintedObjects - pointer to a list with already painted objects
---------------------------------------------------------------------------*/
void CIGBaseObjectList::PaintMinimized(CPPoint ptViewport, CPPoint ptWindow, int Width, int Height, int zoomLevel)
{
    for (int i = 0; i < mList->Count(); i++)
    {
        (mList->Get(i))->PaintMinimized(ptViewport, ptWindow, Width, Height, zoomLevel);
    }
}
//---------------------------------------------------------------------------

/*---------------------------------------------------------------------------
 description: make the objects in the list - selected or not
 parameters : value - shows
---------------------------------------------------------------------------*/
void CIGBaseObjectList::SetSelected(bool value)
{
    for (int i = 0; i < mList->Count(); i++)
        (mList->Get(i))->SetSelected(value);
}
//---------------------------------------------------------------------------

/*---------------------------------------------------------------------------
 description: sets the action to all the objects in the list
 parameters : Action - the one to be set
---------------------------------------------------------------------------*/
void CIGBaseObjectList::SetAction(TIGSubSysActionData actionData)
{
    for (int i = 0; i < mList->Count(); i++)
        (mList->Get(i))->SetAction(actionData);
}
//---------------------------------------------------------------------------

/*---------------------------------------------------------------------------
 description: gets the center point of the objects in the list
 others     : the result is in logic coord
---------------------------------------------------------------------------*/
CPPoint CIGBaseObjectList::GetCenterPoint()
{
    CPPoint lPoint = CPPoint(0, 0);
    for (int i=0; i < mList->Count(); i++)
    {
        lPoint.x += (mList->Get(i))->GetLogicCoord().x;
        lPoint.y += (mList->Get(i))->GetLogicCoord().y;
    }

    lPoint.x = lPoint.x / mList->Count();
    lPoint.y = lPoint.y / mList->Count();
    return lPoint;
}
//---------------------------------------------------------------------------

/*---------------------------------------------------------------------------
 description: gets the bit mask of the objects able actions
---------------------------------------------------------------------------*/
__int32 CIGBaseObjectList::GetActionMask()
{
    __int32 lMask = 0;
    if (mList->Count() > 0)
    {
        lMask = (mList->Get(0))->GetActionMask();
        for (int i=1; i < mList->Count(); i++)
            lMask = lMask & (mList->Get(i))->GetActionMask();
    }
    return lMask;
}
//---------------------------------------------------------------------------

/*---------------------------------------------------------------------------
 description: sets the team for all objects in the list
---------------------------------------------------------------------------*/
void CIGBaseObjectList::SetTeam(int Team)
{
    for (int i = 0; i < mList->Count(); i++)
    {
        if (!mList->Get(i)->GetIsDead()) //Karg : a dead unit cannot be set to a team
        {
            (mList->Get(i))->SetTeam(Team);
        }
    }
}
//---------------------------------------------------------------------------

/*---------------------------------------------------------------------------
 description: remove the object if it exists or add if not
 parameters : Object - the subject
---------------------------------------------------------------------------*/
void CIGBaseObjectList::AddRemove(CIGBaseObject *Object)
{
    /*
    if (IndexOf(Object) == -1)
       Add(Object);
    else
       Remove(Object);
    */
    for(int i = 0; i < mList->Count(); i++)
    {
        if (Object == mList->Get(i))
        {
            mList->Remove(i);
            return;
        }
    }

    mList->Add(Object);
}
//---------------------------------------------------------------------------

/*---------------------------------------------------------------------------
 description: add a certain object to the list
 parameters : Object - the subject
---------------------------------------------------------------------------*/
void CIGBaseObjectList::Add(CIGBaseObject *Object)
{
    //TList::Add(Object);
    //logWriteLn("CIGBaseObjectList::Add");
    mList->Add(Object);
}
//---------------------------------------------------------------------------

/*---------------------------------------------------------------------------
 description: add a certain object to the list only if it's not in it
 parameters : Object - the subject
---------------------------------------------------------------------------*/
void CIGBaseObjectList::AddUnique(CIGBaseObject *Object)
{
    /*
       if (Contains(Object) == false)
          TList::Add(Object);
    */
    if (mList->Contain(Object) == false)
        mList->Add(Object);
}
//---------------------------------------------------------------------------

/*---------------------------------------------------------------------------
 description: add a certain object to the list only if it's not in it
 parameters : Object - the subject
---------------------------------------------------------------------------*/
void CIGBaseObjectList::AddUnique(CIGBaseObjectList *objList)
{
    /*
       for (int i=0; i<objList->Count; i++)
          if (Contains(objList->GetObject(i)) == false)
             TList::Add(objList->GetObject(i));
    */
    for (int i=0; i < objList->Count(); i++)
        if (Contains(objList->GetObject(i)) == false)
            mList->Add(objList->GetObject(i));
}
//---------------------------------------------------------------------------

/*---------------------------------------------------------------------------
 description: remove a certain object from the list
 parameters : Object - the subject
---------------------------------------------------------------------------*/
void CIGBaseObjectList::Remove(CIGBaseObject *Object)
{
    //TList::Remove(Object);
    for(int i = 0; i < mList->Count(); i++)
    {
        if (Object == mList->Get(i))
        {
            mList->Remove(i);
            return;
        }
    }
}
//---------------------------------------------------------------------------

/*---------------------------------------------------------------------------
 description: remove all the objects from the list
---------------------------------------------------------------------------*/
void CIGBaseObjectList::RemoveAll()
{
    mList->Clear();
}
//---------------------------------------------------------------------------

/*---------------------------------------------------------------------------
 description: remove an object and frees it from memory
---------------------------------------------------------------------------*/
void CIGBaseObjectList::Free(CIGBaseObject *Object)
{
    /*
       Remove(Object);
       Capacity = Count;
    */
    for(int i = 0; i < mList->Count(); i++)
    {
        if (Object == mList->Get(i))
        {
            mList->Delete(i);
            return;
        }
    }
}
//---------------------------------------------------------------------------

/*---------------------------------------------------------------------------
 description: clear the list and frees the objects memory
---------------------------------------------------------------------------*/
void CIGBaseObjectList::FreeAll()
{
    /*
       CIGBaseObject *lObj;
       for (int i=0; i<Count; i++)
       {
          lObj = GetObject(i);
          Items[i] = NULL;
          delete lObj;
       }
       Pack();
       Capacity = Count;
    */
//   logWriteLn("CIGBaseObjectList::FreeAll");

    while(mList->Count() > 0)
    {
        mList->Delete(0);
    }

    /*
    int lCount = mList->Count();

    for(int i = 0; i < lCount; i++)
    {
      mList->Delete(0);
    }
    */
}
//---------------------------------------------------------------------------

int  CIGBaseObjectList::Count()
{
    return mList->Count();
}

/*---------------------------------------------------------------------------
 description: gets the Index object
 parameters : Index - index of the object in list
---------------------------------------------------------------------------*/
CIGBaseObject *CIGBaseObjectList::operator[](int Index)
{
    return mList->Get(Index);
}
//---------------------------------------------------------------------------

/*---------------------------------------------------------------------------
 description: gets the Index object
 parameters : Index - index of the object in list
---------------------------------------------------------------------------*/
CIGBaseObject *CIGBaseObjectList::GetObject(int Index)
{
    //return (CIGBaseObject *)Items[Index];
    return mList->Get(Index);
}
//---------------------------------------------------------------------------

bool CIGBaseObjectList::ContainsOnly(CIGBaseObject *Object)
{
    return mList->Contain(Object) && mList->Count() == 1;
}
//---------------------------------------------------------------------------

bool CIGBaseObjectList::Contains(CIGBaseObject *Object)
{
    return mList->Contain(Object);
}
//---------------------------------------------------------------------------

bool CIGBaseObjectList::IsEmpty()
{
    return (mList->Count() <= 0);
}
//---------------------------------------------------------------------------

bool CIGBaseObjectList::CanAny(int capability)
{
    int listCount = mList->Count();

    for(int i = 0; i < listCount; i++)
    {
        if (mList->Get(i)->Can(capability))
        {
            return true;
        }
    }

    return false;
}
//---------------------------------------------------------------------------

bool CIGBaseObjectList::SupportsAction(int action)
{
    int actionMask = GetActionMask();

    return (actionMask & (1 << action)) != 0;
}
//---------------------------------------------------------------------------

CIGBaseObjectList::~CIGBaseObjectList()
{
    try
    {
        delete mList;
    }
    catch(...)
    {
        LOG(ERROR) << "CIGBaseObjectList::~CIGBaseObjectList - cannot delete internal list";
    }
}
//---------------------------------------------------------------------------

#ifdef __BORLANDC__
#pragma package(smart_init)
#endif

