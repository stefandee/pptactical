//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "AIUnits.h"
#include "PPIG_Mission.h"
#include "FBayManager.h"
#include "PPIG_ObjUnit.h"
#include "PPIG_ObjTran.h"
#include "PPIG_ObjDropShip.h"
#include "PPIG_ObjInfantry.h"
#include "Tools.h"
#include "BayManagerObjects.h"
#include "basepath.h"
//---------------------------------------------------------------------------

#pragma package(smart_init)
#pragma resource "*.dfm"
TFormBayManager *FormBayManager;
//---------------------------------------------------------------------------

__fastcall TFormBayManager::TFormBayManager(TComponent* Owner)
        : TForm(Owner)
{
  mUnit = 0;
}
//---------------------------------------------------------------------------

void TFormBayManager::UpdateBayGrid()
{
  CreateBayGridFixed();

  if (!mUnit)
  {
    return;
  }

  // this unit may be a dropship or a transporter
  //gridBay->Cells[0][lRow] = "";
  //gridBay->Cells[1][lRow] = "";
  //gridBay->Cells[2][lRow] = "";

  if (mUnit->ClassName() == CPString(IGCN_DROPSHIPUNIT))
  {
    CIGDropship* lUnit = (CIGDropship*)mUnit;

    statusBay->SimpleText = "Units: " + AnsiString(lUnit->GetCapacity()) + ", Bay capacity:" + AnsiString(lUnit->GetAircraftUnifiedData().mBayCapacity);

    if (lUnit->GetCapacity() > 0)
    {
      gridBay->RowCount = 1 + lUnit->GetCapacity();
      gridBay->Enabled = true;
    }
    else
    {
      CreateBayGridFixed();
      gridBay->Enabled = false;
      return;
    }

    for(int i = 0; i < lUnit->GetCapacity(); i++)
    {
      CIGUnit* lContainedUnit = lUnit->GetUnit(i);

      gridBay->Cells[0][i + 1] = AnsiString(i + 1);
      gridBay->Cells[1][i + 1] = lContainedUnit->ClassName().c_str();
      gridBay->Cells[2][i + 1] = lContainedUnit->GetId();

      //lRow++;
    }
  }

  if (mUnit->ClassName() == CPString(IGCN_TRANUNIT))
  {
    CIGTranUnit* lUnit = (CIGTranUnit*)mUnit;

    statusBay->SimpleText = "Units: " + AnsiString(lUnit->GetCapacity()) + ", Bay capacity:" + AnsiString(lUnit->GetVehicleUnifiedData().mBayCapacity);

    if (lUnit->GetCapacity() > 0)
    {
      gridBay->RowCount = 1 + lUnit->GetCapacity();
      gridBay->Enabled = true;
    }
    else
    {
      CreateBayGridFixed();
      gridBay->Enabled = false;
      return;
    }

    for(int i = 0; i < lUnit->GetCapacity(); i++)
    {
      CIGUnit* lContainedUnit = lUnit->GetUnit(i);

      gridBay->Cells[0][i + 1] = AnsiString(i + 1);
      gridBay->Cells[1][i + 1] = lContainedUnit->ClassName().c_str();
      gridBay->Cells[2][i + 1] = lContainedUnit->GetId();
    }
  }
}
//---------------------------------------------------------------------------

void TFormBayManager::UpdateAvailGrid()
{
  DeleteObjects();

  CreateAvailGridFixed();

  if (!mUnit)
  {
    return;
  }

  if (mUnit->Can(CAN_CARRY_VEHICLES))
  {
    UpdateAvailGridForFileVehicles();
    UpdateAvailGridForMapVehicles();
  }

  if (mUnit->Can(CAN_CARRY_INFANTRY))
  {
    UpdateAvailGridForFileInfantry();
    UpdateAvailGridForMapInfantry();
  }

  if (gridAvail->RowCount > 1)
  {
    gridAvail->RowCount = gridAvail->RowCount - 1;
  }
}
//---------------------------------------------------------------------------

