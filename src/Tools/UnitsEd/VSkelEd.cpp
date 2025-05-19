//---------------------------------------------------------------------------
#include <vcl.h>
#include <math.h>
#pragma hdrstop

#include "VSkelEd.h"
#include "FObservationPointEd.h"
//---------------------------------------------------------------------------

#pragma package(smart_init)
#pragma link "CSPIN"
#pragma resource "*.dfm"
TVSkelEditor *VSkelEditor;
//---------------------------------------------------------------------------

__fastcall TVSkelEditor::TVSkelEditor(TComponent* Owner)
        : TForm(Owner)
{
  turretCellNo = 0;
  bodyCellNo   = 0;
  DivideFactor = 4;
}
//---------------------------------------------------------------------------
void __fastcall TVSkelEditor::FormClose(TObject *Sender,
      TCloseAction &Action)
{
    //SkelEditor->Visible = false;
    //SkelEditor->Enabled = false;
    //MainForm->Visible   = true;
    //MainForm->Enabled   = true;
    //delete Sprite;
}
//---------------------------------------------------------------------------

void __fastcall TVSkelEditor::img_TurretMouseDown(TObject *Sender,
      TMouseButton Button, TShiftState Shift, int X, int Y)
{
  switch(rg_tpoints->ItemIndex)
  {
    case 0 :
      tempV->turret[turretCellNo].x = X;
      tempV->turret[turretCellNo].y = Y;

      //DrawBodyAndTurret();
      break;
    case 1 :
      tempV->turretLightsNo++;
      tempV->turretLightsNo %= 4;
      tempV->turretLights[tempV->turretLightsNo].x = X;
      tempV->turretLights[tempV->turretLightsNo].y = Y;
      break;
    case 2:
      tempV->primary[turretCellNo].x = X;
      tempV->primary[turretCellNo].y = Y;
      break;
    case 3:
      tempV->secondary[turretCellNo].x = X;
      tempV->secondary[turretCellNo].y = Y;
      break;
  }

  DrawBodyAndTurret();
}
//---------------------------------------------------------------------------
void __fastcall TVSkelEditor::FormShow(TObject *Sender)
{
  //if (tempV->vType == 2) DivideFactor = 2; // if the vehicle is a transporter
  //else
  DivideFactor = 4; // all vehicles have the same sprite format
  
  img_turret->Bitmap->Width = Sprite->Width / 8;
  img_turret->Bitmap->Height = Sprite->Height / DivideFactor;

  img_body->Bitmap->Width = Sprite->Width / 8;
  img_body->Bitmap->Height = Sprite->Height / DivideFactor;

  img_ground->Bitmap->Width  = 2 * img_body->Bitmap->Width;
  img_ground->Width          = 2 * img_body->Bitmap->Width;
  img_ground->Bitmap->Height = 2 * img_body->Bitmap->Height;
  img_ground->Height         = 2 * img_body->Bitmap->Height;
  img_ground->Bitmap->Canvas->Brush->Color = clBlack;

  // deseneaza corpul tancului din Sprite
  rs.Left   = 0;
  rs.Top    = 0;
  rs.Right  = img_body->Bitmap->Width;
  rs.Bottom = img_body->Bitmap->Height;
  rd        = rs;
  img_body->Bitmap->Canvas->CopyRect(rd, Sprite->Canvas, rs);
  //img_body->Paint();

  // deseneaza turela
  rd = rs;
  rs.Top = img_body->Bitmap->Height + 1;
  rs.Bottom = 2*img_body->Bitmap->Height;
  img_turret->Bitmap->Canvas->CopyRect(rd, Sprite->Canvas, rs);
  //img_turret->Paint();

  DrawBodyAndTurret();
}
//---------------------------------------------------------------------------
void __fastcall TVSkelEditor::img_bodyMouseDown(TObject *Sender,
      TMouseButton Button, TShiftState Shift, int X, int Y)
{
  switch(rg_bpoints->ItemIndex)
  {
    case 0 :
      tempV->body[bodyCellNo].x = X;
      tempV->body[bodyCellNo].y = Y;

      DrawBodyAndTurret();
      break;

    case 1 :
      tempV->bodyLightsNo++;
      tempV->bodyLightsNo %= 4;
      tempV->bodyLights[tempV->bodyLightsNo].x = X;
      tempV->bodyLights[tempV->bodyLightsNo].y = Y;
      break;

    case 2 :
      if (cbVisual->Items->Count > 0)
      {
        tempV->mObsPoints[cbVisual->ItemIndex].mMountPoints[bodyCellNo] = CPPoint(X, Y);
      }  
      break;
  }
  
  DrawBodyAndTurret();
}
//---------------------------------------------------------------------------
void __fastcall TVSkelEditor::CSpinButton1DownClick(TObject *Sender)
{
  turretCellNo--;
  //if (turretCellNo < 0) turretCellNo = 15;
  if (turretCellNo < 0) turretCellNo = 7;

  // deseneaza noua turela^
  rd.Left = 0;
  rd.Top = 0;
  rd.Right = Sprite->Width / 8;
  rd.Bottom = Sprite->Height / DivideFactor;

  // 8 directions
  rs.Left = (turretCellNo) * (Sprite->Width / 8);
  rs.Right = (turretCellNo + 1) * (Sprite->Width / 8);

  // 16 directions
  //rs.Left = (turretCellNo / 2) * (Sprite->Width / 8);
  //rs.Right = (turretCellNo / 2 + 1) * (Sprite->Width / 8);

  // 8 directions
  rs.Top = Sprite->Height / DivideFactor;
  rs.Bottom = 2*(Sprite->Height / DivideFactor) - 1;
  img_turret->Bitmap->Canvas->CopyRect(rd, Sprite->Canvas, rs);

  // 16 directions
  /*
  if (turretCellNo % 2 == 0)
  {
    rs.Top = Sprite->Height / DivideFactor;
    rs.Bottom = 2*(Sprite->Height / DivideFactor) - 1;
    img_turret->/Bitmap->Canvas->CopyRect(rd, Sprite->Canvas, rs);
  }
  else
  {
    rs.Top = 2*(Sprite->Height / DivideFactor);
    rs.Bottom = 3*(Sprite->Height / DivideFactor) - 1;
    img_turret->Bitmap->Canvas->CopyRect(rd, Sprite->Canvas, rs);
  }
  */

  // refac calculele pentru turret.x si turret.y
  // turret.x = abs(img_turret->Width/2 - tempV->turret.x) * sin(3.1415 * (180.0-(float)turretCellNo*45.0) / 180.0);
  // turret.y = abs(img_turret->Height/2 - tempV->turret.y) * cos(3.1415 * (180.0-(float)turretCellNo*45.0) / 180.0);

  DrawBodyAndTurret();
}
//---------------------------------------------------------------------------
void __fastcall TVSkelEditor::CSpinButton1UpClick(TObject *Sender)
{
  turretCellNo++;
//  turretCellNo %= 16;
  turretCellNo %= 8;

  // deseneaza noua turela^
  rd.Left = 0;
  rd.Top = 0;
  rd.Right = Sprite->Width / 8;
  rd.Bottom = Sprite->Height / DivideFactor;

  // 8 directions
  rs.Left = (turretCellNo) * (Sprite->Width / 8);
  rs.Right = (turretCellNo + 1) * (Sprite->Width / 8);

  // 16 directions
  // rs.Left = (turretCellNo / 2) * (Sprite->Width / 8);
  // rs.Right = (turretCellNo / 2+1) * (Sprite->Width / 8);

  // 8 directions
  rs.Top = Sprite->Height / DivideFactor;
  rs.Bottom = 2*(Sprite->Height / DivideFactor) - 1;
  img_turret->Bitmap->Canvas->CopyRect(rd, Sprite->Canvas, rs);

  // 16 directions
  /*
  if (turretCellNo % 2 == 0)
  {
    rs.Top = Sprite->Height / DivideFactor;
    rs.Bottom = 2*(Sprite->Height / DivideFactor) - 1;
    img_turret->Bitmap->Canvas->CopyRect(rd, Sprite->Canvas, rs);
  }
  else
  {
    rs.Top = 2*(Sprite->Height / DivideFactor);
    rs.Bottom = 3*(Sprite->Height / DivideFactor) - 1;
    img_turret->Bitmap->Canvas->CopyRect(rd, Sprite->Canvas, rs);
  }
  */

  //rs.Top = Sprite->Height / DivideFactor + 1;
  //rs.Bottom = 2*(Sprite->Height / DivideFactor);
  //img_turret->Picture->Bitmap->Canvas->CopyRect(rd, Sprite->Canvas, rs);

  // refac calculele pentru turret.x si turret.y
  // turret.x = abs(img_turret->Width/2 - tempV->turret.x) * sin(3.1415 * (180.0-(float)turretCellNo*45.0) / 180.0);
  // turret.y = abs(img_turret->Height/2 - tempV->turret.y) * cos(3.1415 * (180.0-(float)turretCellNo*45.0) / 180.0);

  DrawBodyAndTurret();
}
//---------------------------------------------------------------------------

