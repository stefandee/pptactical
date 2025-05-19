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
#include <sys\stat.h>
#include <io.h>
#include <stdlib.h>
#include <stdio.h>
#include <fcntl.h>
#pragma hdrstop

#include <FileCtrl.hpp>
#include "m_sless.h"
#include "FAbout.h"
#include "Statics.h"
#include "masked.h"
#include "Serializable.h"
#include "Stream_ANSI_C.h"
//---------------------------------------------------------------------------

#pragma package(smart_init)
#pragma link "ImageEx"
#pragma resource "*.dfm"
TMainForm *MainForm;
//---------------------------------------------------------------------------

__fastcall TMainForm::TMainForm(TComponent* Owner)
        : TForm(Owner)
{
  fileName       = "";

  tempS = new struct _StaticObject;
  memset(tempS, 0, SRecSize);
  tempS->maskX = -1;
  tempS->maskY = -1;
  
  tempS->mLayersCount = 0;

  SRec2Edit();
  SaveDialog->InitialDir = AnsiString("..\\") + AnsiString(PATH_LANDSCAPE);
}
//---------------------------------------------------------------------------

void TMainForm::SRec2Edit()
{
   // group-box-ul general attributes
   MainForm->ed_name->Text = AnsiString(tempS->objName);
   MainForm->m_desc->Text = AnsiString(tempS->objDescription);
   MainForm->cb_type->ItemIndex = tempS->type;
   MainForm->ed_sprite->Text = AnsiString(tempS->sprite);

   MainForm->chb_canfire->Checked = tempS->canFire;
   if (tempS->canFire)
   {
     //MainForm->chb_canfire->State = cbChecked;
     MainForm->gb_weapon->Enabled = true;
   }
   else
   {
     MainForm->gb_weapon->Enabled = false;
     //MainForm->chb_canfire->State = cbUnchecked;
   }

   MainForm->chb_soldiers->Checked  = tempS->canContainSoldiers;
   MainForm->chb_vehicle->Checked   = tempS->canContainVehicles;
   MainForm->chb_indestr->Checked   = tempS->indestructible;
   MainForm->chb_sfire->Checked     = tempS->spreadFire;
   MainForm->chb_sLand->Checked     = tempS->isLandscape;

   /*
   if (tempS->canContainSoldiers) MainForm->chb_soldiers->State = cbChecked;
   else MainForm->chb_soldiers->State = cbUnchecked;

   if (tempS->canContainVehicles) MainForm->chb_vehicle->State = cbChecked;
   else MainForm->chb_vehicle->State = cbUnchecked;

   if (tempS->indestructible) MainForm->chb_indestr->State = cbChecked;
   else MainForm->chb_indestr->State = cbUnchecked;

   if (tempS->spreadFire) MainForm->chb_sfire->State = cbChecked;
   else MainForm->chb_sfire->State = cbUnchecked;

   if (tempS->isLandscape) MainForm->chb_sLand->State = cbChecked;
   else MainForm->chb_sLand->State = cbUnchecked;
   */

   // armors
   MainForm->ed_hp->Text = AnsiString(tempS->hp);
   MainForm->ed_pres->Text = AnsiString(tempS->proj_res);
   MainForm->ed_nres->Text = AnsiString(tempS->nrg_res);
   MainForm->ed_plres->Text = AnsiString(tempS->plasma_res);
   MainForm->ed_bres->Text = AnsiString(tempS->bullet_res);
   MainForm->ed_fres->Text = AnsiString(tempS->fire_res);

   // weapons
   MainForm->ed_wname->Text      = AnsiString(tempS->weapon.wepName);
   MainForm->cb_wtype->ItemIndex = tempS->weapon.type;
   MainForm->ed_range->Text      = AnsiString(tempS->weapon.range);
   MainForm->ed_prec->Text       = AnsiString(tempS->weapon.precision);
   MainForm->ed_radius->Text     = AnsiString(tempS->weapon.radius);
   MainForm->ed_damage->Text     = AnsiString(tempS->weapon.damage);
   MainForm->ed_fr->Text         = AnsiString(tempS->weapon.fire_rate);
   MainForm->ed_proj->Text       = AnsiString(tempS->weapon.projectile);
   MainForm->ed_effb->Text       = AnsiString(tempS->weapon.effect_bitmap);
}
//---------------------------------------------------------------------------

