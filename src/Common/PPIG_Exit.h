//-----------------------------------------------------------------------------
// This file is part of PPTactical Engine.                                   //
//                                                                           //
// PPTactical Engine - engine for tactical/strategy games                    //
// Copyright (C) 1998, 1999, 2000, 2001 Stefan Dicu & Tudor Girba            //
//                                                                           //
// PPTactical Engine is free software; you can redistribute it               //
// and/or modify it under the terms of the GNU Lesser General Public License as     //
// published by the Free Software Foundation; either version 2.1 of the        //
// License, or (at your option) any later version.                           //
//                                                                           //
// PPTactical Engine is distributed in the hope that it will be useful, but  //
// WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY//
// or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU Lesser General Public License  //
// for more details.                                                         //
//                                                                           //
// You should have received a copy of the GNU Lesser General Public License         //
// along with PPTactical Engine; if not, write to the Free Software          //
// Foundation Inc. 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA   //
//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
//  Unit              : Class Piron Exit
//
//  Versiune          : 1.0
//
//  Descriere         :
//    * o clasa care sa interfateze o singura variabila, pentru iesire
//    * continutul ei va fi testat de FormInGame/PPEngine_Win si daca variabila e true, se iese din program
//    * continutul ei va fi setat de CPIMainScreen, in caz ca se doreste iesirea din program
//    * am scris clasa pentru a nu exista cod specific vreunui compilator (initial exista o referinta la
//      FormInGame din CPIMainScreen) - practic se face un soi de decuplare
//    * clasa implementeaza un singleton pattern (evident, o singura instanta/aplicatie)
//
//  Istorie           :
//    [25.10.2001] - [Karg] - scris clasa
//-----------------------------------------------------------------------------

#ifndef PPIG_ExitH
#define PPIG_ExitH
//---------------------------------------------------------------------------

class CPExit
{
  public:
    static CPExit* Instance();
    static void Release();

  protected:
    static CPExit* mInstance;

  public:
    CPExit() { mTerminated = false; }

    void SetTerminated(bool _value) { mTerminated = _value; }
    bool GetTerminated()            { return mTerminated; }

  private:
    bool mTerminated;
};

extern CPExit* GetExitInstance();

#endif
