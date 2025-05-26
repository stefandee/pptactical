//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "FEcoSysGenKernelConfig.h"
#include "FEditCurve2D.h"
#include "PPIG_Mission.h"
#include "PPIG_Map.h"
#include "Tools.h"
#include <Vcl.Imaging.pngimage.hpp>
//---------------------------------------------------------------------------

#pragma package(smart_init)
#pragma link "CSPIN"
#pragma resource "*.dfm"
TFormEcoSysGenKernelConfig *FormEcoSysGenKernelConfig;
//---------------------------------------------------------------------------

__fastcall TFormEcoSysGenKernelConfig::TFormEcoSysGenKernelConfig(TComponent* Owner)
        : TForm(Owner)
{
  mEditData = 0;
}
//---------------------------------------------------------------------------

void TFormEcoSysGenKernelConfig::DataToScreen()
{
  if (!mEditData)
  {
    return;
  }

  edSteps->Value    = mEditData->GetSteps();
  mZone             = mEditData->GetZone();
  mItems            = mEditData->GetItems();
}
//---------------------------------------------------------------------------

void TFormEcoSysGenKernelConfig::ScreenToData()
{
  mEditData->SetSteps(edSteps->Value);
  mEditData->SetZone(mZone);
  mEditData->SetItems(mItems);
}
//---------------------------------------------------------------------------

void TFormEcoSysGenKernelConfig::ItemsToScreen()
{
}
//---------------------------------------------------------------------------

void TFormEcoSysGenKernelConfig::SetData(EcoSysGenKernel* _data)
{
  mEditData = _data;

  DataToScreen();
}
//---------------------------------------------------------------------------

void __fastcall TFormEcoSysGenKernelConfig::paintZonePaint(TObject *Sender)
{
  paintZone->Canvas->Brush->Color = (TColor)0x000000;
  paintZone->Canvas->FillRect(Types::TRect(0, 0, paintZone->Width, paintZone->Height));

  paintZone->Canvas->Brush->Color = (TColor)0x00FF00;
  paintZone->Canvas->FillRect(Types::TRect(mZone.left, mZone.top, mZone.right, mZone.bottom));
}
//---------------------------------------------------------------------------

void __fastcall TFormEcoSysGenKernelConfig::FormShow(TObject *Sender)
{
  paintZone->Width  = GetMissionInstance()->GetMap()->GetLogicWidth();
  paintZone->Height = GetMissionInstance()->GetMap()->GetLogicHeight();

  DrawGrid->ColCount = mItems.size();
}
//---------------------------------------------------------------------------

void __fastcall TFormEcoSysGenKernelConfig::paintZoneMouseDown(
      TObject *Sender, TMouseButton Button, TShiftState Shift, int X,
      int Y)
{
  if (Button == mbLeft)
  {
    mSelection = true;

    mZone.left = X;
    mZone.top  = Y;
  }
}
//---------------------------------------------------------------------------
void __fastcall TFormEcoSysGenKernelConfig::paintZoneMouseMove(
      TObject *Sender, TShiftState Shift, int X, int Y)
{
  if (mSelection)
  {
    mZone.right = X;
    mZone.bottom  = Y;
    paintZone->OnPaint(this);
  }
}
//---------------------------------------------------------------------------

void __fastcall TFormEcoSysGenKernelConfig::paintZoneMouseUp(
      TObject *Sender, TMouseButton Button, TShiftState Shift, int X,
      int Y)
{
  if (mSelection && Button == mbLeft)
  {
    mZone.right = X;
    mZone.bottom  = Y;

    paintZone->OnPaint(this);
    mSelection = false;
  }
}
//---------------------------------------------------------------------------

void __fastcall TFormEcoSysGenKernelConfig::SelectAll1Click(
      TObject *Sender)
{
  mZone = CPRect(0, 0, GetMissionInstance()->GetMap()->GetLogicWidth(), GetMissionInstance()->GetMap()->GetLogicHeight());
  paintZone->OnPaint(this);
}
//---------------------------------------------------------------------------

