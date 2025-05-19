//-----------------------------------------------------------------------------
// This file is part of PPTactical Engine.                                   //
//                                                                           //
// PPTactical Engine - engine for tactical/strategy games                    // 
// Copyright (C) 1998-2004 Stefan Dicu & Tudor Girba                         //
//                                                                           //
// PPTactical Engine is free software; you can redistribute it               //
// and/or modify it under the terms of the GNU Lesser General Public License //
// as published by the Free Software Foundation; either version 2.1 of the   //
// License, or (at your option) any later version.                           //
//                                                                           //
// PPTactical Engine is distributed in the hope that it will be useful, but  //
// WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY//
// or FITNESS FOR A PARTICULAR PURPOSE. See the GNU Lesser General Public    //
// License for more details.                                                 //
//                                                                           //
// You should have received a copy of the GNU Lesser General Public License  //
// along with PPTactical Engine; if not, write to the Free Software          //
// Foundation Inc. 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA   // 
//-----------------------------------------------------------------------------
/*---------------------------------------------------------------------------
 application: PP Units Editor

 descrition : AI Units
 last modify: 24 11 99
        by Grab
---------------------------------------------------------------------------*/
//---------------------------------------------------------------------------
#include <vcl.h>
#include <io.h>
#include <sys/stat.h>
#include <fcntl.h>
#pragma hdrstop

#include "m_aiunits.h"
#include "basepath.h"
#include "Paths.h"
#include "FUnitsPrev.h"
#include "FUnitsAbout.h"
#include "VSkelEd.h"
#include "AirSkelEd.h"
#include "InfSkelEd.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma link "CSPIN"
#pragma resource "*.dfm"
TFormMain *FormMain;
//---------------------------------------------------------------------------
/////////////////////////////////////////////////////////////////////////////
//PUBLIC AREA
/////////////////////////////////////////////////////////////////////////////

__fastcall TFormMain::TFormMain(TComponent* Owner)
   : TForm(Owner)
{
}
//---------------------------------------------------------------------------


/*---------------------------------------------------------------------------
INFANTRY PUBLIC AREA
---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------
 descrition: deschide aiinfantry.dat
---------------------------------------------------------------------------*/
void TFormMain::openInf()
{
   AnsiString path = AnsiString(getGamePath().c_str()) + PATH_AIINFANTRY;
   path = path + FILE_AIINFANTRY;
   // initializeaza variabilele legate de fisierul aiinfantry.dat
   try
   {
      recInf = new TInfantryUnified;
   }
   catch(...)
   {
   	Application->MessageBox(L"Not enough memory to create infantry record", L"Error", MB_OK);
   	Application->Terminate();
   }
   sizeInf = sizeof(TInfantryUnified);
   if ( (handleInf = open(path.c_str(),
                         O_CREAT | O_BINARY | O_RDWR,
                         S_IREAD|S_IWRITE)) == -1 )
   {
   	// se va termina daca nu poate deschide pentru modificari fisierul
   	Application->MessageBox((UnicodeString("Error opening ") + path + UnicodeString(" file")).c_str(), L"Error", MB_OK);
   	Application->Terminate();
   }

   if ((filelength(handleInf) % sizeInf) != 0)
   {
   	// fisierul infantry.dat e corupt - il distruge la 0
   	Application->MessageBox(L"Invalid length for AIINFANTRY.DAT file", L"Error", MB_OK);
      chsize(handleInf, 0);
   }
   else
   {
      loadInfNames();
      if (filelength(handleInf) != 0)
         loadInf(0);
      else
         newInf();
   }
}

/*---------------------------------------------------------------------------
 descrition: inchide aiinfantry.dat
---------------------------------------------------------------------------*/
void TFormMain::closeInf()
{
   delete recInf;
   close(handleInf);
}

/*---------------------------------------------------------------------------
 descrition: initializeaza toate editurile si alte chestii
---------------------------------------------------------------------------*/
bool TFormMain::newInf()
{
   // quick erase the structure so that we won't have strange
   // leftover strings from ide builder in the file
   memset(recInf, 0, sizeof(*recInf));

   //-----------initializarea lui recInf
   strcpy(recInf->name,          "");
   strcpy(recInf->surname,       "");
   recInf->gender    = 0;    //adica male
   recInf->type   = 0;    //adica special
   strcpy(recInf->kind,          "");
   strcpy(recInf->description,   "");
   strcpy(recInf->sprite,        "");
   strcpy(recInf->shadow,        "");
   strcpy(recInf->mPortrait,     "");
   strcpy(recInf->mMiniPortrait, "");
   recInf->hp     = 0;
   
   //skills
   recInf->leadership= 0;
   recInf->experience= 0;
   recInf->bravery   = 0;
   recInf->camo      = 0;
   recInf->antitank  = 0;
   recInf->sr_combat = 0;
   recInf->snipery   = 0;
   recInf->high_tech = 0;
   recInf->stealth   = 0;
   recInf->influence = 0;
   recInf->wisdom    = 0;

   recInf->mMaxUpSlope   = 75;
   recInf->mMaxDownSlope = 75;
   //armor
   recInf->proj_res  = 0;
   recInf->plasma_res= 0;
   recInf->bullet_res= 0;
   recInf->fire_res  = 0;
   recInf->nrg_res   = 0;
   //weapon
   recInf->weptype   = 0;  //rifle
   recInf->damage    = 0;
   recInf->precision = 0;
   recInf->radius    = 0;
   recInf->fire_rate = 0;
   recInf->range     = 0;
   recInf->mTimeToLive          = 10000; // in miliseconds
   recInf->mTrackPersistency    = 3;
   recInf->projectiles_per_shot = 1;
   strcpy(recInf->projectile,         "");
   strcpy(recInf->effect_bitmap,      "");
   strcpy(recInf->aftereffect_bitmap, "");
   strcpy(recInf->onhit,              "");
   strcpy(recInf->onfire,             "");

   // points
   recInf->mObsPointsCount = 0;

   for(int i = 0; i < INFANTRY_MAXVISUAL; i++)
   {
     strcpy(recInf->mObsPoints[i].mName, "");
     recInf->mObsPoints[i].mMinScanRange = 6;
     //recInf->mObsPoints[i].mScanRange    = 6;
     recInf->mObsPoints[i].mMaxScanRange = 10;
     recInf->mObsPoints[i].mFOV          = 90;
     recInf->mObsPoints[i].mScanAngle    = 0;
     recInf->mObsPoints[i].mActive       = true;

     for(int j = 0; j < 8; j++)
     {
       recInf->mObsPoints[i].mMountPoints[j] = CPPoint(0, 0);
     }
   }

   for(int i = 0; i < 8; i++)
   {
     recInf->mFireHoles[i] = CPPoint(0, 0);
   }

   //comportament
   strcpy(recInf->ack1, "");
   strcpy(recInf->ack2, "");
   strcpy(recInf->ack3, "");
   strcpy(recInf->annoyed1, "");
   strcpy(recInf->annoyed2, "");
   strcpy(recInf->annoyed3, "");
   strcpy(recInf->resp1, "");
   strcpy(recInf->resp2, "");
   strcpy(recInf->resp3, "");
   strcpy(recInf->onidle1, "");
   strcpy(recInf->onidle2, "");
   strcpy(recInf->onhit1, "");
   strcpy(recInf->onhit2, "");
   strcpy(recInf->onkill1, "");
   strcpy(recInf->onkill2, "");
   strcpy(recInf->die1, "");
   strcpy(recInf->die2, "");
   //-----------initializarea lui recInf - gata


   lseek(handleInf, 0, SEEK_END);
   if (write(handleInf, recInf, sizeInf) != sizeInf)
      return false;
   comboCurrentInf->Items->Add("Unknown unit");
   lseek(handleInf, -sizeInf, SEEK_END);
   setInfRecordToData();
   updateInfRecords();
   return true;
}

/*---------------------------------------------------------------------------
 descrition: incarca in edituri inregistrarea index din aiinfantry.dat
---------------------------------------------------------------------------*/
bool TFormMain::loadInf(int index)
{
   if (filelength(handleInf) != 0)
   {
      lseek(handleInf, sizeInf*index, SEEK_SET);
      if (read(handleInf, recInf, sizeInf) == -1)
      {
         Application->MessageBox(L"Cannot read from aiinfantry.dat file", L"Error", MB_OK);
         return false;
      }
      else
      {
         lseek(handleInf, -sizeInf, SEEK_CUR);
         setInfRecordToData();
         updateInfRecords();
         return true;
      }
   }
   return false;
}

/*---------------------------------------------------------------------------
 descrition: salveaza din edituri in inregistrarea index din aiinfantry.dat
---------------------------------------------------------------------------*/
bool TFormMain::saveInf(int index)
{
   setInfDataToRecord();
   lseek(handleInf, index*sizeInf, SEEK_SET);
   if (write(handleInf, recInf, sizeInf)!= sizeInf)
      return false;
   lseek(handleInf, -sizeInf, SEEK_CUR);

   if (recInf->type == 0)
   //adica special
      comboCurrentInf->Items->Strings[index] = AnsiString(recInf->name) + " " +
                                               AnsiString(recInf->surname);
   else
      comboCurrentInf->Items->Strings[index] = recInf->kind;
   updateInfRecords();
   return true;
}

