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
//---------------------------------------------------------------------------
#ifndef maskedH
#define maskedH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <ExtCtrls.hpp>
#include "ImageEx.h"
#include <Menus.hpp>
#include <ComCtrls.hpp>
#include <ToolWin.hpp>
#include <ImgList.hpp>
#include <Vcl.Imaging.pngimage.hpp>
//---------------------------------------------------------------------------

#define TRANSPARENT_PERCENT 0.95

enum TToEdit { TOEDIT_MASK, TOEDIT_ALT, TOEDIT_LAYER, TOEDIT_POINT_FIRE, TOEDIT_POINT_VISUAL, TOEDIT_POINT_DROP, TOEDIT_NOTHING};

class TMaskForm : public TForm
{
__published:	// IDE-managed Components
        TPopupMenu *PopupMenu;
        TMenuItem *AutoMask1;
        TMenuItem *N1;
        TMenuItem *ExitMaskEditor1;
        TPaintBox *paintBox;
    TStatusBar *StatusBar1;
    TToolBar *ToolBar1;
        void __fastcall FormShow(TObject *Sender);
        void __fastcall ExitMaskEditor1Click(TObject *Sender);
        void __fastcall AutoMask1Click(TObject *Sender);
        void __fastcall FormHide(TObject *Sender);
        void __fastcall paintBoxPaint(TObject *Sender);
        void __fastcall paintBoxMouseDown(TObject *Sender,
          TMouseButton Button, TShiftState Shift, int X, int Y);

private:	// User declarations
        TPngImage *Image;
        TBitmap* MaskTile;
        TToEdit GetToEdit();
        void PaintMask();

public:		// User declarations
        __fastcall TMaskForm(TComponent* Owner);
        void       DarkRect(TCanvas* canvas, int xs, int ys, int xe, int ye, int darkSide);
        bool       IsTransparentRegion(TCanvas* canvas, int xs, int ys, int xe, int ye);
        void       AutoMask();
        void       RepaintBox();
        void       InitMask();
};
//---------------------------------------------------------------------------
extern PACKAGE TMaskForm *MaskForm;
//---------------------------------------------------------------------------
#endif