void TFormBayManager::UpdateAvailGridForFileVehicles()
{
  int i, lRow = gridAvail->RowCount - 1;
  AnsiString fileAIVeh = AnsiString(getGamePath().c_str()) + PATH_AIVEHICLE + FILE_AIVEHICLE;
  TVehicleUnified recAIVeh;

  // gridAvail->RowCount = gridAvail->RowCount + getAIVehRecords(fileAIVeh.c_str());
  // gridAvail->FixedRows = 1;

  for (i = 0; i < getAIVehRecords(fileAIVeh.c_str()); i++)
  {
    if (getAIVehRecord(fileAIVeh.c_str(), i, &recAIVeh))
    {
      switch(recAIVeh.vType)
      {
        case 4:
        case 5:
        case 6:
        case 7:
          // these are aircrafts; there is no unit that may carry aircrafts
          // if you decide to introduce one, create a similar method
          // (UpdateAvailGridForVehicles) where you may process aircrafts
          break;

        default:
          gridAvail->RowCount = gridAvail->RowCount + 1;
          gridAvail->Cells[0][lRow] = AnsiString(lRow);
          gridAvail->Cells[1][lRow] = AnsiString(recAIVeh.name) + "/" + AnsiString(recAIVeh.callsign);
          gridAvail->Cells[2][lRow] = GetVehicleType(recAIVeh.vType);
          gridAvail->Cells[3][lRow] = FILE_AIVEHICLE;

          // attach a baymanager object
          gridAvail->Objects[0][lRow] = new BayManagerVehicleFromFile(i);

          lRow++;
          break;
      }
    }
  }
}
//---------------------------------------------------------------------------

void TFormBayManager::UpdateAvailGridForFileInfantry()
{
  int i, lRow = gridAvail->RowCount - 1;
  AnsiString fileAIInf = AnsiString(getGamePath().c_str()) + PATH_AIINFANTRY + FILE_AIINFANTRY;
  TInfantryUnified recAIInf;

  //gridAvail->RowCount  = gridAvail->RowCount + getAIInfRecords(fileAIInf.c_str());
  //gridAvail->FixedRows = 1;

  for (i = 0; i < getAIInfRecords(fileAIInf.c_str()); i++)
  {
    if (getAIInfRecord(fileAIInf.c_str(), i, &recAIInf))
    {
      gridAvail->RowCount = gridAvail->RowCount + 1;
      gridAvail->Cells[0][lRow] = AnsiString(lRow);
      gridAvail->Cells[2][lRow] = "Infantry";
      gridAvail->Cells[3][lRow] = FILE_AIINFANTRY;

      if (recAIInf.type == 0)
      {
        //adica special
        gridAvail->Cells[1][lRow] = AnsiString(recAIInf.name) + " " + AnsiString(recAIInf.surname);
      }
      else
      {
        //adica ordinary
        gridAvail->Cells[1][lRow] = AnsiString(recAIInf.kind);
      }

      // attach a baymanager object
      gridAvail->Objects[0][lRow] = new BayManagerInfantryFromFile(i);

      lRow++;
    }
  }
}
//---------------------------------------------------------------------------

void TFormBayManager::UpdateAvailGridForMapInfantry()
{
  int lRow = gridAvail->RowCount - 1;

  for(int i = 0; i < GetMissionInstance()->GetObjects(MOL_ALLOBJECTS)->Count(); i++)
  {
    CIGBaseObject* lObject = GetMissionInstance()->GetObjects(MOL_ALLOBJECTS)->GetObject(i);

    if (!lObject)
    {
      continue;
    }

    // check if we're on the same battlegroup - could also allow to load units
    // from friendly battlegroups into a transporter?
    if (mUnit->GetBattleGroupId() != lObject->GetBattleGroupId())
    {
      continue;
    }
    
    if (lObject->ClassName() == CPString(IGCN_INFANTRYUNIT))
    {
      CIGInfantryUnit* lInfUnit = (CIGInfantryUnit*)lObject;

      gridAvail->RowCount = gridAvail->RowCount + 1;

      gridAvail->Objects[0][lRow] = new BayManagerInfantryFromMap(lInfUnit);

      gridAvail->Cells[0][lRow] = AnsiString(lRow);
      gridAvail->Cells[1][lRow] = AnsiString(lInfUnit->GetInfUnifiedData().name) + " " + lInfUnit->GetInfUnifiedData().surname;
      gridAvail->Cells[2][lRow] = "Infantry";
      gridAvail->Cells[3][lRow] = "Map";

      lRow++;
    }
  }
}
//---------------------------------------------------------------------------

void TFormBayManager::UpdateAvailGridForMapVehicles()
{
  int lRow = gridAvail->RowCount - 1;

  for(int i = 0; i < GetMissionInstance()->GetObjects(MOL_ALLOBJECTS)->Count(); i++)
  {
    CIGBaseObject* lObject = GetMissionInstance()->GetObjects(MOL_ALLOBJECTS)->GetObject(i);

    if (!lObject)
    {
      continue;
    }

    // check if we're on the same battlegroup
    if (mUnit->GetBattleGroupId() != lObject->GetBattleGroupId())
    {
      continue;
    }

    if (lObject->IsDerivedFrom(IGCN_VEHICLEUNIT))
    {
      CIGVehicle* lVehUnit = (CIGVehicle*)lObject;

      gridAvail->RowCount = gridAvail->RowCount + 1;

      gridAvail->Objects[0][lRow] = new BayManagerVehicleFromMap(lVehUnit);

      gridAvail->Cells[0][lRow] = AnsiString(lRow);
      gridAvail->Cells[1][lRow] = AnsiString(lVehUnit->GetVehicleUnifiedData().name) + "/" + lVehUnit->GetVehicleUnifiedData().callsign;
      gridAvail->Cells[2][lRow] = lObject->ClassName().c_str();
      gridAvail->Cells[3][lRow] = "Map";

      lRow++;
    }
  }
}
//---------------------------------------------------------------------------

