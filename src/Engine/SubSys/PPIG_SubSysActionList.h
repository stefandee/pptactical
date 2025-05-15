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

 description: Class In Game Object Action List
              Class In Game Object Action
                  this is created by the object that give the order
                  the handle is always set by the object
 last modify: 09 06 00
        by grabX
---------------------------------------------------------------------------*/
//---------------------------------------------------------------------------
#ifndef PPIG_SubSysActionListH
#define PPIG_SubSysActionListH

#include "PPIG_SubSysAction.h"
#include "KList.h"

/*---------------------------------------------------------------------------
 description: class CIGObjectActionList
---------------------------------------------------------------------------*/
class CIGSubSysActionList/*: public TList*/
{
   public:
      CIGSubSysActionList();
      ~CIGSubSysActionList();

      void Add(CIGSubSysAction* Action);
      //void Delete(int index);
      void Remove(int index);
      void FreeAll();
      //int Count();
      bool IsEmpty();
      CIGSubSysAction* GetAction(int Index);
      CIGSubSysAction* operator[](int Index);

   private:
     CPList<CIGSubSysAction> *mList;   
};

//---------------------------------------------------------------------------
#endif

