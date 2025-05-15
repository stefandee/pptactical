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

#include "PPIG_SubSysLPropulsion.h"
#include "PPIG_ObjLaser.h"
#include "basic3d.h"
//---------------------------------------------------------------------------

CIGSubSysLPropulsion::CIGSubSysLPropulsion(CIGLaserObject* _parent) : CIGSubSysDPropulsion(_parent)
{
}
//---------------------------------------------------------------------------

CIGSubSysLPropulsion::~CIGSubSysLPropulsion()
{
}
//---------------------------------------------------------------------------

/*---------------------------------------------------------------------------
 description: handles the attack action
---------------------------------------------------------------------------*/
void CIGSubSysLPropulsion::OnActionAttack()
{
   CIGLaserObject* lParent = (CIGLaserObject*)GetParent(); 

  // check if the laser has hit the terrain
  /*
  CPPoint lStartPoint = lParent->GetCoord();
  CPPoint lEndPoint = CPPoint(lParent->GetCoord().x + lParent->GetSpeed()*cos(GetCurrentAngle()),
                              lParent->GetCoord().y - lParent->GetSpeed()*sin(GetCurrentAngle()));

  CPPoint lSmashPoint = CheckPath(lStartPoint, lEndPoint);

  if (lSmashPoint.x != -1)
  {
    // move the parent where it hit the terrain and explode it
    lParent->RemoveFromLayerMap();
    lParent->SetCoord(lSmashPoint);
    lParent->NoteInLayerMap();

    lParent->Explode();
    lParent->CreateStimul();
    return;
  }
  */

   // we must record somewhere the z coord of the target
   lParent->Begin();
   lParent->RemoveFromLayerMap();
   //lParent->SetCoord(CPPoint(lParent->GetCoord().x + lParent->GetSpeed()*cos(GetCurrentAngle()),
   //                 lParent->GetCoord().y - lParent->GetSpeed()*sin(GetCurrentAngle())));

   // this vector should be precomputed
   CVector3D lSpeed(lParent->GetTargetPoint().x * MAPCELLX - lParent->GetCoord().x,
                    lParent->GetTargetPoint().y * MAPCELLY - lParent->GetCoord().y,
                    lParent->GetTargetCoordZ() - lParent->GetCoordZ()
                    );

   lSpeed.Normalize();

   CPPoint lP = CPPoint(
     lParent->GetCoord().x + lSpeed.GetX() * lParent->GetSpeed(),
     lParent->GetCoord().y + lSpeed.GetY() * lParent->GetSpeed());

   lParent->SetCoord(lP);

   lParent->SetCoordZ(lParent->GetCoordZ() + lSpeed.GetZ() * lParent->GetSpeed());

   lParent->NoteInLayerMap();
   
   TPos3D pt1, pt2;
   pt1.x = lParent->GetCoord().x;
   pt1.y = lParent->GetCoord().y;
   pt1.z = lParent->GetCoordZ();

   pt2.x = lParent->GetTargetPoint().x * MAPCELLX;
   pt2.y = lParent->GetTargetPoint().y * MAPCELLY;
   pt2.z = lParent->GetTargetCoordZ();
   
   if (Dist3D(pt1, pt2) < lParent->GetSpeed())
   {
      //here it creates the stimul and dies
      lParent->RemoveFromLayerMap();
      //lParent->RemoveFromLogicMap();
      lParent->SetCoord(CPPoint(lParent->GetTargetPoint().x*MAPCELLX, lParent->GetTargetPoint().y*MAPCELLY));

      // temporary
      //lParent->SetCoordZ(0);

      //lParent->NoteInLogicMap();
      lParent->NoteInLayerMap();

      // we should prevent an explosion if the damage object is in the air and
      // there is no unit here
      lParent->Explode();
      lParent->CreateStimul();
   }
}
//---------------------------------------------------------------------------

/*---------------------------------------------------------------------------
 description: handles the attack ground action (same as above)
---------------------------------------------------------------------------*/
void CIGSubSysLPropulsion::OnActionAttackGround()
{
  OnActionAttack();
}
//---------------------------------------------------------------------------

#ifdef __BORLANDC__
#pragma package(smart_init)
#endif