/*---------------------------------------------------------------------------
 descrition: sterge inregistrarea index din aiinfantry.dat
---------------------------------------------------------------------------*/
bool TFormMain::deleteInf(int index)
{
   hInfantryUnified tmpInf = new TInfantryUnified;
   if (filelength(handleInf) != 0)
   {
      lseek(handleInf, sizeInf*(index+1), SEEK_SET);
      while (!eof(handleInf))
      {
         // citesc rec. curenta, ma misc inapoi 2 recorduri
         // o scriu, ma mut in fatza 2 rec.
         read(handleInf, tmpInf, sizeInf);
         lseek(handleInf, -2*sizeInf, SEEK_CUR);
         write(handleInf, tmpInf, sizeInf);
         lseek(handleInf, sizeInf, SEEK_CUR);
      }
      chsize(handleInf, filelength(handleInf)-sizeInf);
      loadInf(index);
   }
   comboCurrentInf->Items->Delete(index);
   delete tmpInf;
   updateInfRecords();
   return true;
}

/*---------------------------------------------------------------------------
 descrition: sterge toate inregistrarile din aiinfantry.dat
---------------------------------------------------------------------------*/
bool TFormMain::deleteAllInf()
{
   chsize(handleInf, 0);
   comboCurrentInf->Clear();
   updateInfRecords();
   return true;
}

/*---------------------------------------------------------------------------
 descrition: pune in recInf ce se gaseste in edit-uri
---------------------------------------------------------------------------*/
void TFormMain::setInfDataToRecord()
{
   strcpy(recInf->surname, UTF8Encode(comboInfSurname->Text).c_str());
   strcpy(recInf->name, UTF8Encode(comboInfFirstName->Text).c_str());

   strcpy(recInf->description, UTF8Encode(memoInfStory->Lines->Text).c_str());
   recInf->gender  = comboGender->ItemIndex;
   recInf->type = radiogroupInfType->ItemIndex;
   strcpy(recInf->kind, UTF8Encode(comboInfKind->Text).c_str());
   strcpy(recInf->sprite, UTF8Encode(editInfSprite->Text).c_str());
   strcpy(recInf->mPortrait, UTF8Encode(editInfPortrait->Text).c_str());
   strcpy(recInf->mMiniPortrait, UTF8Encode(editInfMiniPortrait->Text).c_str());
   recInf->hp   = editInfHP->Text.ToInt();

   //skills
   recInf->leadership    = editInfLeader->Text.ToInt();
   recInf->experience    = editInfExp->Text.ToInt();
   recInf->bravery       = editInfBravery->Text.ToInt();
   recInf->camo          = editInfCamo->Text.ToInt();
   recInf->antitank      = editInfAT->Text.ToInt();
   recInf->sr_combat     = editInfSR->Text.ToInt();
   recInf->snipery       = editInfSnipery->Text.ToInt();
   recInf->high_tech     = editInfHigh->Text.ToInt();
   recInf->stealth       = editInfStealth->Text.ToInt();
   recInf->influence     = editInfInfl->Text.ToInt();
   recInf->wisdom        = editInfWis->Text.ToInt();
   //recInf->mMaxUpSlope   = editInfUpSlope->Text.ToInt();
   //recInf->mMaxDownSlope = editInfDownslope->Text.ToInt();
   //armor
   recInf->proj_res  = editInfXRes->Text.ToInt();
   recInf->plasma_res= editInfPRes->Text.ToInt();
   recInf->bullet_res= editInfBRes->Text.ToInt();
   recInf->fire_res  = editInfFRes->Text.ToInt();     
   recInf->nrg_res   = editInfLRes->Text.ToInt();
   //weapon
   recInf->weptype   = comboInfWepType->ItemIndex;
   recInf->damage    = editInfDamage->Text.ToInt();
   recInf->precision = editInfPrec->Text.ToInt();
   recInf->radius    = editInfRad->Text.ToInt();
   recInf->fire_rate = editInfRate->Text.ToInt();
   recInf->range     = editInfRange->Text.ToInt();
   recInf->projectiles_per_shot = editInfPShot->Text.ToInt();
   recInf->mTimeToLive          = editInfTTL->Text.ToInt();
   recInf->mTrackPersistency    = editInfTrackP->Text.ToInt();

   strcpy(recInf->projectile, UTF8Encode(editInfFxBmp->Text).c_str());
   strcpy(recInf->effect_bitmap, UTF8Encode(editInfProjBmp->Text).c_str());
   strcpy(recInf->onhit, UTF8Encode(editInfOnHit->Text).c_str());
   strcpy(recInf->onfire, UTF8Encode(editInfOnFire->Text).c_str());

   //comportament
   strcpy(recInf->ack1, UTF8Encode(listInfAck->Items->Strings[0]).c_str());
   strcpy(recInf->ack2, UTF8Encode(listInfAck->Items->Strings[1]).c_str());
   strcpy(recInf->ack3, UTF8Encode(listInfAck->Items->Strings[2]).c_str());
   strcpy(recInf->annoyed1, UTF8Encode(listInfAnn->Items->Strings[0]).c_str());
   strcpy(recInf->annoyed2, UTF8Encode(listInfAnn->Items->Strings[1]).c_str());
   strcpy(recInf->annoyed3, UTF8Encode(listInfAnn->Items->Strings[2]).c_str());
   strcpy(recInf->resp1, UTF8Encode(listInfResp->Items->Strings[0]).c_str());
   strcpy(recInf->resp2, UTF8Encode(listInfResp->Items->Strings[1]).c_str());
   strcpy(recInf->resp3, UTF8Encode(listInfResp->Items->Strings[2]).c_str());
   strcpy(recInf->onidle1, UTF8Encode(listInfIdle->Items->Strings[0]).c_str());
   strcpy(recInf->onidle2, UTF8Encode(listInfIdle->Items->Strings[1]).c_str());
   strcpy(recInf->onhit1, UTF8Encode(listInfHit->Items->Strings[0]).c_str());
   strcpy(recInf->onhit2, UTF8Encode(listInfHit->Items->Strings[1]).c_str());
   strcpy(recInf->onkill1, UTF8Encode(listInfKill->Items->Strings[0]).c_str());
   strcpy(recInf->onkill2, UTF8Encode(listInfKill->Items->Strings[1]).c_str());
   strcpy(recInf->die1, UTF8Encode(listInfDie->Items->Strings[0]).c_str());
   strcpy(recInf->die2, UTF8Encode(listInfDie->Items->Strings[1]).c_str());
}

/*---------------------------------------------------------------------------
 descrition: pune in edituri recInf
---------------------------------------------------------------------------*/
void TFormMain::setInfRecordToData()
{
   comboInfFirstName->Text  = recInf->name;
   comboInfSurname->Text = recInf->surname;

   comboGender->ItemIndex   = recInf->gender;

   radiogroupInfType->ItemIndex  = recInf->type;
   memoInfStory->Lines->Text     = recInf->description;
   comboInfKind->Text            = recInf->kind;
   groupInfData->Enabled         = radiogroupInfType->ItemIndex == 0;
   groupInfKind->Enabled         = radiogroupInfType->ItemIndex == 1;
   editInfSprite->Text           = recInf->sprite;
   editInfPortrait->Text         = recInf->mPortrait;
   editInfMiniPortrait->Text     = recInf->mMiniPortrait;
   editInfHP->Text               = recInf->hp;
   //skills
   editInfLeader->Text           = recInf->leadership;
   editInfExp->Text              = recInf->experience;
   editInfBravery->Text          = recInf->bravery;
   editInfCamo->Text             = recInf->camo;
   editInfAT->Text               = recInf->antitank;
   editInfSR->Text               = recInf->sr_combat;
   editInfSnipery->Text          = recInf->snipery;
   editInfHigh->Text             = recInf->high_tech;
   editInfStealth->Text          = recInf->stealth;
   editInfInfl->Text             = recInf->influence;
   editInfWis->Text              = recInf->wisdom;
   //editInfUpSlope->Text        = recInf->mMaxUpSlope;
   //editInfDownSlope->Text      = recInf->mMaxDownSlope;
   //armor
   editInfXRes->Text             = recInf->proj_res;
   editInfPRes->Text             = recInf->plasma_res;
   editInfBRes->Text             = recInf->bullet_res;
   editInfLRes->Text             = recInf->nrg_res;
   editInfFRes->Text             = recInf->fire_res;
   //weapon
   comboInfWepType->ItemIndex    = recInf->weptype;
   editInfDamage->Text           = recInf->damage;
   editInfPrec->Text             = recInf->precision;
   editInfRad->Text              = recInf->radius;
   editInfRate->Text             = recInf->fire_rate;
   editInfRange->Text            = recInf->range;
   editInfPShot->Text            = recInf->projectiles_per_shot;
   editInfTTL->Text              = recInf->mTimeToLive;
   editInfTrackP->Text           = recInf->mTrackPersistency;

   editInfFxBmp->Text            = recInf->projectile;
   editInfProjBmp->Text          = recInf->effect_bitmap;
   editInfAFxBmp->Text           = recInf->aftereffect_bitmap;
   editInfOnHit->Text            = recInf->onhit;
   editInfOnFire->Text           = recInf->onfire;
   //comportament
   listInfAck->Items->Clear();
   listInfAnn->Items->Clear();
   listInfResp->Items->Clear();
   listInfIdle->Items->Clear();
   listInfHit->Items->Clear();
   listInfKill->Items->Clear();
   listInfDie->Items->Clear();
   listInfAck->Items->Add(recInf->ack1);
   listInfAck->Items->Add(recInf->ack2);
   listInfAck->Items->Add(recInf->ack3);
   listInfAnn->Items->Add(recInf->annoyed1);
   listInfAnn->Items->Add(recInf->annoyed2);
   listInfAnn->Items->Add(recInf->annoyed3);
   listInfResp->Items->Add(recInf->resp1);
   listInfResp->Items->Add(recInf->resp2);
   listInfResp->Items->Add(recInf->resp3);
   listInfIdle->Items->Add(recInf->onidle1);
   listInfIdle->Items->Add(recInf->onidle2);
   listInfHit->Items->Add(recInf->onhit1);
   listInfHit->Items->Add(recInf->onhit2);
   listInfKill->Items->Add(recInf->onkill1);
   listInfKill->Items->Add(recInf->onkill2);
   listInfDie->Items->Add(recInf->die1);
   listInfDie->Items->Add(recInf->die2);
}


