//-----------------------------------------------------------------------------
// This file is part of PPTactical Engine.                                   //
//                                                                           //
// PPTactical Engine - engine for tactical/strategy games                    // 
// Copyright (C) 1998, 1999, 2000, 2001 Stefan Dicu & Tudor Girba            //
//                                                                           //
// PPTactical Engine is free software; you can redistribute it               //
// and/or modify it under the terms of the GNU General Public License as     // 
// published by the Free Software Foundation; either version 2 of the        //
// License, or (at your option) any later version.                           //
//                                                                           //  
// PPTactical Engine is distributed in the hope that it will be useful, but  //
// WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY// 
// or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License  //
// for more details.                                                         //
//                                                                           // 
// You should have received a copy of the GNU General Public License         //
// along with PPTactical Engine; if not, write to the Free Software          //
// Foundation Inc. 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA   // 
//-----------------------------------------------------------------------------

#ifndef FMapGenH
#define FMapGenH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <ComCtrls.hpp>
#include <ExtCtrls.hpp>

#include "Tile.h"
#include <Buttons.hpp>
#include <Dialogs.hpp>

class EcoSysGen;
#define MAXTILELENGTH 200

//---------------------------------------------------------------------------
class TFormMapGen : public TForm
{
__published:	// IDE-managed Components
        TGroupBox *GroupBox1;
        TButton *btnCancelGenerate;
        TButton *btnOkGenerate;
        TPageControl *pcGenOptions;
        TTabSheet *TabSheet1;
        TTabSheet *TabSheet2;
        TTabSheet *TabSheet3;
        TPaintBox *paintHeight;
        TButton *btnHMPreview;
        TImage *imgPreview;
        TLabel *Label1;
        TLabel *lbRate;
        TLabel *lbPreview;
        TLabel *lbSetRate;
        TListBox *listTile;
        TListBox *listRate;
        TComboBox *comboRate;
        TBitBtn *btnAdd;
        TBitBtn *btnDelete;
        TBitBtn *btnClear;
        TOpenDialog *dialogOpen;
        TTimer *Timer;
        TStatusBar *statusMain;
        TComboBox *ComboBox1;
        TLabel *Label2;
        TLabel *Label3;
        TComboBox *ComboBox2;
        TButton *Button1;
        TButton *Button2;
        TLabel *Label4;
        TComboBox *cbEcoSysGenAlg;
        TButton *btnEcoSysConfig;
        TPaintBox *paintEcoSys;
        TButton *btnEcoSysPreview;
        TButton *btnEcoSysGen;
        void __fastcall btnAddClick(TObject *Sender);
        void __fastcall btnClearClick(TObject *Sender);
        void __fastcall listTileClick(TObject *Sender);
        void __fastcall listRateClick(TObject *Sender);
        void __fastcall comboRateChange(TObject *Sender);
        void __fastcall TimerTimer(TObject *Sender);
        void __fastcall btnOkGenerateClick(TObject *Sender);
        void __fastcall btnDeleteClick(TObject *Sender);
        void __fastcall btnEcoSysConfigClick(TObject *Sender);
        void __fastcall btnEcoSysPreviewClick(TObject *Sender);
        void __fastcall FormShow(TObject *Sender);
        void __fastcall cbEcoSysGenAlgChange(TObject *Sender);
        void __fastcall btnEcoSysGenClick(TObject *Sender);

private:	// tile generation
   TList *mMapChunkList;

   void setSelectedItem(int index);
   void addTile(CAuxMapChunk* tileobj);
   void removeTile(int index);
   void removeAllTiles();
   int  ComputeSize();
   bool GenerateTiles();
   void PreviewTile(CAuxMapChunk*);

private:        // terrain generation

private:        // ecosys generation
   EcoSysGen* mEcoSysGen;
   void CreateEcoSysGen();

public:		// User declarations
        __fastcall TFormMapGen(TComponent* Owner);
        void       Initialize(AnsiString landscape);
        bool       Generate();
};
//---------------------------------------------------------------------------
extern PACKAGE TFormMapGen *FormMapGen;
//---------------------------------------------------------------------------
#endif
