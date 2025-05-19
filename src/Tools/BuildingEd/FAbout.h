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
 application: PP Units Editor

 descrition : about
 last modify: 4 12 1999
        by Grab
---------------------------------------------------------------------------*/
//---------------------------------------------------------------------------
#ifndef FAboutH
#define FAboutH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <ExtCtrls.hpp>
#include "THttpLabel.h"
//---------------------------------------------------------------------------
class TFormAbout : public TForm
{
__published:	// IDE-managed Components
   TButton *btnOk;
   TPanel *plAbout;
   TLabel *Label1;
   TLabel *Label2;
   TLabel *lbVersion;
        THttpLabel *HttpLabel1;
private:	// User declarations
public:		// User declarations
   __fastcall TFormAbout(TComponent* Owner);
};
//---------------------------------------------------------------------------
extern PACKAGE TFormAbout *FormAbout;
//---------------------------------------------------------------------------
#endif