void __fastcall TMainForm::sb_spriteClick(TObject *Sender)
{
  // cam in plus, dar mi'e lene sa sterg :)
  if (fileName != "")
  {
    OpenPictureDialog->InitialDir = ExtractFilePath(fileName);
  }
  else
  {
     OpenPictureDialog->InitialDir = AnsiString("..\\") + AnsiString(PATH_LANDSCAPE);
  }

  if (OpenPictureDialog->Execute())
  {
    ed_sprite->Text = ExtractFileName(OpenPictureDialog->FileName);

    strncpy(tempS->sprite, UTF8Encode(MainForm->ed_sprite->Text).c_str(), MAX_PP_PATH);

    // initialize the mask size
    MaskForm->InitMask();

    if (fileName == "")
    {
      SaveDialog->InitialDir = ExtractFilePath(OpenPictureDialog->FileName);
    }
    else
    {
      SaveDialog->InitialDir = ExtractFilePath(fileName);
    }
  }
}
//---------------------------------------------------------------------------

void __fastcall TMainForm::sb_projbClick(TObject *Sender)
{
  if (OpenPictureDialog->Execute())
  {
    ed_proj->Text = ExtractFileName(OpenPictureDialog->FileName);
    strncpy(tempS->weapon.projectile, UTF8Encode(MainForm->ed_proj->Text).c_str(), 32);
  }
}
//---------------------------------------------------------------------------

void __fastcall TMainForm::sb_effbClick(TObject *Sender)
{
  if (OpenPictureDialog->Execute())
  {
    ed_effb->Text = ExtractFileName(OpenPictureDialog->FileName);
    strcpy(tempS->weapon.effect_bitmap, UTF8Encode(MainForm->ed_effb->Text).c_str());
  }
}
//---------------------------------------------------------------------------

void __fastcall TMainForm::Load1Click(TObject *Sender)
{
  int lHandle, lLength;

  if (OpenDialog->Execute())
  {
     /*
     if ( (lHandle = open(OpenDialog->FileName.c_str(), O_BINARY | O_RDWR, S_IREAD|S_IWRITE)) == -1 )
     {
          // se va termina daca nu poate deschide pentru modificari fisierul
          Application->MessageBox("Error opening DAT file", "Error", MB_OK);
          return;
     }

     lLength = filelength(lHandle);

     if ((lLength % SRecSize != 0) ||
         (lLength == 0))
     {
          // fisierul .dat e corupt - il distruge la 0
          Application->MessageBox("Invalid length for DAT file", "Error", MB_OK);
     }
     else
     {
       if (read(lHandle, tempS, SRecSize) == -1)
       {
         Application->MessageBox("Cannot read file", "Error", MB_OK);
       }
       else
       {
          fileName = OpenDialog->FileName;
          StatusBar->SimpleText = fileName;
          lseek(lHandle, -(signed)SRecSize, SEEK_CUR);
          SRec2Edit();
       }
     }
     */

    try
    {
     PP::Stream_ANSI_C lA(UTF8Encode(OpenDialog->FileName).c_str());

     CSerString lId;

     lId.DeSerialize(lA);
     tempS->DeSerialize(lA);
    }
    catch(...)
    {
      Application->MessageBox(L"Error opening DAT file", L"Error", MB_OK);
    }

    fileName = OpenDialog->FileName;
    StatusBar->SimpleText = fileName;
    SRec2Edit();
  }

  //close(lHandle);
}
//---------------------------------------------------------------------------

