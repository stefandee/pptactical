//---------------------------------------------------------------------------

#include <vcl.h>
#include <math.h>
#pragma hdrstop

#include "FPolitics.h"
#include "FMain.h"
#include "PPIG_Doctrine.h"
#include "PPIG_BattleGroup.h"
#include "PPIG_Map.h"
#include "PPIG_Mission.h"
#include "PPIG_BGManager.h"

#include <algorithm>
using namespace std;
//---------------------------------------------------------------------------

#pragma package(smart_init)
#pragma resource "*.dfm"
TFormPolitics *FormPolitics;
//---------------------------------------------------------------------------
__fastcall TFormPolitics::TFormPolitics(TComponent* Owner)
        : TForm(Owner)
{
  cbStyle->ItemIndex = 0;
  cbLinks->ItemIndex = 0;
  cbDraw->ItemIndex  = 0;
  mStartBg  = -1;
  mSelected = 1;
}
//---------------------------------------------------------------------------

void __fastcall TFormPolitics::pbScreenPaint(TObject *Sender)
{
  DrawRelations();
  DrawBGroups();
}
//---------------------------------------------------------------------------

int  TFormPolitics::GetBGX(int _bg)
{
  int lNo = GetMissionInstance()->GetBGManager()->BGroupCount();
  float lAngle = (2.0 * M_PI) / lNo;
  int lMidX = pbScreen->Width / 2;
  int lRadius = min(pbScreen->Width / 2, pbScreen->Height / 2) - 30;

  return lMidX + lRadius * sin(lAngle * _bg);
}
//---------------------------------------------------------------------------

int  TFormPolitics::GetBGY(int _bg)
{
  int lNo = GetMissionInstance()->GetBGManager()->BGroupCount();
  float lAngle = (2.0 * M_PI) / lNo;
  int lMidY = pbScreen->Height / 2;
  int lRadius = min(pbScreen->Width / 2, pbScreen->Height / 2) - 30;

  return lMidY + lRadius * cos(lAngle * _bg);
}
//---------------------------------------------------------------------------

int  TFormPolitics::BGroupAtPos(int _x, int _y)
{
  int lX, lY;

  // this is raw, but I'm too lazy and I want to reuse
  for(int i = 0; i < GetMissionInstance()->GetBGManager()->BGroupCount(); i++)
  {
    lX = _x - GetBGX(i);
    lY = _y - GetBGY(i);

    if (lX * lX + lY * lY < 30 * 30)
    {
      return GetMissionInstance()->GetBGManager()->GetBGroupByIndex(i)->GetId();
    }
  }

  return -1;
}
//---------------------------------------------------------------------------

void TFormPolitics::DrawRelations()
{
  int lNo = GetMissionInstance()->GetBGManager()->BGroupCount();
  int lXS, lYS, lXE, lYE;
  TColor lColor;
  bool   lDrawEnabled;
  TPoliticsRelations lRel;

  // draw relations links
  for(int i = 0; i < lNo; i++)
  {
    lXS = GetBGX(i);
    lYS = GetBGY(i);

    for(int j = 0; j < lNo; j++)
    {
      if (j == i)
      {
        continue;
      }

      lXE = GetBGX(j);
      lYE = GetBGY(j);

      lRel =  GetMissionInstance()->GetBGManager()->GetRelation(
              GetMissionInstance()->GetBGManager()->GetBGroupByIndex(i)->GetId(),
              GetMissionInstance()->GetBGManager()->GetBGroupByIndex(j)->GetId());


      lDrawEnabled = false;
      if (cbLinks->ItemIndex == 0)
      {
        lDrawEnabled = true;
      }

      if (cbLinks->ItemIndex == 4)
      {
        if (GetMissionInstance()->GetBGManager()->GetBGroupByIndex(i)->GetId() == mSelected)
        {
          lDrawEnabled = true;
        }
      }

      switch (lRel)
      {
         case REL_NEUTRAL:
            lColor = (TColor)0x008f8f8f;

            if (cbLinks->ItemIndex == 1)
            {
              lDrawEnabled = true;
            }
            break;

         case REL_OWNED:
            lColor = (TColor)0x008f1010;
            break;

         case REL_FRIENDLY:
            lColor = (TColor)0x008F8F10;

            if (cbLinks->ItemIndex == 3)
            {
              lDrawEnabled = true;
            }
            break;

         case REL_ENEMY:
            lColor = (TColor)0x0010108f;

            if (cbLinks->ItemIndex == 2)
            {
              lDrawEnabled = true;
            }
            break;
      }

      // no other option available
      if (lDrawEnabled)
      {
        switch(cbStyle->ItemIndex)
        {
          case 0:
            DrawAsGraph1(lXS, lYS, lXE, lYE, 8.0, lColor);
            break;

          case 1:
            DrawAsGraph2(lXS, lYS, lXE, lYE, 60.0, lColor);
            break;
        }
      }
    }
  }
}
//---------------------------------------------------------------------------

