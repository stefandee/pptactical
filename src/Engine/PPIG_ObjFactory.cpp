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

#pragma hdrstop

#include "PPIG_ObjFactory.h"
#include "PPIG_SubSysBuilder.h"
#include "PPIG_Mission.h"
#include "PP_Graphic.h"
#include "Paths.h"
#include "basepath.h"
//---------------------------------------------------------------------------

CIGFactoryObject::CIGFactoryObject() : CIGStaticObject()
{
  SetActionMask((1<<MA_BUILD)|
                (1<<MA_RALLY_POINT));

  mRallyPoint = CPPoint(-1, -1);

  // add the flag effect
  mRallyFlagSprite = GetGraphicInstance()->AddSprite(CPString(PATH_EXPLODES_GFX) + "flag.png", 1, 1);

  // subsystems
  mBuilder = new CIGSubSysBuilder(this);
}
//---------------------------------------------------------------------------

CIGFactoryObject::~CIGFactoryObject()
{
  delete mBuilder;
  mBuilder = 0;
}
//---------------------------------------------------------------------------

/*---------------------------------------------------------------------------
 description: returns true if the object is derived from the className
---------------------------------------------------------------------------*/
bool CIGFactoryObject::IsDerivedFrom (CPString className)
{
   return (className == CIGFactoryObject::ClassName() || CIGStaticObject::IsDerivedFrom(className));
}
//---------------------------------------------------------------------------

/*---------------------------------------------------------------------------
 description: handles the tick event
---------------------------------------------------------------------------*/
void CIGFactoryObject::Update()
{
  mBuilder->Update();

  CIGStaticObject::Update();
}
//---------------------------------------------------------------------------

void CIGFactoryObject::Paint(CPPoint ptViewPort, CPPoint ptWindow, int Width, int Height, int zoomLevel)
{
  CIGStaticObject::Paint(ptViewPort, ptWindow, Width, Height, zoomLevel);

  /*
  if (mRallyPoint.x != -1)
  {
    GetGraphicInstance()->Circle
    (
      0,
      CPPoint(mRallyPoint.x + (ptViewPort.x - ptWindow.x)/zoomLevel,
              mRallyPoint.y + (ptViewPort.y - ptWindow.y)/zoomLevel),
      30,
      0xFFFFFF
    );
  }
  */


  if (mRallyPoint.x != -1 && mRallyPoint.y != -1)
  {
    GetGraphicInstance()->PaintSprite
    (
      mRallyPoint.x + (ptViewPort.x - ptWindow.x)/zoomLevel - 16,
      mRallyPoint.y + (ptViewPort.y - ptWindow.y)/zoomLevel - 84,
      mRallyFlagSprite,
      0,
      0
    );
  }
}
//---------------------------------------------------------------------------

void CIGFactoryObject::AddAction(TIGSubSysActionData actionData)
{
  switch (actionData.mType)
  {
    case OA_BUILD:
      break;

    case OA_RALLY_POINT:
      break;
  }
}
//---------------------------------------------------------------------------

void CIGFactoryObject::AddAction (CIGSubSysAction *Action)
{
  switch (Action->GetType())
  {
    case OA_BUILD:
      break;

    case OA_RALLY_POINT:
      break;
  }
}
//---------------------------------------------------------------------------

void CIGFactoryObject::AddActionWithPriority (CIGSubSysAction *Action)
{
  switch (Action->GetType())
  {
    case OA_BUILD:
      break;

    case OA_RALLY_POINT:
      break;
  }
}
//---------------------------------------------------------------------------

void CIGFactoryObject::SetAction (TIGSubSysActionData actionData)
{
  switch (actionData.mType)
  {
    case OA_BUILD:
      mBuilder->SetAction(actionData);
      break;

    case OA_RALLY_POINT:
      mRallyPoint = actionData.mTargetPoint;
      break;
  }
}
//---------------------------------------------------------------------------

bool CIGFactoryObject::IsBuilding()
{
  return mBuilder->GetCurrentActionIndex() == OA_BUILD;
}
//---------------------------------------------------------------------------

int  CIGFactoryObject::GetProgress()
{
  return mBuilder->GetProgress();
}
//---------------------------------------------------------------------------

void CIGFactoryObject::DeSerialize(PP::Stream& _a)
{
  CIGStaticObject::DeSerialize(_a);

  // Hack: when the factory will be inserted into the map editor, this line may
  // be removed
  SetActionMask((1<<MA_BUILD)|
                (1<<MA_RALLY_POINT));
}
//---------------------------------------------------------------------------

#ifdef __BORLANDC__
#pragma package(smart_init)
#endif