void __fastcall TMainForm::New1Click(TObject *Sender)
{
  // generez o inregistrare noua
  fileName       = "";

  memset(tempS, 0, SRecSize);
  tempS->maskX = -1;
  tempS->maskY = -1;
  memset(tempS->mask, 0, sizeof(tempS->mask));
  memset(tempS->altitude, 0, sizeof(tempS->altitude));
  tempS->mLayersCount = 0;

  SRec2Edit();
}
//---------------------------------------------------------------------------

// Descriere   : save as
// Param       : builder default
// Rezultat    :
// Comentarii  : se poate simplifica structura de decizie, dar vreau ca functia sa
//               iasa prin close() de fisier
void __fastcall TMainForm::Saveas1Click(TObject *Sender)
{
  int lHandle;

  if ((fileName == "") && (AnsiString(tempS->sprite) == ""))
  {
    SaveDialog->InitialDir = AnsiString("..\\") + AnsiString(PATH_LANDSCAPE);
  }

  if (SaveDialog->Execute())
  {
    /*
    if ( (lHandle = open(SaveDialog->FileName.c_str(), O_CREAT | O_BINARY | O_RDWR, S_IREAD|S_IWRITE)) == -1 )
    {
         // se va termina daca nu poate deschide pentru modificari fisierul
         Application->MessageBox("Error opening DAT file for save", "Error", MB_OK);
    }
    else
    {
      lseek(lHandle, 0, SEEK_SET);
      if (write(lHandle, tempS, SRecSize) == -1)
      {
        Application->MessageBox("Cannot write DAT file", "Error", MB_OK);
      }
      else
      {
        // numele fisierului se seteaza numai daca operatiunea de salvare e
        // reusita
        fileName = SaveDialog->FileName;
        StatusBar->SimpleText = fileName;
      }
    }
    */

    try
    {
     PP::Stream_ANSI_C lA(UTF8Encode(SaveDialog->FileName).c_str(), true);

     CSerString lId(STATIC_FILE_ID);

     lId.Serialize(lA);
     tempS->Serialize(lA);
    }
    catch(...)
    {
      Application->MessageBox(L"Error opening DAT file for saving", L"Error", MB_OK);
    }

    fileName = SaveDialog->FileName;
    StatusBar->SimpleText = fileName;
  }

  close(lHandle);
}
//---------------------------------------------------------------------------

void __fastcall TMainForm::Save1Click(TObject *Sender)
{
  int lHandle;

  if (fileName == "")
  {
    Saveas1Click(this);
    return;
  }

  /*
  if ((lHandle = open(fileName.c_str(), O_BINARY | O_RDWR, S_IREAD|S_IWRITE)) == -1 )
  {
      // se va termina daca nu poate deschide pentru modificari fisierul
      Application->MessageBox("Error opening DAT file", "Error", MB_OK);
      return;
  }

  if (lseek(lHandle, 0, SEEK_SET) == -1)
  {
    Application->MessageBox("Cannot seek in DAT file", "Error", MB_OK);
  }
  else
  {
    if (write(lHandle, tempS, SRecSize) == -1)
    {
      Application->MessageBox("Cannot write DAT file", "Error", MB_OK);
    }
  }

  close(lHandle);
  */
  try
  {
   PP::Stream_ANSI_C lA(fileName.c_str(), true);

   CSerString lId(STATIC_FILE_ID);

   lId.Serialize(lA);
   tempS->Serialize(lA);
  }
  catch(...)
  {
    Application->MessageBox(L"Error opening DAT file for saving", L"Error", MB_OK);
  }
}
//---------------------------------------------------------------------------

void __fastcall TMainForm::ed_nameExit(TObject *Sender)
{
    strcpy(tempS->objName, UTF8Encode(MainForm->ed_name->Text).c_str());
}
//---------------------------------------------------------------------------

void __fastcall TMainForm::m_descExit(TObject *Sender)
{
    strcpy(tempS->objDescription, UTF8Encode(MainForm->m_desc->Text).c_str());
}
//---------------------------------------------------------------------------

