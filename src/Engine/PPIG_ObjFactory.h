//-----------------------------------------------------------------------------
// This file is part of PPTactical Engine.                                   //
//                                                                           //
// PPTactical Engine - engine for tactical/strategy games                    //
// Copyright (C) 1998-2005 Stefan Dicu & Tudor Girba                         //
//                                                                           //
// PPTactical Engine is free software; you can redistribute it               //
// and/or modify it under the terms of the GNU Lesser General Public License as     //
// published by the Free Software Foundation; either version 2.1 of the      //
// License, or (at your option) any later version.                           //
//                                                                           //
// PPTactical Engine is distributed in the hope that it will be useful, but  //
// WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY//
// or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU Lesser General Public License  //
// for more details.                                                         //
//                                                                           //
// You should have received a copy of the GNU Lesser General Public License  //
// along with PPTactical Engine; if not, write to the Free Software          //
// Foundation Inc. 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA   //
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
//  Unit              : Static Factory
//
//  Version           :
//
//  Description       :
//    * object that is able to build units (it's not a factory in the design
//      patterns :)
//    * for the moment they do not handle any OA_TACTIC_XXX
//
//  History           :
//    [20.07.2005] - [Karg]  - created
//-----------------------------------------------------------------------------

#ifndef PPIG_ObjFactoryH
#define PPIG_ObjFactoryH
//---------------------------------------------------------------------------

#include "PPIG_ObjStatic.h"

class CIGSubSysBuilder;

class CIGFactoryObject : public CIGStaticObject
{
  public:
    CIGFactoryObject();
    virtual ~CIGFactoryObject();

    virtual CPString ClassName () { return IGCN_FACTORYOBJECT; }
    virtual bool IsDerivedFrom (CPString className);
    virtual void Update ();
    virtual void Paint(CPPoint ptViewPort, CPPoint ptWindow, int Width, int Height, int zoomLevel = 1);

  public: // action
    virtual void AddAction(TIGSubSysActionData actionData);
    virtual void AddAction (CIGSubSysAction *Action);
    virtual void AddActionWithPriority (CIGSubSysAction *Action);
    virtual void SetAction (TIGSubSysActionData actionData);

  public: // get-set
    CPPoint GetRallyPoint() { return mRallyPoint; }
    bool    IsBuilding();
    int     GetProgress();

  public:
    virtual void DeSerialize(PP::Stream& _a);

  private:
    CPPoint mRallyPoint;
    CPGISprite* mRallyFlagSprite;
    CIGSubSysBuilder* mBuilder;
};

#endif
