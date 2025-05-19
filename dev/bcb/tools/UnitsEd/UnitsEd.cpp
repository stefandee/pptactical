//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop
#include <tchar.h>

#include "easylogging++.h"
INITIALIZE_EASYLOGGINGPP

//---------------------------------------------------------------------------
USEFORM("..\..\..\..\src\Tools\UnitsEd\AirSkelEd.cpp", AirSkelEditor);
USEFORM("..\..\..\..\src\Tools\UnitsEd\FObservationPointEd.cpp", FormObservationPointEditor);
USEFORM("..\..\..\..\src\Tools\UnitsEd\FUnitsAbout.cpp", FormAbout);
USEFORM("..\..\..\..\src\Tools\UnitsEd\FUnitsPrev.cpp", FormPreview);
USEFORM("..\..\..\..\src\Tools\UnitsEd\InfSkelEd.cpp", InfSkelEditor);
USEFORM("..\..\..\..\src\Tools\UnitsEd\m_aiunits.cpp", FormMain);
USEFORM("..\..\..\..\src\Tools\UnitsEd\VSkelEd.cpp", VSkelEditor);
//---------------------------------------------------------------------------
int WINAPI _tWinMain(HINSTANCE, HINSTANCE, LPTSTR, int)
{
    try
    {
         Application->Initialize();
         Application->MainFormOnTaskBar = true;
         Application->CreateForm(__classid(TFormMain), &FormMain);
         Application->CreateForm(__classid(TAirSkelEditor), &AirSkelEditor);
         Application->CreateForm(__classid(TFormObservationPointEditor), &FormObservationPointEditor);
         Application->CreateForm(__classid(TFormAbout), &FormAbout);
         Application->CreateForm(__classid(TFormPreview), &FormPreview);
         Application->CreateForm(__classid(TInfSkelEditor), &InfSkelEditor);
         Application->CreateForm(__classid(TVSkelEditor), &VSkelEditor);
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
