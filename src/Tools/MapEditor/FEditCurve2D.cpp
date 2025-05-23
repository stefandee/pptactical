//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "FEditCurve2D.h"
#include "CurveLinear.h"
#include "CurveElipsoid.h"
#include "CurveScript.h"
#include "CurvePolynome.h"
#include "Curve2DFactory.h"
#include "Stream.h"
#include "Stream_ANSI_C.h"
#include "ObjectStream.h"
//---------------------------------------------------------------------------

#pragma package(smart_init)
#pragma link "ScriptPopupMenu"
#pragma resource "*.dfm"
TFormEditCurve2D *FormEditCurve2D;
//---------------------------------------------------------------------------

__fastcall TFormEditCurve2D::TFormEditCurve2D(TComponent* Owner)
        : TForm(Owner)
{
  mViewport.x = CURVE2D_DEFAULT_VIEWPORT_X;
  mViewport.y = CURVE2D_DEFAULT_VIEWPORT_Y;
  mZoom       = CURVE2D_DEFAULT_ZOOM;
  mCurve      = 0;

  shapeCurve->Brush->Color  = (TColor)CURVE2D_CURVE_COLOR;
  shapeBack->Brush->Color   = (TColor)CURVE2D_BACK_COLOR;
  shapeAxes->Brush->Color   = (TColor)CURVE2D_AXES_COLOR;
  shapeMarker->Brush->Color = (TColor)CURVE2D_MARKER_COLOR;
  mMarkerDensity            = CURVE2D_MARKER_DENSITY;

  TScriptPopupMenu* lPopup = new TScriptPopupMenu(memoScript);

  lPopup->SetInitialDir(AnsiString(".\\"));
  lPopup->SetFilter("C Script Language files|*.csl|All files|*.*");
  lPopup->SetDefaultExt("csl");

  memoScript->PopupMenu   = lPopup;
}
//---------------------------------------------------------------------------

void TFormEditCurve2D::PaintAxesAndMarkers(TPaintBox* _paintBox, CPPoint _viewport, double _zoom)
{
  // clear the canvas
  _paintBox->Canvas->Brush->Color = shapeBack->Brush->Color;
  _paintBox->Canvas->FillRect(TRect(0, 0, _paintBox->Width, _paintBox->Height));

  // move to origin
  _paintBox->Canvas->Pen->Color = shapeAxes->Brush->Color;

  _paintBox->Canvas->MoveTo(_paintBox->Width / 2 - _viewport.y, _paintBox->Height / 2 - _viewport.x);
  _paintBox->Canvas->LineTo(_paintBox->Width + _paintBox->Width / 2 - _viewport.y, _paintBox->Height / 2 - _viewport.x);

  _paintBox->Canvas->MoveTo(_paintBox->Width / 2 - _viewport.y, _paintBox->Height / 2 - _viewport.x);
  _paintBox->Canvas->LineTo(-_paintBox->Width + _paintBox->Width / 2 - _viewport.y, _paintBox->Height / 2 - _viewport.x);

  _paintBox->Canvas->MoveTo(_paintBox->Width / 2 - _viewport.y, _paintBox->Height / 2 - _viewport.x);
  _paintBox->Canvas->LineTo(_paintBox->Width / 2 - _viewport.y, _paintBox->Height + _paintBox->Height / 2 - _viewport.x);

  _paintBox->Canvas->MoveTo(_paintBox->Width / 2 - _viewport.y, _paintBox->Height / 2 - _viewport.x);
  _paintBox->Canvas->LineTo(_paintBox->Width / 2 - _viewport.y, -_paintBox->Height + _paintBox->Height / 2 - _viewport.x);

  // draw the markers
  //_paintBox->Canvas->Font->Name   = "Small Fonts";
  //_paintBox->Canvas->Font->Height = -7;
  //_paintBox->Canvas->Font->Color  = 0xFF0000;

  _paintBox->Canvas->Pen->Color = shapeMarker->Brush->Color;

  for(int x = _paintBox->Width / 2 - _viewport.y - _paintBox->Width; x < _paintBox->Width / 2 - _viewport.y + _paintBox->Width; x += mMarkerDensity)
  {
    _paintBox->Canvas->MoveTo(x, _paintBox->Height / 2 - _viewport.x + CURVE2D_MARKER_SIZE);
    _paintBox->Canvas->LineTo(x, _paintBox->Height / 2 - _viewport.x - CURVE2D_MARKER_SIZE);

    //_paintBox->Canvas->TextOut(x, _paintBox->Height / 2 - _viewport.x + CURVE2D_MARKER_SIZE, AnsiString("X"));
  }

  for(int y = _paintBox->Height / 2 - _viewport.x - _paintBox->Height; y < _paintBox->Height / 2 - _viewport.x + _paintBox->Height; y += mMarkerDensity)
  {
    _paintBox->Canvas->MoveTo(_paintBox->Width / 2 - _viewport.y + CURVE2D_MARKER_SIZE, y);
    _paintBox->Canvas->LineTo(_paintBox->Width / 2 - _viewport.y - CURVE2D_MARKER_SIZE, y);
  }
}
//---------------------------------------------------------------------------

