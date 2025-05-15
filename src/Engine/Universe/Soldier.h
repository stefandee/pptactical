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
//  Unit              : Soldier containment class (Soldier.h)
//
//  Versiune          : 1.0
//
//  Descriere         :
//    * realizeaza o incapsulare a capabilitatilor fizice ale soldatului care
//      se gasesc in structurile din Sources\Soldiers.h
//    * are citiva membri noi, care nu sint calitati fizice/psihice
//
//  Istorie           :
//    [Karg] - [11.04.2000] - comentarii
//    [Karg] - [03.10.2000] - am adaugat metodele de serializare; CSoldier am
//                            derivat'o din CSerializable
//-----------------------------------------------------------------------------

//---------------------------------------------------------------------------
#ifndef SoldierH
#define SoldierH
//---------------------------------------------------------------------------

#include "Comportament.h"
#include "Equipment.h"

#define SOLDIER_VALID_HP 40
#define SOLDIER_WEAPON_MAX_SKILL 10

enum TSoldierStatus {SOLDIER_STATUS_RETIRED, SOLDIER_STATUS_KILLED, SOLDIER_STATUS_OK};

// caracteristicile soldatului
typedef struct _Infantry
{
  public:
    char name[32];
    char surname[32];
    char description[256];
    int  weight;
    int  height;
    int  gender;
    int  leadership;
    int  experience;
    int  bravery;
    int  camo;
    int  antitank_skill;
    int  sr_combat;
    int  sniper_skill;
    int  high_tech;
    int  maxCarry;
    int  crtCarry;
    int  morale;
    int  price;
    int  kills;
    int  hp;
    int  maxHp;
    bool secWepAbility;
    char face[MAX_PP_PATH];

  public: // serialization - deserialization
    void Serialize(PP::Stream& _a);
    void DeSerialize(PP::Stream& _a);

} TUInfantry;

// clasa propriu-zisa
class CSoldier : public CSerializable
{
  friend class CStarShip;
  private:
    struct _Infantry mData;

    int    mDaysWeaponUse;
    int    mDaysMoraleCt;

    int    mFirstWeaponIndex;
    int    mSecondWeaponIndex;
    int    mArmorIndex;
    int    mSpecialDevice;

    CIWeapon* mIWeapon;
    CIArmor*  mIArmor;

    bool           mEmbarked;
    TSoldierStatus mStatus;

    CComportament  mComp;

  protected:
    bool SetFirstWeapon (CIWeapon* _weapon);
    bool SetArmor       (CIArmor* _armor);

  public:
    CSoldier();
    CSoldier(struct _Infantry _data);
    CSoldier(const CSoldier &_soldier);

    void GetData(struct _Infantry* _data);
    void SetData(struct _Infantry _data) { mData = _data; };

    void ModifyMorale(int _step);
    void Evolve();

    void SetEmbark(bool _embark);
    bool GetEmbark();

    CIWeapon* GetFirstWeapon();
    CIArmor*  GetArmor();

    TSoldierStatus GetStatus() { return mStatus; };
    void SetStatus(TSoldierStatus _status) { mStatus = _status; };

    bool IsValid();

    void SetComportament(CComportament _c) { mComp = _c; };
    CComportament GetComportament() { return mComp; };

    virtual void Serialize(PP::Stream& _a);
    virtual void DeSerialize(PP::Stream& _a);

    ~CSoldier();
};

#endif
