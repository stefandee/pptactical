//-----------------------------------------------------------------------------
// This file is part of PPTactical Engine.                                   //
//                                                                           //
// PPTactical Engine - engine for tactical/strategy games                    //
// Copyright (C) 1998 - 2003 Stefan Dicu & Tudor Girba                       //
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
//  Unit              : Factory Info In-game Control
//
//  Version           : 1.0
//
//  Description       :
//    * shows information about a factory
//
//  History           :
//    [06.08.2005] - [Karg] - unit created
//-----------------------------------------------------------------------------

#ifndef PPIG_CFactoryInfoH
#define PPIG_CFactoryInfoH
//---------------------------------------------------------------------------

#include "PP_CWinControl.h"

class CPILabel;
class CPIImage;
class CIGFactoryObject;

class CPIFactoryInfo : public CPIWinControl
{
  public: //c-d
    CPIFactoryInfo(CPIWinControl *Parent = NULL);
    virtual ~CPIFactoryInfo();

  public:
    virtual void Update();

  public: // get-set
    void SetFactory(CIGFactoryObject* _factory);

  private:
    CPILabel* mLabelProgress, *mLabelName;

    CIGFactoryObject* mFactory;
};

#endif
