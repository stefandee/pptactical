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

#pragma hdrstop

#include "PPIG_SubSysBody.h"
#include "PPIG_ObjReal.h"
//---------------------------------------------------------------------------

CIGSubSysBody::CIGSubSysBody(CIGRealObject* _parent) : CIGSubSysBase(_parent)
{
}
//---------------------------------------------------------------------------

CIGSubSysBody::~CIGSubSysBody()
{
}
//---------------------------------------------------------------------------

void CIGSubSysBody::OnStimul (CIGObjectStimul& Stimul)
{
  // verify if the parent is a
  CIGRealObject* lParent = (CIGRealObject*)GetParent();

  switch (Stimul.GetType())
  {
    case ST_EXPLODE:
       lParent->SetCurrentHP(lParent->GetCurrentHP() - Stimul.GetValue()*(100 - lParent->GetExplodeRes())/100);
       break;
    case ST_FIRE:
       lParent->SetCurrentHP(lParent->GetCurrentHP() - Stimul.GetValue()*(100 - lParent->GetFireRes())/100);
       break;
    case ST_BULLET:
       lParent->SetCurrentHP(lParent->GetCurrentHP() - Stimul.GetValue()*(100 - lParent->GetBulletRes())/100);
       break;
    case ST_LASER:
       lParent->SetCurrentHP(lParent->GetCurrentHP() - Stimul.GetValue()*(100 - lParent->GetEnergyRes())/100);
       break;
    case ST_PLASMA:
       lParent->SetCurrentHP(lParent->GetCurrentHP() - Stimul.GetValue()*(100 - lParent->GetPlasmaRes())/100);
       break;
  }
}
//---------------------------------------------------------------------------

#ifdef __BORLANDC__
#pragma package(smart_init)
#endif

