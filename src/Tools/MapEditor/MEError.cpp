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

 descrition : un mic handler de errori
 last modify: 15 11 99
        by Grab
---------------------------------------------------------------------------*/
//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop

#include "MEError.h" 

int ME_ERROR = ERR_NONE;
UnicodeString Errors[MAXERRORS] = {"No error", "Error loading/reading from file!",
                                "Error saving/writing into file!", "Error openning file!",
                                "Error allocating memory!"};
UnicodeString additionalError;

UnicodeString compileErrors[MAXCOMPILEERRORS] = {"Errors: none", "Error: No drop ship"};
UnicodeString compileWarnings[MAXCOMPILEWARNINGS] = {"Warnings: none", "Warning: No enemy"};

/*---------------------------------------------------------------------------
 descrition: sseteaza eroarea globala
---------------------------------------------------------------------------*/
void setMEError(int error)
{
   additionalError = " ";
   ME_ERROR        = error;
}

/*---------------------------------------------------------------------------
 descrition: sseteaza eroarea globala
---------------------------------------------------------------------------*/
void setMEError(int error, UnicodeString additional)
{
   ME_ERROR        = error;
   additionalError = additional;
}

/*---------------------------------------------------------------------------
 descrition: un fel de n-ar mai fi
---------------------------------------------------------------------------*/
void printMEError()
{
   if (ME_ERROR > 0)
      Application->MessageBox((additionalError + L" : " + Errors[ME_ERROR]).c_str(), L"Error", MB_OK);
}

/*---------------------------------------------------------------------------
 descrition: tipareste eroarea si un text
---------------------------------------------------------------------------*/
void printMEError(UnicodeString text)
{
   if (ME_ERROR > 0)
      Application->MessageBox((text + L" : " + Errors[ME_ERROR]).c_str(), L"Error", MB_OK);
}

//---------------------------------------------------------------------------
#pragma package(smart_init)
