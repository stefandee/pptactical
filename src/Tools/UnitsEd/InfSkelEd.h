//---------------------------------------------------------------------------
#ifndef InfSkelEdH
#define InfSkelEdH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <ExtCtrls.hpp>
#include "AIUnits.h"
#include "ImageEx.h"
#include "CSPIN.h"
#include <Buttons.hpp>
#include <Vcl.Imaging.pngimage.hpp>
//---------------------------------------------------------------------------

class TInfSkelEditor : public TForm
{
__published:	// IDE-managed Components
        TGroupBox *GroupBox1;
        TImageEx *img_body;
        TCSpinButton *CSpinButton2;
        TGroupBox *GroupBox2;
        TRadioButton *rbWeapon;
        TRadioButton *rbVisual;
        TComboBox *cbVisual;
        TSpeedButton *btnEditVisual;
        TSpeedButton *btnAddVisual;
        TSpeedButton *btnRemoveVisual;
        
        void __fastcall FormShow(TObject *Sender);
        void __fastcall CSpinButton2UpClick(TObject *Sender);
        void __fastcall CSpinButton2DownClick(TObject *Sender);
        void __fastcall img_bodyMouseDown(TObject *Sender,
          TMouseButton Button, TShiftState Shift, int X, int Y);
        void __fastcall FormCreate(TObject *Sender);
        void __fastcall btnAddVisualClick(TObject *Sender);
        void __fastcall btnRemoveVisualClick(TObject *Sender);
        void __fastcall btnEditVisualClick(TObject *Sender);
private:	// User declarations
public:		// User declarations
        __fastcall TInfSkelEditor(TComponent* Owner);
        TPngImage* Sprite;
        TRect rs, rd, rg;
        int  bodyCellNo, DivideFactor;
        void DrawBody();
        hInfantryUnified tempInf;
        void DataToScreen();

        void SetInfRecord(hInfantryUnified _value);
        void SetSprite(AnsiString _fileName);
};
//---------------------------------------------------------------------------
extern PACKAGE TInfSkelEditor *InfSkelEditor;
//---------------------------------------------------------------------------
#endif