void TVSkelEditor::DrawBodyAndTurret()
{
  // umple imaginea unde se fac testele
  rg = rd;
  rg.Right *= 2;
  rg.Bottom *= 2;
  img_ground->Bitmap->Canvas->Brush->Color = clBlack;
  img_ground->Bitmap->Canvas->FillRect(rg);

  // acum deseneaza body-ul si turela peste el
  //img_ground->Canvas->CopyMode = cmSrcCopy;
  img_ground->Bitmap->Canvas->Draw(img_ground->Width / 2 - img_body->Bitmap->Width / 2,
                           img_ground->Height / 2 - img_body->Bitmap->Height / 2,
                           img_body->Bitmap);

  img_turret->Transparent = true;
  img_turret->Bitmap->TransparentMode = tmAuto;
  img_ground->Bitmap->Canvas->Draw(img_ground->Width / 2 - img_body->Bitmap->Width / 2 + tempV->body[bodyCellNo].x  - tempV->turret[turretCellNo].x,
                           img_ground->Height / 2 - img_body->Bitmap->Height / 2 + tempV->body[bodyCellNo].y - tempV->turret[turretCellNo].y,
                           img_turret->Bitmap);
  img_turret->Transparent = false;

  // paint on the test ground the primary fire hole
  int lFireX = img_ground->Width / 2 - img_body->Bitmap->Width / 2 + tempV->body[bodyCellNo].x  - tempV->turret[turretCellNo].x + tempV->primary[turretCellNo].x;
  int lFireY = img_ground->Height / 2 - img_body->Bitmap->Height / 2 + tempV->body[bodyCellNo].y - tempV->turret[turretCellNo].y + tempV->primary[turretCellNo].y;

  img_ground->Bitmap->Canvas->Pen->Color = clGreen;
  img_ground->Bitmap->Canvas->MoveTo(lFireX - 2, lFireY);
  img_ground->Bitmap->Canvas->LineTo(lFireX + 2, lFireY);
  img_ground->Bitmap->Canvas->MoveTo(lFireX, lFireY - 2);
  img_ground->Bitmap->Canvas->LineTo(lFireX, lFireY + 2);

  // paint the current observation point
  if (tempV->mObsPointsCount > 0 && cbVisual->ItemIndex >= 0)
  {
    int lObsPointX = img_ground->Width / 2 - img_body->Bitmap->Width / 2 + tempV->mObsPoints[cbVisual->ItemIndex].mMountPoints[bodyCellNo].x;
    int lObsPointY = img_ground->Width / 2 - img_body->Bitmap->Width / 2 + tempV->mObsPoints[cbVisual->ItemIndex].mMountPoints[bodyCellNo].y;

    img_ground->Bitmap->Canvas->Pen->Color = clYellow;
    img_ground->Bitmap->Canvas->MoveTo(lObsPointX - 2, lObsPointY);
    img_ground->Bitmap->Canvas->LineTo(lObsPointX + 2, lObsPointY);
    img_ground->Bitmap->Canvas->MoveTo(lObsPointX    , lObsPointY - 2);
    img_ground->Bitmap->Canvas->LineTo(lObsPointX    , lObsPointY + 2);
  }

  img_ground->Paint();
}

