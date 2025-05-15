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
//  Unit              : Class In Game Tank Unit
//
//  Version           : 1.0
//
//  Description       :
//    * implementation for tank unit
//    * a tank has a body, a turret, armor, weapons and a crew
//    * objbase classes (including CIGTankUnit) are strategy - distribute user
//      actions (from CIGMission) to subsystems actions
//    * objbase classes are also mediators - they mediate between subsystems
//    * leaves in objbase hierarchy have hardcoded behavior - tanks, infantry, artillery
//    * depending on the application (game ;), subsystems communicate in different ways
//    * script-related methods will be moved around (with the refactoring of the obj hierarchy)
//
//  History           :
//    [25.10.2002] - [Karg] - first tank tests
//    [11.11.2002] - [Karg] - refactored; added set/add actions
//    [27.11.2002] - [Karg] - corrected turret painting; added updateview in update
//    [24.01.2003] - [Karg] - added script handlers/callbacks (temporary)
//    [02.03.2003] - [Karg] - added oscilator
//-----------------------------------------------------------------------------

#ifndef PPIG_ObjTankH
#define PPIG_ObjTankH
//---------------------------------------------------------------------------

#include "PPIG_ObjVehicle.h"
#include "PPIG_SubSysTurret.h"
#include "PPIG_SubSysVPropulsion.h"
#include "PPIG_SubSysVisual.h"
#include "PPIG_SubSysOscilator.h"
#include <vector>

class CIGTankUnit : public CIGVehicle
{
  public:
      CIGTankUnit();
      virtual ~CIGTankUnit();

      virtual CPString ClassName () {return IGCN_TANKUNIT;}
      virtual bool IsDerivedFrom (CPString className);

      virtual void Update();
      virtual void Paint(CPPoint ptViewPort, CPPoint ptWindow, int Width, int Height, int zoomLevel);
      virtual CPRect GetPaintRect();

      virtual void SetVehicleUnifiedData(TVehicleUnified& value);
      virtual void SetSprite(CPSprite * value);
      virtual bool CanBePlaced(CPPoint _point);

   public: // mediator interface (various subsystems communicate through these methods)
      void        UpdateView(int _value);
      TUnitLook GetLookAt() { return mVisuals[0]->GetLookAt(); }

      int GetScanAngle() { return mVisuals[0]->GetScanAngle(); }
      void SetScanAngle(int _v) { mVisuals[0]->SetScanAngle(_v); }

      int GetScanRange() { return mVisuals[0]->GetScanRange(); }
      void SetScanRange(int _v) { mVisuals[0]->SetScanRange(_v); }

      int GetBodySenseCounter() { return mPropulsion->GetAnimCounter()(); }
      void SetBodySenseCounter(int _v) { mPropulsion->SetAnimCounter(_v); }

      int  GetTurretSenseCounter() { return mTurret->GetSense(); }
      void SetTurretSenseCounter(int _v) { mTurret->SetSense(_v); }

  public: // action
    virtual void AddAction(TIGSubSysActionData actionData);
    virtual void AddAction (CIGSubSysAction *Action);
    virtual void AddActionWithPriority (CIGSubSysAction *Action);
    virtual void SetAction (TIGSubSysActionData actionData);

  public: // serialization - deserialization
    virtual void Serialize(PP::Stream& _a);
    virtual unsigned __int32 DataSize();
    virtual void DeSerialize(PP::Stream& _a);

  public:
    virtual void NoteStimul(CIGObjectStimul &stimul);
    virtual void ComputeWay();
    virtual void Die();

  protected: // script handlers
    ZCsl_callback_t  mHandleUnload;
    ZCsl_callback_t  mHandleUnloadAll;

    ZCsl_callback_t  mHandleGetTurretSense;
    ZCsl_callback_t  mHandleSetTurretSense;

    // the unload actions are only specific to transporters, not to all
    // aircrafts. however, since only transporters can handle this action, there
    // is no problem making the scripting available for all aircrafts
    ZString mcUnload(ZCsl* csl);
    ZString mcUnloadAll(ZCsl* csl);

    ZString mcGetTurretSense(ZCsl* csl);
    ZString mcSetTurretSense(ZCsl* csl);

  protected:
    //virtual void InitCsl();
    virtual void InitHandlers();
    virtual void InitScriptingInterface();

  protected: // subsystems - should they be private?
    CIGSubSysTurret        *mTurret;
    CIGSubSysVPropulsion   *mPropulsion;
    //CIGSubSysBody          *mBody;
    //CIGSubSysSensors       *mSensors;
    //CIGSubSysVisual        *mVisual;
    CIGSubSysOscilator     *mOscilator;

    //std::vector<CIGSubSysVisual*> mVisuals;
};


#endif