/*---------------------------------------------------------------------------
 descrition: face refresh pe numarul de obiecte din fisier si pe pozitia curenta
---------------------------------------------------------------------------*/
void TFormMain::updateInfRecords()
{
   updownInf->Max      = (short)(filelength(handleInf)/sizeInf - 1);
   updownInf->Position = (short)(lseek(handleInf, 0, SEEK_CUR)/sizeInf);
   updateInfRecordsCaption();
}

/*---------------------------------------------------------------------------
 descrition: scrie unde trebuie a cata inregistrare este activa
---------------------------------------------------------------------------*/
void TFormMain::updateInfRecordsCaption()
{
   if (updownInf->Max < 0)
      lbRecordPos->Caption= "empty";
   else
      lbRecordPos->Caption= AnsiString(updownInf->Position+1) + "/" +
                            AnsiString(updownInf->Max+1);
   comboCurrentInf->ItemIndex = updownInf->Position;
}

/*---------------------------------------------------------------------------
 descrition: pune in comboCurrentInf->Items numele sau kind
---------------------------------------------------------------------------*/
void TFormMain::loadInfNames()
{
   int i;
   lseek(handleInf, 0, SEEK_SET);
   for (i=0; i<filelength(handleInf)/sizeInf; i++)
   {
      read(handleInf, recInf, sizeInf);
      if (recInf->type == 0)
      //adica special
         comboCurrentInf->Items->Add(AnsiString(recInf->name) + " " +
                                     AnsiString(recInf->surname));
      else
         comboCurrentInf->Items->Add(recInf->kind);
   }
   comboCurrentInf->ItemIndex = 0;
}

/*---------------------------------------------------------------------------
INFANTRY PUBLIC AREA - over
---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------
VEHICLE PUBLIC AREA
---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------
 descrition: deschide aiinfantry.dat
---------------------------------------------------------------------------*/
void TFormMain::openVeh()
{
   AnsiString path = AnsiString(getGamePath().c_str()) + PATH_AIVEHICLE;
   path = path + FILE_AIVEHICLE;
   // initializeaza variabilele legate de fisierul aiinfantry.dat
   try
   {
      recVeh = new TVehicleUnified;
   }
   catch(...)
   {
   	Application->MessageBox(L"Not enough memory to create vehicle record", L"Error", MB_OK);
   	Application->Terminate();
   }
   sizeVeh = sizeof(TVehicleUnified);
   if ( (handleVeh = open(path.c_str(),
                         O_CREAT | O_BINARY | O_RDWR,
                         S_IREAD|S_IWRITE)) == -1 )
   {
   	// se va termina daca nu poate deschide pentru modificari fisierul
   	Application->MessageBox(UnicodeString("Error opening " + path + " file").c_str(), L"Error", MB_OK);
   	Application->Terminate();
   }

   if ((filelength(handleVeh) % sizeVeh) != 0)
   {
   	// fisierul infantry.dat e corupt - il distruge la 0
   	Application->MessageBox(L"Invalid length for AIVEHICLE.DAT file", L"Error", MB_OK);
      chsize(handleVeh, 0);
   }
   else
   {
      loadVehNames();
      if (filelength(handleVeh) != 0)
         loadVeh(0);
      else
         newVeh();
   }
}

/*---------------------------------------------------------------------------
 descrition: inchide aiinfantry.dat
---------------------------------------------------------------------------*/
void TFormMain::closeVeh()
{
   delete recVeh;
   close(handleVeh);
}

/*---------------------------------------------------------------------------
 descrition: initializeaza toate editurile si alte chestii
---------------------------------------------------------------------------*/
bool TFormMain::newVeh()
{
   // quick erase the structure so that we won't have strange
   // leftover strings from ide builder in the file
   memset(recInf, 0, sizeof(*recInf));
   
   //initialize vehicle record
   strcpy(recVeh->callsign, "New callsign");
   strcpy(recVeh->name, "New name");
   strcpy(recVeh->description, "No data available."); // crew info
   strcpy(recVeh->sprite, "");
   strcpy(recVeh->shadow, "");
   strcpy(recVeh->mPortrait, "");
   strcpy(recVeh->mMiniPortrait, "");

   // dying behaviour
   strcpy(recVeh->mExplodeSprite, "");
   recVeh->mExplodeCount = 0;

   recVeh->hp              = 100;
   recVeh->mTurretTurnRate = 1;
   recVeh->engine_type     = 0; // ???
   recVeh->selfdestruct    = false;
   recVeh->vType           = 0;
   recVeh->movement        = 1;
   recVeh->mMaxUpSlope     = 60;
   recVeh->mMaxDownSlope   = 70;

   for(int i = 0; i < 8; i++)
   {
     recVeh->body[i] = CPPoint(0, 0);
   }

   for(int i = 0; i < 16; i++)
   {
     recVeh->turret[i] = CPPoint(0, 0);
   }

   for(int i = 0; i < 8; i++)
   {
     recVeh->primary[i] = CPPoint(0, 0);
     recVeh->secondary[i] = CPPoint(0, 0);
   }  

   for(int i = 0; i < VEHICLE_MAXBODYLIGHTS; i++)
   {
     recVeh->bodyLights[i] = CPPoint(0, 0);
   }

   recVeh->bodyLightsNo = 0;

   for(int i = 0; i < VEHICLE_MAXTURRETLIGHTS; i++)
   {
     recVeh->turretLights[i] = CPPoint(0, 0);
   }

   recVeh->turretLightsNo = 0;
   recVeh->mBayCapacity   = 6;

   recVeh->mObsPointsCount = 0;

   for(int i = 0; i < VEHICLE_MAXVISUAL; i++)
   {
     strcpy(recInf->mObsPoints[i].mName, "");
     recInf->mObsPoints[i].mMinScanRange = 6;
     //recInf->mObsPoints[i].mScanRange    = 6;
     recInf->mObsPoints[i].mMaxScanRange = 10;
     recInf->mObsPoints[i].mFOV          = 90;
     recInf->mObsPoints[i].mScanAngle    = 0;
     recInf->mObsPoints[i].mActive       = true;

     for(int j = 0; j < 8; j++)
     {
       recInf->mObsPoints[i].mMountPoints[j] = CPPoint(0, 0);
     }
   }

   //recVeh->gunnery    = 0;
   recVeh->experience = 0;
   recVeh->driveskill = 0;
   recVeh->bravery    = 0;
   recVeh->morale     = 0;
   recVeh->leadership = 0;
   recVeh->anti_infantry = 0;
   recVeh->anti_vehicle  = 0;
   recVeh->anti_air      = 0;
   recVeh->cm_skill      = 0;
   recVeh->teamwork      = 0; 

   // weapon 1
   recVeh->wep1_enabled   = true;
   recVeh->wep1_type      = 1;
   recVeh->wep1_damage    = 100;
   recVeh->wep1_minrange  = 4;
   recVeh->wep1_maxrange  = 16;
   recVeh->wep1_precision = 50;
   recVeh->wep1_radius    = 3;
   recVeh->wep1_firerate  = 3;
   recVeh->wep1_projectiles_per_shot  = 1;
   recVeh->mWep1_TimeToLive           = 10000; // miliseconds
   recVeh->wep1_trackPersistency      = 3;

   strcpy(recVeh->wep1_projectile        , "");
   strcpy(recVeh->wep1_aftereffectbitmap , "");
   strcpy(recVeh->wep1_effectbitmap      , "");
   strcpy(recVeh->wep1_onhit             , "");
   strcpy(recVeh->wep1_onfire            , "");

   // weapon 2
   recVeh->wep2_enabled   = false;
   recVeh->wep2_type      = 14;
   recVeh->wep2_damage    = 20;
   recVeh->wep2_minrange  = 2;
   recVeh->wep2_maxrange  = 5;
   recVeh->wep2_precision = 40;
   recVeh->wep2_radius    = 0;
   recVeh->wep2_firerate  = 2;
   recVeh->wep2_projectiles_per_shot  = 1;
   recVeh->mWep2_TimeToLive           = 10000; // miliseconds
   recVeh->wep2_trackPersistency      = 3;

   strcpy(recVeh->wep2_projectile   , "");
   strcpy(recVeh->wep2_effectbitmap , "");
   strcpy(recVeh->wep2_aftereffectbitmap , "");
   strcpy(recVeh->wep2_onhit        , "");
   strcpy(recVeh->wep2_onfire       , "");

   //armor
   recVeh->exp_res    = 0;          
   recVeh->nrg_res    = 0;
   recVeh->plasma_res = 0;
   recVeh->bullet_res = 0;
   recVeh->fire_res   = 0;

   //audio comportament
   strcpy(recVeh->resp1, "");
   strcpy(recVeh->resp2, "");
   strcpy(recVeh->resp3, "");
   strcpy(recVeh->ack1, "");
   strcpy(recVeh->ack2, "");
   strcpy(recVeh->ack3, "");
   strcpy(recVeh->annoyed1, "");
   strcpy(recVeh->annoyed2, "");
   strcpy(recVeh->annoyed3, "");
   strcpy(recVeh->onhit1, "");
   strcpy(recVeh->onhit2, "");
   strcpy(recVeh->onkill1, "");
   strcpy(recVeh->onkill2, "");
   strcpy(recVeh->die1, "");
   strcpy(recVeh->die2, "");
   strcpy(recVeh->onidle1, "");
   strcpy(recVeh->onidle2, "");
   //initialize vehicle record - over

   lseek(handleVeh, 0, SEEK_END);
   if (write(handleVeh, recVeh, sizeVeh) != sizeVeh)
      return false;
   comboCurrentVeh->Items->Add("Unknown unit");
   lseek(handleVeh, -sizeVeh, SEEK_END);
   setVehRecordToData();
   updateVehRecords();
   return true;
}

