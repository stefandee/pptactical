//-----------------------------------------------------------------------------
// This file is part of PPTactical Engine.                                   //
//                                                                           //
// PPTactical Engine - engine for tactical/strategy games                    //
// Copyright (C) 1998-2005 Stefan Dicu & Tudor Girba                         //
//                                                                           //
// PPTactical Engine is free software; you can redistribute it               //
// and/or modify it under the terms of the GNU Lesser General Public License as     //
// published by the Free Software Foundation; either version 2.1 of the      //
// License, or (at your option) any later version.                           //
//                                                                           //
// PPTactical Engine is distributed in the hope that it will be useful, but  //
// WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY//
// or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU Lesser General Public License  //
// for more details.                                                         //
//                                                                           //
// You should have received a copy of the GNU Lesser General Public License  //
// along with PPTactical Engine; if not, write to the Free Software          //
// Foundation Inc. 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA   //
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
//  Unit              : Subsystem Builder
//
//  Version           :
//
//  Description       :
//    * subsystem for building things (soldiers, vehicles, whatever)
//    * testing for SDJ article, to be further expanded
//
//  History           :
//    [27.07.2005] - [Karg]  - created
//-----------------------------------------------------------------------------

#ifndef PPIG_SubSysBuilderH
#define PPIG_SubSysBuilderH
//---------------------------------------------------------------------------

#include "PPIG_SubSysBase.h"
#include <stdlib.h>

class CIGFactoryObject;

class CIGSubSysBuilder : public CIGSubSysBase
{
  public: // c-d
    CIGSubSysBuilder(CIGFactoryObject* _parent);
    virtual ~CIGSubSysBuilder();

    virtual void Update();

  public: // ops
    virtual CIGSubSysAction* GetActionInstance(TIGSubSysActionData actionData);

  public: // get-set
    int GetProgress() { return mPercentComplete; }

  public: // actions
    virtual void OnActionBuild();

  public: // serialization - deserialization
    virtual void Serialize(PP::Stream& _a);
    virtual unsigned __int32 DataSize();
    virtual void DeSerialize(PP::Stream& _a);

  protected:
    CIGSubSysBuilder();

  private:
    int  mPercentComplete;
};

#endif
