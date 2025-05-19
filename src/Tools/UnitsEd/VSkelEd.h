//---------------------------------------------------------------------------
#ifndef VSkelEdH
#define VSkelEdH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <ExtCtrls.hpp>
#include "AIUnits.h"
#include "cspin.h"
#include "CSPIN.h"
#include "ImageEx.h"
#include <Buttons.hpp>
#include <Vcl.Imaging.pngimage.hpp>
//---------------------------------------------------------------------------
class TVSkelEditor : public TForm
{
__published:	// IDE-managed Components
        TLabel *l_body;
        TLabel *l_turret;
        TRadioGroup *rg_bpoints;
        TRadioGroup *rg_tpoints;
        TLabel *l_stat2;
        TCSpinButton *CSpinButton1;
        TCSpinButton *CSpinButton2;
        TImageEx *img_ground;
        TImageEx *img_turret;
        TImageEx *img_body;
        TGroupBox *GroupBox1;
        TComboBox *cbVisual;
        TSpeedButton *btnEditVisual;
        TSpeedButton *btnAddVisual;
        TSpeedButton *btnRemoveVisual;
        void __fastcall FormClose(TObject *Sender, TCloseAction &Action);
        void __fastcall img_TurretMouseDown(TObject *Sender,
          TMouseButton Button, TShiftState Shift, int X, int Y);

        void __fastcall FormShow(TObject *Sender);
        void __fastcall img_bodyMouseDown(TObject *Sender,
          TMouseButton Button, TShiftState Shift, int X, int Y);
        void __fastcall CSpinButton1DownClick(TObject *Sender);
        void __fastcall CSpinButton1UpClick(TObject *Sender);
        void __fastcall CSpinButton2UpClick(TObject *Sender);
        void __fastcall CSpinButton2DownClick(TObject *Sender);
        void __fastcall FormCreate(TObject *Sender);
        void __fastcall btnRemoveVisualClick(TObject *Sender);
        void __fastcall btnEditVisualClick(TObject *Sender);
        void __fastcall btnAddVisualClick(TObject *Sender);
private:	// User declarations
        void DataToScreen();
public:		// User declarations
        __fastcall TVSkelEditor(TComponent* Owner);
        TPngImage* Sprite;
        TRect rs, rd, rg;
        int turretCellNo, bodyCellNo, DivideFactor;
        void DrawBodyAndTurret();
        hVehicleUnified tempV;

        void SetVehicleRecord(hVehicleUnified _vehicle);
        void SetSprite(AnsiString _fileName);
};
//---------------------------------------------------------------------------
extern PACKAGE TVSkelEditor *VSkelEditor;
//---------------------------------------------------------------------------
#endif
