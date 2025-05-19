//---------------------------------------------------------------------------

#include <vcl.h>
#include "easylogging++.h"
#pragma hdrstop
#include <tchar.h>

INITIALIZE_EASYLOGGINGPP

USEFORM("..\..\..\..\src\Tools\BuildingEd\FToolProp.cpp", FormToolProp);
USEFORM("..\..\..\..\src\Tools\BuildingEd\masked.cpp", MaskForm);
USEFORM("..\..\..\..\src\Tools\BuildingEd\m_sless.cpp", MainForm);
USEFORM("..\..\..\..\src\Tools\BuildingEd\FSprSetup.cpp", FormSprSetup);
USEFORM("..\..\..\..\src\Tools\BuildingEd\FAbout.cpp", FormAbout);
USEFORM("..\..\..\..\src\Tools\BuildingEd\FObservationPointEd.cpp", FormObservationPointEditor);
//---------------------------------------------------------------------------
int WINAPI _tWinMain(HINSTANCE, HINSTANCE, LPTSTR, int)
{
    try
    {
         Application->Initialize();
         Application->MainFormOnTaskBar = true;
         Application->CreateForm(__classid(TMainForm), &MainForm);
         Application->CreateForm(__classid(TFormAbout), &FormAbout);
         Application->CreateForm(__classid(TFormObservationPointEditor), &FormObservationPointEditor);
         Application->CreateForm(__classid(TFormSprSetup), &FormSprSetup);
         Application->CreateForm(__classid(TFormToolProp), &FormToolProp);
         Application->CreateForm(__classid(TMaskForm), &MaskForm);
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
