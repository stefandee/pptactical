//---------------------------------------------------------------------------

#ifndef FPoliticsH
#define FPoliticsH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <ExtCtrls.hpp>
//---------------------------------------------------------------------------
class TFormPolitics : public TForm
{
__published:	// IDE-managed Components
        TGroupBox *GroupBox1;
        TButton *btnOk;
        TGroupBox *GroupBox2;
        TGroupBox *GroupBox3;
        TLabel *Label1;
        TComboBox *cbStyle;
        TLabel *Label2;
        TComboBox *cbLinks;
        TLabel *Label3;
        TComboBox *cbDraw;
        TCheckBox *cboxNames;
        TCheckBox *cboxId;
        TPaintBox *pbScreen;
        void __fastcall pbScreenPaint(TObject *Sender);
        void __fastcall pbScreenMouseDown(TObject *Sender,
          TMouseButton Button, TShiftState Shift, int X, int Y);
        void __fastcall pbScreenMouseUp(TObject *Sender,
          TMouseButton Button, TShiftState Shift, int X, int Y);
        void __fastcall pbScreenMouseMove(TObject *Sender,
          TShiftState Shift, int X, int Y);
        void __fastcall cboxNamesClick(TObject *Sender);
        void __fastcall cboxIdClick(TObject *Sender);
        void __fastcall cbLinksChange(TObject *Sender);
        void __fastcall cbDrawChange(TObject *Sender);
        void __fastcall cbStyleChange(TObject *Sender);
private:	// User declarations
       int mStartBg, mSelected;
       int mStartX, mStartY, mOldX, mOldY;

       int  GetBGX(int _bg);
       int  GetBGY(int _bg);

       int  BGroupAtPos(int _x, int _y);

       void DrawBGroups();
       void DrawRelations();
       void DrawAsGraph1(int _xs, int _ys, int _xe, int _ye, int _dist, TColor _color);
       void DrawAsGraph2(int _xs, int _ys, int _xe, int _ye, int _dist, TColor _color);
       void DrawArrow(int _xs, int _ys, int _xe, int _ye, float _angle, float _dist, TColor _color);

public:		// User declarations
        __fastcall TFormPolitics(TComponent* Owner);
};
//---------------------------------------------------------------------------
extern PACKAGE TFormPolitics *FormPolitics;
//---------------------------------------------------------------------------
#endif
