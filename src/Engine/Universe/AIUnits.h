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
/*---------------------------------------------------------------------------
 application: PP Units Editor, PP Mission Editor

 descrition : structuri unificate de inf, vehicule si avioane
 last modify: 29 11 99
        by Grab
---------------------------------------------------------------------------*/
//---------------------------------------------------------------------------
#ifndef AIUnitsH
#define AIUnitsH

#include "basic2d.h"
#include "Stream.h"

const int VEHICLE_MAXBODYLIGHTS   = 4;
const int VEHICLE_MAXTURRETLIGHTS = 4;
const int VEHICLE_MAXVISUAL       = 4;
const int INFANTRY_MAXVISUAL      = 4;

//enum _aiiwtype {rifle, machine_gun, double_gun, laser_gun, plasma_thrower, flame_thrower, rocket_launcher, grenade_launcher};
//enum _vType {tank = 0, artillery, trn, recon, aircraft_fighter, aircraft_bomber, aircraft_chopper, aircraft_dropship};
//enum _movement {air, tracked, hover, low_g, amph_track, amph_wheel, wheel};

//
// This structure is used for infantry, vehicles and statics to specify a visual subsystem
//
typedef struct _UnitObservationPoint
{
  public:
    char mName[32];
    int  mMinScanRange, mMaxScanRange;
    int  mFOV, mScanAngle;
    int  mMinScanAngle, mMaxScanAngle;
    bool mActive;

    CPPoint mMountPoints[8]; // redesign this if you have a subsystem that uses
                             // more than 8 mount points

  public: // serialization - deserialization
    void Serialize(PP::Stream& _a);
    unsigned __int32 DataSize();
    void DeSerialize(PP::Stream& _a);
} *hUnitObservationPoint, TUnitObservationPoint;

//structura de inf in varianta unificata
typedef struct _InfantryUnified
{
   //personal data
   char name[16];
   char surname[16];
   char description[192];
   int  gender;            //0-male, 1-female, define other values as necessary for your game
   int  type;              //0-special, 1-ordinary
   char kind[15];          //pentru ordinary un fel de descriere
   int  morale;
   int  hp;                // hit points

   char sprite[32];        // sprite-ul = cum va arata omul in joc
                           // doar numele fisierului, calea fiind data implicit
   char shadow[32];        // is not used for the moment
   char mPortrait[32];     // portrait - will appear in the units info bar
   char mMiniPortrait[32]; // mini portrait - will appear in the group selection and message box

   // skills
   int  leadership;
   int  experience;
   int  bravery;
   int  camo;
   int  antitank;
   int  sr_combat;
   int  snipery;
   int  high_tech;         // un plus la infiltrarea cladirilor
   int  stealth;
   int  influence;
   int  wisdom;
   int  mMaxUpSlope, mMaxDownSlope; // max slope the soldier may climb/descend (soldiers with rockets may not be able to climb high slopes)

   // armor
   int  proj_res;          // projectile resistance
   int  nrg_res;           // energy resistance
   int  plasma_res;
   int  bullet_res;
   int  fire_res;

   // weapon
   //  _aiiwtype weptype;
   int  weptype;      //vezi _aiiwtype
   int  damage;
   int  range;
   int  precision;
   int  radius;
   int  fire_rate;
   int  projectiles_per_shot; // number of projectiles fired per shot (useful for simulating shotguns, flak cannons)
                              // for single shot weapons, use 1
   int  mTimeToLive;          // the life of the projectile, in miliseconds

   int  mTrackPersistency; // time for projectile track effect to fade out
   char projectile[32];    // bitmap cu proiectilul
   char effect_bitmap[32]; // bitmap cu explozia proiectilului
   char aftereffect_bitmap[32]; // bitmap with the effect after the explosion (smoke for example)
   char onhit[32];         // comportament audio
   char onfire[32];        // comportament audio

   // points
   TUnitObservationPoint mObsPoints[INFANTRY_MAXVISUAL];
   int                   mObsPointsCount;
   CPPoint               mFireHoles[8];

   //audio comportament
   char resp1[12], resp2[12], resp3[12];
   char ack1[12], ack2[12], ack3[12];
   char annoyed1[12], annoyed2[12], annoyed3[12];
   char onhit1[12], onhit2[12];
   char onkill1[12], onkill2[12];
   char die1[12], die2[12];
   char onidle1[12], onidle2[12];

  public: // serialization - deserialization
    void Serialize(PP::Stream& _a);
    unsigned __int32 DataSize();
    void DeSerialize(PP::Stream& _a);

} *hInfantryUnified, TInfantryUnified;

#define AIInfRecSize sizeof(TInfantryUnified)

bool verifyAIInfFile(const char *filename);
int getAIInfRecords(const char *filename);
bool getAIInfRecord(const char* filename, int index, hInfantryUnified infantry);

