//-----------------------------------------------------------------------------
// This file is part of PPTactical Engine.                                   //
//                                                                           //
// PPTactical Engine - engine for tactical/strategy games                    //
// Copyright (C) 1998-2025 Stefan Dicu/Piron Games                           //
//                                                                           //
// This library is free software; you can redistribute it and/or             //
// modify it under the terms of the GNU Lesser General Public                //
// License as published by the Free Software Foundation; either              //
// version 2.1 of the License, or (at your option) any later version.        //
//                                                                           //
// This library is distributed in the hope that it will be useful,           //
// but WITHOUT ANY WARRANTY; without even the implied warranty of            //
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU         //
// Lesser General Public License for more details.                           //
//                                                                           //
// You should have received a copy of the GNU Lesser General Public          //
// License along with this library; if not, write to the Free Software       //
// Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA //
//-----------------------------------------------------------------------------

/*---------------------------------------------------------------------------
 application: Tile Editor

 descrition : Form Main
 last modify: 02 09 2000
        by Grab
---------------------------------------------------------------------------*/
//---------------------------------------------------------------------------
#ifndef FMapChunkH
#define FMapChunkH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <Buttons.hpp>
#include <Dialogs.hpp>
#include <ExtCtrls.hpp>
#include <ExtDlgs.hpp>
#include <Menus.hpp>
#include <ComCtrls.hpp>
//---------------------------------------------------------------------------

#include "Tile.h"
#include "basic2d.h"

class TFormMain : public TForm
{
__published:	// IDE-managed Components
   TMainMenu *menuMain;
   TMenuItem *mitFile;
   TMenuItem *itNew;
   TMenuItem *itLoad;
   TMenuItem *itSave;
   TMenuItem *itSaveAs;
   TMenuItem *N1;
   TMenuItem *itExit;
   TMenuItem *mitHelp;
   TMenuItem *itHelp;
   TMenuItem *itAbout;
   TOpenPictureDialog *dialogOpenPicture;
   TOpenDialog *dialogOpen;
   TSaveDialog *dialogSave;
   TPopupMenu *popupMain;
        TMenuItem *Addwater1;
   TMenuItem *itThumbnail;
   TMenuItem *N2;
        TMenuItem *Addshore1;
        TMenuItem *Addground1;
        TMenuItem *ClearTile1;
        TMenuItem *N3;
        TGroupBox *gbSettings;
        TLabel *lbKind;
        TComboBox *comboSize;
        TComboBox *comboAppearance;
        TLabel *lbAppearance;
        TStatusBar *statusMain;
        TGroupBox *gbImage;
        TBitBtn *btnSprite;
        TEdit *edName;
        TLabel *Label1;
        TMenuItem *N4;
        TMenuItem *Import1;
    TPaintBox *PaintBox;
   void __fastcall itExitClick(TObject *Sender);
   void __fastcall btnSpriteClick(TObject *Sender);
   void __fastcall itNewClick(TObject *Sender);
   void __fastcall itLoadClick(TObject *Sender);
   void __fastcall itSaveClick(TObject *Sender);
   void __fastcall itHelpClick(TObject *Sender);
   void __fastcall itAboutClick(TObject *Sender);
   void __fastcall itSaveAsClick(TObject *Sender);
   void __fastcall FormCreate(TObject *Sender);
        void __fastcall Addshore1Click(TObject *Sender);
        void __fastcall Addwater1Click(TObject *Sender);
        void __fastcall Addground1Click(TObject *Sender);
        void __fastcall ClearTile1Click(TObject *Sender);
        void __fastcall comboSizeChange(TObject *Sender);
        void __fastcall edNameExit(TObject *Sender);
        void __fastcall Import1Click(TObject *Sender);
    void __fastcall comboSizeCloseUp(TObject *Sender);
    void __fastcall PaintBoxMouseUp(TObject *Sender, TMouseButton Button,
          TShiftState Shift, int X, int Y);
    void __fastcall PaintBoxMouseMove(TObject *Sender, TShiftState Shift,
          int X, int Y);
    void __fastcall PaintBoxPaint(TObject *Sender);
private:	// User declarations
   bool         mModified;
   AnsiString   mFileName;
   CAuxMapChunk *mMapChunk;
   CPPoint      mSquare;   //x,y - linie si col

   void PaintMapChunk();

public:		// User declarations
   __fastcall TFormMain(TComponent* Owner);
   void SetTileName(AnsiString tilename);
   void SaveMapChunk(AnsiString filename);

   //bool GetModified() { return mModified; }
   //void SetModified(bool _v) { mModified = _v; }

   //CPPoint GetSquare() { return mSquare; }
   //void    SetSquare(CPPoint _v) { mSquare = _v; }
};
//---------------------------------------------------------------------------
extern PACKAGE TFormMain *FormMain;
//---------------------------------------------------------------------------
#endif