void __fastcall TFormEditCurve2D::paintBoxPaint(TObject *Sender)
{
  PaintAxesAndMarkers(paintBox, mViewport, mZoom);

  if (!mCurve)
  {
    return;
  }

  double lMin, lMax, lStep;

  try
  {
    lMin = edDrawIntervalMin->Text.ToDouble();
  }
  catch(...)
  {
    lMin = 0.;
  }

  try
  {
    lMax = edDrawIntervalMax->Text.ToDouble();
  }
  catch(...)
  {
    lMax = 0.;
  }

  try
  {
    lStep = edDrawIntervalStep->Text.ToDouble();
  }
  catch(...)
  {
    lStep = 1.;
  }

  std::vector<double> lYVectorTemp;

  paintBox->Canvas->Pen->Color = shapeCurve->Brush->Color;

  CPString lParamName;

  if (mCurve->GetParamList().GetName() == CPString("Explicit"))
  {
    lParamName = "X";
  }

  if (mCurve->GetParamList().GetName() == CPString("Parametric"))
  {
    lParamName = "U";
  }

  if (mCurve->GetParamList().GetName() == CPString("Implicit"))
  {
    lParamName = "X";
  }

  for(double v = lMin; v < lMax; v += lStep)
  {
    TemplateParamList<double> lList;

    lList.SetParam(lParamName, v);

    std::vector<double> lYVector = mCurve->Compute(lList);

    if (lYVectorTemp.size() != 0 && lYVector.size() != 0)
    {
      paintBox->Canvas->MoveTo(paintBox->Width / 2 + lYVectorTemp[0], paintBox->Height / 2 - lYVectorTemp[1]);
      paintBox->Canvas->LineTo(paintBox->Width / 2 + lYVector[0]    , paintBox->Height / 2 - lYVector[1]);
    }

    lYVectorTemp = lYVector;
  }
}
//---------------------------------------------------------------------------

void __fastcall TFormEditCurve2D::paintBoxMouseDown(TObject *Sender,
      TMouseButton Button, TShiftState Shift, int X, int Y)
{
  mStartPoint.x = X;
  mStartPoint.y = Y;

  mCaptured = GetCaptureControl();
  SetCaptureControl(paintBox);

  mScroll = true;
}
//---------------------------------------------------------------------------

void __fastcall TFormEditCurve2D::paintBoxMouseUp(TObject *Sender,
      TMouseButton Button, TShiftState Shift, int X, int Y)
{
  SetCaptureControl(this);
  mScroll = false;
}
//---------------------------------------------------------------------------
void __fastcall TFormEditCurve2D::SpeedButton2Click(TObject *Sender)
{
  mViewport = CPPoint(0, 0);

  paintBox->OnPaint(this);
}
//---------------------------------------------------------------------------

