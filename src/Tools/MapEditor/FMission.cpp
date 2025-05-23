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
//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop

#include "FMission.h" 
#include "MissionData.h"
#include "SolarSys.h"
#include "MapUnit.h"

//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TFormMission *FormMission;
//---------------------------------------------------------------------------
__fastcall TFormMission::TFormMission(TComponent* Owner)
        : TForm(Owner)
{
}
//---------------------------------------------------------------------------
void __fastcall TFormMission::FormShow(TObject *Sender)
{
  AnsiString systemsFile = AnsiString("..\\") + AnsiString(dirSystems) + AnsiString(SysDatFile);
  int systemsNo;
  struct _SolarSys solarSys;

  FormMission->ed_mName->Text = AnsiString(MapForm->mapManager->map->mission.name);
  FormMission->m_mBrief->Text = AnsiString(MapForm->mapManager->map->mission.briefing);

  // curatz solar systems si planet comboboxuri si le recitesc din fisier
  cb_ssys->Items->Clear();
  cb_planet->Items->Clear();

  // citesc fisierul SysDatFile
  systemsNo = SysRecordsInFile(systemsFile.c_str());
  if (systemsNo == 0)
  {
    Application->MessageBox("Systems file contains no record.", "Semi-error", MB_OK);
    gb_location->Enabled = false;
    return;
  }
  else gb_location->Enabled = true;

  for(int i = 0; i < systemsNo; i++)
  {
    if (SysGetRecord(systemsFile.c_str(), i, &solarSys) != 0)
    {
      Application->MessageBox("Error reading systems dat file", "Semi-error", MB_OK);
      gb_location->Enabled = false;
      return;
    }
    cb_ssys->Items->Add(solarSys.name);
  }

  // setez locatia misiunii
  try
  {
    cb_ssys->ItemIndex   = MapForm->mapManager->map->mission.whatSystem;

    if (SysGetRecord(systemsFile.c_str(), cb_ssys->ItemIndex, &solarSys) != 0)
    {
      Application->MessageBox("Error reading systems dat file", "Semi-error", MB_OK);
      gb_location->Enabled = false;
      return;
    }

    // acum adaug planetele
    cb_planet->Items->Clear();
    for(int i = 0; i < solarSys.CrtPlanets; i++)
    {
      cb_planet->Items->Add(solarSys.Planets[i].name);
    }
    
    cb_planet->ItemIndex = MapForm->mapManager->map->mission.whatPlanet;
  }
  catch(...)
  {
    cb_ssys->ItemIndex   = 0;
    cb_planet->ItemIndex = 0;

    if (SysGetRecord(systemsFile.c_str(), 0, &solarSys) != 0)
    {
      Application->MessageBox("Error reading systems dat file", "Semi-error", MB_OK);
      gb_location->Enabled = false;
      return;
    }

    // acum adaug planetele
    cb_planet->Items->Clear();
    for(int i = 0; i < solarSys.CrtPlanets; i++)
    {
      cb_planet->Items->Add(solarSys.Planets[i].name);
    }
  }

  // pun si timpul misiunii
  if ((MapForm->mapManager->map->mission.dayStart < 1) ||
      (MapForm->mapManager->map->mission.dayStart > 30))
      MapForm->mapManager->map->mission.dayStart = 1;
  if ((MapForm->mapManager->map->mission.monthStart < 1) ||
      (MapForm->mapManager->map->mission.monthStart > 12))
      MapForm->mapManager->map->mission.monthStart = 1;
  if (MapForm->mapManager->map->mission.yearStart < 1999)
      MapForm->mapManager->map->mission.yearStart = 1999;

  if ((MapForm->mapManager->map->mission.dayEnd < 1) ||
      (MapForm->mapManager->map->mission.dayEnd > 30))
      MapForm->mapManager->map->mission.dayEnd = 1;
  if ((MapForm->mapManager->map->mission.monthEnd < 1) ||
      (MapForm->mapManager->map->mission.monthEnd > 12))
      MapForm->mapManager->map->mission.monthEnd = 1;
  if (MapForm->mapManager->map->mission.yearEnd < 1999)
      MapForm->mapManager->map->mission.yearEnd = 1999;

  ed_Start->Text = AnsiString(MapForm->mapManager->map->mission.dayStart) + "." +
                   AnsiString(MapForm->mapManager->map->mission.monthStart) + "." +
                   AnsiString(MapForm->mapManager->map->mission.yearStart);
  ed_End->Text = AnsiString(MapForm->mapManager->map->mission.dayEnd) + "." +
                   AnsiString(MapForm->mapManager->map->mission.monthEnd) + "." +
                   AnsiString(MapForm->mapManager->map->mission.yearEnd);
}
//---------------------------------------------------------------------------
void __fastcall TFormMission::cb_ssysChange(TObject *Sender)
{
  AnsiString systemsFile = AnsiString("..\\") + AnsiString(dirSystems) + AnsiString(SysDatFile);
  struct _SolarSys solarSys;

  if (cb_ssys->ItemIndex != -1)
  {
    if (SysGetRecord(systemsFile.c_str(), cb_ssys->ItemIndex, &solarSys) != 0)
    {
      Application->MessageBox("Error reading systems dat file", "Semi-error", MB_OK);
      gb_location->Enabled = false;
      return;
    }

    // acum adaug planetele
    cb_planet->Items->Clear();
    for(int i = 0; i < solarSys.CrtPlanets; i++)
    {
      cb_planet->Items->Add(solarSys.Planets[i].name);
    }

    if (solarSys.CrtPlanets > 0) cb_planet->ItemIndex = 0;
  }
}
//---------------------------------------------------------------------------
void __fastcall TFormMission::b_OkClick(TObject *Sender)
{
  int day, month, year;

  strcpy(MapForm->mapManager->map->mission.name, FormMission->ed_mName->Text.c_str());
  strcpy(MapForm->mapManager->map->mission.briefing, FormMission->m_mBrief->Text.c_str());

  MapForm->mapManager->map->mission.whatPlanet = cb_planet->ItemIndex;
  MapForm->mapManager->map->mission.whatSystem = cb_ssys->ItemIndex;

  if (ExtractDate(ed_Start->Text, &day, &month, &year))
  {
    MapForm->mapManager->map->mission.dayStart   = day;
    MapForm->mapManager->map->mission.monthStart = month;
    MapForm->mapManager->map->mission.yearStart  = year;
  }

  if (ExtractDate(ed_End->Text, &day, &month, &year))
  {
    MapForm->mapManager->map->mission.dayEnd   = day;
    MapForm->mapManager->map->mission.monthEnd = month;
    MapForm->mapManager->map->mission.yearEnd  = year;
  }
}
//---------------------------------------------------------------------------
void __fastcall TFormMission::ed_StartExit(TObject *Sender)
{
  int day, month, year;

  if (ExtractDate(ed_Start->Text, &day, &month, &year))
    ed_Start->Text = AnsiString(day) + "." +
                     AnsiString(month) + "." +
                     AnsiString(year);
  else
    ed_Start->Text = "01.01.1999";
}
//---------------------------------------------------------------------------

