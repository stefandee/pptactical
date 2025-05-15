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

#include <math.h>
#pragma hdrstop

#include "PPIG_SubSysRPropulsion.h"
#include "PPIG_ObjRocket.h"
#include "PPIG_ObjEffect.h"
#include "PPIG_Mission.h"
#include "basic3d.h"
//---------------------------------------------------------------------------

CIGSubSysRPropulsion::CIGSubSysRPropulsion(CIGRocketObject* _parent) : CIGSubSysDPropulsion(_parent)
{
  GetSenseCounter().GetThis()->SetMaxValue(15);
  GetAnimCounter().GetThis()->SetMaxValue(15);
  mOrientation = 0;
}
//---------------------------------------------------------------------------

CIGSubSysRPropulsion::~CIGSubSysRPropulsion()
{
}
//---------------------------------------------------------------------------

/*---------------------------------------------------------------------------
 description: on attack action handler
---------------------------------------------------------------------------*/
void CIGSubSysRPropulsion::OnActionAttack()
{
  CIGDamageObject *lParent = (CIGDamageObject*)GetParent();
  float lAngle;

  lAngle      = CenterRadianAngle(- lParent->GetCoord().x + lParent->GetTargetObject()->GetClosestPointTo(lParent->GetLogicCoord()).x*MAPCELLX,
                            - lParent->GetCoord().y + lParent->GetTargetObject()->GetClosestPointTo(lParent->GetLogicCoord()).y*MAPCELLY);
  TurnToAngle(lAngle);

  //
  // Z axis setup (not the best & optimized thing :)
  //
  int lZInc = 0;

  if (lParent->GetCoordZ() > lParent->GetTargetObject()->GetCoordZ())
  {
    lZInc = -3;
  }
  else if (lParent->GetCoordZ() < lParent->GetTargetObject()->GetCoordZ())
  {
    lZInc = 3;
  }

  //
  // move the object
  //
  CIGSubSysDPropulsion::SetSenseCounter(gfGetComplexDirFromAngle(GetCurrentAngle()));

  CPPoint lEndPoint = CPPoint(lParent->GetCoord().x + lParent->GetSpeed()*cos(GetCurrentAngle()),
                              lParent->GetCoord().y + lParent->GetSpeed()*sin(GetCurrentAngle()));

  lParent->RemoveFromLayerMap();
  //lParent->SetCoord(CPPoint(lParent->GetCoord().x + lParent->GetSpeed()*cos(GetCurrentAngle()),
  //                lParent->GetCoord().y - lParent->GetSpeed()*sin(GetCurrentAngle())));
  lParent->SetCoord(lEndPoint);
  lParent->NoteInLayerMap();

  lParent->SetCoordZ(lParent->GetCoordZ() + lZInc);

  CreateTrackEffect();

  /*
  if ((fabs(lParent->GetLogicCoord().x - lParent->GetTargetObject()->GetClosestPointTo(lParent->GetLogicCoord()).x) == 0) &&
     (fabs(lParent->GetLogicCoord().y - lParent->GetTargetObject()->GetClosestPointTo(lParent->GetLogicCoord()).y) == 0))
  {
    // the rocket reached its destination - creates the stimul and dies
    lParent->Explode();
    lParent->CreateStimul();
  }
  */
  TPos3D lP1, lP2;

  lP1.x = lParent->GetCoord().x;
  lP1.y = lParent->GetCoord().y;
  lP1.z = lParent->GetCoordZ();

  lP2.x = lParent->GetTargetObject()->GetClosestPointTo(lParent->GetLogicCoord()).x * MAPCELLX;
  lP2.y = lParent->GetTargetObject()->GetClosestPointTo(lParent->GetLogicCoord()).y * MAPCELLY;
  lP2.z = lParent->GetTargetObject()->GetCoordZ();

  if (Dist3D(lP1, lP2) < lParent->GetSpeed())
  {
    // fix the coordinates of the rocket to fit the target
    // otherwise it might explode a little bit away of the object
    lParent->RemoveFromLayerMap();

    lParent->SetCoord(CPPoint(lP2.x, lP2.y));
    lParent->SetCoordZ(lP2.z);

    lParent->NoteInLayerMap();

    // the rocket reached its destination - creates the stimul and dies
    lParent->Explode();
    lParent->CreateStimul();
  }
}
//---------------------------------------------------------------------------

