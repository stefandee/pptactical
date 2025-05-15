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
//  Unit              : Class SubSystem Body
//
//  Version           : 1.0
//
//  Description       :
//    * the body responses to damage stimulus, modifying the hitpoints of a
//      the parent
//    * only a CIGRealObject and derivate classes may have a body
//
//  History           :
//    [10.11.2002] - [Karg] - unit created
//-----------------------------------------------------------------------------

#ifndef PPIG_SubSysBodyH
#define PPIG_SubSysBodyH
//---------------------------------------------------------------------------

#include "PPIG_SubSysBase.h"

class CIGRealObject;

class CIGSubSysBody : public CIGSubSysBase
{
  public: // c-d
    CIGSubSysBody(CIGRealObject* _parent);
    virtual ~CIGSubSysBody();

  public: // actions
    virtual void OnStimul(CIGObjectStimul& Stimul);

  protected:
  private:
};

#endif