/*---------------------------------------------------------------------------
 descrition: incarca inregistrarea index din aivehicle.dat
---------------------------------------------------------------------------*/
bool TFormMain::loadVeh(int index)
{
   if (filelength(handleVeh) != 0)
   {
      lseek(handleVeh, sizeVeh*index, SEEK_SET);
      if (read(handleVeh, recVeh, sizeVeh) == -1)
      {
         Application->MessageBox(L"Cannot read from aivehicle.dat file", L"Error", MB_OK);
         return false;
      }
      else
      {
         lseek(handleVeh, -sizeVeh, SEEK_CUR);
         setVehRecordToData();
         updateVehRecords();
         return true;
      }
   }
   return false;
}

/*---------------------------------------------------------------------------
 descrition: salveaza in inregistrarea index din aivehicle.dat
---------------------------------------------------------------------------*/
bool TFormMain::saveVeh(int index)
{
   setVehDataToRecord();
   lseek(handleVeh, index*sizeVeh, SEEK_SET);
   if (write(handleVeh, recVeh, sizeVeh)!= sizeVeh)
      return false;
   lseek(handleVeh, -sizeVeh, SEEK_CUR);

   //trebuie adaugat in comboCurrentVehicle
   comboCurrentVeh->Items->Strings[index] = recVeh->callsign;

   updateVehRecords();
   return true;
}

/*---------------------------------------------------------------------------
 descrition: sterge index din aivehicle.dat
---------------------------------------------------------------------------*/
bool TFormMain::deleteVeh(int index)
{
   hVehicleUnified tmpVeh = new TVehicleUnified;
   if (filelength(handleVeh) != 0)
   {
      lseek(handleVeh, sizeVeh*(index+1), SEEK_SET);
      while (!eof(handleVeh))
      {
         // citesc rec. curenta, ma misc inapoi 2 recorduri
         // o scriu, ma mut in fatza 2 rec.
         read(handleVeh, tmpVeh, sizeVeh);
         lseek(handleVeh, -2*sizeVeh, SEEK_CUR);
         write(handleVeh, tmpVeh, sizeVeh);
         lseek(handleVeh, sizeVeh, SEEK_CUR);
      }
      chsize(handleVeh, filelength(handleVeh)-sizeVeh);
      loadVeh(index);
   }
   comboCurrentVeh->Items->Delete(index);
   delete tmpVeh;
   updateVehRecords();
   return true;
}

/*---------------------------------------------------------------------------
 descrition: sterge toate inregistrarile din aivehicle.dat
---------------------------------------------------------------------------*/
bool TFormMain::deleteAllVeh()
{
   chsize(handleVeh, 0);
   comboCurrentVeh->Clear();
   updateVehRecords();
   return true;
}

/*---------------------------------------------------------------------------
 descrition: pune in recVeh ceea ce a completat utilizatorul
---------------------------------------------------------------------------*/
void TFormMain::setVehDataToRecord()
{
   strcpy(recVeh->callsign,      UTF8Encode(comboVehCallsign->Text).c_str());
   strcpy(recVeh->name,          UTF8Encode(comboVehName->Text).c_str());
   strcpy(recVeh->description,   UTF8Encode(memoVehCrewInfo->Text).c_str()); // crew info
   strcpy(recVeh->sprite,        UTF8Encode(editVehSprite->Text).c_str());
   strcpy(recVeh->mPortrait,     UTF8Encode(editVehPortrait->Text).c_str());
   strcpy(recVeh->mMiniPortrait, UTF8Encode(editVehMiniPortrait->Text).c_str());

   recVeh->hp              = editVehHP->Text.ToInt();
   recVeh->mTurretTurnRate = editVehTurretTurn->Text.ToInt();
   recVeh->selfdestruct    = ch_vSelf->Checked;
   recVeh->vType           = comboVehType->ItemIndex;
   recVeh->movement        = comboVehMove->ItemIndex;
   recVeh->mMaxUpSlope     = editVehUpSlope->Text.ToInt();
   recVeh->mMaxDownSlope   = editVehDownSlope->Text.ToInt();
   recVeh->mBayCapacity    = editVehBayCapacity->Text.ToInt();

   // dying
   strcpy(recVeh->mExplodeSprite, UTF8Encode(edVExplodeSprite->Text).c_str());
   recVeh->mExplodeCount = edVExplodeCount->Value;

   //recVeh->gunnery    = editVehGun->Text.ToInt();
   recVeh->experience    = editVehExp->Text.ToInt();
   recVeh->driveskill    = editVehDrive->Text.ToInt();
   recVeh->bravery       = editVehBrave->Text.ToInt();
   recVeh->morale        = editVehMorale->Text.ToInt();
   recVeh->leadership    = editVehLead->Text.ToInt();
   recVeh->anti_infantry = editVehAntiInf->Text.ToInt();
   recVeh->anti_vehicle  = editVehAntiVeh->Text.ToInt();
   recVeh->anti_air      = editVehAntiAir->Text.ToInt();
   recVeh->cm_skill      = editVehECM->Text.ToInt();
   recVeh->teamwork      = editVehTeam->Text.ToInt();

   // weapon 1
   recVeh->wep1_enabled   = checkVehW1Enabled->Checked;
   recVeh->wep1_type      = comboVehW1Type->ItemIndex;
   recVeh->wep1_damage    = editVehW1Damage->Text.ToInt();
   recVeh->wep1_minrange  = editVehW1MinRange->Text.ToInt();
   recVeh->wep1_maxrange  = editVehW1MaxRange->Text.ToInt();
   recVeh->wep1_precision = editVehW1Prec->Text.ToInt();
   recVeh->wep1_radius    = editVehW1Rad->Text.ToInt();
   recVeh->wep1_firerate  = editVehW1Rate->Text.ToInt();
   recVeh->wep1_projectiles_per_shot = editVehW1PShot->Text.ToInt();
   recVeh->mWep1_TimeToLive = editVehW1TTL->Text.ToInt();
   recVeh->wep1_trackPersistency = editVehW1TrackP->Text.ToInt();

   strcpy(recVeh->wep1_projectile   , UTF8Encode(editVehW1Proj->Text).c_str());
   strcpy(recVeh->wep1_effectbitmap , UTF8Encode(editVehW1Fx->Text).c_str());
   strcpy(recVeh->wep1_aftereffectbitmap , UTF8Encode(editVehW1AFx->Text).c_str());
   strcpy(recVeh->wep1_onhit        , UTF8Encode(editVehW1Hit->Text).c_str());
   strcpy(recVeh->wep1_onfire       , UTF8Encode(editVehW1Fire->Text).c_str());

   // weapon 2
   recVeh->wep2_enabled   = checkVehW2Enabled->Checked;
   recVeh->wep2_type      = comboVehW2Type->ItemIndex;
   recVeh->wep2_damage    = editVehW2Damage->Text.ToInt();
   recVeh->wep2_minrange  = editVehW2MinRange->Text.ToInt();
   recVeh->wep2_maxrange  = editVehW2MaxRange->Text.ToInt();
   recVeh->wep2_precision = editVehW2Prec->Text.ToInt();
   recVeh->wep2_radius    = editVehW2Rad->Text.ToInt();
   recVeh->wep2_firerate  = editVehW2Rate->Text.ToInt();
   recVeh->wep2_projectiles_per_shot = editVehW2PShot->Text.ToInt();
   recVeh->mWep2_TimeToLive = editVehW2TTL->Text.ToInt();
   recVeh->wep2_trackPersistency     = editVehW2TrackP->Text.ToInt();

   strcpy(recVeh->wep2_projectile   , UTF8Encode(editVehW2Proj->Text).c_str());
   strcpy(recVeh->wep2_effectbitmap , UTF8Encode(editVehW2Fx->Text).c_str());
   strcpy(recVeh->wep2_aftereffectbitmap , UTF8Encode(editVehW2AFx->Text).c_str());
   strcpy(recVeh->wep2_onhit        , UTF8Encode(editVehW2Hit->Text).c_str());
   strcpy(recVeh->wep2_onfire       , UTF8Encode(editVehW2Fire->Text).c_str());

   //armor
   recVeh->exp_res    = editVehXRes->Text.ToInt();
   recVeh->nrg_res    = editVehLRes->Text.ToInt();
   recVeh->plasma_res = editVehPRes->Text.ToInt();
   recVeh->bullet_res = editVehBRes->Text.ToInt();
   recVeh->fire_res   = editVehFRes->Text.ToInt();

   //audio comportament
   strcpy(recVeh->resp1, UTF8Encode(listVehResp->Items->Strings[0]).c_str());
   strcpy(recVeh->resp2, UTF8Encode(listVehResp->Items->Strings[1]).c_str());
   strcpy(recVeh->resp3, UTF8Encode(listVehResp->Items->Strings[2]).c_str());
   strcpy(recVeh->ack1, UTF8Encode(listVehAck->Items->Strings[0]).c_str());
   strcpy(recVeh->ack2, UTF8Encode(listVehAck->Items->Strings[1]).c_str());
   strcpy(recVeh->ack3, UTF8Encode(listVehAck->Items->Strings[2]).c_str());
   strcpy(recVeh->annoyed1, UTF8Encode(listVehAnn->Items->Strings[0]).c_str());
   strcpy(recVeh->annoyed2, UTF8Encode(listVehAnn->Items->Strings[1]).c_str());
   strcpy(recVeh->annoyed3, UTF8Encode(listVehAnn->Items->Strings[2]).c_str());
   strcpy(recVeh->onhit1, UTF8Encode(listVehHit->Items->Strings[0]).c_str());
   strcpy(recVeh->onhit2, UTF8Encode(listVehHit->Items->Strings[1]).c_str());
   strcpy(recVeh->onkill1, UTF8Encode(listVehKill->Items->Strings[0]).c_str());
   strcpy(recVeh->onkill2, UTF8Encode(listVehKill->Items->Strings[1]).c_str());
   strcpy(recVeh->die1, UTF8Encode(listVehDie->Items->Strings[0]).c_str());
   strcpy(recVeh->die2, UTF8Encode(listVehKill->Items->Strings[1]).c_str());
   strcpy(recVeh->onidle1, UTF8Encode(listVehIdle->Items->Strings[0]).c_str());
   strcpy(recVeh->onidle2, UTF8Encode(listVehKill->Items->Strings[1]).c_str());
}

