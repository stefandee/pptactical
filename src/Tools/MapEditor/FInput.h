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
/*---------------------------------------------------------------------------
 application: PP Mission Editor

 description: Input Form 
 last modify: 16 03 00
        by grabX
---------------------------------------------------------------------------*/
//---------------------------------------------------------------------------
#ifndef FInputH
#define FInputH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <Buttons.hpp>
#include "CSPIN.h"
#include <Dialogs.hpp>

#define MAXINPUTVALUES 20
#define FREESPACE      15 

//---------------------------------------------------------------------------
class TFormInput : public TForm
{
__published:	// IDE-managed Components
   TBitBtn *btnOk;
   TBitBtn *btnCancel;
private:	// User declarations
   int getValueHandler(AnsiString valuename);
public:		// User declarations
   AnsiString inputValues[MAXINPUTVALUES];   //valorile introduse
   TObject *tagObjects[MAXINPUTVALUES];      //obiecte - folosite de input controls
   TObject *tagRelatedObjects[MAXINPUTVALUES]; //obiecte folosite de alte controale
   int     valueCount; 

   int inputControlCount;     //numarul de controale
   int tagRelatedCount;
   int nextControlTop;        //arata unde trebuie pus urmatorul control

   __fastcall TFormInput(TComponent* Owner);
   void Format(AnsiString title, AnsiString format, AnsiString initial);
   void createInputControl(AnsiString title, AnsiString type,
                           AnsiString range, AnsiString initvalue);
   void __fastcall controlValueChange(TObject *Sender);

   void createINT(TControl **control, AnsiString title,
                   AnsiString range, AnsiString initvalue);

   void createLONG(TControl **control, AnsiString title,
                   AnsiString range, AnsiString initvalue);

   void createBOOL(TControl **control, AnsiString title,
                   AnsiString range, AnsiString initvalue);

   void createTEXT(TControl **control, AnsiString title,
                   AnsiString range, AnsiString initvalue);

   void createRELATION(TControl **control, AnsiString title,
                   AnsiString range, AnsiString initvalue);

   void createFILE(TControl **control, AnsiString title,
                   AnsiString range, AnsiString initvalue);
   void __fastcall getFileFromDialog(TObject *Sender);

};
//---------------------------------------------------------------------------
extern PACKAGE TFormInput *FormInput;

void createInputForm(AnsiString title, AnsiString format, AnsiString initial);


//---------------------------------------------------------------------------
#endif
