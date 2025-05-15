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

 description: Class In Game Object Bullet
              implements a bullet object
 last modify: 13 07 00
        by grabX
---------------------------------------------------------------------------*/

//---------------------------------------------------------------------------
#pragma hdrstop

#include "PPIG_ObjBullet.h"
#include "PPIG_ObjEffect.h"
#include "PPIG_Mission.h"
#include "PP_Stdlib.h"

/*---------------------------------------------------------------------------
 description: constructor
---------------------------------------------------------------------------*/
CIGBulletObject::CIGBulletObject() : CIGLaserObject()
{
   SetStimulType(ST_BULLET);
   SetSpeed(CPCounter(10));
}
//---------------------------------------------------------------------------

/*---------------------------------------------------------------------------
 description: returns true if this class is derived from className
 parameters : className - the parent class
---------------------------------------------------------------------------*/
bool CIGBulletObject::IsDerivedFrom (CPString className)
{
   return (className == CIGBulletObject::ClassName() || CIGLaserObject::IsDerivedFrom(className));
}
//---------------------------------------------------------------------------

void CIGBulletObject::Explode()
{
/*
   CIGEffectObject *lTargetEffect;
   CPPoint lCoord = GetLogicCoord();

   //for (int i = 0; i < 2; i++)
   //{
      lTargetEffect = new CIGEffectObject();
      lTargetEffect->SetAnimCyclesCount(1);
      lTargetEffect->SetSprite(GetEffectSprite());
      lTargetEffect->CenterInPoint(CPPoint(lCoord.x * MAPCELLX + 5 + gfRandom(MAPCELLX), lCoord.y * MAPCELLY + gfRandom(MAPCELLY)));
      (lTargetEffect->GetUpdateCounter().GetThis())->SetMaxValue(3);
      lTargetEffect->NoteInLayerMap();
      GetMissionInstance()->GetObjects(MOL_ALLOBJECTS)->Add(lTargetEffect);  //???????
      logWrite("Effect: "); logWriteI(lTargetEffect->GetCoord().x/MAPCELLX); logWrite(","); logWriteILn(lTargetEffect->GetCoord().y/MAPCELLY);
   //}

   RemoveFromLayerMap();
   SetVisible(false);
   GetMissionInstance()->AddToDeleteObjects(this);

   try
   {
      CIGSpaceSoundSystem *lSSSI;
      lSSSI = lSSSI->Instance();
      lSSSI->Play(GetSoundOnHit(), GetLogicCoord().x, GetLogicCoord().y);
   }
   catch(...)
   {
   }

   logWrite("Bullet-target-sprite");
   if (GetEffectSprite())
   {
     logWriteLn(GetEffectSprite()->GetFileName());
   }
*/
   CIGLaserObject::Explode();
}
//---------------------------------------------------------------------------

#ifdef __BORLANDC__
#pragma package(smart_init)
#endif
