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
//---------------------------------------------------------------------------
#ifndef PPIG_ObjListH
#define PPIG_ObjListH

#include "PPIG_ObjBase.h"
#include "KList.h"

//class CIGBaseObjectList: public TList
class CIGBaseObjectList
{
   private:
      // TMN: Make sure we prevent slicing
      CIGBaseObjectList(const CIGBaseObjectList& rhs);	// no impl.
      void operator=(const CIGBaseObjectList& rhs);	// no impl.

      CPList<CIGBaseObject> *mList;

   public:
      //CIGBaseObjectList(): TList() {}
      CIGBaseObjectList();
      void Update();
      void Paint(CPPoint ptViewport, CPPoint ptWindow, int Width, int Height, int zoomLevel, CIGBaseObjectList *paintedObjects);
      void PaintSelection(CPPoint ptViewport, CPPoint ptWindow, int Width, int Height, int zoomLevel, CIGBaseObjectList *paintedObjects);
      void PaintHealthBar(CPPoint ptViewport, CPPoint ptWindow, int Width, int Height, int zoomLevel, CIGBaseObjectList *paintedObjects);
      void PaintMinimized(CPPoint ptViewport, CPPoint ptWindow, int Width, int Height, int zoomLevel);
      void SetSelected(bool value);
      void SetAction(TIGSubSysActionData actionData);
      CPPoint GetCenterPoint();
      __int32 GetActionMask();

      void SetTeam(int Team);

      void AddRemove(CIGBaseObject *Object);
      void Add(CIGBaseObject *Object);
      void AddUnique(CIGBaseObject *Object);
      void AddUnique(CIGBaseObjectList *objList);

      void Remove(CIGBaseObject *Object);
      void RemoveAll();

      void Free(CIGBaseObject *Object);
      void FreeAll();

      int  Count();

      CIGBaseObject *operator[](int Index);
      CIGBaseObject *GetObject(int Index);

      bool ContainsOnly(CIGBaseObject *Object);
      bool Contains(CIGBaseObject *Object);
      bool IsEmpty();

      bool CanAny(int capability); /**< checks if any of the objects in the list has the capability */
      bool SupportsAction(int action);/**< checks if the objects in the list support the action by checking against CIGBaseObject::mActionMask */

      ~CIGBaseObjectList();
};

//---------------------------------------------------------------------------
#endif

