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
//---------------------------------------------------------------------------
#ifndef staticsH
#define staticsH
//---------------------------------------------------------------------------

#include "Paths.h"
#include "Stream.h"
#include "AIUnits.h"

const CPString STATIC_FILE_ID = "PPTACTICAL::STATIC";

const int MaxSoldiersInside = 32;
const int MaxMask           = 32;
const int MaxLayers         = 64;
const int STATICS_MAXVISUAL = 8;

enum _swtype {s_shell_cannon, s_sam, s_ssm, s_rocket, s_flamer, s_plasma_flamer, s_laser_cannon, s_mortar, s_proj_gun};
enum _mwtype {s_rifle, s_machine_gun, s_double_gun, s_laser_gun, s_plasma_thrower, s_flame_thrower, s_rocket_launcher, s_grenade_launcher};
enum TStaticType { ST_NORMAL, ST_TURRET, ST_PILLBOX, ST_FACTORY };
//enum _reactAt {r_fire, r_projectile, r_plasma, r_laser, r_explosion};

// tip de soldat care poate exista in cladire
typedef struct _MiniSoldier
{
  public:
    // armura
    int  proj_res;                 // in procente - reduce cu atit la suta dintr-o lovitura
    int  nrg_res;                  // analog
    int  plasma_res;
    int  bullet_res;
    int  fire_res;
    // arma
    int  range;
    int  precision;
    int  radius;
    int  fire_rate;
    char projectile[MAX_PP_PATH];
    int  projectile_frames;
    char effect_bitmap[MAX_PP_PATH];
    //int  effect_frames;
    // caracteristici personale
    int  morale;
    int  hp;                       // hit points
    int  bravery;
    int  sr_combat;
    int  sniper_skill;
    char sprite[MAX_PP_PATH];               // sprite-ul = cum va arata omul in joc

  public: // serialization - deserialization
    void Serialize(PP::Stream& _a);
    unsigned __int32 DataSize();
    void DeSerialize(PP::Stream& _a);
    
} *hMiniSoldier, TMiniSoldier;

// arma pe care o poate avea o cladire
typedef struct _StaticWeapon
{
  public:
    char    wepName[32];
    int     range;
    int     precision;
    int     radius;
    int     damage;
    char    projectile[MAX_PP_PATH];
    //int     projectile_frames;
    char    effect_bitmap[MAX_PP_PATH];
    //int     effect_frames;
    int     fire_rate;
    char    onFire[MAX_PP_PATH], onHit[MAX_PP_PATH];
    /*_swtype*/unsigned char type;

  public: // serialization - deserialization
    void Serialize(PP::Stream& _a);
    unsigned __int32 DataSize();
    void DeSerialize(PP::Stream& _a);

} *hStaticWeapon, TStaticWeapon;

typedef struct _StaticLayer
{
  public: // data
    char  mName[32];
    char  mSprite[512];
    int   mFrameDelay, // delay in ms between two consecutive frames
          mCycleDelay; // delay in ms between two cycles
    int   mX, mY;
    float mAlpha; // [0.0 - invisible, 1.0 - full visible]
    int   mSpriteRows, mSpriteCols; // the rows and columns of the sprite (like horizontal
                        // cells and vertical cells)

  public: // serialization - deserialization
    void Serialize(PP::Stream& _a);
    unsigned __int32 DataSize();
    void DeSerialize(PP::Stream& _a);

} TStaticLayer;

// caracteristicile obectului
typedef struct _StaticObject
{
  public:
    char objName[32];
    char objDescription[256];

    int  hp;                       // hit-points

    int  proj_res;                 // in procente - reduce cu atit la suta dintr-o lovitura
    int  nrg_res;                  // analog
    int  plasma_res;
    int  bullet_res;
    int  fire_res;

    bool spreadFire;
    bool indestructible;
    bool isLandscape;

    bool canFire;
    _StaticWeapon
         weapon;

    bool canContainSoldiers;
    int  soldiersNo;
    int  defendTable;
    _MiniSoldier
         soldiersTable[MaxSoldiersInside];

    bool canContainVehicles;
    int  vehiclesNo;

    CPPoint mDropPoint, mFirePoint;

    TUnitObservationPoint mObsPoints[STATICS_MAXVISUAL];
    int                   mObsPointsCount;

    /*_anim*/
    unsigned char type;

    char sprite[MAX_PP_PATH];

    int   maskX, maskY;
    char  mask[MaxMask][MaxMask];
    int   altitude[MaxMask][MaxMask];

    int          mLayersCount;
    TStaticLayer mLayers[MaxLayers];

    /*
    _reactAt
            reactAt;
    */
  public: // serialization - deserialization
    void Serialize(PP::Stream& _a);
    unsigned __int32 DataSize();
    void DeSerialize(PP::Stream& _a);

} *hStaticObject, TStaticObject;

#define SRecSize sizeof(struct _StaticObject)

#endif
