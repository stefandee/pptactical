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

 description: Class Space Sound System
              Singleton Class
 last modify: 15 07 00
        by grabX
---------------------------------------------------------------------------*/

//---------------------------------------------------------------------------
#include <math.h>
#pragma hdrstop

#include "PPIG_SpaceSound.h"
#include "PP_Sound.h"
#include "basic3d.h"

CIGSpaceSoundSystem* CIGSpaceSoundSystem::mInstance = 0;

/*---------------------------------------------------------------------------
 description:
---------------------------------------------------------------------------*/
CIGSpaceSoundSystem* CIGSpaceSoundSystem::Instance()
{
   if (mInstance == 0)
      mInstance = new CIGSpaceSoundSystem();
   return mInstance;
}
//---------------------------------------------------------------------------

/*---------------------------------------------------------------------------
 description: deletes the mInstance if it's not NULL
---------------------------------------------------------------------------*/
void CIGSpaceSoundSystem::Release()
{
   if (mInstance != 0)
   {
      delete mInstance;
      mInstance = 0;
   }
}
//---------------------------------------------------------------------------

/////////////////////////////////////////////////////////////////////////////
//PROTECTED SECTION
/////////////////////////////////////////////////////////////////////////////
/*---------------------------------------------------------------------------
 description: constructor
---------------------------------------------------------------------------*/
CIGSpaceSoundSystem::CIGSpaceSoundSystem()
{
   mReferencePoint = CPPoint(0, 0);
   mEarConeAngle   = 160.0;
}
//---------------------------------------------------------------------------

/*---------------------------------------------------------------------------
 description: destructor
---------------------------------------------------------------------------*/
CIGSpaceSoundSystem::~CIGSpaceSoundSystem()
{
}
//---------------------------------------------------------------------------

/////////////////////////////////////////////////////////////////////////////
//PUBLIC SECTION
/////////////////////////////////////////////////////////////////////////////
/*---------------------------------------------------------------------------
 description: plays the sound accordingly to X, Y
 parameters : X, Y - in logic coord
---------------------------------------------------------------------------*/
void CIGSpaceSoundSystem::Play(int soundIndex, int X, int Y)
{
   float lDist;
   int lNewVolume, lNewPan;
   TPos3D lP1, lP2;

   // calcule pentru volum
   lP1.x = (float)X; lP1.y = (float)Y; lP1.z = 0.0;
   lP2.x = (float)mReferencePoint.x; lP2.y = (float)mReferencePoint.y; lP2.z = 0.0f;

   lDist = Dist3D(lP1, lP2);

   if (lDist == 0)
   {
     lNewVolume = 100; // max volume - 100%
   }
   else
   {
     // log(lDist) da valoarea atenuarii volumului; la distanta de 1000 de metri, atenuarea
     // e aproape completa; formula e empirica
     // ar merge si cu un bezier, la care sa poti controla valorile
     // ar merge si cu o tabela de atenuare a volumelor
     // SSS_EMPIRIC_VALUE : cu cit e mai mare, cu atit sunetul necesita o
     // distanta mai mare sa se atenueze
     //lNewVolume = -10000.0 + 10000.0 * (15 - log(lDist)) / 15;
     //lNewVolume = (int)(SS_VOL_MIN * log(lDist) / SSS_EMPIRIC_VALUE);
     lNewVolume = (int)(100 * log(lDist) / SSS_EMPIRIC_VALUE);
   }

   // calcule pentru pan
   // exista doua drepte, date de ecuatiile y = x * tg(u) si y = x * tg (180-u)
   // panul e dat de intersectia celor doua semiplane negative
   float lRes1 = (float)((Y - mReferencePoint.y) - (X - mReferencePoint.x) * tan((mEarConeAngle / 2) * 3.1415 / 180.0));
   float lRes2 = (float)((Y - mReferencePoint.y) - (X - mReferencePoint.x) * tan((180.0 - mEarConeAngle / 2) * 3.1415 / 180.0));

   if ( (lRes1 < 0.0) && (lRes2 > 0.0) ||
        (lRes1 > 0.0) && (lRes2 < 0.0))
   {
     if (X - mReferencePoint.x > 0)
     {
       lNewPan = 80; // 80%
     }
     else
     {
       lNewPan = -80; // -80%
     }
   }
   else
   {
      lNewPan = 0; // full volume in both channels
   }

   // abia acum se playeaza
   GetSoundInstance()->PlayWithVol(soundIndex, lNewPan, lNewVolume);
}
//---------------------------------------------------------------------------

/*---------------------------------------------------------------------------
 description: sets the reference point
 parameters : value  - in logic coord
---------------------------------------------------------------------------*/
void CIGSpaceSoundSystem::SetReferencePoint(const CPPoint& value)
{
   mReferencePoint = value;
}
//---------------------------------------------------------------------------

#ifdef __BORLANDC__
#pragma package(smart_init)
#endif
