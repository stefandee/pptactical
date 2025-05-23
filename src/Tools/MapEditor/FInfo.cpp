//-----------------------------------------------------------------------------
// This file is part of PPTactical Engine.                                   //
//                                                                           //
// PPTactical Engine - engine for tactical/strategy games                    // 
// Copyright (C) 1998, 1999, 2000, 2001 Stefan Dicu & Tudor Girba            //
//                                                                           //
// PPTactical Engine is free software; you can redistribute it               //
// and/or modify it under the terms of the GNU General Public License as     // 
// published by the Free Software Foundation; either version 2 of the        //
// License, or (at your option) any later version.                           //
//                                                                           //  
// PPTactical Engine is distributed in the hope that it will be useful, but  //
// WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY// 
// or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License  //
// for more details.                                                         //
//                                                                           // 
// You should have received a copy of the GNU General Public License         //
// along with PPTactical Engine; if not, write to the Free Software          //
// Foundation Inc. 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA   // 
//-----------------------------------------------------------------------------
/*---------------------------------------------------------------------------
 application: PP Mission Editor

 descrition : diferite informatii despre misiune
 last modify: 16 11 99
        by Grab
---------------------------------------------------------------------------*/
//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop

#include "FInfo.h"
#include "PPIG_ObjInfantry.h"
#include "PPIG_ObjStatic.h"
#include "PPIG_ObjVehicle.h"
//---------------------------------------------------------------------------

#pragma package(smart_init)
#pragma resource "*.dfm"
TFormInfo *FormInfo;
//---------------------------------------------------------------------------
/////////////////////////////////////////////////////////////////////////////
//PUBLIC SECTION
/////////////////////////////////////////////////////////////////////////////
__fastcall TFormInfo::TFormInfo(TComponent* Owner)
   : TForm(Owner)
{
   itCheck = NULL;
}
//---------------------------------------------------------------------------
/*---------------------------------------------------------------------------
 description: sets infos related to the object
---------------------------------------------------------------------------*/
void TFormInfo::setInfo(CIGBaseObject *obj)
{
   CPString lClassName;

   if (obj != NULL)
   {
      lbCoord->Caption = AnsiString(obj->GetCoord().x) + "," + AnsiString(obj->GetCoord().y);
      lbID->Caption    = obj->GetId();

      lClassName = obj->ClassName();

      if (lClassName == CPString(IGCN_INFANTRYUNIT))
      {
        lbHealth->Caption= ((CIGInfantryUnit*)obj)->GetCurrentHP();
        lbType->Caption  = "Infantry";
        lbName->Caption  = AnsiString(((CIGInfantryUnit*)obj)->GetInfData().name) + " " + AnsiString(((CIGInfantryUnit*)obj)->GetInfData().surname);
      }

      if (lClassName == CPString(IGCN_STATICOBJECT))
      {
        lbHealth->Caption= ((CIGStaticObject*)obj)->GetCurrentHP();
        lbType->Caption  = "Static";
        lbName->Caption  = AnsiString(((CIGStaticObject*)obj)->GetStaticData().objName);
      }

      if (obj->IsDerivedFrom(IGCN_VEHICLEUNIT))
      {
        lbHealth->Caption= ((CIGVehicle*)obj)->GetCurrentHP();

        if (lClassName == CPString(IGCN_TANKUNIT))
        {
          lbType->Caption  = "Tank";
        }

        if (lClassName == CPString(IGCN_RECONUNIT))
        {
          lbType->Caption  = "Recon";
        }

        if (lClassName == CPString(IGCN_TRANUNIT))
        {
          lbType->Caption  = "Transport";
        }

        if (lClassName == CPString(IGCN_ARTILLERYUNIT))
        {
          lbType->Caption  = "Artillery";
        }
        
        if (lClassName == CPString(IGCN_DROPSHIPUNIT))
        {
          lbType->Caption  = "Dropship";
        }

        if (lClassName == CPString(IGCN_FIGHTERUNIT))
        {
          lbType->Caption  = "Fighter";
        }

        if (lClassName == CPString(IGCN_CHOPPERUNIT))
        {
          lbType->Caption  = "Chopper";
        }

        if (lClassName == CPString(IGCN_BOMBERUNIT))
        {
          lbType->Caption  = "Chopper";
        }

        lbName->Caption  = AnsiString(((CIGVehicle*)obj)->GetVehicleUnifiedData().name) + "/" +AnsiString(((CIGVehicle*)obj)->GetVehicleUnifiedData().callsign);
      }

      if (obj->IsDerivedFrom(IGCN_AIRUNIT))
      {
        if (lClassName == CPString(IGCN_DROPSHIPUNIT))
        {
          lbType->Caption  = "Dropship";
        }

        if (lClassName == CPString(IGCN_FIGHTERUNIT))
        {
          lbType->Caption  = "Fighter";
        }

        if (lClassName == CPString(IGCN_CHOPPERUNIT))
        {
          lbType->Caption  = "Chopper";
        }

        if (lClassName == CPString(IGCN_BOMBERUNIT))
        {
          lbType->Caption  = "Bomber";
        }

        lbName->Caption  = AnsiString(((CIGVehicle*)obj)->GetVehicleUnifiedData().name) + "/" +AnsiString(((CIGVehicle*)obj)->GetVehicleUnifiedData().callsign);

        //lbHealth->Caption= obj->GetCurrentHP();
        //lbType->Caption  = "Vehicle";
        //lbName->Caption  = "...";
      }

      lbSide->Caption  = obj->GetBattleGroupId();
   }
   else
   {
      lbCoord->Caption = "";
      lbID->Caption    = "";
      lbType->Caption  = "";
      lbName->Caption  = "";
      lbHealth->Caption= "";
      lbSide->Caption  = "";
   }
}

/////////////////////////////////////////////////////////////////////////////
//PUBLISHED SECTION
/////////////////////////////////////////////////////////////////////////////
void __fastcall TFormInfo::FormHide(TObject *Sender)
{
   if (itCheck)
      itCheck->Checked = false;
}
//---------------------------------------------------------------------------

void __fastcall TFormInfo::FormShow(TObject *Sender)
{
   if (itCheck)
      itCheck->Checked = true;
}
//---------------------------------------------------------------------------

