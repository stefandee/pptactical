//---------------------------------------------------------------------------

#ifndef FEditCurve2DH
#define FEditCurve2DH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <Grids.hpp>
#include <ValEdit.hpp>
#include <ExtCtrls.hpp>
#include <Buttons.hpp>
#include "basic2d.h"
#include "Curve2D.h"
#include "ScriptPopupMenu.h"
#include <Menus.hpp>
#include <Dialogs.hpp>
//---------------------------------------------------------------------------

const int CURVE2D_DEFAULT_ZOOM       = 1;
const int CURVE2D_DEFAULT_VIEWPORT_X = 0;
const int CURVE2D_DEFAULT_VIEWPORT_Y = 0;
const int CURVE2D_MARKER_SIZE        = 2;
const int CURVE2D_MARKER_COLOR       = 0xFFFFFF;
const int CURVE2D_MARKER_DENSITY     = 10;
const int CURVE2D_AXES_COLOR         = 0xFFFFFF;
const int CURVE2D_CURVE_COLOR        = 0xFF0000;
const int CURVE2D_BACK_COLOR         = 0x000000;

class TFormEditCurve2D : public TForm
{
__published:	// IDE-managed Components
        TGroupBox *GroupBox1;
        TGroupBox *GroupBox2;
        TGroupBox *GroupBox3;
        TGroupBox *GroupBox4;
        TButton *Button1;
        TButton *Button2;
        TLabel *Label1;
        TComboBox *cbCurveType;
        TValueListEditor *listValues;
        TLabel *Label2;
        TEdit *edName;
        TLabel *Label3;
        TComboBox *cbParamType;
        TPaintBox *paintBox;
        TSpeedButton *SpeedButton1;
        TSpeedButton *SpeedButton2;
        TEdit *edIntervalMin;
        TLabel *Label4;
        TEdit *edIntervalMax;
        TLabel *Label5;
        TMemo *memoScript;
        TLabel *labelScript;
        TScriptPopupMenu *ScriptPopupMenu1;
        TLabel *Label6;
        TEdit *edDrawIntervalMin;
        TLabel *Label7;
        TEdit *edDrawIntervalMax;
        TLabel *Label8;
        TEdit *edDrawIntervalStep;
        TColorDialog *dlgColor;
        TShape *shapeCurve;
        TShape *shapeAxes;
        TShape *shapeBack;
        TLabel *Label9;
        TLabel *Label10;
        TLabel *Label11;
        TShape *shapeMarker;
        TLabel *Label12;
        TLabel *Label13;
        TEdit *edSpacing;
        void __fastcall paintBoxPaint(TObject *Sender);
        void __fastcall paintBoxMouseDown(TObject *Sender,
          TMouseButton Button, TShiftState Shift, int X, int Y);
        void __fastcall paintBoxMouseUp(TObject *Sender,
          TMouseButton Button, TShiftState Shift, int X, int Y);
        void __fastcall SpeedButton2Click(TObject *Sender);
        void __fastcall paintBoxMouseMove(TObject *Sender,
          TShiftState Shift, int X, int Y);
        void __fastcall SpeedButton1Click(TObject *Sender);
        void __fastcall shapeCurveMouseUp(TObject *Sender,
          TMouseButton Button, TShiftState Shift, int X, int Y);
        void __fastcall shapeBackMouseUp(TObject *Sender,
          TMouseButton Button, TShiftState Shift, int X, int Y);
        void __fastcall shapeAxesMouseUp(TObject *Sender,
          TMouseButton Button, TShiftState Shift, int X, int Y);
        void __fastcall shapeMarkerMouseUp(TObject *Sender,
          TMouseButton Button, TShiftState Shift, int X, int Y);
        void __fastcall edSpacingExit(TObject *Sender);
        void __fastcall cbCurveTypeChange(TObject *Sender);
private:	// User declarations
       CPPoint   mViewport, mStartPoint;
       double    mZoom;
       bool      mScroll;
       int       mMarkerDensity;
       TControl* mCaptured;
       Curve2D*   mCurve;

       void PaintAxesAndMarkers(TPaintBox* _paintBox, CPPoint _viewport, double _zoom);
       
       Curve2D* ComputeCurve();

       Interval GetInterval();
       TemplateParamList<double> GetParamList();

       void SetInterval(Interval _v);
       void SetParamList(TemplateParamList<double>& _v);


       void DataToScreen(Curve2D* _curve);

public:		// User declarations
        __fastcall TFormEditCurve2D(TComponent* Owner);
        Curve2D*   GetCurve();
        void       SetCurve(Curve2D* _curve);
};
//---------------------------------------------------------------------------
extern PACKAGE TFormEditCurve2D *FormEditCurve2D;
//---------------------------------------------------------------------------
#endif