/*---------------------------------------------------------------------------
 descrition: pune din recVeh in edituri, llistboxuri si altele
---------------------------------------------------------------------------*/
void TFormMain::setVehRecordToData()
{
   // crew/vehicle text stuff
   comboVehCallsign->Text    = recVeh->callsign;
   comboVehName->Text        = recVeh->name;
   memoVehCrewInfo->Text     = recVeh->description;
   editVehSprite->Text       = recVeh->sprite;
   editVehPortrait->Text     = recVeh->mPortrait;
   editVehMiniPortrait->Text = recVeh->mMiniPortrait;

   // generic vechicle data
   editVehHP->Text         = recVeh->hp;
   editVehTurretTurn->Text = recVeh->mTurretTurnRate;
   ch_vSelf->Checked       = recVeh->selfdestruct;
   comboVehType->ItemIndex = recVeh->vType;
   comboVehMove->ItemIndex = recVeh->movement;
   editVehUpSlope->Text    = recVeh->mMaxUpSlope;
   editVehDownSlope->Text  = recVeh->mMaxDownSlope;
   editVehBayCapacity->Text= recVeh->mBayCapacity;

   // dying
   edVExplodeSprite->Text = recVeh->mExplodeSprite;
   edVExplodeCount->Value = recVeh->mExplodeCount;

   // crew skills
   //editVehGun->Text   = recVeh->gunnery;
   editVehExp->Text     = recVeh->experience;
   editVehDrive->Text   = recVeh->driveskill;
   editVehBrave->Text   = recVeh->bravery;
   editVehMorale->Text  = recVeh->morale;
   editVehLead->Text    = recVeh->leadership;
   editVehAntiInf->Text = recVeh->anti_infantry;
   editVehAntiVeh->Text = recVeh->anti_vehicle;
   editVehAntiAir->Text = recVeh->anti_air;
   editVehECM->Text     = recVeh->cm_skill;
   editVehTeam->Text    = recVeh->teamwork;

   // weapon 1
   checkVehW1Enabled->Checked = recVeh->wep1_enabled;
   comboVehW1Type->ItemIndex = recVeh->wep1_type;
   editVehW1Damage->Text   = recVeh->wep1_damage;
   editVehW1MinRange->Text = recVeh->wep1_minrange;
   editVehW1MaxRange->Text = recVeh->wep1_maxrange;
   editVehW1Prec->Text     = recVeh->wep1_precision;
   editVehW1Rad->Text      = recVeh->wep1_radius;
   editVehW1Rate->Text     = recVeh->wep1_firerate;
   editVehW1PShot->Text    = recVeh->wep1_projectiles_per_shot;
   editVehW1TTL->Text      = recVeh->mWep1_TimeToLive;
   editVehW1TrackP->Text   = recVeh->wep1_trackPersistency;

   editVehW1Proj->Text = recVeh->wep1_projectile;
   editVehW1Fx->Text   = recVeh->wep1_effectbitmap;
   editVehW1AFx->Text  = recVeh->wep1_aftereffectbitmap;
   editVehW1Hit->Text  = recVeh->wep1_onhit;
   editVehW1Fire->Text = recVeh->wep1_onfire;

   // weapon 2
   checkVehW2Enabled->Checked = recVeh->wep2_enabled;
   comboVehW2Type->ItemIndex = recVeh->wep2_type;
   editVehW2Damage->Text   = recVeh->wep2_damage;
   editVehW2MinRange->Text = recVeh->wep2_minrange;
   editVehW2MaxRange->Text = recVeh->wep2_maxrange;
   editVehW2Prec->Text     = recVeh->wep2_precision;
   editVehW2Rad->Text      = recVeh->wep2_radius;
   editVehW2Rate->Text     = recVeh->wep2_firerate;
   editVehW2PShot->Text    = recVeh->wep2_projectiles_per_shot;
   editVehW2TTL->Text      = recVeh->mWep2_TimeToLive;
   editVehW2TrackP->Text   = recVeh->wep2_trackPersistency;

   editVehW2Proj->Text = recVeh->wep2_projectile;
   editVehW2Fx->Text   = recVeh->wep2_effectbitmap;
   editVehW2AFx->Text  = recVeh->wep2_aftereffectbitmap;
   editVehW2Hit->Text  = recVeh->wep2_onhit;
   editVehW2Fire->Text = recVeh->wep2_onfire;

   // vehicle armor
   editVehXRes->Text  = recVeh->exp_res;
   editVehLRes->Text  = recVeh->nrg_res;
   editVehPRes->Text  = recVeh->plasma_res;
   editVehBRes->Text  = recVeh->bullet_res;
   editVehFRes->Text  = recVeh->fire_res;

   // comportaments
   listVehAck->Items->Clear();
   listVehAnn->Items->Clear();
   listVehResp->Items->Clear();
   listVehIdle->Items->Clear();
   listVehHit->Items->Clear();
   listVehKill->Items->Clear();
   listVehDie->Items->Clear();
   listVehAck->Items->Add(recVeh->ack1);
   listVehAck->Items->Add(recVeh->ack2);
   listVehAck->Items->Add(recVeh->ack3);
   listVehResp->Items->Add(recVeh->resp1);
   listVehResp->Items->Add(recVeh->resp2);
   listVehResp->Items->Add(recVeh->resp3);
   listVehAnn->Items->Add(recVeh->annoyed1);
   listVehAnn->Items->Add(recVeh->annoyed2);
   listVehAnn->Items->Add(recVeh->annoyed3);
   listVehHit->Items->Add(recVeh->onhit1);
   listVehHit->Items->Add(recVeh->onhit2);
   listVehKill->Items->Add(recVeh->onkill1);
   listVehKill->Items->Add(recVeh->onkill2);
   listVehDie->Items->Add(recVeh->die1);
   listVehDie->Items->Add(recVeh->die2);
   listVehIdle->Items->Add(recVeh->onidle1);
   listVehIdle->Items->Add(recVeh->onidle2);

   // misc
   // btnVehSkeleton->Enabled = (recVeh->vType != 4);
}

