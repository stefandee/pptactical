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
//---------------------------------------------------------------------------
#include <stdlib.h>
#pragma hdrstop
//---------------------------------------------------------------------------

// Includes
//#define ELPP_NO_DEFAULT_LOG_FILE
//#define ELPP_NO_LOG_TO_FILE
#include "easylogging++.h"
INITIALIZE_EASYLOGGINGPP

#include "basepath.h"
#include "physfs.h"
#include "Paths.h"         //pentru const de fisiere
#include "PP_Defines.h"    //pentru constante globale
#include "PP_Input.h"      //pentru initializarea lui DInput
#include "PPIG_Cursor.h"   //pentru initializarea lui Mouse
#include "PP_Keyboard.h"     //pentru initializarea lui Keyboard
#include "PP_Sound.h"      //pentru initializarea lui SoundSystem
#include "PPIG_SpaceSound.h"
#include "PP_Graphic.h"    //pentru intializarea lui graphic system
#include "PPIG_CScreen.h"  //pentru intializarea lui main screen
#include "PPIG_Mission.h"  //pentru intializarea misiunii
#include "PPIG_MissionDiskOp.h"
#include "PPIG_PathServer.h"
#include "PPIG_Exit.h"     // pentru iesirea din program
#include "PPIG_Map.h"
#include "PPIG_CBattleControl.h"
#include "PPIG_CRadar.h"
#include "PP_Stdlib.h"
#include "PlayerProfile.h"
#include "EngineConfig.h"

#include "PP_VfsSystem.h"
#include "Stream_PhysFS.h"

// TMN: Special during development. Remove when complete.
//#pragma comment(lib, "P:/SDL-1.2.4/VisualC/SDL/Debug/SDL.lib")
//#pragma comment(lib, "P:/SDL-1.2.4/VisualC/SDLmain/Debug/SDLmain.lib")

// Variables declaration
namespace {
int      gLastTick = 0;
int      gUpdatePeriod = 33;
//HWND     ghWnd;
}

// Functions
bool InitVfs();
void InitLog();
void ConfigLog(EngineConfig *engineConfig);
bool InitEngine();
bool InitGraphicsSDL(EngineConfig *engineConfig);
bool InitSound();
bool InitInput();
bool InitScreen();
bool InitMission(EngineConfig *engineConfig);
bool InitPathServer();
bool InitLoop(EngineConfig *engineConfig);
bool InitPlayerProfile();
void Terminate();
bool LoopInGame();

CPGISprite* gSprite;

// Implementation
//int main(int argc, char *argv[])
int APIENTRY WinMain( HINSTANCE hInst, HINSTANCE hPrevInst, LPSTR pCmdLine, int nCmdShow )
{
    // needs to be before any SDL init, otherwise gdb will crash randomly
    SDL_SetHint(SDL_HINT_WINDOWS_DISABLE_THREAD_NAMING, "1");

    srand(SDL_GetTicks());

    if (SDL_Init(SDL_INIT_EVERYTHING & ~SDL_INIT_HAPTIC) != 0)
    {
        return 1;
    }

    try
    {
        if (!InitEngine()) {
            return 2;
        }
    }
    catch(...)
    {
      return 2;
    }

    SDL_ShowCursor(SDL_DISABLE);

    for (;;)
    {
        SDL_PumpEvents();

        /*(SDL_GetAppState() & SDL_APPINPUTFOCUS) != 0*/
        if (GetGraphicInstance()->IsWindowActive() )
        {
            if (!LoopInGame() || GetExitInstance()->GetTerminated())
            {
              Terminate();
              return 0;
            }
#if defined _WIN32
            // Special handling to not let the mouse cursor out of the window
            // NOTE: This isn't 100% correct when starting the app Windowed.
            HWND hWnd = GetActiveWindow();
            RECT rc;
            GetWindowRect(hWnd, &rc);
            //rc.left   += GetSystemMetrics(SM_CXFIXEDFRAME);
            //rc.right  -= GetSystemMetrics(SM_CXFIXEDFRAME);
            rc.top    += GetSystemMetrics(SM_CYCAPTION) +
                         GetSystemMetrics(SM_CYFIXEDFRAME);;
            rc.bottom -= GetSystemMetrics(SM_CYFIXEDFRAME);
            ClipCursor(&rc);
            ShowCursor(false);
#endif
        } else {
            // Sleep in 100ms intervals. In plain Win32 we would
            // have called WaitMessage() to let the system wake us
            // up when we got something to do.
            // solaris port

            #if defined (_WIN32)
	      ClipCursor(NULL);
            #endif

            //SDL_Delay(100);
//          SDL_WaitEvent(0);
            // Ignore time spent inactive
            gLastTick = SDL_GetTicks();
		}
    }
}