void __fastcall TFormEcoSysGenKernelConfig::LeftTopQuarter1Click(
      TObject *Sender)
{
  mZone = CPRect(0, 0, GetMissionInstance()->GetMap()->GetLogicWidth() / 2, GetMissionInstance()->GetMap()->GetLogicHeight() / 2);
  paintZone->OnPaint(this);
}
//---------------------------------------------------------------------------
void __fastcall TFormEcoSysGenKernelConfig::LeftBottomQuarter1Click(
      TObject *Sender)
{
  mZone = CPRect(0, GetMissionInstance()->GetMap()->GetLogicHeight() / 2, GetMissionInstance()->GetMap()->GetLogicWidth() / 2, GetMissionInstance()->GetMap()->GetLogicHeight());
  paintZone->OnPaint(this);
}
//---------------------------------------------------------------------------
void __fastcall TFormEcoSysGenKernelConfig::RightTopQuarter1Click(
      TObject *Sender)
{
  mZone = CPRect(GetMissionInstance()->GetMap()->GetLogicWidth() / 2, 0, GetMissionInstance()->GetMap()->GetLogicWidth(), GetMissionInstance()->GetMap()->GetLogicHeight() / 2);
  paintZone->OnPaint(this);
}
//---------------------------------------------------------------------------
void __fastcall TFormEcoSysGenKernelConfig::RightBottomQuarter1Click(
      TObject *Sender)
{
  mZone = CPRect(GetMissionInstance()->GetMap()->GetLogicWidth() / 2, GetMissionInstance()->GetMap()->GetLogicHeight() / 2, GetMissionInstance()->GetMap()->GetLogicWidth(), GetMissionInstance()->GetMap()->GetLogicHeight());
  paintZone->OnPaint(this);
}
//---------------------------------------------------------------------------
void __fastcall TFormEcoSysGenKernelConfig::DrawGridDrawCell(
      TObject *Sender, int ACol, int ARow, TRect &Rect,
      TGridDrawState State)
{
  if (mItems.size() <= 0 || ACol >= (int)mItems.size())
  {
    return;
  }

  // draw the current cell; asuming that the row is always 1, the ACol is
  // an index in mItems
  using namespace Windows;

  TPngImage* lBmp = new TPngImage();

  try
  {
    lBmp->LoadFromFile(getObjectSpriteName(mItems[ACol]->mFilePath.c_str()));

    //DrawGrid->ColWidths[ACol] = lBmp->Width;
    //DrawGrid->RowHeights[ARow] = lBmp->Height;

    DrawGrid->Canvas->Draw(Rect.left, Rect.top, lBmp);

    DrawGrid->Canvas->Font->Color = (TColor)0x000000;
    DrawGrid->Canvas->TextOut(Rect.left + 2, Rect.top + 2, AnsiString(ACol));

    //DrawGrid->Canvas->Pen->Color = 0xFFFFFF;

    TRect lCodeRect = Rect;

    TSize lTextSize = DrawGrid->Canvas->TextExtent(AnsiString(ACol));

    lCodeRect.left   = lCodeRect.left + lTextSize.cx + 2 + 2;
    lCodeRect.right  = lCodeRect.left + lTextSize.cy;
    lCodeRect.top    = lCodeRect.top + 2;
    lCodeRect.bottom = lCodeRect.top + lTextSize.cy;

    DrawGrid->Canvas->TextFlags = DrawGrid->Canvas->TextFlags & (~ETO_OPAQUE);
    DrawGrid->Canvas->Brush->Color = (TColor)mItems[ACol]->mColorCode;
    DrawGrid->Canvas->FillRect(lCodeRect);

    DrawGrid->Canvas->Brush->Color = (TColor)0xFFFFFF;
    DrawGrid->Canvas->FrameRect(lCodeRect);
  }
  catch(...)
  {
    //DrawGrid->ColWidths[ACol] = 32;
    //DrawGrid->RowHeights[ARow] = 32;

    DrawGrid->Canvas->Brush->Color = (TColor)0xFF00FF;
    DrawGrid->Canvas->FillRect(DrawGrid->CellRect(ACol, ARow));
    //Application->MessageBox();
  }

  delete lBmp;
}
//---------------------------------------------------------------------------