void __fastcall TMainForm::cb_typeExit(TObject *Sender)
{
    tempS->type = (TStaticType)cb_type->ItemIndex;
}
//---------------------------------------------------------------------------

void __fastcall TMainForm::ed_spriteExit(TObject *Sender)
{
  strcpy(tempS->sprite, UTF8Encode(ExtractFileName(ed_sprite->Text)).c_str());

  if (ExtractFilePath(ed_sprite->Text) == "")
  {
    SaveDialog->InitialDir = AnsiString("..\\") + AnsiString(PATH_LANDSCAPE);
  }
  else
  {
    SaveDialog->InitialDir = ExtractFilePath(ed_sprite->Text);
  }

  ed_sprite->Text = AnsiString(tempS->sprite);
}
//---------------------------------------------------------------------------

void __fastcall TMainForm::chb_vehicleClick(TObject *Sender)
{
  /*
  if (chb_vehicle->State == cbChecked) tempS->canContainVehicles = true;
  else tempS->canContainVehicles = false;
  */
  tempS->canContainVehicles = chb_vehicle->Checked;
}
//---------------------------------------------------------------------------

void __fastcall TMainForm::chb_soldiersClick(TObject *Sender)
{
  /*
  if (chb_soldiers->State == cbChecked) tempS->canContainSoldiers = true;
  else tempS->canContainSoldiers = false;
  */
  tempS->canContainSoldiers = chb_soldiers->Checked;
}
//---------------------------------------------------------------------------

void __fastcall TMainForm::chb_canfireClick(TObject *Sender)
{
  tempS->canFire = chb_canfire->Checked;
  if (tempS->canFire)
  {
    gb_weapon->Enabled = true;
  }
  else
  {
    gb_weapon->Enabled= false;
  }
}
//---------------------------------------------------------------------------

void __fastcall TMainForm::ed_hpExit(TObject *Sender)
{
   tempS->hp = 0;
   try
   {
      tempS->hp = ed_hp->Text.ToInt();
   }
   catch(const EConvertError &e)
   {
      tempS->hp = 0;
      ed_hp->Text = "0";
   }

   // verificari de limita
   if (tempS->hp < 0) tempS->hp = 0;
   ed_hp->Text = AnsiString(tempS->hp);
}
//---------------------------------------------------------------------------

void __fastcall TMainForm::ed_presExit(TObject *Sender)
{
   tempS->proj_res = 0;
   try
   {
      tempS->proj_res = ed_pres->Text.ToInt();
   }
   catch(const EConvertError &e)
   {
      tempS->proj_res = 0;
      ed_pres->Text = "0";
   }

   // verificari de limita
   if (tempS->proj_res < 0) tempS->proj_res = 0;
   if (tempS->proj_res > 99) tempS->proj_res = 99;
   ed_pres->Text = AnsiString(tempS->proj_res);
}
//---------------------------------------------------------------------------

void __fastcall TMainForm::ed_nresExit(TObject *Sender)
{
   tempS->nrg_res = 0;
   try
   {
      tempS->nrg_res = ed_nres->Text.ToInt();
   }
   catch(const EConvertError &e)
   {
      tempS->nrg_res = 0;
      ed_nres->Text = "0";
   }

   // verificari de limita
   if (tempS->nrg_res < 0) tempS->nrg_res = 0;
   if (tempS->nrg_res > 99) tempS->nrg_res = 99;
   ed_nres->Text = AnsiString(tempS->nrg_res);
}
//---------------------------------------------------------------------------

void __fastcall TMainForm::ed_plresExit(TObject *Sender)
{
   tempS->plasma_res = 0;
   try
   {
      tempS->plasma_res = ed_plres->Text.ToInt();
   }
   catch(const EConvertError &e)
   {
      tempS->plasma_res = 0;
      ed_plres->Text = "0";
   }

   // verificari de limita
   if (tempS->plasma_res < 0) tempS->plasma_res = 0;
   if (tempS->plasma_res > 99) tempS->plasma_res = 99;
   ed_plres->Text = AnsiString(tempS->plasma_res);
}
//---------------------------------------------------------------------------