/*---------------------------------------------------------------------------
 description: Initiates the engine
---------------------------------------------------------------------------*/
bool InitEngine()
{
   // iesirea din program
   GetExitInstance()->SetTerminated(false);

   // vfs init
   if (!InitVfs())
   {
       return false;
   }

   // log init
   InitLog();

#ifdef PP_TEST_VERSION
   initGamePath();
#endif

   //Config - init begin
   EngineConfig *engineConfig = NULL;
   try
   {
      engineConfig = new EngineConfig();
      if (!engineConfig->Load(Vfs()->GetBaseDir() + FILE_INGAME_CFG))
      {
         LOG(WARNING) << "---------- Unable to open config file ( " << FILE_INGAME_CFG << " ), will use default values!";
      }
   }
   catch(...)
   {
      LOG(FATAL) << "---------- UNABLE TO INITIALIZE CONFIG CLASS !!!!!!!!!!!";

      return false;
   }
   //Config - init end

   // config the log system
   ConfigLog(engineConfig);

   // log info about the vfs
   Vfs()->LogInfo();

   if (InitGraphicsSDL(engineConfig) == false)
   {
      Terminate();
      return false;
   }

   if (!InitSound())
   {
       Terminate();
       return false;
   }

   if (!InitInput())
   {
       Terminate();
       return false;
   }

   InitPlayerProfile();

   //Screen - init begin
   if (InitScreen() == false)
   {
      Terminate();
      return false;
   }
   //Screen - init end

   //Mission - init begin
   if (InitMission(engineConfig) == false)
   {
      Terminate();
      return false;
   }
   //Mission - init end

   //Path Server - init begin
   if (InitPathServer() == false)
   {
      Terminate();
      return false;
   }
   //Path Server - init end

   //Loop - init begin
   if (InitLoop(engineConfig) == false)
   {
      Terminate();
      return false;
   }
   //Loop - init end

   if (engineConfig)
   {
      delete engineConfig;
   }

   return true;
}

/*---------------------------------------------------------------------------
 description: initiaza log file
---------------------------------------------------------------------------*/
void InitLog()
{
   CPString logPath = Vfs()->GetWriteDir() + PATH_LOG;

   el::Configurations defaultConf;
   defaultConf.setToDefault();
   defaultConf.setGlobally(el::ConfigurationType::ToFile, "true");
   defaultConf.setGlobally(el::ConfigurationType::Filename, (logPath + "pptactical_engine.log").c_str());
   defaultConf.setGlobally(el::ConfigurationType::Format, "%datetime %level %msg");
   el::Loggers::reconfigureLogger("default", defaultConf);

   LOG(INFO) << "Starting Engine Log";
}
//---------------------------------------------------------------------------

/*---------------------------------------------------------------------------
 description: seteaza lowest log detail level
 parameters : Config - pointer la clasa unde s-a deschis fisierul de config
---------------------------------------------------------------------------*/
void ConfigLog(EngineConfig *engineConfig)
{
   int level = engineConfig->GetVerboseLogLevel();

    if (level > 0)
    {
        el::Loggers::setVerboseLevel(level);
    }
}
//---------------------------------------------------------------------------

bool InitVfs()
{
  GetVfsInstance();

  // make sure the logs dir exists in the write dir
  if (!Vfs()->EnsureWriteDir(PATH_LOG))
  {
      return false;
  }

  // well, if nothing else is wrong...
  return true;
}
//---------------------------------------------------------------------------

