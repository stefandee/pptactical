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

#include "PPIG_CGroupInfo.h"
#include "PPIG_ObjBase.h"
#include "basepath.h"
#include "Paths.h"
#include "PPIG_Mission.h"
//---------------------------------------------------------------------------
#ifdef PPT_COMPILER_BORLAND
#pragma package(smart_init)
#endif

CPIGroupInfo::CPIGroupInfo(CPIWinControl *Parent) : CPIWinControl(Parent)
{
  mList = new CPList<CIGBaseObject>(4);

  mSpriteInfantry = GetGraphicInstance()->AddSprite(CPString(PATH_PORTRAITS_GFX) + "inf_icon.png", 1, 1);
  mSpriteVehicle  = GetGraphicInstance()->AddSprite(CPString(PATH_PORTRAITS_GFX) + "vech_icon.png", 1, 1);
  mSpriteFighter  = GetGraphicInstance()->AddSprite(CPString(PATH_PORTRAITS_GFX) + "fighter_icon.png", 1, 1);
  mSpriteChopper  = GetGraphicInstance()->AddSprite(CPString(PATH_PORTRAITS_GFX) + "heli_icon.png", 1, 1);
  mSpriteDropship = GetGraphicInstance()->AddSprite(CPString(PATH_PORTRAITS_GFX) + "dropship_icon.png", 1, 1);
}
//---------------------------------------------------------------------------

CPIGroupInfo::~CPIGroupInfo()
{
  delete mList;
  mList = 0;
}
//---------------------------------------------------------------------------

void CPIGroupInfo::RemoveAll()
{
  mList->Clear();
}
//---------------------------------------------------------------------------

void CPIGroupInfo::AddUnit(CIGBaseObject* _unit)
{
  if (!_unit)
  {
    return;
  }

  mList->Add(_unit);
}
//---------------------------------------------------------------------------

void CPIGroupInfo::Paint()
{
  int lCount = mList->Count();
  int lX = 0;
  int lY = 0;
  int  lL = GetAbsoluteLeft(), lT = GetAbsoluteTop();

  SetClipRegion();

  for(int i = 0; i < lCount; i++)
  {
    CIGBaseObject* lUnit = mList->Get(i);

    if (!lUnit)
    {
      continue;
    }

    if (lUnit->ClassName() == CPString(IGCN_INFANTRYUNIT))
    {
      GetGraphicInstance()->PaintSprite(lL + lX, lT + lY, mSpriteInfantry, 0, 0);
    }

    if (lUnit->ClassName() == CPString(IGCN_ARTILLERYUNIT) || lUnit->ClassName() == CPString(IGCN_TANKUNIT) ||
        lUnit->ClassName() == CPString(IGCN_RECONUNIT) || lUnit->ClassName() == CPString(IGCN_TRANUNIT))
    {
      GetGraphicInstance()->PaintSprite(lL + lX, lT + lY, mSpriteVehicle, 0, 0);
    }

    if (lUnit->ClassName() == CPString(IGCN_BOMBERUNIT) || lUnit->ClassName() == CPString(IGCN_FIGHTERUNIT))
    {
      GetGraphicInstance()->PaintSprite(lL + lX, lT + lY, mSpriteFighter, 0, 0);
    }

    if (lUnit->ClassName() == CPString(IGCN_DROPSHIPUNIT))
    {
      GetGraphicInstance()->PaintSprite(lL + lX, lT + lY, mSpriteDropship, 0, 0);
    }

    if (lUnit->ClassName() == CPString(IGCN_CHOPPERUNIT))
    {
      GetGraphicInstance()->PaintSprite(lL + lX, lT + lY, mSpriteChopper, 0, 0);
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

void CPIGroupInfo::OnLeftMouseDown(int X, int Y)
{
  int lCount = mList->Count();
  int lX = 0;
  int lY = 0;

  CPPoint lPointClick = CoordFromScreen(CPPoint(X, Y));

  for(int i = 0; i < lCount; i++)
  {
    CIGBaseObject* lUnit = mList->Get(i);

    if (!lUnit)
    {
      continue;
    }

    if ((lX < lPointClick.x) && (lPointClick.x < lX + 32) && (lY < lPointClick.y) && (lPointClick.y < lY + 32))
    {
      // doh
      GetMissionInstance()->GetObjects(MOL_SELECTEDOWNEDUNITS)->SetSelected(false);
      GetMissionInstance()->GetObjects(MOL_SELECTEDOWNEDOTHERS)->SetSelected(false);
      GetMissionInstance()->GetObjects(MOL_SELECTEDOTHERS)->SetSelected(false);

      GetMissionInstance()->GetObjects(MOL_SELECTEDOWNEDUNITS)->RemoveAll();

      GetMissionInstance()->GetObjects(MOL_SELECTEDOWNEDUNITS)->AddUnique(lUnit);
      GetMissionInstance()->GetObjects(MOL_SELECTEDOWNEDUNITS)->SetSelected(true);
      break;
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


