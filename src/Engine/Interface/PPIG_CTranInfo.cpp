//-----------------------------------------------------------------------------
// This file is part of PPTactical Engine.                                   //
//                                                                           //
// PPTactical Engine - engine for tactical/strategy games                    //
// Copyright (C) 1998 - 2003 Stefan Dicu & Tudor Girba                       //
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

#pragma hdrstop

#include "PPIG_CTranInfo.h"
#include "PPIG_ObjInfantry.h"
#include "basepath.h"
#include "Paths.h"
#include "PPIG_ObjTran.h"
//---------------------------------------------------------------------------

CPITranInfo::CPITranInfo(CPIWinControl *Parent) : CPIWinControl(Parent)
{
  mTranUnit = 0;
  mSpriteInfantry = GetGraphicInstance()->AddSprite(CPString(PATH_PORTRAITS_GFX) + "inf_icon.png", 1, 1);
}
//---------------------------------------------------------------------------

CPITranInfo::~CPITranInfo()
{
  GetGraphicInstance()->ReleaseSprite(mSpriteInfantry);
}
//---------------------------------------------------------------------------

void CPITranInfo::SetTranUnit(CIGTranUnit* _unit)
{
  if (!_unit)
  {
    mTranUnit = 0;
    SetVisible(false);
    return;
  }

  mTranUnit = _unit;
}
//---------------------------------------------------------------------------

void CPITranInfo::Paint()
{
  if (!mTranUnit)
  {
    return;
  }

  int lCount = mTranUnit->GetCapacity();
  int lX = 0;
  int lY = 0;
  int lL = GetAbsoluteLeft(), lT = GetAbsoluteTop();

  for(int i = 0; i < lCount; i++)
  {
    CIGBaseObject* lUnit = mTranUnit->GetUnit(i);

    if (!lUnit)
    {
      continue;
    }

    if (lUnit->ClassName() == CPString(IGCN_INFANTRYUNIT))
    {
      GetGraphicInstance()->PaintSprite(lL + lX, lT + lY, mSpriteInfantry, 0, 0);
    }

    // coordinates computation
    lX += 32 + 4;

    if (lX > 128)
    {
      lX = 0;
      lY += 32 + 4;

      if (lY > GetHeight())
      {
        return;
      }
    }
  }
}
//---------------------------------------------------------------------------

void CPITranInfo::OnLeftMouseDown(int X, int Y)
{
  CPIWinControl::OnLeftMouseDown(X, Y);

  int lCount = mTranUnit->GetCapacity();
  int lX = 0;
  int lY = 0;

  // convert the click point coordinates into control coordinates
  CPPoint lPointClick = CoordFromScreen(CPPoint(X, Y));

  for(int i = 0; i < lCount; i++)
  {
    CIGBaseObject* lUnit = mTranUnit->GetUnit(i);

    if (!lUnit)
    {
      continue;
    }

    // on click, unload this unit from the transporter
    if ((lX < lPointClick.x) && (lPointClick.x < lX + 32) && (lY < lPointClick.y) && (lPointClick.y < lY + 32))
    {
      //mTranUnit->GetUnit(i);
      TIGSubSysActionData lTmpActionData;
      lTmpActionData.mType = OA_UNLOAD;
      lTmpActionData.mTargetObject = 0;
      lTmpActionData.mTargetPoint = CPPoint(0, 0);
      lTmpActionData.mData[0] = i;
      mTranUnit->SetAction(lTmpActionData);
    }

    // coordinates computation
    lX += 32 + 4;

    if (lX > 128)
    {
      lX = 0;
      lY += 32 + 4;

      if (lY > GetHeight())
      {
        return;
      }
    }
  }
}
//---------------------------------------------------------------------------

#ifdef __BORLANDC__
#pragma package(smart_init)
#endif

