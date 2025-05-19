//---------------------------------------------------------------------------

#ifndef FToolPropH
#define FToolPropH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <ComCtrls.hpp>
#include "CSPIN.h"
#include <Grids.hpp>
#include <Menus.hpp>
#include <Buttons.hpp>
#include <Dialogs.hpp>
#include <Vcl.ExtCtrls.hpp>
#include <Vcl.Imaging.pngimage.hpp>
#include <vector>
//---------------------------------------------------------------------------

class CBmpName
{
  public:
    AnsiString  Name;
    TPngImage*  Bitmap;
};

enum TEditPoint { EDITPOINT_FIRE, EDITPOINT_DROP, EDITPOINT_VISUAL, EDITPOINT_NOTHING };

class TFormToolProp : public TForm
{
__published:	// IDE-managed Components
        TPageControl *PageControl;
        TTabSheet *TabMask;
        TTabSheet *TabAlt;
        TTabSheet *TabLayers;
        TTrackBar *trackMaskSize;
        TLabel *Label1;
        TCSpinEdit *editAlt;
        TLabel *Label2;
        TLabel *Label3;
        TTrackBar *trackAltSize;
        TStringGrid *gridLayers;
        TPopupMenu *PopupMenu1;
        TMenuItem *NewLayer1;
        TMenuItem *DeleteLayer1;
        TMenuItem *RenameLayer1;
        TMenuItem *Sprite1;
        TCheckBox *cbVisible;
        TCSpinEdit *edFrameDelay;
        TLabel *Label4;
        TSpeedButton *btnUp;
        TSpeedButton *btnLeft;
        TSpeedButton *btnRight;
        TSpeedButton *btnDown;
        TOpenDialog *OpenDialog;
        TCSpinEdit *edStep;
        TLabel *Label5;
        TMenuItem *SpriteSetup1;
        TLabel *Label6;
        TCSpinEdit *edCycleDelay;
        TTabSheet *TabPoints;
        TRadioButton *rbDropPoint;
        TRadioButton *rbFirePoint;
        TRadioButton *rbVisualPoint;
        TComboBox *cbVisual;
        TSpeedButton *btnEditVisual;
        TSpeedButton *btnAddVisual;
        TSpeedButton *btnRemoveVisual;
    TColorBox *MaskColor;
    TLabel *Label7;
    TLabel *Label8;
    TTrackBar *trackMaskOpacity;
        void __fastcall NewLayer1Click(TObject *Sender);
        void __fastcall FormShow(TObject *Sender);
        void __fastcall DeleteLayer1Click(TObject *Sender);
        void __fastcall Sprite1Click(TObject *Sender);
        void __fastcall cbVisibleClick(TObject *Sender);
        void __fastcall edFrameDelayChange(TObject *Sender);
        void __fastcall gridLayersClick(TObject *Sender);
        void __fastcall btnUpClick(TObject *Sender);
        void __fastcall btnDownClick(TObject *Sender);
        void __fastcall btnLeftClick(TObject *Sender);
        void __fastcall btnRightClick(TObject *Sender);
        void __fastcall SpriteSetup1Click(TObject *Sender);
        void __fastcall edCycleDelayChange(TObject *Sender);
        void __fastcall btnEditVisualClick(TObject *Sender);
        void __fastcall btnAddVisualClick(TObject *Sender);
        void __fastcall btnRemoveVisualClick(TObject *Sender);
        void __fastcall cbVisualChange(TObject *Sender);
        void __fastcall rbDropPointClick(TObject *Sender);
    void __fastcall MaskColorChange(TObject *Sender);
    void __fastcall trackMaskOpacityChange(TObject *Sender);
private:	// User declarations
        void RecToEdit(int _layer);
        void EditToRec(int _layer);
        void FillStringList();
        void DataToScreen();

        void ManageBmpList(); // used when a sprite for a layer is set or a
                              // layer is removed

        void AddBmp(AnsiString _name);
        void DeleteBmp(AnsiString _name);                      

        std::vector<CBmpName> mBmpList; // i'm too lazy to use stl map :p

public:		// User declarations
        __fastcall TFormToolProp(TComponent* Owner);

        TPngImage* GetBmpByName(AnsiString _name);
public:
        int GetMaskBrushSize();

        int GetAltBrushSize();
        int GetAlt();

        TEditPoint GetEditPoint();

        int GetCurrentVisual();
};
//---------------------------------------------------------------------------
extern PACKAGE TFormToolProp *FormToolProp;
//---------------------------------------------------------------------------
#endif
