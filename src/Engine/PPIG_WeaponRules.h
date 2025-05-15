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
//  Unit              : Weapon Rules
//
//  Version           : 1.0
//
//  Description       :
//    * contains methods that check object-object fire
//    * a unit must call upon this class to check if it may fire
//      another unit or may fire ground
//    * the methods check the object state and weapon type to see
//      if firing is possible (e.g. if a chopper is on the ground,
//      it may not fire air-to-air weapons)
//
//  History           :
//    [12.06.2004] - [Karg] - unit&class created
//-----------------------------------------------------------------------------

#ifndef PPIG_WeaponRulesH
#define PPIG_WeaponRulesH
//---------------------------------------------------------------------------

#include "Serializable.h"

// forward
class CIGBaseObject;
class CIGChopperUnit;
class CIGFighterUnit;
class CIGInfantryUnit;
class CIGTankUnit;

class CIGWeaponRules : public CSerializable
{
  public: // c-d
    CIGWeaponRules();
    ~CIGWeaponRules();

  public: // ops
    bool CanFireObject(CIGChopperUnit*  _attacker,  int _weapon, CIGBaseObject* _target);
    bool CanFireObject(CIGFighterUnit*   _attacker,  int _weapon, CIGBaseObject* _target);
    bool CanFireObject(CIGInfantryUnit* _attacker,  int _weapon, CIGBaseObject* _target);
    bool CanFireObject(CIGTankUnit* _attacker,  int _weapon, CIGBaseObject* _target);

    bool CanFireGround(CIGChopperUnit*  _attacker,  int _weapon);
    bool CanFireGround(CIGFighterUnit*   _attacker,  int _weapon);
    bool CanFireGround(CIGInfantryUnit* _attacker,  int _weapon);
    bool CanFireGround(CIGTankUnit* _attacker,  int _weapon);

  public: // based on unit id
    bool CanFireObject(int _attackerId, int _weapon, int _targetId);
    bool CanFireGround(int _attackerId, int _weapon);

  public:
    virtual void Serialize(PP::Stream& _a) {}
    virtual unsigned __int32 DataSize() { return 0; }
    virtual void DeSerialize(PP::Stream& _a) {}

  protected:

  private:
    bool IsAir(CIGBaseObject* _target);
    bool IsGround(CIGBaseObject* _target);
};

#endif