bool InitGraphicsSDL(EngineConfig *engineConfig)
{
   VLOG(2) << "SDL INITIALIZING ... ";

   try
   {
      GetGraphicInstance(engineConfig->GetResolutionX(),
                         engineConfig->GetResolutionY(),
                         engineConfig->GetFullscreen() ? PGI_FULLSCREEN : PGI_WINDOWED,
                         engineConfig->GetWindowTitle());
   }
   catch(...)
   {
      LOG(FATAL) << "UNABLE TO INITIALIZE SDL !!!!!!!!!!!";
      return false;
   }

   /* Initialize the TTF library */
   if ( TTF_Init() < 0 )
   {
     LOG(FATAL) << "Couldn't initialize TTF: " << SDL_GetError();
     return false;
   }

   VLOG(2) << "SDL INITIALIZED\n-------------------- ";
   return true;
}

/*---------------------------------------------------------------------------
 description: initiaza SoundSystem
---------------------------------------------------------------------------*/
bool InitSound()
{
   VLOG(2) << "AUDIO INITIALIZING ...";
   try
   {
      GetSoundInstance(/*ghWnd*/);
      GetSoundInstance()->SetMasterVolume(GetProfileInstance()->GetSFXVolume());
   }
   catch(...)
   {
      LOG(FATAL) << "UNABLE TO INITIALIZE AUDIO !!!!!!!!!!!";
      return false;
   }

   try
   {
      CIGSpaceSoundSystem::Instance();
	  /*
      CIGSpaceSoundSystem *lSSSI;
      lSSSI->Instance();
	  */
   }
   catch(...)
   {
      LOG(FATAL) << "UNABLE TO INITIALIZE SPACE SOUND SYSTEM !!!!!!!!!!!";
      return false;
   }

   VLOG(2) << "AUDIO INITIALIZED\n-------------------- ";
   return true;
}
//---------------------------------------------------------------------------

bool InitInput()
{
   VLOG(2) << "INPUT SYSTEM INITIALIZING ...";

   CPIIInputSystem* lISI;
   CPGIGraphicSystem* lGI;

   try
   {
      lGI = GetGraphicInstance();
      lISI = GetInputInstance(lGI->GetWindow());
   }
   catch(...)
   {
      LOG(FATAL) << "UNABLE TO INITIALIZE INPUT SYSTEM !!!!!!!!!!!";
      return false;
   }

   // at start-up, move the mouse in the middle of the screen
   lISI->SetMouseAt(lGI->GetResX() / 2, lGI->GetResY() / 2);

   VLOG(2) << "INPUT SYSTEM INITIALIZED\n-------------------- ";
   return true;
}
//---------------------------------------------------------------------------

bool InitPlayerProfile()
{
  VLOG(2) << "PLAYER PROFILE INITIALIZING\n-------------------- ";

  try
  {
    GetProfileInstance()->Load(Vfs()->GetWriteDir() + PATH_PLAYER_PROFILE + FILE_DEFAULT_PROFILE);
  }
  catch(...)
  {
    LOG(FATAL) << "UNABLE TO INITIALIZE PLAYER PROFILE !!!!!!!!!!!";
    return false;
  }

  VLOG(2) << "PLAYER PROFILE INITIALIZED\n-------------------- ";
  return true;
}
//---------------------------------------------------------------------------

/*---------------------------------------------------------------------------
 description: initiaza Screen
---------------------------------------------------------------------------*/
bool InitScreen()
{
   VLOG(2) << "SCREEN INITIALIZING ...";
   try
   {
      GetMainScreenInstance();
   }
   catch(...)
   {
      LOG(FATAL) << "UNABLE TO INITIALIZE MAIN SCREEN !!!!!!!!!!!";
      return false;
   }

   VLOG(2) << "SCREEN INITIALIZED\n-------------------- ";
   return true;
}
//---------------------------------------------------------------------------