void TFormBayManager::CreateBayGridFixed()
{
  gridBay->RowCount = 2;
  gridBay->FixedRows = 1;

  gridBay->Cells[0][0] = "No.";
  gridBay->Cells[1][0] = "Name";
  gridBay->Cells[2][0] = "Class";

  gridBay->Cells[0][1] = "";
  gridBay->Cells[1][1] = "";
  gridBay->Cells[2][1] = "";
}
//---------------------------------------------------------------------------

void TFormBayManager::CreateAvailGridFixed()
{
  gridAvail->RowCount  = 2;
  gridAvail->FixedRows = 1;

  gridAvail->Cells[0][0] = "No.";
  gridAvail->Cells[1][0] = "Name";
  gridAvail->Cells[2][0] = "Class";
  gridAvail->Cells[3][0] = "Where";
}
//---------------------------------------------------------------------------

void TFormBayManager::SetTransport(CIGUnit* _tran)
{
  mUnit = _tran;

  UpdateBayGrid();
  UpdateAvailGrid();
}
//---------------------------------------------------------------------------

void TFormBayManager::DeleteObjects()
{
  for(int i = 0; i < gridAvail->RowCount; i++)
  {
    delete gridAvail->Objects[0][i];
  }
}
//---------------------------------------------------------------------------

void __fastcall TFormBayManager::FormDestroy(TObject *Sender)
{
  // clean exit
  DeleteObjects();
}
//---------------------------------------------------------------------------

void __fastcall TFormBayManager::btnAddClick(TObject *Sender)
{
  if (mUnit->ClassName() == CPString(IGCN_DROPSHIPUNIT))
  {
    if (gridAvail->Objects[0][gridAvail->Row])
    {
      ((BayManagerObject*)(gridAvail->Objects[0][gridAvail->Row]))->Board((CIGDropship*)mUnit);

      UpdateBayGrid();
      UpdateAvailGrid();
    }
  }

  if (mUnit->ClassName() == CPString(IGCN_TRANUNIT))
  {
    if (gridAvail->Objects[0][gridAvail->Row])
    {
      ((BayManagerObject*)(gridAvail->Objects[0][gridAvail->Row]))->Board((CIGTranUnit*)mUnit);

      UpdateBayGrid();
      UpdateAvailGrid();
    }
  }
}
//---------------------------------------------------------------------------

void __fastcall TFormBayManager::btnRemoveClick(TObject *Sender)
{
  if (mUnit->ClassName() == CPString(IGCN_DROPSHIPUNIT))
  {
    CIGDropship* lDropshipUnit = (CIGDropship*)mUnit;

    CIGUnit* lBayUnit = lDropshipUnit->GetUnit(gridBay->Row - 1);

    if (lBayUnit)
    {
      lDropshipUnit->RemoveUnit(lBayUnit);
    }

    UpdateBayGrid();
  }

  if (mUnit->ClassName() == CPString(IGCN_TRANUNIT))
  {
    CIGTranUnit* lTranUnit = (CIGTranUnit*)mUnit;

    CIGUnit* lBayUnit = lTranUnit->GetUnit(gridBay->Row - 1);

    if (lBayUnit)
    {
      lTranUnit->RemoveUnit(lBayUnit);
    }

    UpdateBayGrid();
  }
}
//---------------------------------------------------------------------------

void __fastcall TFormBayManager::btnClearBayClick(TObject *Sender)
{
  if (mUnit->ClassName() == CPString(IGCN_DROPSHIPUNIT))
  {
    CIGDropship* lDropshipUnit = (CIGDropship*)mUnit;

    while(lDropshipUnit->GetCapacity() > 0)
    {
      CIGUnit* lBayUnit = lDropshipUnit->GetUnit(0);

      if (lBayUnit)
      {
        lDropshipUnit->RemoveUnit(lBayUnit);
      }
    }

    UpdateBayGrid();
  }

  if (mUnit->ClassName() == CPString(IGCN_TRANUNIT))
  {
    CIGTranUnit* lTranUnit = (CIGTranUnit*)mUnit;

    while(lTranUnit->GetCapacity() > 0)
    {
      CIGUnit* lBayUnit = lTranUnit->GetUnit(0);

      if (lBayUnit)
      {
        lTranUnit->RemoveUnit(lBayUnit);
      }
    }

    UpdateBayGrid();
  }
}
//---------------------------------------------------------------------------