/*---------------------------------------------------------------------------
 description: face refresh pe numarul de obiecte din fisier si pe pozitia curenta
---------------------------------------------------------------------------*/
void TFormMain::updateVehRecords()
{
   updownVeh->Max      = (short)(filelength(handleVeh)/sizeVeh - 1);
   updownVeh->Position = (short)(lseek(handleVeh, 0, SEEK_CUR)/sizeVeh);
   updateVehRecordsCaption();
}

/*---------------------------------------------------------------------------
 description: face refresh pe numarul de obiecte din fisier si pe pozitia curenta
---------------------------------------------------------------------------*/
void TFormMain::updateVehRecordsCaption()
{
   if (updownVeh->Max < 0)
      lbVehRecordPos->Caption= "empty";
   else
      lbVehRecordPos->Caption= AnsiString(updownVeh->Position+1) + "/" +
                            AnsiString(updownVeh->Max+1);
   comboCurrentVeh->ItemIndex = updownVeh->Position;
}

/*---------------------------------------------------------------------------
 description: incarca toate numele vehiculelor din fisier in comboCurrentVeh
---------------------------------------------------------------------------*/
void TFormMain::loadVehNames()
{
   int i;
   lseek(handleVeh, 0, SEEK_SET);
   for (i=0; i<filelength(handleVeh)/sizeVeh; i++)
   {
      read(handleVeh, recVeh, sizeVeh);
      comboCurrentVeh->Items->Add(AnsiString(recVeh->callsign));
   }
   comboCurrentVeh->ItemIndex = 0;
}
/*---------------------------------------------------------------------------
VEHICLE PUBLIC AREA - over
---------------------------------------------------------------------------*/

void __fastcall TFormMain::DisplayHint(TObject *Sender)
{
  statusMain->Panels->Items[0]->Text = GetLongHint(Application->Hint);
}

/////////////////////////////////////////////////////////////////////////////
//PUBLISHED AREA
/////////////////////////////////////////////////////////////////////////////
void __fastcall TFormMain::FormCreate(TObject *Sender)
{
   initGamePath();
   openInf();
   openVeh();
   Application->OnHint = DisplayHint;
}
//---------------------------------------------------------------------------

void __fastcall TFormMain::FormClose(TObject *Sender, TCloseAction &Action)
{
   closeInf();
   closeVeh();
}
//---------------------------------------------------------------------------

void __fastcall TFormMain::itExitClick(TObject *Sender)
{
   Application->Terminate();
}
//---------------------------------------------------------------------------

void __fastcall TFormMain::itPreviewClick(TObject *Sender)
{
   FormPreview->Show();
}
//---------------------------------------------------------------------------

void __fastcall TFormMain::itAboutClick(TObject *Sender)
{
   FormAbout->ShowModal();
}
//---------------------------------------------------------------------------

/*---------------------------------------------------------------------------
INFANTRY AREA PUBLISHED
---------------------------------------------------------------------------*/
void __fastcall TFormMain::radiogroupInfTypeClick(TObject *Sender)
{
   groupInfData->Enabled = (radiogroupInfType->ItemIndex == 0);
   groupInfKind->Enabled = (radiogroupInfType->ItemIndex == 1);
}
//-------------------------------------------------------------------

void __fastcall TFormMain::btnInfNewClick(TObject *Sender)
{
   newInf();
}
//---------------------------------------------------------------------------

void __fastcall TFormMain::btnInfSaveClick(TObject *Sender)
{
   saveInf(updownInf->Position);
}
//---------------------------------------------------------------------------

void __fastcall TFormMain::btnInfDeleteClick(TObject *Sender)
{
   deleteInf(updownInf->Position);
}
//---------------------------------------------------------------------------

void __fastcall TFormMain::btnInfDeleteAllClick(TObject *Sender)
{
   if (Application->MessageBox(L"This will erase all records in AIINFANTRY.DAT. Continue ?", L"Warning", MB_YESNO)==IDYES)
      deleteAllInf();
}
//---------------------------------------------------------------------------

/*---------------------------------------------------------------------------
 smecherie: am atasat de un updown un edit care nu se vede
            si asa pot sa vad cand s-a scimbat :)
---------------------------------------------------------------------------*/
void __fastcall TFormMain::editCurrentInfChange(TObject *Sender)
{
   int tmp = editCurrentInf->Text.ToInt();
   loadInf(tmp);
}
//---------------------------------------------------------------------------

void __fastcall TFormMain::comboCurrentInfChange(TObject *Sender)
{
   loadInf(comboCurrentInf->ItemIndex);
}
//---------------------------------------------------------------------------

void __fastcall TFormMain::btnInfLoadSpriteClick(TObject *Sender)
{
   dialogOpen->FileName    = "";
   dialogOpen->InitialDir  = FixPath(AnsiString(getGamePath().c_str()) + PATH_INF_GFX); //PATH_GFXINFANTRY;
   dialogOpen->Title       = "Select infantry sprite";
   dialogOpen->FilterIndex = 1;
   if (dialogOpen->Execute())
      editInfSprite->Text = ExtractFileName(dialogOpen->FileName);
}
//---------------------------------------------------------------------------

void __fastcall TFormMain::btnLoadInfFxBmpClick(TObject *Sender)
{
   dialogOpen->FileName    = "";
   dialogOpen->InitialDir  = FixPath(AnsiString(getGamePath().c_str()) + PATH_EXPLODES_GFX); //PATH_GFXEXPLODE;
   dialogOpen->Title       = "Select infantry weapon effect";
   dialogOpen->FilterIndex = 1;
   if (dialogOpen->Execute())
      editInfFxBmp->Text = ExtractFileName(dialogOpen->FileName);
}
//---------------------------------------------------------------------------

void __fastcall TFormMain::btnLoadInfProjClick(TObject *Sender)
{
   dialogOpen->FileName    = "";
   dialogOpen->InitialDir  = FixPath(AnsiString(getGamePath().c_str()) + PATH_EXPLODES_GFX); //PATH_GFXEXPLODE;
   dialogOpen->Title       = "Select infantry weapon projectile";
   dialogOpen->FilterIndex = 1;
   if (dialogOpen->Execute())
      editInfProjBmp->Text = ExtractFileName(dialogOpen->FileName);
}
//---------------------------------------------------------------------------

void __fastcall TFormMain::btnLoadInfAFxClick(TObject *Sender)
{
   dialogOpen->FileName    = "";
   dialogOpen->InitialDir  = FixPath(AnsiString(getGamePath().c_str()) + PATH_EXPLODES_GFX); //PATH_GFXEXPLODE;
   dialogOpen->Title       = "Select infantry weapon after effect";
   dialogOpen->FilterIndex = 1;
   if (dialogOpen->Execute())
   {
     editInfAFxBmp->Text = ExtractFileName(dialogOpen->FileName);
   }
}
//---------------------------------------------------------------------------

void __fastcall TFormMain::btnLoadInfOnHitClick(TObject *Sender)
{
   dialogOpen->FileName    = "";
   dialogOpen->InitialDir  = FixPath(AnsiString(getGamePath() + PATH_SFX));
   dialogOpen->Title       = "Select infantry weapon sound - on hit";
   dialogOpen->FilterIndex = 2;
   if (dialogOpen->Execute())
      editInfOnHit->Text = ExtractFileName(dialogOpen->FileName);
}
//---------------------------------------------------------------------------

void __fastcall TFormMain::btnLoadInfOnFireClick(TObject *Sender)
{
   dialogOpen->FileName    = "";
   dialogOpen->InitialDir  = FixPath(AnsiString(getGamePath() + PATH_SFX));
   dialogOpen->Title       = "Select infantry weapon sound - on fire";
   dialogOpen->FilterIndex = 2;
   if (dialogOpen->Execute())
      editInfOnFire->Text = ExtractFileName(dialogOpen->FileName);
}
//---------------------------------------------------------------------------

void __fastcall TFormMain::listInfAckDblClick(TObject *Sender)
{
   dialogOpen->FileName    = "";
   dialogOpen->InitialDir  = FixPath(AnsiString(getGamePath() + PATH_SFX));
   dialogOpen->Title       = "Select infantry acknowledge sound";
   dialogOpen->FilterIndex = 2;
   if (dialogOpen->Execute())
      listInfAck->Items->Strings[listInfAck->ItemIndex] = ExtractFileName(dialogOpen->FileName);
}
//---------------------------------------------------------------------------

void __fastcall TFormMain::listInfAnnDblClick(TObject *Sender)
{
   dialogOpen->FileName    = "";
   dialogOpen->InitialDir  = FixPath(AnsiString(getGamePath() + PATH_SFX));
   dialogOpen->Title       = "Select infantry annoyed sound";
   dialogOpen->FilterIndex = 2;
   if (dialogOpen->Execute())
      listInfAnn->Items->Strings[listInfAnn->ItemIndex] = ExtractFileName(dialogOpen->FileName);
}
//---------------------------------------------------------------------------

void __fastcall TFormMain::listInfRespDblClick(TObject *Sender)
{
   dialogOpen->FileName    = "";
   dialogOpen->InitialDir  = FixPath(AnsiString(getGamePath() + PATH_SFX));
   dialogOpen->Title       = "Select infantry response sound";
   dialogOpen->FilterIndex = 2;
   if (dialogOpen->Execute())
      listInfResp->Items->Strings[listInfResp->ItemIndex] = ExtractFileName(dialogOpen->FileName);
}
//---------------------------------------------------------------------------

