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

#include "PPIG_SubSysBlPropulsion.h"
#include "PPIG_ObjBallistic.h"
#include "basic3d.h"
#include "math.h"
//---------------------------------------------------------------------------

CIGSubSysBallisticPropulsion::CIGSubSysBallisticPropulsion(CIGBallisticObject* _parent) : CIGSubSysDPropulsion(_parent)
{
  mHighZ = 0;
}
//---------------------------------------------------------------------------

CIGSubSysBallisticPropulsion::~CIGSubSysBallisticPropulsion()
{
}
//---------------------------------------------------------------------------

void CIGSubSysBallisticPropulsion::ComputeSpeed(float _dist)
{
  CIGBallisticObject* lParent = (CIGBallisticObject*)GetParent();
  lParent->SetSpeed(CPCounter(sqrt((0.81 * _dist) / (2 * cos(PI / 4) * cos(PI / 4)))));
}
//---------------------------------------------------------------------------

// Description : handle for the attack action
// Param       : n/a
// Result      : visual
// Comments    :
//
// this computes the ballistic trajectory
// the ballistic means: launch a projectile from a point (origin) with
// a speed making an angle alpha with the x axis; the ballistic trajectory
// is in a plane, to be precise (x, z) plane
// the (paramatric) equations that define the move are:
// {x = t * vo * cos(alpha)
// {z = -gt^2/2 + t * v0 * sin(alpha)
// where
// *t is time
// *v0 is the speed value
// *alpha is the angle of the speed with x axis
//
// the ballistic trajectory is, of course, a parabola
// this leads to the implicit equation:
// z = -g / (2 * v0^2 * cos(alpha) ^ 2) * x^2 + x * tan(alpha)
//
// the case I have chosen is: we know the distance, we fire with the angle PI/4
// which means that we need to compute the speed (we could have done choosing
// a speed and computing the angle, but this would have been complicated since
// the speed is harder to guess)
//
// in the current case, however, the equations are extended since the movement
// is spatial (somehow); we launch the projectile not from origin, but from
// an arbitrary point in plane (the position of the tank/artillery/whatever)
// so variable substitutions need to be made
//
// gravity should be 9.81 m/s, but since the coordinates (of the map) are not
// meters but units, I have chosen 0.81 instead of 9.81 (if you want the projectile
// to move faster, increase the value and decrease otherwise)
void CIGSubSysBallisticPropulsion::OnActionAttack()
{
   CIGBallisticObject* lParent = (CIGBallisticObject*)GetParent();

   lParent->RemoveFromLayerMap();

   lParent->SetCoord(CPPoint(lParent->GetCoord().x + lParent->GetSpeed()*cos(GetCurrentAngle())*cos(PI/4),
                    lParent->GetCoord().y - lParent->GetSpeed()*sin(GetCurrentAngle())*sin(PI/4)));


   float lX = lParent->GetCoord().x - mStartPoint.x;
   float lY = lParent->GetCoord().y - mStartPoint.y;
   float lD = lX * lX + lY * lY;
   float lZ = -(0.81/(2.0 * lParent->GetSpeed() * lParent->GetSpeed() * cos(PI / 4) * cos(PI / 4))) * lD  + sqrt(lD);
   lParent->SetCoordZ(lZ);

   if (lZ > mHighZ)
   {
     mHighZ = lZ;
   }

   lParent->NoteInLayerMap();

   if (lParent->GetCoordZ() < 0)
   {
      //here it creates the stimul and dies
      //lParent->RemoveFromLogicMap();

      lParent->RemoveFromLayerMap();
      lParent->SetCoord(CPPoint(lParent->GetTargetPoint().x*MAPCELLX, lParent->GetTargetPoint().y*MAPCELLY));
      lParent->NoteInLayerMap();
      //lParent->NoteInLogicMap();
      lParent->Explode();
      lParent->CreateStimul();
      //logWrite("ZMax: "); logWriteILn((int)mHighZ);
      //logWrite("Dist: "); logWriteILn((int)gfGetDist2D(lParent->GetCoord(), mStartPoint));
   }
}
//---------------------------------------------------------------------------

void CIGSubSysBallisticPropulsion::OnActionAttackGround()
{
  OnActionAttack();
}
//---------------------------------------------------------------------------


void CIGSubSysBallisticPropulsion::Serialize(PP::Stream& _a)
{
  CSerPoint   lStartPoint(mStartPoint);

  // base class serialize
  CIGSubSysDPropulsion::Serialize(_a);

  // this serialize
  lStartPoint.Serialize(_a);
}
//---------------------------------------------------------------------------

unsigned __int32 CIGSubSysBallisticPropulsion::DataSize()
{
  return 0;
}
//---------------------------------------------------------------------------

void CIGSubSysBallisticPropulsion::DeSerialize(PP::Stream& _a)
{
  CSerPoint lStartPoint;

  // base class serialize
  CIGSubSysDPropulsion::DeSerialize(_a);

  // this serialize
  lStartPoint.DeSerialize(_a);
}
//---------------------------------------------------------------------------

#ifdef __BORLANDC__
#pragma package(smart_init)
#endif