void __fastcall TFormEditCurve2D::paintBoxMouseMove(TObject *Sender,
      TShiftState Shift, int X, int Y)
{
  if (mScroll)
  {
    mViewport.y += (X - mStartPoint.x);
    mViewport.x += (Y - mStartPoint.y);

    mStartPoint = CPPoint(X, Y);

    paintBox->OnPaint(this);
  }
}
//---------------------------------------------------------------------------

Interval TFormEditCurve2D::GetInterval()
{
  double lMin, lMax;

  try
  {
    lMin = edIntervalMin->Text.ToDouble();
  }
  catch(...)
  {
    lMin = 0.;
  }

  try
  {
    lMax = edIntervalMax->Text.ToDouble();
  }
  catch(...)
  {
    lMax = 0.;
  }

  return Interval(lMin, lMax);
}
//---------------------------------------------------------------------------

TemplateParamList<double> TFormEditCurve2D::GetParamList()
{
  TemplateParamList<double> lList;

  switch(cbParamType->ItemIndex)
  {
    case 0:
      lList.SetName("Parametric");
      break;

    case 1:
      lList.SetName("Implicit");
      break;

    case 2:
      lList.SetName("Explicit");
      break;
  }

  for(int i = 1; i < listValues->RowCount; i++)
  {
    AnsiString lKey    = listValues->Keys[i];
    AnsiString lValue  = listValues->Cells[1][i];

    double lConvertedValue = 0.;

    try
    {
      lConvertedValue = lValue.ToDouble();
    }
    catch(...)
    {
      lConvertedValue = 0.;
    }

    lList.SetParam(lKey.c_str(), lConvertedValue);
  }

  return lList;
}
//---------------------------------------------------------------------------

Curve2D* TFormEditCurve2D::ComputeCurve()
{
  Interval lInterval                   = GetInterval();
  TemplateParamList<double> lParamList = GetParamList();

  Curve2D* lCurve;

  switch(cbCurveType->ItemIndex)
  {
    case 0:
      lCurve = new CurveLinear(lInterval, lParamList);
      break;

    case 1:
      lCurve = new CurveElipsoid(lInterval, lParamList);
      break;

    case 2:
      lCurve = new CurvePolynome(lInterval, lParamList);
      break;

    case 3:
      lCurve = new CurveScript(lInterval, lParamList);
      ((CurveScript*)lCurve)->SetScript(System::UTF8String(memoScript->Text).c_str());
      break;
  }

  lCurve->SetName(System::UTF8String(edName->Text).c_str());

  return lCurve;
}
//---------------------------------------------------------------------------

void __fastcall TFormEditCurve2D::SpeedButton1Click(TObject *Sender)
{
  delete mCurve;
  mCurve = ComputeCurve();

  paintBox->OnPaint(this);
}
//---------------------------------------------------------------------------

void __fastcall TFormEditCurve2D::shapeCurveMouseUp(TObject *Sender,
      TMouseButton Button, TShiftState Shift, int X, int Y)
{
  if (Button == mbLeft)
  {
    if (dlgColor->Execute())
    {
      shapeCurve->Brush->Color = dlgColor->Color;

      paintBox->OnPaint(this);
    }
  }
}
//---------------------------------------------------------------------------

void __fastcall TFormEditCurve2D::shapeBackMouseUp(TObject *Sender,
      TMouseButton Button, TShiftState Shift, int X, int Y)
{
  if (Button == mbLeft)
  {
    if (dlgColor->Execute())
    {
      shapeBack->Brush->Color = dlgColor->Color;

      paintBox->OnPaint(this);
    }
  }
}
//---------------------------------------------------------------------------