void __fastcall TFormMain::listInfIdleDblClick(TObject *Sender)
{
   dialogOpen->FileName    = "";
   dialogOpen->InitialDir  = FixPath(AnsiString(getGamePath() + PATH_SFX));
   dialogOpen->Title       = "Select infantry on idle sound";
   dialogOpen->FilterIndex = 2;
   if (dialogOpen->Execute())
      listInfIdle->Items->Strings[listInfIdle->ItemIndex] = ExtractFileName(dialogOpen->FileName);
}
//---------------------------------------------------------------------------

void __fastcall TFormMain::listInfHitDblClick(TObject *Sender)
{
   dialogOpen->FileName    = "";
   dialogOpen->InitialDir  = FixPath(AnsiString(getGamePath() + PATH_SFX));
   dialogOpen->Title       = "Select infantry on hit sound";
   dialogOpen->FilterIndex = 2;
   if (dialogOpen->Execute())
      listInfHit->Items->Strings[listInfHit->ItemIndex] = ExtractFileName(dialogOpen->FileName);
}
//---------------------------------------------------------------------------

void __fastcall TFormMain::listInfKillDblClick(TObject *Sender)
{
   dialogOpen->FileName    = "";
   dialogOpen->InitialDir  = FixPath(AnsiString(getGamePath() + PATH_SFX));
   dialogOpen->Title       = "Select infantry on killing sound";
   dialogOpen->FilterIndex = 2;
   if (dialogOpen->Execute())
      listInfKill->Items->Strings[listInfKill->ItemIndex] = ExtractFileName(dialogOpen->FileName);
}
//---------------------------------------------------------------------------

void __fastcall TFormMain::listInfDieDblClick(TObject *Sender)
{
   dialogOpen->FileName    = "";
   dialogOpen->InitialDir  = FixPath(AnsiString(getGamePath() + PATH_SFX));
   dialogOpen->Title       = "Select infantry on dying sound";
   dialogOpen->FilterIndex = 2;
   if (dialogOpen->Execute())
      listInfDie->Items->Strings[listInfDie->ItemIndex] = ExtractFileName(dialogOpen->FileName);
}
//---------------------------------------------------------------------------
/*---------------------------------------------------------------------------
INFANTRY AREA PUBLISHED - over
---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------
VEHICLE AREA PUBLISHED
---------------------------------------------------------------------------*/

void __fastcall TFormMain::btnVehNewClick(TObject *Sender)
{
   newVeh();
}
//---------------------------------------------------------------------------

void __fastcall TFormMain::btnVehSaveClick(TObject *Sender)
{
   saveVeh(updownVeh->Position);
}
//---------------------------------------------------------------------------

void __fastcall TFormMain::btnVehDeleteClick(TObject *Sender)
{
   deleteVeh(updownVeh->Position);
}
//---------------------------------------------------------------------------

void __fastcall TFormMain::btnVehDeleteAllClick(TObject *Sender)
{
   if (Application->MessageBox(L"This will erase all records in AIVEHICLE.DAT. Continue ?", L"Warning", MB_YESNO)==IDYES)
      deleteAllVeh();
}
//---------------------------------------------------------------------------

/*---------------------------------------------------------------------------
 smecherie: am atasat de un updown un edit care nu se vede
            si asa pot sa vad cand s-a scimbat :)
---------------------------------------------------------------------------*/
void __fastcall TFormMain::editCurrentVehChange(TObject *Sender)
{
   int tmp = editCurrentVeh->Text.ToInt();
   loadVeh(tmp);
}
//---------------------------------------------------------------------------

void __fastcall TFormMain::comboCurrentVehChange(TObject *Sender)
{
   loadVeh(comboCurrentVeh->ItemIndex);
}
//---------------------------------------------------------------------------

void __fastcall TFormMain::btnVehLoadSpriteClick(TObject *Sender)
{
   dialogOpen->FileName    = "";
   dialogOpen->InitialDir  = FixPath(AnsiString(getGamePath().c_str()) + PATH_VEHICLES_GFX);
   dialogOpen->Title       = "Select the vehicle sprite";
   dialogOpen->FilterIndex = 1;
   if (dialogOpen->Execute())
   {
      editVehSprite->Text = ExtractFileName(dialogOpen->FileName);
   }
}
//---------------------------------------------------------------------------

void __fastcall TFormMain::listVehAckDblClick(TObject *Sender)
{
   dialogOpen->FileName    = "";
   dialogOpen->InitialDir  = FixPath(AnsiString(getGamePath() + PATH_SFX));
   dialogOpen->Title       = "Select vehicle acknowledged sound";
   dialogOpen->FilterIndex = 2;
   if (dialogOpen->Execute())
   {
      listVehAck->Items->Strings[listVehAck->ItemIndex] = ExtractFileName(dialogOpen->FileName);
   }
}
//---------------------------------------------------------------------------

void __fastcall TFormMain::listVehAnnDblClick(TObject *Sender)
{
   dialogOpen->FileName    = "";
   dialogOpen->InitialDir  = FixPath(AnsiString(getGamePath() + PATH_SFX));
   dialogOpen->Title       = "Select vehicle annoyed sound";
   dialogOpen->FilterIndex = 2;
   if (dialogOpen->Execute())
   {
      listVehAnn->Items->Strings[listVehAnn->ItemIndex] = ExtractFileName(dialogOpen->FileName);
   }
}
//---------------------------------------------------------------------------

void __fastcall TFormMain::listVehRespDblClick(TObject *Sender)
{
   dialogOpen->FileName    = "";
   dialogOpen->InitialDir  = FixPath(AnsiString(getGamePath() + PATH_SFX));
   dialogOpen->Title       = "Select vehicle response sound";
   dialogOpen->FilterIndex = 2;
   if (dialogOpen->Execute())
   {
      listVehResp->Items->Strings[listVehResp->ItemIndex] = ExtractFileName(dialogOpen->FileName);
   }
}
//---------------------------------------------------------------------------

void __fastcall TFormMain::listVehIdleDblClick(TObject *Sender)
{
   dialogOpen->FileName    = "";
   dialogOpen->InitialDir  = FixPath(AnsiString(getGamePath() + PATH_SFX));
   dialogOpen->Title       = "Select vehicle on idle sound";
   dialogOpen->FilterIndex = 2;
   if (dialogOpen->Execute())
   {
      listVehIdle->Items->Strings[listVehIdle->ItemIndex] = ExtractFileName(dialogOpen->FileName);
   }
}
//---------------------------------------------------------------------------

void __fastcall TFormMain::listVehHitDblClick(TObject *Sender)
{
   dialogOpen->FileName    = "";
   dialogOpen->InitialDir  = FixPath(AnsiString(getGamePath() + PATH_SFX));
   dialogOpen->Title       = "Select vehicle on hit sound";
   dialogOpen->FilterIndex = 2;
   if (dialogOpen->Execute())
   {
      listVehHit->Items->Strings[listVehHit->ItemIndex] = ExtractFileName(dialogOpen->FileName);
   }
}
//---------------------------------------------------------------------------

void __fastcall TFormMain::listVehKillDblClick(TObject *Sender)
{
   dialogOpen->FileName    = "";
   dialogOpen->InitialDir  = FixPath(AnsiString(getGamePath() + PATH_SFX));
   dialogOpen->Title       = "Select vehicle on kill sound";
   dialogOpen->FilterIndex = 2;
   if (dialogOpen->Execute())
   {
      listVehKill->Items->Strings[listVehKill->ItemIndex] = ExtractFileName(dialogOpen->FileName);
   }
}
//---------------------------------------------------------------------------

void __fastcall TFormMain::listVehDieDblClick(TObject *Sender)
{
   dialogOpen->FileName    = "";
   dialogOpen->InitialDir  = FixPath(AnsiString(getGamePath() + PATH_SFX));
   dialogOpen->Title       = "Select vehicle on die sound";
   dialogOpen->FilterIndex = 2;
   if (dialogOpen->Execute())
   {
      listVehDie->Items->Strings[listVehDie->ItemIndex] = ExtractFileName(dialogOpen->FileName);
   }
}
//---------------------------------------------------------------------------

void __fastcall TFormMain::btnVehSkeletonClick(TObject *Sender)
{
  // go and edit the vehicle skeleton
  AnsiString fileName = FixPath(AnsiString(getGamePath().c_str()) + AnsiString(PATH_VEHICLES_GFX) + AnsiString(recVeh->sprite));

  if (!FileExists(fileName))
  {
    return;
  }

  switch(recVeh->vType)
  {
    case 0:
    case 1:
    case 2:
    case 3:
      try
      {
        VSkelEditor->SetSprite(fileName);
      }
      catch(...)
      {
        // exceptie, nu pot afisa forma de editare a skeletului
        return;
      }

      VSkelEditor->SetVehicleRecord(recVeh);
      VSkelEditor->ShowModal();
      break;


    case 4:
    case 5:
    case 6:
    case 7:
      try
      {
        AirSkelEditor->SetSprite(fileName);
        //AirSkelEditor->Sprite->LoadFromFile(fileName);
      }
      catch(...)
      {
        // exceptie, nu pot afisa forma de editare a skeletului
        return;
      }

      AirSkelEditor->SetVehicleRecord(recVeh);
      AirSkelEditor->ShowModal();
      break;
   }
}
//---------------------------------------------------------------------------

