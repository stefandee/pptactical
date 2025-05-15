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

 details :
 - coordonatele care se folosesc sind exprimate in metri (calculele sint mai
   usor de facut);

 last modify:
 09 09 04 Karg
 - modified so that the volume is expressed in percents
 - need also to modify the pan

 27 09 00 Karg
 - metoda "play" a fost modificata, algoritmul de calcul al volumului si
   al pan value care dau impresia de sunet spatial fiind imbunatatit
 - a fost adaugat cimpul de ear cone angle, reprezentind unghiul in
   care se face pan (unghiul e in jurul axei x)

 15 07 00
        by grabX

---------------------------------------------------------------------------*/
//---------------------------------------------------------------------------
#ifndef PPIG_SpaceSoundH
#define PPIG_SpaceSoundH

//#include "PP_GuiDef.h"
#include "basic2d.h"

#define SSS_EMPIRIC_VALUE 15

class CIGSpaceSoundSystem
{
   public:
      static CIGSpaceSoundSystem* Instance();
      static void Release();

      void   Play(int soundIndex, int X, int Y);
      void   SetReferencePoint(const CPPoint& value);

      void   SetEarConeAngle(float _angle) { mEarConeAngle = _angle; };
      float  GetEarConeAngle() { return mEarConeAngle; };

   protected:
      CIGSpaceSoundSystem();
      virtual ~CIGSpaceSoundSystem();

   private:
      static CIGSpaceSoundSystem* mInstance;
      CPPoint                     mReferencePoint;
      float                       mEarConeAngle;
};

//---------------------------------------------------------------------------
#endif