/*---------------------------------------------------------------------------
 description: initiaza misiunea
 others     :
---------------------------------------------------------------------------*/
bool InitMission(EngineConfig *engineConfig)
{
   CIGMission *lMI;

   VLOG(2) << "MISSION INITIALIZING ...";

   std::string lMissionFile = "";

   try
   {
      lMI = GetMissionInstance();
      lMI->SetSelectNeutralUnits(engineConfig->GetSelectNeutralUnits());
      lMI->SetNeutralUnitHint(engineConfig->GetNeutralUnitHint());
   }
   catch(...)
   {
      LOG(FATAL) << "UNABLE TO INITIALIZE MISSION !!!!!!!!!!!";
      return false;
   }

#ifdef PP_TEST_VERSION
    lMissionFile = engineConfig->GetTestMap();
#endif

#ifdef PP_FULL_VERSION
   lMissionFile = "mis1.ppm"; //Universe->StarShip->Deck->GetCurrentMission()->GetFile();
#endif

   try
   {
     CIGMissionDiskOp::Load(CPString(PATH_MISSIONS) + lMissionFile.c_str());

     GetMainScreenInstance()->mBattleControl->SetMapWidth(GetMissionInstance()->GetMap()->GetLogicWidth());
     GetMainScreenInstance()->mBattleControl->SetMapHeight(GetMissionInstance()->GetMap()->GetLogicHeight());
     GetMainScreenInstance()->SetScrollBounds(GetMissionInstance()->GetMap()->GetWidth(), GetMissionInstance()->GetMap()->GetHeight());
   }
   catch(...)
   {
      LOG(FATAL) << "UNABLE TO LOAD " << lMissionFile << " MISSION !!!!!!!!!!!";
      return false;
   }

   VLOG(2) << "MISSION INITIALIZED\n-------------------- ";
   return true;
}
//---------------------------------------------------------------------------

/*---------------------------------------------------------------------------
 description: initiate the path server
---------------------------------------------------------------------------*/
bool InitPathServer()
{
   VLOG(2) << "PATH SERVER INITIALIZING ...";
   try
   {
      GetPathServerInstance();
   }
   catch(...)
   {
      LOG(FATAL) << "UNABLE TO CREATE PATH SERVER !!!!!!!!!!!";
      return false;
   }
   VLOG(2) << "PATH SERVER INITIALIZED\n-------------------- ";
   return true;
}
//---------------------------------------------------------------------------

/*---------------------------------------------------------------------------
 description: initiaza Timerul
---------------------------------------------------------------------------*/
bool InitLoop(EngineConfig *engineConfig)
{
   VLOG(2) << "MISSION LOOP INITIALIZING ...";

   gLastTick     = 0;
   gUpdatePeriod = engineConfig->GetTimerUpdatePeriod();

   VLOG(2) << "MISSION LOOP INITIALIZED\n-------------------- ";

   return true;
}
//---------------------------------------------------------------------------

/*---------------------------------------------------------------------------
 description: it dealocates all memory associated with the game
---------------------------------------------------------------------------*/
void Terminate()
{
   VLOG(2) << "MISSION OVER";

   //Path Server
   try
   {
      CIGPathServer::Release();
   }
   catch(...)
   {
      LOG(ERROR) << "UNABLE TO REMOVE PATH SERVER !!!!!!!!!!!";
   }
   //Path Server - over

   //Mission
   try
   {
	   CIGMission::Release();
   }
   catch(...)
   {
     LOG(ERROR) << "UNABLE TO REMOVE MISSION !!!!!!!!!!!";
   }
   //Mission - over

   //Screen
   try
   {
	   CPIMainScreen::Release();
   }
   catch(...)
   {
      LOG(ERROR) << "UNABLE TO INITIALIZE RELEASE SCREEN !!!!!!!!!!!";
   }
   //Screen - over

   //Space Sound System
   try
   {
	   CIGSpaceSoundSystem::Release();
   }
   catch(...)
   {
      LOG(ERROR) << "UNABLE TO REMOVE SPACE SOUND SYSTEM !!!!!!!!!!!";
   }
   //Space Sound System - over

   //Direct Sound
   try
   {
	   CPSISoundSystem::Release();
   }
   catch(...)
   {
      LOG(ERROR) << "UNABLE TO REMOVE AUDIO !!!!!!!!!!!";
   }
   //Direct Sound - over

   //Keyboard
   try
   {
	   CPGIKeyboard::Release();
   }
   catch(...)
   {
      LOG(ERROR) << "UNABLE TO REMOVE KEYBOARD !!!!!!!!!!!";
   }
   //Keyboard - over

   //Mouse Cursor
   try
   {
	   CPGICursor::Release();
   }
   catch(...)
   {
      LOG(ERROR) << "UNABLE TO REMOVE CURSOR !!!!!!!!!!!";
   }
   //Mouse Cursor - over

   // Input system
   try
   {
	   CPIIInputSystem::Release();
   }
   catch(...)
   {
      LOG(ERROR) << "UNABLE TO REMOVE INPUT !!!!!!!!!!!";
   }
   // Input System - over

   // Graphics system
   try
   {
	   CPGIGraphicSystem::Release();
   }
   catch(...)
   {
      LOG(ERROR) << "UNABLE TO REMOVE SDL GRAPHICS !!!!!!!!!!!";
   }
   // Graphics system - over

   VLOG(2) << "Saving the player profile";
   GetProfileInstance()->Save(Vfs()->GetWriteDir() + PATH_PLAYER_PROFILE + FILE_DEFAULT_PROFILE);

   VLOG(2) << "ALL MISSION DATA REMOVED FROM MEMORY";
   LOG(INFO) << "PP IN-GAME LOG FILE OVER";

   // close ttf library
   TTF_Quit();

   SDL_Quit();
}
//---------------------------------------------------------------------------

