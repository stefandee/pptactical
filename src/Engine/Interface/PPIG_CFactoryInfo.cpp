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

#include "PPIG_CFactoryInfo.h"
#include "PPIG_ObjFactory.h"
#include "PP_CLabel.h"
//---------------------------------------------------------------------------

#pragma package(smart_init)

CPIFactoryInfo::CPIFactoryInfo(CPIWinControl *Parent) : CPIWinControl(Parent)
{
  mLabelName = new CPILabel(this);
  AddControl(mLabelName);
  mLabelName->SetText("BUILDING NAME");
  //mLabelCrew->SetText("");
  mLabelName->SetLeft(10);
  mLabelName->SetTop(0);
  mLabelName->SetVisible(true);
  mLabelName->SetEnabled(false);
  mLabelName->SetColor(0xB0B0B0);
  
  mLabelProgress = new CPILabel(this);
  AddControl(mLabelProgress);
  mLabelProgress->SetText("Idle");
  //mLabelCrew->SetText("");
  mLabelProgress->SetLeft(10);
  mLabelProgress->SetTop(10);
  mLabelProgress->SetVisible(true);
  mLabelProgress->SetEnabled(false);
  mLabelProgress->SetColor(0xB0B0B0);

  mFactory = 0;
}
//---------------------------------------------------------------------------

CPIFactoryInfo::~CPIFactoryInfo()
{
  mFactory = 0;
}
//---------------------------------------------------------------------------

void CPIFactoryInfo::SetFactory(CIGFactoryObject* _factory)
{
  mFactory = _factory;

  mLabelName->SetText(mFactory->GetStaticData().objName);
}
//---------------------------------------------------------------------------

void CPIFactoryInfo::Update()
{
  if (mFactory)
  {
    if (mFactory->IsBuilding())
    {
      mLabelProgress->SetText(CPString("Progress: ") + CPString(mFactory->GetProgress()) + "%");
    }
    else
    {
      mLabelProgress->SetText("Idle");
    }
  }

  CPIWinControl::Update();
}
//---------------------------------------------------------------------------