void __fastcall TFormMission::ed_EndExit(TObject *Sender)
{
  int day, month, year;

  if (ExtractDate(ed_End->Text, &day, &month, &year))
    ed_End->Text = AnsiString(day) + "." +
                   AnsiString(month) + "." +
                   AnsiString(year);
  else
    ed_End->Text = "01.01.1999";
}
//---------------------------------------------------------------------------

bool TFormMission::ExtractDate(AnsiString date, int* day, int* month, int* year)
{
  int dotCount = 0;
  int dotPos[2];

  *day   = 1;
  *month = 1;
  *year  = 1999;

  for(int i = 1; i <= date.Length(); i++)
  {
    if (!((date[i] == '.') || ((date[i] <= '9') && (date[i] >= '0')))) return false;
    if (date[i] == '.')
    {
      dotCount++;
      if (dotCount > 2) return false;
      dotPos[dotCount - 1] = i;
    }
  }

  if (dotCount < 2) return false;

  AnsiString dayStr   = date.SubString(1, dotPos[0] - 1);
  AnsiString monthStr = date.SubString(dotPos[0] + 1, dotPos[1] - dotPos[0] - 1);
  AnsiString yearStr  = date.SubString(dotPos[1] + 1, date.Length() - dotPos[1]);

  *day   = dayStr.ToIntDef(1);
  *month = monthStr.ToIntDef(1);
  *year  = yearStr.ToIntDef(1999);

  if (*day   > 30) *day = 1;
  if (*month > 12) *month = 1;
  if (*year  < 1999) *year = 1999;

  return true;
}
//---------------------------------------------------------------------------