void __fastcall TVSkelEditor::CSpinButton2UpClick(TObject *Sender)
{
  bodyCellNo++;
  bodyCellNo %= 8;

  // deseneaza noul corp
  rd.Left = 0;
  rd.Top = 0;
  rd.Right = Sprite->Width / 8;
  rd.Bottom = Sprite->Height / DivideFactor;

  rs.Left = bodyCellNo * (Sprite->Width / 8);
  rs.Right = (bodyCellNo+1) * (Sprite->Width / 8);
  rs.Top = 0;
  rs.Bottom = Sprite->Height / DivideFactor;
  img_body->Bitmap->Canvas->CopyRect(rd, Sprite->Canvas, rs);
  //Image1->Canvas->CopyRect(rd, Sprite->Canvas, rs);
  img_body->Paint();

  DrawBodyAndTurret();
}
//---------------------------------------------------------------------------

void __fastcall TVSkelEditor::CSpinButton2DownClick(TObject *Sender)
{
  bodyCellNo--;
  if (bodyCellNo < 0) bodyCellNo = 7;

  // deseneaza noul corp
  rd.Left = 0;
  rd.Top = 0;
  rd.Right = Sprite->Width / 8;
  rd.Bottom = Sprite->Height / DivideFactor;

  rs.Left = bodyCellNo * (Sprite->Width / 8);
  rs.Right = (bodyCellNo+1) * (Sprite->Width / 8);
  rs.Top = 0;
  rs.Bottom = Sprite->Height / DivideFactor;
  img_body->Bitmap->Canvas->CopyRect(rd, Sprite->Canvas, rs);
  //Image1->Canvas->CopyRect(rd, Sprite->Canvas, rs);
  img_body->Paint();

  DrawBodyAndTurret();
}
//---------------------------------------------------------------------------