void __fastcall TFormMain::listVehAckKeyDown(TObject *Sender, WORD &Key,
      TShiftState Shift)
{
  if ((Key == VK_DELETE) && (listVehAck->ItemIndex >= 0))
  {
    listVehAck->Items->Strings[listVehAck->ItemIndex] = "";
  }
}
//---------------------------------------------------------------------------

void __fastcall TFormMain::listVehAnnKeyDown(TObject *Sender, WORD &Key,
      TShiftState Shift)
{
  if ((Key == VK_DELETE) && (listVehAnn->ItemIndex >= 0))
  {
    listVehAnn->Items->Strings[listVehAnn->ItemIndex] = "";
  }
}
//---------------------------------------------------------------------------

void __fastcall TFormMain::listVehRespKeyDown(TObject *Sender, WORD &Key,
      TShiftState Shift)
{
  if ((Key == VK_DELETE) && (listVehResp->ItemIndex >= 0))
  {
    listVehResp->Items->Strings[listVehResp->ItemIndex] = "";
  }
}
//---------------------------------------------------------------------------

void __fastcall TFormMain::listVehIdleKeyDown(TObject *Sender, WORD &Key,
      TShiftState Shift)
{
  if ((Key == VK_DELETE) && (listVehIdle->ItemIndex >= 0))
  {
    listVehIdle->Items->Strings[listVehIdle->ItemIndex] = "";
  }
}
//---------------------------------------------------------------------------

void __fastcall TFormMain::listVehHitKeyDown(TObject *Sender, WORD &Key,
      TShiftState Shift)
{
  if ((Key == VK_DELETE) && (listVehHit->ItemIndex >= 0))
  {
    listVehHit->Items->Strings[listVehHit->ItemIndex] = "";
  }
}
//---------------------------------------------------------------------------

void __fastcall TFormMain::listVehKillKeyDown(TObject *Sender, WORD &Key,
      TShiftState Shift)
{
  if ((Key == VK_DELETE) && (listVehKill->ItemIndex >= 0))
  {
    listVehKill->Items->Strings[listVehKill->ItemIndex] = "";
  }
}
//---------------------------------------------------------------------------

void __fastcall TFormMain::listVehDieKeyDown(TObject *Sender, WORD &Key,
      TShiftState Shift)
{
  if ((Key == VK_DELETE) && (listVehDie->ItemIndex >= 0))
  {
    listVehDie->Items->Strings[listVehDie->ItemIndex] = "";
  }
}
//---------------------------------------------------------------------------

void __fastcall TFormMain::comboVehTypeChange(TObject *Sender)
{
  /*
  if (comboVehType->ItemIndex == 4)
  {
    btnVehSkeleton->Enabled = false;
  }
  else
  {
    btnVehSkeleton->Enabled = true;
  }
  */
  recVeh->vType = comboVehType->ItemIndex;
}
//---------------------------------------------------------------------------

void __fastcall TFormMain::btnVehLoadW1FxClick(TObject *Sender)
{
   dialogOpen->FileName    = "";
   dialogOpen->InitialDir  = FixPath(AnsiString(getGamePath().c_str()) + PATH_EXPLODES_GFX);
   dialogOpen->Title       = "Select the vehicle first weapon effect sprite";
   dialogOpen->FilterIndex = 1;
   if (dialogOpen->Execute())
   {
      editVehW1Fx->Text = ExtractFileName(dialogOpen->FileName);
   }
}
//---------------------------------------------------------------------------

void __fastcall TFormMain::btnVehLoadW1ProjClick(TObject *Sender)
{
   dialogOpen->FileName    = "";
   dialogOpen->InitialDir  = FixPath(AnsiString(getGamePath().c_str()) + PATH_EXPLODES_GFX);
   dialogOpen->Title       = "Select the vehicle first weapon projectile sprite";
   dialogOpen->FilterIndex = 1;
   if (dialogOpen->Execute())
   {
      editVehW1Proj->Text = ExtractFileName(dialogOpen->FileName);
   }
}
//---------------------------------------------------------------------------

void __fastcall TFormMain::btnLoadVehW1AFxClick(TObject *Sender)
{
   dialogOpen->FileName    = "";
   dialogOpen->InitialDir  = FixPath(AnsiString(getGamePath().c_str()) + PATH_EXPLODES_GFX);
   dialogOpen->Title       = "Select the vehicle first weapon after effect sprite";
   dialogOpen->FilterIndex = 1;
   if (dialogOpen->Execute())
   {
      editVehW1AFx->Text = ExtractFileName(dialogOpen->FileName);
   }
}
//---------------------------------------------------------------------------

void __fastcall TFormMain::btnVehLoadW1HitClick(TObject *Sender)
{
   dialogOpen->FileName    = "";
   dialogOpen->InitialDir  = FixPath(AnsiString(getGamePath() + PATH_SFX));
   dialogOpen->Title       = "Select the vehicle first weapon on hit sound";
   dialogOpen->FilterIndex = 2;
   if (dialogOpen->Execute())
   {
      editVehW1Hit->Text = ExtractFileName(dialogOpen->FileName);
   }
}
//---------------------------------------------------------------------------

void __fastcall TFormMain::btnVehLoadW1FireClick(TObject *Sender)
{
   dialogOpen->FileName    = "";
   dialogOpen->InitialDir  = FixPath(AnsiString(getGamePath() + PATH_SFX));
   dialogOpen->Title       = "Select the vehicle first weapon on fire sound";
   dialogOpen->FilterIndex = 2;
   if (dialogOpen->Execute())
   {
      editVehW1Fire->Text = ExtractFileName(dialogOpen->FileName);
   }
}
//---------------------------------------------------------------------------

void __fastcall TFormMain::btnVehLoadPortraitClick(TObject *Sender)
{
   dialogOpen->FileName    = "";
   dialogOpen->InitialDir  = FixPath(AnsiString(getGamePath() + PATH_PORTRAITS_GFX));
   dialogOpen->Title       = "Select the vehicle portrait";
   dialogOpen->FilterIndex = 1;
   if (dialogOpen->Execute())
   {
      editVehPortrait->Text = ExtractFileName(dialogOpen->FileName);
   }
}
//---------------------------------------------------------------------------

void __fastcall TFormMain::btnInfLoadPortraitClick(TObject *Sender)
{
   dialogOpen->FileName    = "";
   dialogOpen->InitialDir  = FixPath(AnsiString(getGamePath().c_str()) + PATH_PORTRAITS_GFX); //PATH_GFXINFANTRY;
   dialogOpen->Title       = "Select infantry portrait";
   dialogOpen->FilterIndex = 1;
   
   if (dialogOpen->Execute())
   {
     editInfPortrait->Text = ExtractFileName(dialogOpen->FileName);
   }
}
//---------------------------------------------------------------------------

void __fastcall TFormMain::btnVLoadExplodeClick(TObject *Sender)
{
   dialogOpen->FileName    = "";
   dialogOpen->InitialDir  = FixPath(AnsiString(getGamePath().c_str()) + PATH_EXPLODES_GFX);
   dialogOpen->Title       = "Select the exploding sprite";
   dialogOpen->FilterIndex = 1;

   if (dialogOpen->Execute())
   {
      edVExplodeSprite->Text = ExtractFileName(dialogOpen->FileName);
   }
}
//---------------------------------------------------------------------------

AnsiString TFormMain::FixPath(AnsiString _value)
{
  for(int i = 0; i < _value.Length(); i++)
  {
    if (_value[i + 1] == '/')
    {
      _value[i + 1] = '\\';
    }
  }

  return _value;
}
//---------------------------------------------------------------------------

void __fastcall TFormMain::btnInfSkelClick(TObject *Sender)
{
  // go and edit the vehicle skeleton
  AnsiString fileName = FixPath(AnsiString(getGamePath().c_str()) + AnsiString(PATH_INFANTRY_GFX) + AnsiString(recInf->sprite));

  if (!FileExists(fileName))
  {
    return;
  }

  try
  {
    InfSkelEditor->SetSprite(fileName);
  }
  catch(...)
  {
    // exceptie, nu pot afisa forma de editare a skeletului
    return;
  }

  InfSkelEditor->SetInfRecord(recInf);
  InfSkelEditor->ShowModal();
}
//---------------------------------------------------------------------------

void __fastcall TFormMain::btnInfLoadMiniPortraitClick(TObject *Sender)
{
   dialogOpen->FileName    = "";
   dialogOpen->InitialDir  = FixPath(AnsiString(getGamePath().c_str()) + PATH_PORTRAITS_GFX); //PATH_GFXINFANTRY;
   dialogOpen->Title       = "Select infantry mini portrait";
   dialogOpen->FilterIndex = 1;

   if (dialogOpen->Execute())
   {
     editInfMiniPortrait->Text = ExtractFileName(dialogOpen->FileName);
   }
}
//---------------------------------------------------------------------------