//structura de inf in varianta unificata
typedef struct _VehicleUnified
{
  public:
   //vehicle data
   char name[32];                   // numele vehiculului
   int hp;                          // hit-points
   int engine_type;                 // nu stiu la ce foloseste, poate stie stef
   bool selfdestruct;
   char sprite[32];                 // sprite
   char shadow[32];                 // sprite for shadows (unused, will be used if the sprites grow too large)
   char mPortrait[32];              // portrait - will appear in the units info bar
   char mMiniPortrait[32];          // will appear in the group selection and in the message box

//   _vType vType;
//   _movement movement;
   int vType;                       // vezi vType
   int mTurretTurnRate;             // the rate for turret turning (in frame-times - 33ms * (mTurretTurnRate + 1) = actual turn rate)
   CPPoint body[8], turret[16];                  // punctele de fixare ale turelei pe carcasa
   CPPoint primary[8], secondary[8];             // fire "mount" point for primary and secondary weapon
                                                 // relative to the turret for vehicle, relative to the body for air units
   CPPoint bodyLights[VEHICLE_MAXBODYLIGHTS];    // lumini pe carcasa
   int  bodyLightsNo;
   CPPoint turretLights[VEHICLE_MAXTURRETLIGHTS];      // lights place on the turret
   int  turretLightsNo;

   TUnitObservationPoint mObsPoints[INFANTRY_MAXVISUAL];            // observation points (for visual subsys)
   int                   mObsPointsCount;

   int  mBayCapacity;               // in arbitrary units; LogicDimX*LogicDimY is the space taken by a unit (an infantry takes 1 space)
                                    // maybe it's not the realistic method, but it's damn simple ;)
   int  movement;                   // vezi _movement
   int  mMaxUpSlope, mMaxDownSlope; // max slope the vehicle may climb/descend

   //int  maxSpeed; /**< maximum speed of the unit, see CIGBaseUnit::GetMaxSpeed(); as soon as the editors can be compiled, introduce this */

   // die :)
   char mExplodeSprite[32];         // a sprite with an explosion that will be drawn (inserted) over
                                    // when the unit dies; it's useful for aircrafts so they
                                    // won't suddenly dissapear
   int  mExplodeCount;              // there will be mExplodeCount explosions randomly
                                    // distributed over the unit surface

   //crew
   char callsign[16];               // cum ii zice echipajului
   char description[192];           // descrierea echipajului, cred ca nu va fi folosita
   //int  gunnery;                  // skill de nimerit la tinta
   int  experience;                 // experienta in domeniu
   int  driveskill;                 // nota de la scoala de soferi :)
   int  bravery;                    // o bra(d)v frumos
   int  morale;                     // cum sta cu moralu'
   int  leadership;                 // cat de hagi ii
   int  anti_infantry;              // gunnery against infantry
   int  anti_vehicle;               // gunnery against vehicles
   int  anti_air;                   // gunnery against aircrafts
   int  cm_skill;                   // countermeasures - avoiding enemy fire
   int  teamwork;

   //weapon1
   bool wep1_enabled;
   int wep1_type;
   int wep1_damage;
   int wep1_minrange;
   int wep1_maxrange;
   int wep1_precision;
   int wep1_radius;
   int wep1_firerate;               // the rate for weapon firing (in frame-times - 33ms * (wep1_firerate + 1) = actual fire rate)
   int wep1_projectiles_per_shot;   // number of projectiles fired per shot (useful for simulating shotguns, flak cannons)
                                    // for single shot weapons, use 1
   int mWep1_TimeToLive;            // the life of the projectile, in miliseconds
   int wep1_trackPersistency;       // time for projectile track effect to fade out

   char wep1_projectile[32];        // bitmap cu proiectilul
   char wep1_effectbitmap[32];      // bitmap cu explozia proiectilului
   char wep1_aftereffectbitmap[32]; // bitmap with the effect after the explosion (smoke for example)
   char wep1_onhit[32];             // comportament audio
   char wep1_onfire[32];            // comportament audio

   //weapon2
   bool wep2_enabled;
   int wep2_type;
   int wep2_damage;
   int wep2_minrange;
   int wep2_maxrange;
   int wep2_precision;
   int wep2_radius;
   int wep2_firerate;               // the rate for weapon firing (in frame-times - 33ms * (wep2_firerate + 1) = actual fire rate)
   int wep2_projectiles_per_shot;   // number of projectiles fired per shot (useful for simulating shotguns, flak cannons)
                                    // for single shot weapons, use 1
   int mWep2_TimeToLive;            // the life of the projectile, in miliseconds
   int wep2_trackPersistency;       // time for projectile track effect to fade out

   char wep2_projectile[32];        // bitmap cu proiectilul
   char wep2_effectbitmap[32];      // bitmap cu explozia proiectilului
   char wep2_aftereffectbitmap[32]; // bitmap with the effect after the explosion (smoke for example)
   char wep2_onhit[32];             // comportament audio
   char wep2_onfire[32];            // comportament audio

   //armor
   int  exp_res;                    // explosion resistance
   int  nrg_res;                    // energy resistance
   int  plasma_res;                 // plasma resistance
   int  bullet_res;                 // bullet resistance
   int  fire_res;                   // fire resistance

   //audio comportament
   char resp1[32], resp2[32], resp3[32];
   char ack1[32], ack2[32], ack3[32];
   char annoyed1[32], annoyed2[32], annoyed3[32];
   char onhit1[32], onhit2[32];
   char onkill1[32], onkill2[32];
   char die1[32], die2[32];
   char onidle1[32], onidle2[32];

  public: // serialization - deserialization
    void Serialize(PP::Stream& _a);
    unsigned __int32 DataSize();
    void DeSerialize(PP::Stream& _a);

} *hVehicleUnified, TVehicleUnified;

#define AIVehRecSize sizeof(TVehicleUnified)

bool verifyAIVehFile(const char *filename);
int getAIVehRecords(const char *filename);
bool getAIVehRecord(const char* filename, int index, hVehicleUnified infantry);
//---------------------------------------------------------------------------
#endif
