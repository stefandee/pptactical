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

USEFORM("..\..\..\..\src\Tools\MapEditor\FNew.cpp", FormNewMap);
USEFORM("..\..\..\..\src\Tools\MapEditor\FMissionScript.cpp", FormMissionScript);
USEFORM("..\..\..\..\src\Tools\MapEditor\FPolitics.cpp", FormPolitics);
USEFORM("..\..\..\..\src\Tools\MapEditor\FObjProp.cpp", FormObjManager);
USEFORM("..\..\..\..\src\Tools\MapEditor\FMissionInfo.cpp", FormMissionInfo);
USEFORM("..\..\..\..\src\Tools\MapEditor\FInput.cpp", FormInput);
USEFORM("..\..\..\..\src\Tools\MapEditor\FInfo.cpp", FormInfo);
USEFORM("..\..\..\..\src\Tools\MapEditor\FMapGen.cpp", FormMapGen);
USEFORM("..\..\..\..\src\Tools\MapEditor\FMain.cpp", FormMain);
USEFORM("..\..\..\..\src\Tools\MapEditor\FPref.cpp", FormPreferences);
USEFORM("..\..\..\..\src\Tools\MapEditor\FTree.cpp", FormTree);
USEFORM("..\..\..\..\src\Tools\MapEditor\FRadar.cpp", FormRadar);
USEFORM("..\..\..\..\src\Tools\MapEditor\FPreview.cpp", FormPreview);
USEFORM("..\..\..\..\src\Tools\MapEditor\FTool.cpp", FormTool);
USEFORM("..\..\..\..\src\Tools\MapEditor\FTMP.cpp", FormObjectManager);
USEFORM("..\..\..\..\src\Tools\MapEditor\FGroup.cpp", FormGroup);
USEFORM("..\..\..\..\src\Tools\MapEditor\FBGEdit.cpp", FormBGEdit);
USEFORM("..\..\..\..\src\Tools\MapEditor\FBayManager.cpp", FormBayManager);
USEFORM("..\..\..\..\src\Tools\MapEditor\FEditCurve2D.cpp", FormEditCurve2D);
USEFORM("..\..\..\..\src\Tools\MapEditor\FCustDlg.cpp", FormDialog);
USEFORM("..\..\..\..\src\Tools\MapEditor\FAlt.cpp", FormAltitude);
USEFORM("..\..\..\..\src\Tools\MapEditor\DataM.cpp", Datas); /* TDataModule: File Type */
USEFORM("..\..\..\..\src\Tools\MapEditor\FAbout.cpp", FormAbout);
USEFORM("..\..\..\..\src\Tools\MapEditor\ecosysconfig\FEcoSysGenRandomConfig.cpp", FormEcoSysGenRandomConfig);
USEFORM("..\..\..\..\src\Tools\MapEditor\ecosysconfig\FEcoSysGenKernelConfig.cpp", FormEcoSysGenKernelConfig);
//---------------------------------------------------------------------------
int WINAPI _tWinMain(HINSTANCE, HINSTANCE, LPTSTR, int)
{
    try
    {
         Application->Initialize();
         Application->MainFormOnTaskBar = true;
         Application->CreateForm(__classid(TFormMain), &FormMain);
         Application->CreateForm(__classid(TFormAbout), &FormAbout);
         Application->CreateForm(__classid(TFormAltitude), &FormAltitude);
         Application->CreateForm(__classid(TFormBayManager), &FormBayManager);
         Application->CreateForm(__classid(TFormBGEdit), &FormBGEdit);
         Application->CreateForm(__classid(TFormDialog), &FormDialog);
         Application->CreateForm(__classid(TFormEditCurve2D), &FormEditCurve2D);
         Application->CreateForm(__classid(TFormGroup), &FormGroup);
         Application->CreateForm(__classid(TFormInfo), &FormInfo);
         Application->CreateForm(__classid(TFormInput), &FormInput);
         Application->CreateForm(__classid(TFormMapGen), &FormMapGen);
         Application->CreateForm(__classid(TFormMissionInfo), &FormMissionInfo);
         Application->CreateForm(__classid(TFormMissionScript), &FormMissionScript);
         Application->CreateForm(__classid(TFormNewMap), &FormNewMap);
         Application->CreateForm(__classid(TFormObjManager), &FormObjManager);
         Application->CreateForm(__classid(TFormPolitics), &FormPolitics);
         Application->CreateForm(__classid(TFormPreferences), &FormPreferences);
         Application->CreateForm(__classid(TFormPreview), &FormPreview);
         Application->CreateForm(__classid(TFormRadar), &FormRadar);
         Application->CreateForm(__classid(TFormObjectManager), &FormObjectManager);
         Application->CreateForm(__classid(TFormTool), &FormTool);
         Application->CreateForm(__classid(TFormTree), &FormTree);
         Application->CreateForm(__classid(TFormEcoSysGenKernelConfig), &FormEcoSysGenKernelConfig);
         Application->CreateForm(__classid(TFormEcoSysGenRandomConfig), &FormEcoSysGenRandomConfig);
         Application->CreateForm(__classid(TDatas), &Datas);
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