void __fastcall TFormEditCurve2D::shapeAxesMouseUp(TObject *Sender,
      TMouseButton Button, TShiftState Shift, int X, int Y)
{
  if (Button == mbLeft)
  {
    if (dlgColor->Execute())
    {
      shapeAxes->Brush->Color = dlgColor->Color;

      paintBox->OnPaint(this);
    }
  }
}
//---------------------------------------------------------------------------

void __fastcall TFormEditCurve2D::shapeMarkerMouseUp(TObject *Sender,
      TMouseButton Button, TShiftState Shift, int X, int Y)
{
  if (Button == mbLeft)
  {
    if (dlgColor->Execute())
    {
      shapeMarker->Brush->Color = dlgColor->Color;

      paintBox->OnPaint(this);
    }
  }
}
//---------------------------------------------------------------------------

void __fastcall TFormEditCurve2D::edSpacingExit(TObject *Sender)
{
  mMarkerDensity = edSpacing->Text.ToIntDef(CURVE2D_MARKER_DENSITY);

  paintBox->OnPaint(this);
}
//---------------------------------------------------------------------------

Curve2D* TFormEditCurve2D::GetCurve()
{
  return ComputeCurve();
}
//---------------------------------------------------------------------------

void TFormEditCurve2D::DataToScreen(Curve2D* _curve)
{
  if (!_curve)
  {
    return;
  }

  edName->Text = _curve->GetName().c_str();

  SetInterval(_curve->GetDefInterval());
  SetParamList(_curve->GetParamList());

  if (_curve->ClassName() == CPString("Curve2DLinear"))
  {
    cbCurveType->ItemIndex = 0;
  }

  if (_curve->ClassName() == CPString("Curve2DElipsoid"))
  {
    cbCurveType->ItemIndex = 1;
  }

  if (_curve->ClassName() == CPString("Curve2DPolynome"))
  {
    cbCurveType->ItemIndex = 2;
  }

  if (_curve->ClassName() == CPString("Curve2DScript"))
  {
    cbCurveType->ItemIndex = 3;
    memoScript->Text = ((CurveScript*)_curve)->GetScript().c_str();
  }
  
  cbCurveTypeChange(this);

  delete mCurve;
  mCurve = _curve;

  paintBox->OnPaint(this);
}
//---------------------------------------------------------------------------

void TFormEditCurve2D::SetInterval(Interval _v)
{
  edIntervalMin->Text = AnsiString(_v.GetMin());
  edIntervalMax->Text = AnsiString(_v.GetMax());
}
//---------------------------------------------------------------------------

void TFormEditCurve2D::SetParamList(TemplateParamList<double>& _v)
{
  TemplateParamList<double> lList;

  if (_v.GetName() == CPString("Parametric"))
  {
    cbParamType->ItemIndex = 0;
  }

  if (_v.GetName() == CPString("Implicit"))
  {
    cbParamType->ItemIndex = 1;
  }

  if (_v.GetName() == CPString("Explicit"))
  {
    cbParamType->ItemIndex = 2;
  }

  std::vector<std::string> lKeys = _v.GetKeys();

  //listValues->RowCount = lKeys.size() + 1;

  listValues->Strings->Clear();

  for(unsigned int i = 0; i < lKeys.size(); i++)
  {
    listValues->InsertRow(lKeys[i].c_str(), _v.GetParam(CPString(lKeys[i].c_str())), true);
  }
}
//---------------------------------------------------------------------------

void TFormEditCurve2D::SetCurve(Curve2D* _curve)
{
  DataToScreen(_curve);
}
//---------------------------------------------------------------------------

void __fastcall TFormEditCurve2D::cbCurveTypeChange(TObject *Sender)
{
  // scripted curve?
  if (cbCurveType->ItemIndex == 3)
  {
    memoScript->Visible  = true;
    labelScript->Visible = true;
  }
  else
  {
    memoScript->Visible  = false;
    labelScript->Visible = false;
  }
}
//---------------------------------------------------------------------------

