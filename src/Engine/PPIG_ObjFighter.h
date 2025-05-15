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

#ifndef PPIG_ObjFighterH
#define PPIG_ObjFighterH

#include "PPIG_ObjAircraft.h"
#include "PPIG_SubSysFPropulsion.h"
#include "PPIG_SubSysFighterWeapons.h"

class CIGFighterUnit : public CIGAircraft
{
    public:
        CIGFighterUnit();
        virtual ~CIGFighterUnit(); 

    public:     
        virtual CPString    ClassName(){ return IGCN_FIGHTERUNIT; }
        virtual bool        IsDerivedFrom(CPString className); 
    public:     
        virtual void        Update(); 
        virtual void        Paint(CPPoint ptViewPort, CPPoint ptWindow, int Width, int Height, int zoomLevel); 
        virtual CPRect      GetPaintRect();
        
    public:
        virtual void        AddAction(TIGSubSysActionData actionData); 
        virtual void        AddAction(CIGSubSysAction *Action); 
        virtual void        AddActionWithPriority(CIGSubSysAction *Action); 
        virtual void        SetAction(TIGSubSysActionData actionData); 
    public:     
        virtual void        Serialize(PP::Stream &a);
        virtual void        DeSerialize(PP::Stream &a); 
        virtual unsigned __int32 DataSize();

    public: // should be refactored
      void        UpdateView(int _value) { mVisuals[0]->UpdateView(_value); }
      TUnitLook GetLookAt() { return mVisuals[0]->GetLookAt(); }

      int GetScanAngle() { return mVisuals[0]->GetScanAngle(); }
      void SetScanAngle(int _v) { mVisuals[0]->SetScanAngle(_v); }

      int GetScanRange() { return mVisuals[0]->GetScanRange(); }
      void SetScanRange(int _v) { mVisuals[0]->SetScanRange(_v); }

      void SetSenseCounter(int _v);
      int  GetSenseCounter();
      
    protected:
        virtual void        InitCsl(); 
        virtual void        InitHandlers(); 

    protected:
        CIGSubSysFPropulsion  *m_Propulsion;
        //CIGSubSysBody         *mBody;
        //CIGSubSysSensors      *mSensors;
        //CIGSubSysVisual       *mVisual;
        CIGSubSysFighterWeapons *m_Weapons; 
};

#endif