void __fastcall TMainForm::ed_bresExit(TObject *Sender)
{
   tempS->bullet_res = 0;
   try
   {
      tempS->bullet_res = ed_bres->Text.ToInt();
   }
   catch(const EConvertError &e)
   {
      tempS->bullet_res = 0;
      ed_bres->Text = "0";
   }

   // verificari de limita
   if (tempS->bullet_res < 0) tempS->bullet_res = 0;
   if (tempS->bullet_res > 99) tempS->bullet_res = 99;
   ed_bres->Text = AnsiString(tempS->bullet_res);
}
//---------------------------------------------------------------------------

void __fastcall TMainForm::ed_wnameExit(TObject *Sender)
{
    strcpy(tempS->weapon.wepName, UTF8Encode(MainForm->ed_wname->Text).c_str());
}
//---------------------------------------------------------------------------

void __fastcall TMainForm::cb_wtypeExit(TObject *Sender)
{
    tempS->weapon.type = (_swtype)cb_wtype->ItemIndex;
}
//---------------------------------------------------------------------------

void __fastcall TMainForm::ed_projExit(TObject *Sender)
{
    strcpy(tempS->weapon.projectile, UTF8Encode(MainForm->ed_proj->Text).c_str());
}
//---------------------------------------------------------------------------

void __fastcall TMainForm::ed_effbExit(TObject *Sender)
{
    strcpy(tempS->weapon.effect_bitmap, UTF8Encode(MainForm->ed_effb->Text).c_str());
}
//---------------------------------------------------------------------------

void __fastcall TMainForm::ed_rangeExit(TObject *Sender)
{
   tempS->weapon.range = 0;
   try
   {
      tempS->weapon.range = ed_range->Text.ToInt();
   }
   catch(const EConvertError &e)
   {
      tempS->weapon.range = 0;
      ed_range->Text = "0";
   }

   // verificari de limita
   if (tempS->weapon.range < 0) tempS->weapon.range = 0;
   ed_range->Text = AnsiString(tempS->weapon.range);
}
//---------------------------------------------------------------------------

void __fastcall TMainForm::ed_precExit(TObject *Sender)
{
   tempS->weapon.precision = 0;
   try
   {
      tempS->weapon.precision = ed_prec->Text.ToInt();
   }
   catch(const EConvertError &e)
   {
      tempS->weapon.precision = 0;
      ed_prec->Text = "0";
   }

   // verificari de limita
   if (tempS->weapon.precision < 0) tempS->weapon.precision = 0;
   if (tempS->weapon.precision > 99) tempS->weapon.precision = 99;
   ed_prec->Text = AnsiString(tempS->weapon.precision);
}
//---------------------------------------------------------------------------

void __fastcall TMainForm::ed_radiusExit(TObject *Sender)
{
   tempS->weapon.radius = 0;
   try
   {
      tempS->weapon.radius = ed_radius->Text.ToInt();
   }
   catch(const EConvertError &e)
   {
      tempS->weapon.radius = 0;
      ed_radius->Text = "0";
   }

   // verificari de limita
   if (tempS->weapon.radius< 0) tempS->weapon.radius= 0;
   ed_radius->Text = AnsiString(tempS->weapon.radius);
}
//---------------------------------------------------------------------------

void __fastcall TMainForm::ed_damageExit(TObject *Sender)
{
   tempS->weapon.damage = 0;
   try
   {
      tempS->weapon.damage = ed_damage->Text.ToInt();
   }
   catch(const EConvertError &e)
   {
      tempS->weapon.damage = 0;
      ed_damage->Text = "0";
   }

   // verificari de limita
   if (tempS->weapon.damage < 0) tempS->weapon.damage = 0;
   ed_damage->Text = AnsiString(tempS->weapon.damage);
}
//---------------------------------------------------------------------------

