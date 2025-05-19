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

#include <vcl.h>
#pragma hdrstop
#include <tchar.h>

#include "easylogging++.h"
INITIALIZE_EASYLOGGINGPP
//---------------------------------------------------------------------------

USEFORM("..\..\..\..\src\Tools\MapChunkEd\FMapChunk.cpp", FormMain);
USEFORM("..\..\..\..\src\Tools\MapChunkEd\FAbout.cpp", FormAbout);
//---------------------------------------------------------------------------

int WINAPI _tWinMain(HINSTANCE, HINSTANCE, LPTSTR, int)
{
    try
    {
         Application->Initialize();
         Application->MainFormOnTaskBar = true;
         Application->CreateForm(__classid(TFormMain), &FormMain);
         Application->CreateForm(__classid(TFormAbout), &FormAbout);
         Application->Run();
    }
    catch (Exception &exception)
    {
         Application->ShowException(&exception);
    }
    catch (...)
    {
         try
         {
             throw Exception("");
         }
         catch (Exception &exception)
         {
             Application->ShowException(&exception);
         }
    }
    return 0;
}
//---------------------------------------------------------------------------
