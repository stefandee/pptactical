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

 description: Class In Game Object Laser
              implements a laser object
 last modify: 13 07 00
        by grabX
---------------------------------------------------------------------------*/
//---------------------------------------------------------------------------
#pragma hdrstop

#include "PPIG_ObjLaser.h"
#include <math.h>
#include "basic3d.h"
#include "PPIG_EngineDef.h"
#include "PPIG_Mission.h"
#include "PPIG_SubSysLPropulsion.h"

/*---------------------------------------------------------------------------
 description: constructor
---------------------------------------------------------------------------*/
CIGLaserObject::CIGLaserObject() : CIGDamageObject()
{
  SetSpeed(CPCounter(16));
  SetStimulType(ST_LASER);

  if (mPropulsion)
  {
    delete mPropulsion;
  }

  mPropulsion = new CIGSubSysLPropulsion(this);
}
//---------------------------------------------------------------------------

/*---------------------------------------------------------------------------
 description: returns true if this class is derived from className
 parameters : className - the parent class
---------------------------------------------------------------------------*/
bool CIGLaserObject::IsDerivedFrom (CPString className)
{
   return (className == CIGLaserObject::ClassName() || CIGDamageObject::IsDerivedFrom(className));
}
//---------------------------------------------------------------------------

/*---------------------------------------------------------------------------
 description: begins the action
---------------------------------------------------------------------------*/
void CIGLaserObject::Begin()
{
   CIGDamageObject::Begin();
}
//---------------------------------------------------------------------------

#ifdef __BORLANDC__
#pragma package(smart_init)
#endif
