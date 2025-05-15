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
//  Unit              : Visual Subsystem
//
//  Version           : 1.1
//
//  Description       :
//    * simulates an observation point (for an infantry unit, this is the eyes)
//    * a unit may have a list - 1 or more observation points; if the unit has no
//      observation points (added in the editor), it will be provided one
//    * there is the concept of "main observation point" (even if it's not
//      obviously coded) - infantry, tanks and aircrafts classes that provide
//      mediator code to the visual subsys will use the first visual subsys
//      in the list as the "main"
//
//  History           :
//    [03.03.2002?] - [Karg] - unit created
//    [07.10.2004]  - [Karg] - added more behaviour
//-----------------------------------------------------------------------------

#ifndef PPIG_SubSysVisualH
#define PPIG_SubSysVisualH
//---------------------------------------------------------------------------

#include "PPIG_SubSysBase.h"

enum TUnitLook {UL_NOTHING, UL_POINT, UL_OBJECT};

class CIGBaseObject;
class CIGRealObject;
class CIGBaseObjectList;

class CIGSubSysVisual : public CIGSubSysBase
{
  public: // c-d
    CIGSubSysVisual(CIGRealObject* _parent);
    virtual ~CIGSubSysVisual();

  public: // get-set
    virtual TUnitLook GetLookAt() { return mLookAt; }
    virtual void SetLookAt(TUnitLook _v) { mLookAt = _v; }

    virtual CPPoint GetLookPoint() { return mLookPoint; }
    virtual void SetLookPoint(CPPoint _v);

    virtual CIGBaseObject* GetLookObj() { return mLookObj; }
    virtual void SetLookObj(CIGBaseObject* _v);

    virtual int GetScanAngle() { return mScanAngle; }
    virtual void SetScanAngle(int _v) { mScanAngle = _v % 360; }

    virtual int GetFOV() { return mFOV; }
    virtual void SetFOV(int _v);

    virtual int GetScanRange() { return mScanRange; }
    virtual void SetScanRange(int _v);

    virtual int GetMaxScanRange() { return mMaxScanRange; }
    virtual void SetMaxScanRange(int _v);

    virtual int GetMinScanRange() { return mMinScanRange; }
    virtual void SetMinScanRange(int _v);

    virtual int GetMaxScanAngle() { return mMaxScanAngle; }
    virtual void SetMaxScanAngle(int _v);

    virtual int GetMinScanAngle() { return mMinScanAngle; }
    virtual void SetMinScanAngle(int _v);

  public: // ops
    virtual void ResetScan();
    virtual void UnitsInView();
    virtual void UpdateView(int _value);
    virtual void Update();
    virtual bool InVisualRange(int _x, int _y);
    virtual int  GetUnitsInViewCount();
    virtual int  GetUnitsInViewId(int _index);

  public: // serialization - deserialization
    virtual void Serialize(PP::Stream& _a);
    virtual unsigned __int32 DataSize();
    virtual void DeSerialize(PP::Stream& _a);

  protected:

  private:
    void CreateViewPoints(std::vector<CPPoint>& lPoints);

    int                    mScanAngle,
                           mScanRange;
    int                    mMinScanRange,   // the range where the scanrange will start
                           mMaxScanRange;
    int                    mMinScanAngle,   // the extent of the scan angle
                           mMaxScanAngle,
                           mFOV;
    TUnitLook              mLookAt;         // am I looking to something
    CPPoint                mLookPoint;      // the point I'm looking at
    CIGBaseObject*         mLookObj;

    CIGBaseObjectList*     mUnitsInView;    // list of units in my view

    CPCounter              mScanRangeCounter;
};
//---------------------------------------------------------------------------

#endif