void __fastcall TFormEcoSysGenKernelConfig::DrawGridDblClick(
      TObject *Sender)
{
  //
  if (DrawGrid->Row < 0 || DrawGrid->Col > 0 || mItems.size() == 0)
  {
    return;
  }

  if (OpenDialog->Execute())
  {
    mItems[DrawGrid->Col]->mFilePath = CPString(UTF8Encode(OpenDialog->FileName).c_str());

    DrawGrid->Repaint();
  }
}
//---------------------------------------------------------------------------

void __fastcall TFormEcoSysGenKernelConfig::Add1Click(TObject *Sender)
{
  if (OpenDialog->Execute())
  {
    EcoSysItemKernelIn* lItem = new EcoSysItemKernelIn();

    lItem->mFilePath  = CPString(UTF8Encode(OpenDialog->FileName).c_str());
    lItem->mColorCode = 0xFF0000;
    lItem->mSize      = CPPoint(2, 2);

    mItems.push_back(lItem);

    DrawGrid->ColCount = mItems.size();
    DrawGrid->Col = mItems.size() - 1;

    TPngImage* lBmp = new TPngImage();

    try
    {
      lBmp->LoadFromFile(getObjectSpriteName(mItems[DrawGrid->Col]->mFilePath.c_str()));
      DrawGrid->ColWidths[DrawGrid->Col] = lBmp->Width;
      DrawGrid->RowHeights[0]            = lBmp->Height;
    }
    catch(...)
    {
      DrawGrid->ColWidths[DrawGrid->Col] = 32;
      DrawGrid->RowHeights[0]            = 32;
    }

    delete lBmp;
  }
}
//---------------------------------------------------------------------------
void __fastcall TFormEcoSysGenKernelConfig::Edit1Click(TObject *Sender)
{
  if (mItems.size() == 0)
  {
    return;
  }

  if (ColorDialog->Execute())
  {
    mItems[DrawGrid->Col]->mColorCode = ColorDialog->Color;

    DrawGrid->Repaint();
  }
}
//---------------------------------------------------------------------------

void __fastcall TFormEcoSysGenKernelConfig::Remove1Click(TObject *Sender)
{
  if (mItems.size() == 0)
  {
    return;
  }

  delete mItems[DrawGrid->Col];
  mItems[DrawGrid->Col] = 0;

  std::vector<EcoSysItemIn*>::iterator it = mItems.begin();

  it += DrawGrid->Col;

  mItems.erase(it);

  DrawGrid->ColCount = DrawGrid->ColCount - 1;
  DrawGrid->Repaint();
}
//---------------------------------------------------------------------------

void __fastcall TFormEcoSysGenKernelConfig::Clear1Click(TObject *Sender)
{
  mItems.clear();
  DrawGrid->ColCount = 0;

  DrawGrid->Repaint();
}
//---------------------------------------------------------------------------

void __fastcall TFormEcoSysGenKernelConfig::btnOkClick(TObject *Sender)
{
  ScreenToData();
}
//---------------------------------------------------------------------------

void __fastcall TFormEcoSysGenKernelConfig::Deformationfunction1Click(
      TObject *Sender)
{
  if (mItems.size() == 0)
  {
    return;
  }

  TFormEditCurve2D* lFormEditCurve2D = new TFormEditCurve2D(this);

  lFormEditCurve2D->SetCurve(((EcoSysItemKernelIn*)mItems[DrawGrid->Col])->mCurve);
  lFormEditCurve2D->ShowModal();
  ((EcoSysItemKernelIn*)mItems[DrawGrid->Col])->mCurve = lFormEditCurve2D->GetCurve();

  delete lFormEditCurve2D;
}
//---------------------------------------------------------------------------

