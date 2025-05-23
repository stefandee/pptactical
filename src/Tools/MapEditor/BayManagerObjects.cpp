//---------------------------------------------------------------------------
#pragma hdrstop

#include "BayManagerObjects.h"
#include "PPIG_ObjInfantry.h"
#include "PPIG_ObjVehicle.h"
#include "PPIG_ObjTran.h"
#include "PPIG_ObjDropship.h"
#include "Paths.h"
#include "PPIG_Mission.h"
#include "PPIG_BGManager.h"
#include "Tools.h"
#include "basepath.h"
//---------------------------------------------------------------------------

BayManagerInfantryFromFile::BayManagerInfantryFromFile(int _index)
{
  mIndex = _index;
}
//---------------------------------------------------------------------------

void BayManagerInfantryFromFile::Board(CIGDropship* _dropship)
{
  CIGInfantryUnit* lInfUnit = createInfantryUnit(
      GetMissionInstance(),
      AnsiString(getGamePath().c_str()) + PATH_AIINFANTRY + FILE_AIINFANTRY,
      mIndex,
      GetMissionInstance()->GetUniqueUnitId(),
      _dropship->GetBattleGroupId(),
      -1
    );

  if (!_dropship->Load(lInfUnit))
  {
    // the unit cannot be loaded, remove it
    GetMissionInstance()->AddToDeleteObjects(lInfUnit);
    GetMissionInstance()->GetBGManager()->RemoveUnit(lInfUnit);
    GetMissionInstance()->GetToDeleteObjects()->FreeAll();
  }
  else
  {
    // add the unit to the transported units layer
    GetMissionInstance()->GetObjects(MOL_TRANSPORTEDUNITS)->AddUnique(lInfUnit);
  }  
}
//---------------------------------------------------------------------------

void BayManagerInfantryFromFile::Board(CIGTranUnit* _tran)
{
  CIGInfantryUnit* lInfUnit = createInfantryUnit(
      GetMissionInstance(),
      AnsiString(getGamePath().c_str()) + PATH_AIINFANTRY + FILE_AIINFANTRY,
      mIndex,
      GetMissionInstance()->GetUniqueUnitId(),
      _tran->GetBattleGroupId(),
      -1
    );

  if (!_tran->Load(lInfUnit))
  {
    GetMissionInstance()->AddToDeleteObjects(lInfUnit);
    GetMissionInstance()->GetBGManager()->RemoveUnit(lInfUnit);
    GetMissionInstance()->GetToDeleteObjects()->FreeAll();
  }
  else
  {
    // add the unit to the transported units layer
    GetMissionInstance()->GetObjects(MOL_TRANSPORTEDUNITS)->AddUnique(lInfUnit);
  }
}
//---------------------------------------------------------------------------

BayManagerInfantryFromMap::BayManagerInfantryFromMap(CIGInfantryUnit* _unit)
{
  mUnit = _unit;
}
//---------------------------------------------------------------------------

void BayManagerInfantryFromMap::Board(CIGDropship* _dropship)
{
  if (_dropship->Load(mUnit))
  {
    // remove the unit from mission unit lists
    GetMissionInstance()->GetObjects(MOL_ALLOBJECTS)->Remove(mUnit);
    mUnit->RemoveFromLogicMap();
    mUnit->RemoveFromLayerMap();

    // add the unit to the transported units layer
    GetMissionInstance()->GetObjects(MOL_TRANSPORTEDUNITS)->AddUnique(mUnit);
  }
}
//---------------------------------------------------------------------------

void BayManagerInfantryFromMap::Board(CIGTranUnit* _tran)
{
  if (_tran->Load(mUnit))
  {
    // remove the unit from mission unit lists
    GetMissionInstance()->GetObjects(MOL_ALLOBJECTS)->Remove(mUnit);
    mUnit->RemoveFromLogicMap();
    mUnit->RemoveFromLayerMap();

    // add the unit to the transported units layer
    GetMissionInstance()->GetObjects(MOL_TRANSPORTEDUNITS)->AddUnique(mUnit);
  }
}
//---------------------------------------------------------------------------

BayManagerVehicleFromFile::BayManagerVehicleFromFile(int _index)
{
  mIndex = _index;
}
//---------------------------------------------------------------------------

void BayManagerVehicleFromFile::Board(CIGDropship* _dropship)
{
  CIGVehicle* lVehicleUnit = createVehicleUnit(
      GetMissionInstance(),
      AnsiString(getGamePath().c_str()) + PATH_AIVEHICLE + FILE_AIVEHICLE,
      mIndex,
      GetMissionInstance()->GetUniqueUnitId(),
      _dropship->GetBattleGroupId(),
      -1
    );

  if (!_dropship->Load(lVehicleUnit))
  {
    GetMissionInstance()->AddToDeleteObjects(lVehicleUnit);
    GetMissionInstance()->GetBGManager()->RemoveUnit(lVehicleUnit);
    GetMissionInstance()->GetToDeleteObjects()->FreeAll();
  }
  else
  {
    // add the unit to the transported units layer
    GetMissionInstance()->GetObjects(MOL_TRANSPORTEDUNITS)->AddUnique(lVehicleUnit);
  }
}
//---------------------------------------------------------------------------

void BayManagerVehicleFromFile::Board(CIGTranUnit* _tran)
{
  // cannot do that - simply empty ;)
}
//---------------------------------------------------------------------------

BayManagerVehicleFromMap::BayManagerVehicleFromMap(CIGVehicle* _unit)
{
  mUnit = _unit;
}
//---------------------------------------------------------------------------

void BayManagerVehicleFromMap::Board(CIGDropship* _dropship)
{
  if(_dropship->Load(mUnit))
  {
    // and remove the unit from mission unit lists
    GetMissionInstance()->GetObjects(MOL_ALLOBJECTS)->Remove(mUnit);
    mUnit->RemoveFromLogicMap();
    mUnit->RemoveFromLayerMap();

    // add the unit to the transported units layer
    GetMissionInstance()->GetObjects(MOL_TRANSPORTEDUNITS)->AddUnique(mUnit);
  }
}
//---------------------------------------------------------------------------

void BayManagerVehicleFromMap::Board(CIGTranUnit* _tran)
{
  // cannot do that - simply empty ;)
}
//---------------------------------------------------------------------------

#pragma package(smart_init)
