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

//-----------------------------------------------------------------------------
//  Unit              : Rocket class
//
//  Version           : 1.05
//
//  Description       :
//    * implementes a image-guided rocket :)
//
//  History           :
//    [17.06.2000] - [grabX] - unit created
//    [23.01.2002] - [Karg]  - eliminated an orientation bug: corrected first
//                             the axis-system, by-passed a counter
//                             (SenseCounter/AnimCounter) bug, then corected a
//                             Counter initialization in constructor
//    [24.01.2002] - [Karg]  - added better colision detection
//    [24.01.2002] - [Karg]  - added splash damage
//-----------------------------------------------------------------------------

#ifndef PPIG_ObjRocketH
#define PPIG_ObjRocketH

#include "PPIG_ObjDamage.h"

class CIGRocketObject: public CIGDamageObject
{
   public:
      CIGRocketObject();
      virtual CPString ClassName () { return IGCN_ROCKETOBJECT; }
      virtual bool IsDerivedFrom (CPString className);

      virtual void Begin();
      virtual void Paint(CPPoint ptViewport, CPPoint ptWindow, int Width, int Height, int zoomLevel);

  public: // serialization - deserialization
    virtual void Serialize(PP::Stream& _a);
    virtual unsigned __int32 DataSize();
    virtual void DeSerialize(PP::Stream& _a);
};

//---------------------------------------------------------------------------
#endif

