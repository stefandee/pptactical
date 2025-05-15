//---------------------------------------------------------------------------

#pragma hdrstop

#include "PPIG_CUnitInfo.h"
#include "PPIG_Mission.h"
#include "PP_CLabel.h"
#include "PPIG_CInfantryInfo.h"
#include "PPIG_CVehicleInfo.h"
#include "PPIG_CGroupInfo.h"
#include "PPIG_CTranInfo.h"
#include "PPIG_CDropshipInfo.h"
#include "PPIG_CAircraftInfo.h"
#include "PPIG_CFactoryInfo.h"
//---------------------------------------------------------------------------

CPIUnitInfo::CPIUnitInfo(CPIWinControl *Parent) : CPIWinControl(Parent)
{
  mInfantryInfo = new CPIInfantryInfo(this);
  AddControl(mInfantryInfo);
  mInfantryInfo->SetLeft(0);
  mInfantryInfo->SetTop(0);
  mInfantryInfo->SetVisible(true);
  mInfantryInfo->SetEnabled(false);

  mGenericVehicleInfo = new CPIVehicleInfo(this);
  AddControl(mGenericVehicleInfo);
  mGenericVehicleInfo->SetLeft(0);
  mGenericVehicleInfo->SetTop(0);
  mGenericVehicleInfo->SetVisible(true);
  mGenericVehicleInfo->SetEnabled(false);

  mGroupInfo = new CPIGroupInfo(this);
  AddControl(mGroupInfo);
  mGroupInfo->SetLeft(0);
  mGroupInfo->SetTop(0);
  mGroupInfo->SetVisible(true);
  mGroupInfo->SetEnabled(false);

  mTranInfo = new CPITranInfo(this);
  AddControl(mTranInfo);
  mTranInfo->SetLeft(0);
  mTranInfo->SetTop(0);
  mTranInfo->SetVisible(true);
  mTranInfo->SetEnabled(false);

  mDropshipInfo = new CPIDropshipInfo(this);
  AddControl(mDropshipInfo);
  mDropshipInfo->SetLeft(0);
  mDropshipInfo->SetTop(0);
  mDropshipInfo->SetVisible(true);
  mDropshipInfo->SetEnabled(false);

  mGenericAircraftInfo = new CPIAircraftInfo(this);
  AddControl(mGenericAircraftInfo);
  mGenericAircraftInfo->SetLeft(0);
  mGenericAircraftInfo->SetTop(0);
  mGenericAircraftInfo->SetVisible(true);
  mGenericAircraftInfo->SetEnabled(false);

  // TEST CODE FOR SDJ
  mFactoryInfo = new CPIFactoryInfo(this);
  AddControl(mFactoryInfo);
  mFactoryInfo->SetLeft(0);
  mFactoryInfo->SetTop(0);
  mFactoryInfo->SetVisible(true);
  mFactoryInfo->SetEnabled(false);
}
//---------------------------------------------------------------------------

CPIUnitInfo::~CPIUnitInfo()
{
}
//---------------------------------------------------------------------------

void CPIUnitInfo::Paint()
{
  CPIWinControl* lControl = ComputeControlType();

  if (lControl)
  {
    lControl->SetHeight(GetHeight());
    lControl->SetWidth(GetWidth());
    lControl->Paint();
  }
}
//---------------------------------------------------------------------------

/*
void CPIUnitInfo::OnLeftMouseDown(int X, int Y)
{
}
//---------------------------------------------------------------------------

void CPIUnitInfo::OnLeftMouseUp(int X, int Y)
{
}
//---------------------------------------------------------------------------

void CPIUnitInfo::OnRightMouseClick()
{
}
//---------------------------------------------------------------------------

void CPIUnitInfo::OnMouseMove(int X, int Y)
{
}
//---------------------------------------------------------------------------

void CPIUnitInfo::OnMouseOn(int X, int Y)
{
}
//---------------------------------------------------------------------------
*/

/*
void CPIUnitInfo::SetLeft(int value)
{
  CPIWinControl::SetLeft(value);
  mGroupInfo->Update();
}
//---------------------------------------------------------------------------

void CPIUnitInfo::SetTop(int value)
{
  CPIWinControl::SetTop(value);
  mGroupInfo->Update();
}
//---------------------------------------------------------------------------
*/