// Description : attack ground action "callback"
// Param       : n/a
// Result      : visual
// Comments    : same shit as OnActtionAttack, only angle computation is done differently
//               luckily this code is stable, so I won't have to move code around ;)
void CIGSubSysRPropulsion::OnActionAttackGround()
{
  CIGDamageObject *lParent = (CIGDamageObject*)GetParent();
  float lAngle;

  lAngle      = CenterRadianAngle(-lParent->GetCoord().x + lParent->GetTargetPoint().x*MAPCELLX,
                            - lParent->GetCoord().y + lParent->GetTargetPoint().y*MAPCELLY);
  TurnToAngle(lAngle);

  CIGSubSysDPropulsion::SetSenseCounter(gfGetComplexDirFromAngle(GetCurrentAngle()));

  CPPoint lEndPoint = CPPoint(lParent->GetCoord().x + lParent->GetSpeed()*cos(GetCurrentAngle()),
                              lParent->GetCoord().y + lParent->GetSpeed()*sin(GetCurrentAngle()));

  lParent->RemoveFromLayerMap();
  lParent->SetCoord(lEndPoint);
  lParent->NoteInLayerMap();

  CreateTrackEffect();

  //if ((fabs(lParent->GetLogicCoord().x - lParent->GetTargetObject()->GetClosestPointTo(lParent->GetLogicCoord()).x) == 0) &&
  //   (fabs(lParent->GetLogicCoord().y - lParent->GetTargetObject()->GetClosestPointTo(lParent->GetLogicCoord()).y) == 0))
  if ((fabs((float)(lParent->GetLogicCoord().x - lParent->GetTargetPoint().x)) == 0) &&
     (fabs((float)(lParent->GetLogicCoord().y - lParent->GetTargetPoint().y)) == 0))
  {
    // the rocket reached its destination - creates the stimulus and dies
    lParent->Explode();
    lParent->CreateStimul();
  }
}
//---------------------------------------------------------------------------

/*---------------------------------------------------------------------------
 description: on self destroy action handler
---------------------------------------------------------------------------*/
void CIGSubSysRPropulsion::OnActionSelfDestruct()
{
}
//---------------------------------------------------------------------------

void CIGSubSysRPropulsion::CreateTrackEffect()
{
  CIGDamageObject *lParent = (CIGDamageObject*)GetParent();

  // if there's no sprite for track effect, abort
  if (!lParent->GetTrackEffectSprite())
  {
    return;
  }

  //
  // create the rocket tail effect - eye candy! :)
  // the track effect must be at the end of the projectile, hence the
  // complicate computations
  //
  //for (int i = lParent->GetSpeed()-4; i>=0; i=i-4)
  //{
    CIGEffectObject *lTargetEffect;
    lTargetEffect = new CIGEffectObject();
    lTargetEffect->SetAnimCyclesCount(0);
    lTargetEffect->SetSprite(lParent->GetTrackEffectSprite());

    //lTargetEffect->CenterInPoint(CPPoint(lParent->GetCoord().x + lParent->GetDimX()/2 + 4*cos(GetCurrentAngle()),
    //                                     lParent->GetCoord().y + lParent->GetDimY()/2 - 4*sin(GetCurrentAngle())));

    lTargetEffect->CenterInPoint(CPPoint(lParent->GetCoord().x + lParent->GetDimX() / 2 + (lParent->GetSpeed() - lParent->GetDimX()) * cos(GetCurrentAngle()),
                                         lParent->GetCoord().y + lParent->GetDimY() / 2 + (lParent->GetSpeed() - lParent->GetDimY()) * sin(GetCurrentAngle())));

    lTargetEffect->SetCoordZ(lParent->GetCoordZ());

    //
    // setting 0 to the track persistency will not create the tail; maybe it is a good thing to
    // check it before creating the effect?
    //
    lTargetEffect->GetUpdateCounter().GetThis()->SetMaxValue(lParent->GetTrackPersistency());

    lTargetEffect->NoteInLayerMap();
    GetMissionInstance()->GetObjects(MOL_ALLOBJECTS)->Add(lTargetEffect);  //???????
  //}
}
//---------------------------------------------------------------------------


//---------------------------------------------------------------------------
//
// Class serialize/deserialize section
//
//---------------------------------------------------------------------------

void CIGSubSysRPropulsion::Serialize(PP::Stream& _a)
{
  // base class serialize
  CIGSubSysDPropulsion::Serialize(_a);

  // members
  _a.writeInt(&mOrientation);
}
//---------------------------------------------------------------------------

unsigned __int32 CIGSubSysRPropulsion::DataSize()
{
  return 0;
}
//---------------------------------------------------------------------------

void CIGSubSysRPropulsion::DeSerialize(PP::Stream& _a)
{
  // base class deserialize
  CIGSubSysDPropulsion::DeSerialize(_a);

  // members
  _a.readInt(&mOrientation);
}
//---------------------------------------------------------------------------

#ifdef __BORLANDC__
#pragma package(smart_init)
#endif