void __fastcall TMainForm::ed_frExit(TObject *Sender)
{
   tempS->weapon.fire_rate = 0;
   try
   {
      tempS->weapon.fire_rate = ed_fr->Text.ToInt();
   }
   catch(const EConvertError &e)
   {
      tempS->weapon.fire_rate = 0;
      ed_fr->Text = "0";
   }

   // verificari de limita
   if (tempS->weapon.fire_rate< 0) tempS->weapon.fire_rate= 0;
   ed_fr->Text = AnsiString(tempS->weapon.fire_rate);
}
//---------------------------------------------------------------------------

void __fastcall TMainForm::Exit1Click(TObject *Sender)
{
  Application->Terminate();
}
//---------------------------------------------------------------------------

void __fastcall TMainForm::About2Click(TObject *Sender)
{
  FormAbout->ShowModal();
}
//---------------------------------------------------------------------------

void __fastcall TMainForm::b_maskClick(TObject *Sender)
{
  if (strlen(tempS->sprite) != 0)
  {
    MaskForm->ShowModal();
  }  
}
//---------------------------------------------------------------------------

void __fastcall TMainForm::chb_indestrClick(TObject *Sender)
{
  /*
  if (chb_vehicle->State == cbChecked) tempS->indestructible = true;
  else tempS->indestructible = false;
  */
  tempS->indestructible = chb_indestr->Checked;
}
//---------------------------------------------------------------------------

void __fastcall TMainForm::chb_sfireClick(TObject *Sender)
{
  /*
  if (chb_vehicle->State == cbChecked) tempS->spreadFire = true;
  else tempS->spreadFire = false;
  */
  tempS->spreadFire = chb_sfire->Checked;
}
//---------------------------------------------------------------------------

void __fastcall TMainForm::ed_fresExit(TObject *Sender)
{
   tempS->fire_res = 0;
   try
   {
      tempS->fire_res = ed_fres->Text.ToInt();
   }
   catch(const EConvertError &e)
   {
      tempS->fire_res = 0;
      ed_fres->Text = "0";
   }

   // verificari de limita
   if (tempS->fire_res < 0) tempS->fire_res = 0;
   if (tempS->fire_res > 99) tempS->fire_res = 99;
   ed_fres->Text = AnsiString(tempS->fire_res);
}
//---------------------------------------------------------------------------

void __fastcall TMainForm::ed_onFireExit(TObject *Sender)
{
    strcpy(tempS->weapon.onFire, UTF8Encode(MainForm->ed_onFire->Text).c_str());
}
//---------------------------------------------------------------------------

void __fastcall TMainForm::ed_onHitExit(TObject *Sender)
{
    strcpy(tempS->weapon.onHit, UTF8Encode(MainForm->ed_onHit->Text).c_str());
}
//---------------------------------------------------------------------------

void __fastcall TMainForm::sb_onFireClick(TObject *Sender)
{
  if (OpenSoundDialog->Execute())
  {
    ed_onFire->Text = ExtractFileName(OpenSoundDialog->FileName);
    strcpy(tempS->weapon.onFire, UTF8Encode(MainForm->ed_onFire->Text).c_str());
  }
}
//---------------------------------------------------------------------------

void __fastcall TMainForm::sb_onHitClick(TObject *Sender)
{
  if (OpenSoundDialog->Execute())
  {
    ed_onHit->Text = ExtractFileName(OpenSoundDialog->FileName);
    strcpy(tempS->weapon.onHit, UTF8Encode(MainForm->ed_onHit->Text).c_str());
  }
}
//---------------------------------------------------------------------------

void __fastcall TMainForm::FormDestroy(TObject *Sender)
{
  delete tempS;
}
//---------------------------------------------------------------------------

void __fastcall TMainForm::chb_sLandClick(TObject *Sender)
{
  /*
  if (chb_sLand->State == cbChecked) tempS->isLandscape = true;
  else tempS->isLandscape = false;
  */
  tempS->isLandscape = chb_sLand->Checked;
}
//---------------------------------------------------------------------------