void TFormPolitics::DrawAsGraph1(int _xs, int _ys, int _xe, int _ye, int _dist, TColor _color)
{
  pbScreen->Canvas->Pen->Color = _color;
  pbScreen->Canvas->Pen->Mode  = pmCopy;
  pbScreen->Canvas->MoveTo(_xs, _ys);
  pbScreen->Canvas->LineTo((_xe + _xs) / 2.0, (_ys + _ye) / 2.0);
  DrawArrow(_xs, _ys, (_xe + _xs) / 2.0, (_ye + _ys) / 2.0, 30.0, _dist, _color);
  DrawArrow((_xe + _xs) / 2.0, (_ye + _ys) / 2.0, _xe, _ye, -30.0, _dist, _color);
}
//---------------------------------------------------------------------------

void TFormPolitics::DrawAsGraph2(int _xs, int _ys, int _xe, int _ye, int _dist, TColor _color)
{
  float lXn = _xs - _xe, lYn = _ys - _ye, lD, lX1, lY1, lX2, lY2;

  float lAngle = 90.0 * M_PI / 180.0;

  lD  = sqrt(lXn * lXn + lYn * lYn);

  if (lD == 0)
  {
    return;
  }

  lXn = _dist * lXn / lD;
  lYn = _dist * lYn / lD;

  lX1 = lXn * cos(lAngle) - lYn * sin(lAngle);
  lY1 = lXn * sin(lAngle) + lYn * cos(lAngle);
  lX1 += _xs;
  lY1 += _ys;

  lX2 = lXn * cos(lAngle) - lYn * sin(lAngle);
  lY2 = lXn * sin(lAngle) + lYn * cos(lAngle);
  lX2 += _xe;
  lY2 += _ye;

  pbScreen->Canvas->Pen->Color = _color;
  pbScreen->Canvas->Pen->Mode  = pmCopy;

  Types::TPoint lPoints[4];
  lPoints[0].x = _xs; lPoints[0].y = _ys;
  lPoints[1].x = lX1; lPoints[1].y = lY1;
  lPoints[2].x = lX2; lPoints[2].y = lY2;
  lPoints[3].x = _xe; lPoints[3].y = _ye;

  pbScreen->Canvas->PolyBezier(lPoints, 3);
}
//---------------------------------------------------------------------------

void TFormPolitics::DrawArrow(int _xs, int _ys, int _xe, int _ye, float _angle, float _dist, TColor _color)
{
  float lXn = _xs - _xe, lYn = _ys - _ye, lD, lX1, lY1, lX2, lY2;
  float lAngle = _angle * M_PI / 180.0;

  lD  = sqrt(lXn * lXn + lYn * lYn);
  
  if (lD == 0)
  {
    return;
  }

  lXn = _dist * lXn / lD;
  lYn = _dist * lYn / lD;

  lX1 = lXn * cos(lAngle) - lYn * sin(lAngle);
  lY1 = lXn * sin(lAngle) + lYn * cos(lAngle);
  lX1 += _xe;
  lY1 += _ye;

  lX2 = lXn * cos(-lAngle) - lYn * sin(-lAngle);
  lY2 = lXn * sin(-lAngle) + lYn * cos(-lAngle);
  lX2 += _xe;
  lY2 += _ye;
  
  pbScreen->Canvas->Pen->Color = _color;
  pbScreen->Canvas->Pen->Mode  = pmCopy;
  
  pbScreen->Canvas->MoveTo(_xe, _ye);
  pbScreen->Canvas->LineTo(lX1, lY1);

  pbScreen->Canvas->MoveTo(_xe, _ye);
  pbScreen->Canvas->LineTo(lX2, lY2);
}
//---------------------------------------------------------------------------