/*---------------------------------------------------------------------------
 description: this is a replace for a timer, it's just a loop
---------------------------------------------------------------------------*/
bool LoopInGame()
{
  int gTick = SDL_GetTicks();

  if (gTick - gLastTick > gUpdatePeriod)
  {
     if (gLastTick == 0)
     {
        VLOG(9) << "\n\nLET THE GAME BEGIN !";
     }
     else
     {
        VLOG(9) << "PERIOD TIME : " << (gTick - gLastTick) << "/" << gUpdatePeriod << " BEGIN";
     }

     gLastTick = gTick;

     VLOG(9) << "Path Server update begins... ";
     int lTempTick = gTick;
     GetPathServerInstance()->Update(gTick+10);
     gTick = SDL_GetTicks();

     VLOG(9) << "Path Server update Time: " << (gTick - lTempTick);

     // mission update is now performed in CPIBattleControl::Update
     //logWriteLn("Mission update begins... ", LOGDET_HIGH);
     //lTempTick = gTick;
     //GetMissionInstance()->Update();
     //gTick = SDL_GetTicks();
     //logWrite("Mission update Time: ", LOGDET_HIGH);
     //logWriteLn(CPString(gTick - lTempTick).c_str(), LOGDET_HIGH);

     VLOG(9) << "BeginRender begins... ";
     lTempTick = gTick;
     GetGraphicInstance()->SetColor(0xFF000000);
     GetGraphicInstance()->BeginRender();
     gTick = SDL_GetTicks();
     VLOG(9) << "BeginRender Time: " << (gTick - lTempTick);

     VLOG(9) << "Cursor Update begins... ";
     lTempTick = gTick;
     GetCursorInstance()->Update();
     gTick = SDL_GetTicks();
     VLOG(9) << "Cursor Update Time: " << (gTick - lTempTick);

     VLOG(9) << "Keyboard Update begins...";
     lTempTick = gTick;
     GetKeyboardInstance()->Update();
     gTick = SDL_GetTicks();
     VLOG(9) << "Keyboard Update Time: " << (gTick - lTempTick);

     try
     {
        VLOG(9) << "Screen Update begins... ";
        lTempTick = gTick;
        GetMainScreenInstance()->Update();
        gTick = SDL_GetTicks();
        VLOG(9) << "Screen Update Time: " << (gTick - lTempTick);
     }
     catch(...)
     {
        //return false;
     }

     VLOG(9) << "Cursor Paint begins... ";
     lTempTick = gTick;
     GetCursorInstance()->Paint();
     gTick = SDL_GetTicks();
     VLOG(9) << "Cursor Paint Time: " << (gTick - lTempTick);

     // use this place for any test code :)

     VLOG(9) << "EndRender begins... ";
     lTempTick = gTick;
     GetGraphicInstance()->EndRender();
     gTick = SDL_GetTicks();
     VLOG(9) << "EndRender Time: " << (gTick - lTempTick);

     //SDL_Delay(1);
  }

  return true;
}
//---------------------------------------------------------------------------
