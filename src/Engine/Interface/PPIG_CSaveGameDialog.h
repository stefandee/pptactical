//-----------------------------------------------------------------------------
// This file is part of PPTactical Engine.                                   //
//                                                                           //
// PPTactical Engine - engine for tactical/strategy games                    //
// Copyright (C) 1998-2004 Stefan Dicu & Tudor Girba                         //
//                                                                           //
// PPTactical Engine is free software; you can redistribute it               //
// and/or modify it under the terms of the GNU Lesser General Public License //
// as published by the Free Software Foundation; either version 2.1 of the   //
// License, or (at your option) any later version.                           //
//                                                                           //
// PPTactical Engine is distributed in the hope that it will be useful, but  //
// WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY//
// or FITNESS FOR A PARTICULAR PURPOSE. See the GNU Lesser General Public    //
// License for more details.                                                 //
//                                                                           //
// You should have received a copy of the GNU Lesser General Public License  //
// along with PPTactical Engine; if not, write to the Free Software          //
// Foundation Inc. 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA   //
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
//  Unit              : PPIG_CSaveGameDialog
//
//  Version           : 1.0
//
//  Description       :
//    * implements a save game dialog
//    * the user is displayed the list of saved games; info about the saved game
//      is displayed (time/date, screenshot, other info)
//    * the user may save, delete or cancel this dialog
//    * need to refactor to merge the common features of this class with CPP_CLoadGameDialog
//      and CPP_CSFormDialog
//
//  History           :
//    [14.09.2005] - [Karg] - unit created
//-----------------------------------------------------------------------------

//---------------------------------------------------------------------------

#ifndef PPIG_CSaveGameDialogH
#define PPIG_CSaveGameDialogH
//---------------------------------------------------------------------------

#include "PPIG_CBaseSaveGameDialog.h"

class CPP_CSaveGameDialog : public CPP_CBaseSaveGameDialog
{
  public:
    CPP_CSaveGameDialog(CPIWinControl *Parent = NULL);

    CPString GetFileName();
    void SetFileName(CPString fileName);
    virtual bool FileExists();

    virtual ~CPP_CSaveGameDialog();

  protected:
    virtual void SaveListClick(CPIObject *Sender, int X, int Y);

  private:
    CPIEdit*        mEdSaveName;
};

#endif