void TFormPolitics::DrawBGroups()
{
  int lNo = GetMissionInstance()->GetBGManager()->BGroupCount(), lX, lY;
  AnsiString lCaption;

  pbScreen->Canvas->Pen->Color   = (TColor)0xD00000;
  pbScreen->Canvas->Brush->Color = (TColor)0x800000;

  for(int i = 0; i < lNo; i++)
  {
    lX = GetBGX(i);
    lY = GetBGY(i);

    // circle
    pbScreen->Canvas->Ellipse(
    lX - 15, lY - 15,
    lX + 15, lY + 15
    );

    // caption
    if ((cboxId->Checked) && (cboxNames->Checked))
    {
      lCaption = AnsiString(GetMissionInstance()->GetBGManager()->GetBGroupByIndex(i)->GetName().c_str()) +
                 " [" +
                 AnsiString(GetMissionInstance()->GetBGManager()->GetBGroupByIndex(i)->GetId()) +
                 "]";
    }
    else
    {
      if (cboxNames->Checked)
      {
        lCaption = AnsiString(GetMissionInstance()->GetBGManager()->GetBGroupByIndex(i)->GetName().c_str());
      }
      else
      {
        lCaption = AnsiString(GetMissionInstance()->GetBGManager()->GetBGroupByIndex(i)->GetId());
      }
    }

    if (mSelected == GetMissionInstance()->GetBGManager()->GetBGroupByIndex(i)->GetId())
    {
      pbScreen->Canvas->Font->Color  = clYellow;
    }
    else
    {
      pbScreen->Canvas->Font->Color  = clWhite;
    }

    SetBkMode(pbScreen->Canvas->Handle, TRANSPARENT);
      pbScreen->Canvas->TextOut(
        lX,
        lY,
        lCaption
      );
  }
}
//---------------------------------------------------------------------------

void __fastcall TFormPolitics::pbScreenMouseDown(TObject *Sender,
      TMouseButton Button, TShiftState Shift, int X, int Y)
{
  mStartBg  = BGroupAtPos(X, Y);
  mStartX = X;
  mStartY = Y;
  mOldX   = X;
  mOldY   = Y;
}
//---------------------------------------------------------------------------

void __fastcall TFormPolitics::pbScreenMouseUp(TObject *Sender,
      TMouseButton Button, TShiftState Shift, int X, int Y)
{
  TPoliticsRelations lRel;
  int lBg = BGroupAtPos(X, Y);

  // terminate XOR Line
  pbScreen->Canvas->Pen->Mode  = pmXor;
  pbScreen->Canvas->Pen->Color = (TColor)0xFFFF00;

  pbScreen->Canvas->MoveTo(mStartX, mStartY);
  pbScreen->Canvas->LineTo(mOldX, mOldY);

  if ((lBg != -1) && (mStartBg != -1))
  {
    if (lBg == mStartBg)
    {
      mSelected = mStartBg;
    }
    else
    {
      // we got a valid link drawed
      switch(cbDraw->ItemIndex)
      {
        case 0:
          lRel = REL_NEUTRAL;
          break;
        case 1:
          lRel = REL_ENEMY;
          break;
        case 2:
          lRel = REL_FRIENDLY;
          break;
      }

     GetMissionInstance()->GetBGManager()->SetUniRelation(mStartBg, lBg, lRel);
    }

    GroupBox3->Repaint();
    pbScreen->OnPaint(this);
  }

  mStartBg = -1;
}
//---------------------------------------------------------------------------

void __fastcall TFormPolitics::pbScreenMouseMove(TObject *Sender,
      TShiftState Shift, int X, int Y)
{
  if (mStartBg == -1)
  {
    return;
  }

  // erase previous
  pbScreen->Canvas->Pen->Mode  = pmXor;
  pbScreen->Canvas->Pen->Color = (TColor)0xFFFF00;

  pbScreen->Canvas->MoveTo(mStartX, mStartY);
  pbScreen->Canvas->LineTo(mOldX, mOldY);

  // draw new
  pbScreen->Canvas->MoveTo(mStartX, mStartY);
  pbScreen->Canvas->LineTo(X, Y);

  mOldX = X;
  mOldY = Y;
}
//---------------------------------------------------------------------------

void __fastcall TFormPolitics::cboxNamesClick(TObject *Sender)
{
  GroupBox3->Repaint();
  pbScreen->OnPaint(this);
}
//---------------------------------------------------------------------------

void __fastcall TFormPolitics::cboxIdClick(TObject *Sender)
{
  GroupBox3->Repaint();
  pbScreen->OnPaint(this);
}
//---------------------------------------------------------------------------

void __fastcall TFormPolitics::cbLinksChange(TObject *Sender)
{
  GroupBox3->Repaint();
  pbScreen->OnPaint(this);
}
//---------------------------------------------------------------------------

void __fastcall TFormPolitics::cbDrawChange(TObject *Sender)
{
  GroupBox3->Repaint();
  pbScreen->OnPaint(this);
}
//---------------------------------------------------------------------------

void __fastcall TFormPolitics::cbStyleChange(TObject *Sender)
{
  GroupBox3->Repaint();
  pbScreen->OnPaint(this);
}
//---------------------------------------------------------------------------

