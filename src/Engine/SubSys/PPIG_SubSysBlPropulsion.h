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

#ifndef PPIG_SubSysBlPropulsionH
#define PPIG_SubSysBlPropulsionH
//---------------------------------------------------------------------------

#include "PPIG_SubSysDPropulsion.h"
class CIGBallisticObject;

class CIGSubSysBallisticPropulsion : public CIGSubSysDPropulsion
{
  public: // c-d
    CIGSubSysBallisticPropulsion(CIGBallisticObject* _parent);
    virtual ~CIGSubSysBallisticPropulsion();
    void ComputeSpeed(float _dist);
    void SetStartPoint(CPPoint _p) { mStartPoint = _p; };

  public: // actions
    virtual void OnActionAttack();
    virtual void OnActionAttackGround();

   public: // serialization - deserialization
     virtual void Serialize(PP::Stream& _a);
     virtual unsigned __int32 DataSize();
     virtual void DeSerialize(PP::Stream& _a);

  protected:

  private:
    CPPoint mStartPoint;
    float   mHighZ; 
};
#endif
