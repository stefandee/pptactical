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
//  Unit              : Echipament militar (Equipment.h)
//
//  Versiune          : 1.0
//
//  Descriere         :
//    * wrapper class pentru datele din Equip.h
//
//  Istorie           :
//    [Karg] - [11.04.2000] - a fost creat fisierul si clasele CIArmor si CIWeapon
//    [Karg] - [08.10.2000] - am desfiintat equip.cpp/h si am mutat aici toate
//                            tipurile de data
//-----------------------------------------------------------------------------

//---------------------------------------------------------------------------
#ifndef EquipmentH
#define EquipmentH
//---------------------------------------------------------------------------

#include "Paths.h"
#include "Serializable.h"

enum TVWeapon { VWT_ARTILLERY_SHELL, VWT_SHELL_CANNON, VWT_SAM, VWT_SSM, VWT_ROCKET,
               VWT_FLAMER, VWT_PLASMA_FLAMER, VWT_LASER_CANNON, VWT_AG_MISSILE,
               VWT_AA_MISSILE, VWT_AIR_LASER, VWT_AIR_CANNON, VWT_MORTAR,
               VWT_AIR_BOMB, VWT_VEHICLE_MACHINE_GUN, VWT_NONE};
enum TIWeapon { IWT_RIFLE, IWT_MACHINE_GUN, IWT_DOUBLE_GUN, IWT_LASER_GUN,
                IWT_PLASMA_THROWER, IWT_FLAME_THROWER, IWT_ROCKET_LAUNCHER,
                IWT_GRENADE_LAUNCHER, IWT_NONE};
enum TSideFx { FX_NONE, FX_SCR_QUAKE, FX_SCR_RED, FX_SCR_WHITE, FX_SCR_WAVE, FX_SCR_CIRCULARWAVE};

typedef struct _Engine
{
  public:
    char name[20];
    char description[200];
    int max_speed;
    int max_carry;
    int weight;
    int price;
    //_eType eType;
    char bitmap[MAX_PP_PATH];
    int frames;
    char sound[MAX_PP_PATH];

  public:
    void Serialize(PP::Stream& _a);
    void DeSerialize(PP::Stream& _a);

} TUEngine;

typedef struct _VArmor
{
  public:
    char name[32];
    char description[256];
    int proj_res; // in procente - reduce cu atit la suta dintr-o lovitura
    int nrg_res; // analog
    int plasma_res;
    int bullet_res;
    int fire_res;
    int type; // 0 - ground units, 1 - air units
    int value;
    int tech_level;
    char bitmap[MAX_PP_PATH];

  public:
    void Serialize(PP::Stream& _a);
    void DeSerialize(PP::Stream& _a);

} TUVArmor;

typedef struct _IArmor
{
  public:
    char name[32];
    char description[256];
    int  proj_res; // in procente - reduce cu atit la suta dintr-o lovitura
    int  nrg_res; // analog
    int  plasma_res;
    int  bullet_res;
    int  fire_res;
    int  weight;
    int  value;
    int  tech_level;
    char bitmap[MAX_PP_PATH];

  public:
    void Serialize(PP::Stream& _a);
    void DeSerialize(PP::Stream& _a);

} TUIArmor;

typedef struct _VWeapon
{
  public:
    char name[32];
    char description[256];

    TVWeapon type;
    TSideFx side_effect;

    int minRange, maxRange;
    int damage;
    int precision;
    int speed;
    int weight;
    int value;
    int tech_level;
    int radius;
    int effect_range;
    int fire_rate;

    char bitmap[MAX_PP_PATH];
    char projectile[MAX_PP_PATH];
    char effect_bitmap[MAX_PP_PATH];
    char onhit[MAX_PP_PATH],
         onfire[MAX_PP_PATH];            // comportament audio
         
  public:
    void Serialize(PP::Stream& _a);
    void DeSerialize(PP::Stream& _a);
    
} TUVWeapon;

typedef struct _IWeapon
{
  public:
    char     name[32];
    char     description[256];
    int      minRange, maxRange;
    int      precision;
    int      radius;
    int      value;
    int      tech_level; // from 0 to 100
    int      speed; // nu va fi folosita
    int      weight;
    int      damage; //very low
    int      fire_rate;              // din cite in cite tick'uri
    TIWeapon type;
    TSideFx  side_effect; // huh !?
    char     bitmap[MAX_PP_PATH];
    char     projectile[MAX_PP_PATH];
    char     effect_bitmap[MAX_PP_PATH];
    char     onhit[MAX_PP_PATH],
             onfire[MAX_PP_PATH]; // comportament audio

  public:
    void Serialize(PP::Stream& _a);
    void DeSerialize(PP::Stream& _a);

} TUIWeapon;

// infantry weapon
class CIWeapon : public CSerializable
{
  friend class CStarShip;
  private:
    TUIWeapon mData;

    bool   mUsed;
    //bool   mNone;

  protected:
    void SetUsed(bool _used);

  public:
    CIWeapon();
    CIWeapon(TUIWeapon _data);
    CIWeapon(const CIWeapon &_weapon);

    void SetData(TUIWeapon  _data) { mData = _data; };
    void GetData(TUIWeapon* _data);
    bool IsUsed();
    bool IsNone();

    virtual void Serialize(PP::Stream& _a);
    virtual void DeSerialize(PP::Stream& _a);

    ~CIWeapon();
};

// infantry armor
class CIArmor : public CSerializable
{
  friend class CStarShip;
  private:
    struct _IArmor mData;

    bool   mUsed;
    bool   mNone;

  protected:
    void SetUsed(bool _used);

  public:
    CIArmor();
    CIArmor(struct _IArmor _data);
    CIArmor(const CIArmor &_armor);

    void GetData(struct _IArmor* _data);
    void SetData(TUIArmor _data) { mData = _data; };
    bool IsUsed();
    bool IsNone();

    virtual void Serialize(PP::Stream& _a);
    virtual void DeSerialize(PP::Stream& _a);

    ~CIArmor();
};

// vehicle weapon
class CVWeapon : public CSerializable
{
  //friend class CStarShip;
  friend class CEngineer;
  friend class CCreatorEngineer;

  private:
    TUVWeapon mData;

    bool   mUsed;
    //bool   mNone;

  protected:
    void SetUsed(bool _used);

  public:
    CVWeapon();
    CVWeapon(struct _VWeapon _data);
    CVWeapon(const CVWeapon &_weapon);

    void SetData(TUVWeapon  _data) { mData = _data; };
    void GetData(TUVWeapon* _data);
    bool IsUsed();
    bool IsNone();

    virtual void Serialize(PP::Stream& _a);
    virtual void DeSerialize(PP::Stream& _a);

    ~CVWeapon();
};

#endif
