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
#ifndef MEErrorH
#define MEErrorH 

#define MAXERRORS          5
#define MAXCOMPILEERRORS   2
#define MAXCOMPILEWARNINGS 2


extern int ME_ERROR;       //variabila globala care arata eroarea data
extern UnicodeString Errors[MAXERRORS];
extern UnicodeString additionalError;

extern UnicodeString compileErrors[MAXCOMPILEERRORS];
extern UnicodeString compileWarnings[MAXCOMPILEWARNINGS];
//errors
#define ERR_NONE     0
#define ERR_LOAD     1
#define ERR_READ     1
#define ERR_SAVE     2
#define ERR_WRITE    2
#define ERR_OPEN     3
#define ERR_ALLOCATE 4

//compile errors
#define CERR_NONE          0
#define CERR_NODROPSHIP    1

//compile warnings
#define CWAR_NONE          0
#define CWAR_NOENEMY       1


void setMEError(int error);
void setMEError(int error, UnicodeString additional);
void printMEError();
void printMEError(UnicodeString text);

//---------------------------------------------------------------------------
#endif