void TVSkelEditor::SetVehicleRecord(hVehicleUnified _vehicle)
{
  tempV = _vehicle;

  DataToScreen();
}
//---------------------------------------------------------------------------

void __fastcall TVSkelEditor::FormCreate(TObject *Sender)
{
  try
  {
    Sprite = new TPngImage();
    //Sprite->Width = 16;
    //Sprite->Height = 16;
  }
  catch(...)
  {
    Sprite = NULL;
  }
}
//---------------------------------------------------------------------------

void TVSkelEditor::SetSprite(AnsiString _fileName)
{
  delete Sprite;
  
  try
  {
    Sprite = new TPngImage();
    //Sprite->Width = 16;
    //Sprite->Height = 16;
  }
  catch(...)
  {
    Sprite = NULL;
  }

  Sprite->LoadFromFile(_fileName);
}
//---------------------------------------------------------------------------

void TVSkelEditor::DataToScreen()
{
  cbVisual->Clear();

  for(int i = 0; i < tempV->mObsPointsCount; i++)
  {
    cbVisual->AddItem(AnsiString(tempV->mObsPoints[i].mName), 0);
  }

  if (cbVisual->Items->Count > 0)
  {
    cbVisual->ItemIndex = 0;
  }
  else
  {
    cbVisual->ItemIndex = -1;
  }
}
//---------------------------------------------------------------------------

void __fastcall TVSkelEditor::btnRemoveVisualClick(TObject *Sender)
{
  if (tempV->mObsPointsCount <= 0)
  {
    return;
  }

  // better safe than sorry ;)
  if (cbVisual->Items->Count <= 0 || cbVisual->Items->Count > tempV->mObsPointsCount)
  {
    return;
  }

  if (cbVisual->ItemIndex < 0)
  {
    return;
  }

  for(int i = cbVisual->ItemIndex; i < tempV->mObsPointsCount; i++)
  {
    tempV->mObsPoints[i] = tempV->mObsPoints[i + 1];
  }

  tempV->mObsPointsCount--;

  DataToScreen();
}
//---------------------------------------------------------------------------


void __fastcall TVSkelEditor::btnEditVisualClick(TObject *Sender)
{
  if (tempV->mObsPointsCount <= 0)
  {
    return;
  }

  // better safe than sorry ;)
  if (cbVisual->Items->Count <= 0 || cbVisual->Items->Count > tempV->mObsPointsCount)
  {
    return;
  }

  if (cbVisual->ItemIndex < 0)
  {
    return;
  }

  FormObservationPointEditor->SetData(&(tempV->mObsPoints[cbVisual->ItemIndex]));
  FormObservationPointEditor->ShowModal();

  DataToScreen();
}
//---------------------------------------------------------------------------

void __fastcall TVSkelEditor::btnAddVisualClick(TObject *Sender)
{
  if (tempV->mObsPointsCount >= VEHICLE_MAXVISUAL)
  {
    return;
  }

  strcpy(tempV->mObsPoints[tempV->mObsPointsCount].mName, "Observation Point");
  tempV->mObsPoints[tempV->mObsPointsCount].mMinScanRange = 6;
  //tempV->mObsPoints[tempV->mObsPointsCount].mScanRange    = 6;
  tempV->mObsPoints[tempV->mObsPointsCount].mMaxScanRange = 10;
  tempV->mObsPoints[tempV->mObsPointsCount].mFOV          = 90;
  tempV->mObsPoints[tempV->mObsPointsCount].mScanAngle    = 0;
  tempV->mObsPoints[tempV->mObsPointsCount].mActive       = true;

  for(int j = 0; j < 8; j++)
  {
    tempV->mObsPoints[tempV->mObsPointsCount].mMountPoints[j] = CPPoint(0, 0);
  }

  tempV->mObsPointsCount++;

  DataToScreen();
}
//---------------------------------------------------------------------------

