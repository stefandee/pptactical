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
//  Unit              : Class SubSystem Sensors
//
//  Version           : 1.0
//
//  Description       :
//    * sensors trated by this class are noises and attackers
//    * damage is handled by the body subsystem
//    * soon there will be derivated classes which will handle infrared stimulus
//
//  History           :
//    [10.11.2002] - [Karg] - unit created
//    [25.11.2002] - [Karg] - moved attackers and noises list from base object file
//-----------------------------------------------------------------------------

#ifndef PPIG_SubSysSensorsH
#define PPIG_SubSysSensorsH
//---------------------------------------------------------------------------

#include "PPIG_SubSysBase.h"

//#define MAX_I_ATTACKERS_MEM  4
//#define I_ATTACKER_TIMEOUT   6000
//#define I_NOISES_TIMEOUT     1000

const int MAX_I_ATTACKERS_MEM = 4;
const int I_ATTACKER_TIMEOUT = 6000;
const int I_NOISES_TIMEOUT = 1000;

class CIGAuxAttacker;
class CIGAuxNoise;

class CIGSubSysSensors : public CIGSubSysBase
{
  public: // c-d
    CIGSubSysSensors(CIGBaseObject* _parent);
    virtual ~CIGSubSysSensors();

  public:
    virtual void Update();

  public: // get lists
    virtual CPList<CIGAuxAttacker>*    GetAttackersList() { return mAttackers; } ;
    virtual CPList<CIGAuxNoise>*       GetNoisesList()    { return mNoises; }

  public: // actions
    virtual void OnStimul(CIGObjectStimul& Stimul);

  public: // serialization - deserialization
    virtual void Serialize(PP::Stream& _a);
    virtual unsigned __int32 DataSize();
    virtual void DeSerialize(PP::Stream& _a);

  protected:

  private:
    CPList<CIGAuxAttacker>*  mAttackers;             // list of attackers
    CPList<CIGAuxNoise>*     mNoises;                // list of stimulus and their last position
};

// noise heard by an object - will probably be unified with attacker
class CIGAuxNoise
{
  private:
    CPPoint        mSourcePoint;
    CIGBaseObject* mSourceObject;
    int            mType;
    int            mTime;

  public: // c-d
    CIGAuxNoise() { mSourcePoint = CPPoint(0, 0); mTime = 0; mType = -1; mSourceObject = NULL; }; // will be modified to member init list
    CIGAuxNoise(CPPoint _point, CIGBaseObject* _source, int _type, int _time) { mSourcePoint = _point; mTime = _time; mType = _type; mSourceObject = _source; };
    ~CIGAuxNoise() {};

  public: // serialization - deserialization
    virtual void Serialize(PP::Stream& _a);
    virtual unsigned __int32 DataSize();
    virtual void DeSerialize(PP::Stream& _a);

  public: // get-set
    CPPoint GetSourcePoint() { return mSourcePoint; };
    void    SetSourcePoint(CPPoint _value) { mSourcePoint = _value; };

    CIGBaseObject* GetSourceObject() { return mSourceObject; };
    void           SetSourceObject(CIGBaseObject* _value) { mSourceObject = _value; };

    int  GetType() { return mType; };
    void SetType(int _value) { mType = _value; };

    int  GetTime() { return mTime; };
    void SetTime(int _value) { mTime = _value; };
};

// attacker of an object class
class CIGAuxAttacker
{
  private:
    CIGBaseObject* mAttacker;
    int            mTime;

  public: // c-d
    CIGAuxAttacker() { mAttacker = NULL; mTime = -1; };
    CIGAuxAttacker(CIGBaseObject* _attacker, int _time) { mAttacker = _attacker; mTime = _time;  }; // will be modified to member init list
    ~CIGAuxAttacker() {};

  public: // serialization - deserialization
    virtual void Serialize(PP::Stream& _a);
    virtual unsigned __int32 DataSize();
    virtual void DeSerialize(PP::Stream& _a);

  public: // get-set
    CIGBaseObject* GetAttacker() {  return mAttacker; }
    void SetAttacker(CIGBaseObject* _v) { mAttacker = _v; };

    int GetTime() { return mTime; };
    void SetTime(int _v) { mTime = _v; };
};

#endif