CPIWinControl* CPIUnitInfo::ComputeControlType()
{
  // disable all controls
  mGroupInfo->SetEnabled(false);
  mTranInfo->SetEnabled(false);
  mGenericVehicleInfo->SetEnabled(false);
  mGenericAircraftInfo->SetEnabled(false);
  mInfantryInfo->SetEnabled(false);
  mFactoryInfo->SetEnabled(false);

  // hide all controls
  mGroupInfo->SetVisible(false);
  mTranInfo->SetVisible(false);
  mGenericVehicleInfo->SetVisible(false);
  mGenericAircraftInfo->SetVisible(false);
  mInfantryInfo->SetVisible(false);
  mFactoryInfo->SetVisible(false);
    
  if (GetMissionInstance()->GetObjects(MOL_SELECTEDOWNEDUNITS)->Count() == 1)
  {
    CIGBaseObject* lObject = GetMissionInstance()->GetObjects(MOL_SELECTEDOWNEDUNITS)->GetObject(0);
    CPString lClassName = lObject->ClassName();

    //
    // a dead object may not be selected and obtained info on, used when a
    // selected object died
    //
    if (lObject->GetIsDead())
    {
      return 0;
    }

    // for infantry units
    if (lClassName == CPString(IGCN_INFANTRYUNIT))
    {
      mInfantryInfo->SetInfantryUnit((CIGInfantryUnit*)lObject);
      mInfantryInfo->SetEnabled(true);
      mInfantryInfo->SetVisible(true);
      return mInfantryInfo;
    }

    // for transporters
    if (lClassName == CPString(IGCN_TRANUNIT))
    {
      mTranInfo->SetTranUnit((CIGTranUnit*)lObject);
      mTranInfo->SetEnabled(true);
      mTranInfo->SetVisible(true);
      return mTranInfo;
    }

    // for artillery/tanks/recons
    if (lClassName == CPString(IGCN_TANKUNIT) || lClassName == CPString(IGCN_ARTILLERYUNIT) || lClassName == CPString(IGCN_RECONUNIT))
    {
      mGenericVehicleInfo->SetVehicleUnit((CIGVehicle*)lObject);
      mGenericVehicleInfo->SetEnabled(true);
      mGenericVehicleInfo->SetVisible(true);
      return mGenericVehicleInfo;
    }

    // for dropships
    if (lClassName == CPString(IGCN_DROPSHIPUNIT))
    {
      mDropshipInfo->SetDropshipUnit((CIGDropship*)lObject);
      mDropshipInfo->SetEnabled(true);
      mDropshipInfo->SetVisible(true);
      return mDropshipInfo;
    }

    // for fighters/choppers
    if (lClassName == CPString(IGCN_CHOPPERUNIT) || lClassName == CPString(IGCN_FIGHTERUNIT) || lClassName == CPString(IGCN_BOMBERUNIT))
    {
      mGenericAircraftInfo->SetAircraftUnit((CIGAircraft*)lObject);
      mGenericAircraftInfo->SetEnabled(true);
      mGenericAircraftInfo->SetVisible(true);
      return mGenericAircraftInfo;
    }
  }

  // TEST CODE FOR SDJ
  if (GetMissionInstance()->GetObjects(MOL_SELECTEDOWNEDOTHERS)->Count() == 1)
  {
    CIGBaseObject* lObject = GetMissionInstance()->GetObjects(MOL_SELECTEDOWNEDOTHERS)->GetObject(0);
    CPString lClassName = lObject->ClassName();

    // for static factories
    if (lClassName == CPString(IGCN_FACTORYOBJECT))
    {
      mFactoryInfo->SetFactory((CIGFactoryObject*)lObject);
      mFactoryInfo->SetEnabled(true);
      mFactoryInfo->SetVisible(true);
      return mFactoryInfo;
    }
  }

  if (GetMissionInstance()->GetObjects(MOL_SELECTEDOWNEDUNITS)->Count() > 0)
  {
    mGroupInfo->RemoveAll();
    mGroupInfo->SetEnabled(true);
    mGroupInfo->SetVisible(true);

    for(int i = 0; i < GetMissionInstance()->GetObjects(MOL_SELECTEDOWNEDUNITS)->Count(); i++)
    {
      mGroupInfo->AddUnit(GetMissionInstance()->GetObjects(MOL_SELECTEDOWNEDUNITS)->GetObject(i));
    }

    return mGroupInfo;
  }

  // there should be no such case...
  return 0;
}
//---------------------------------------------------------------------------

#ifdef PPT_COMPILER_BORLAND
#pragma package(smart_init)
#endif

