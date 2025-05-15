//---------------------------------------------------------------------------

#ifndef FEditObjectStreamH
#define FEditObjectStreamH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <Menus.hpp>
#include <ExtCtrls.hpp>
#include <ComCtrls.hpp>
#include <ToolWin.hpp>
#include <Grids.hpp>
#include "ObjectStream.h"
//---------------------------------------------------------------------------
class TFormEditObjectStream : public TForm
{
__published:	// IDE-managed Components
        TMainMenu *MainMenu;
        TMenuItem *File1;
        TMenuItem *Open1;
        TMenuItem *Save1;
        TMenuItem *About1;
        TMenuItem *About2;
        TToolBar *ToolBar;
        TToolButton *ToolButton1;
        TToolButton *ToolButton2;
        TToolButton *ToolButton3;
        TStatusBar *StatusBar;
        TStringGrid *StringGrid;
        TPopupMenu *PopupMenu;
        TMenuItem *AddRecord1;
        TMenuItem *RemoveRecord1;
        TMenuItem *N1;
        TMenuItem *Save2;
        TMenuItem *SaveAs1;
        TMenuItem *Load1;
        TMenuItem *Clear1;
        TMenuItem *N2;
        TMenuItem *Exit1;
        TMenuItem *InsertRecord1;
private:	// User declarations
        bool mModified;

protected:
        virtual void SetModified(bool _v);
        virtual bool GetModified();

        virtual void SetupStringList();

        // a vector must be instantiated here - containing objects
        // of the base class; this is the memory representation of
        // the objectstream (e.g. operations will be performed in memory,
        // then written to disk)
        // std::vector<BaseClass*> mObjects;

public:		// User declarations
        __fastcall TFormEditObjectStream(TComponent* Owner);
};
//---------------------------------------------------------------------------
extern PACKAGE TFormEditObjectStream *FormEditObjectStream;
//---------------------------------------------------------------------------
#endif
