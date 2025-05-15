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
//  Unit              : Vehicle containment class (VehicleX.h)
//
//  Versiune          : 1.0
//
//  Descriere         :
//    * contine o incapsulare a structului declarat in Sources\vehicle.h
//
//  Istorie           :
//    [Karg] - [13.04.2000] - a fost creat
//-----------------------------------------------------------------------------

//---------------------------------------------------------------------------
#ifndef VehicleXH
#define VehicleXH
//---------------------------------------------------------------------------

#include "Equipment.h"
#include "PPU_Resources.h"

enum TVehicleStatus {VEHICLE_STATUS_SCRAP, VEHICLE_STATUS_REPAIR, VEHICLE_STATUS_MODIFY, VEHICLE_STATUS_OK};
enum _vType {tank, artillery, trn, recon, aircraft};
enum _movement {air, tracked, hover, low_g, amph_track, amph_wheel, wheel};

typedef struct
{
  int x, y;
} _vPoint2D;

// Define-uri legate de spriteuri
#define MaxBodyLights   4
#define MaxTurretLights 4

typedef struct _Vehicle {
   public:
        char name[32];
        char description[512];
        _vType vType;
        _movement movement;

	     int hp, maxHp;                           // hp curent si max hp
        int mMaxCarry, mCrtCarry;                // cantitatea de echipament pe care o poate cara
        int mRstKinetic, mRstEnergy, mRstPlasma, mRstFire, mRstExplode; // resistentele armurii de pe vehicul
        int armor_weight;
        int engine_type;                         // won't be public

        TVWeapon basicWepType;                   // tipurile de arma
        TVWeapon secWepType;                     // tip de arma

        int value;
        int tech_level;

        int turretSpeed;                         // la cite frame'uri se roteste turela (frecventa) - numar intreg
        bool selfdestruct;

        char visual_comp[MAX_PP_PATH];
        char thumbnail[MAX_PP_PATH];             // un bitmap care apare in interfata

        _vPoint2D body[8], turret[16];           // punctele de fixare ale turelei pe carcasa
        _vPoint2D primary, secondary;            // s'ar putea sa nu le folosesc

        _vPoint2D bodyLights[MaxBodyLights];     // lumini pe carcasa
        int  bodyLightsNo;

        _vPoint2D turretLights[MaxTurretLights]; // lumini pe turela
        int  turretLightsNo;

  public: // serialization - deserialization
    void Serialize(PP::Stream& _a);
    unsigned __int32 DataSize();
    void DeSerialize(PP::Stream& _a);

} TUVehicle;

class CVehicle : public CSerializable
{
  friend class CStarShip;
  friend class CEngineer;
  friend class CCreatorEngineer;

  private:

  TUVehicle mData;
  CVWeapon* mFirst;
  CVWeapon* mSecond;

  bool           mUsed;
  bool           mNone;
  TVehicleStatus mStatus;

  protected:
    //bool SetFirstWeapon(CVWeapon* _weapon);
    //bool SetSecondWeapon(CVWeapon* _weapon);
    void SetUsed(bool _used);

  public:
    CVehicle();
    CVehicle(TUVehicle _vehicle);
    CVehicle(const CVehicle& _vehicle);

    void SetData(TUVehicle _vehicle) { mData = _vehicle; };
    void GetData(TUVehicle* _vehicle);
    void Modify();

    CVWeapon* GetFirstWeapon();
    CVWeapon* GetSecondWeapon();

    bool IsUsed();
    bool IsNone();

    TVehicleStatus GetStatus() { return mStatus; };
    void SetStatus(TVehicleStatus _status) { mStatus = _status; };

    TTechResources ResForRepair();

    virtual void Serialize(PP::Stream& _a);
    virtual void DeSerialize(PP::Stream& _a);

    ~CVehicle();
};

#endif
