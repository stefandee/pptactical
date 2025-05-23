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

 descrition : main form
              cred ca aici am pus mai multe chestii decat ar trebui
 last modify: 17 02 00
        by Grab
---------------------------------------------------------------------------*/
//---------------------------------------------------------------------------
#ifndef FMainH
#define FMainH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <Menus.hpp>
#include <ComCtrls.hpp>
#include <ExtCtrls.hpp>
#include <Buttons.hpp>
#include <Dialogs.hpp>
#include <ActnList.hpp>
#include <jpeg.hpp>
#include <ImgList.hpp>
#include <Types.hpp>

#include "PPME_Defines.h"
#include "PPME_Desktop.h"
#include "PP_Graphic.h"
#include "ddraw.h"
#include "CfgTxt.h"
#include "CFileHistory.h"
#include <System.Actions.hpp>
#include <System.ImageList.hpp>

//interface action
#define IACT_SELECT

//---------------------------------------------------------------------------
class TFormMain : public TForm
{
__published:	// IDE-managed Components
   TMainMenu *menuMain;
   TMenuItem *mitFile;
   TMenuItem *itNew;
   TMenuItem *itOpen;
   TMenuItem *itSave;
   TMenuItem *itSaveAs;
   TMenuItem *itN1;
   TMenuItem *itExit;
   TMenuItem *mitHelp;
   TMenuItem *itHelp;
   TMenuItem *itAbout;
   TMenuItem *mitTools;
   TMenuItem *itRefreshTree;
   TMenuItem *itTool;
   TMenuItem *itPreview;
   TMenuItem *itRadar;
   TMenuItem *itInfo;
   TOpenDialog *dialogOpen;
   TSaveDialog *dialogSave;
   TMenuItem *itMission;
   TMenuItem *itTree;
   TMenuItem *mitWindow;
   TMenuItem *itSaveDesktop;
   TStatusBar *statusMain;
   TActionList *actionsMain;
   TAction *actCancel;
   TAction *actScrollLeft;
   TAction *actScrollRight;
   TAction *actScrollDown;
   TAction *actScrollUp;
   TScrollBar *scrollHoriz;
   TScrollBar *scrollVert;
   TMenuItem *itLoadDesktop;
   TMenuItem *N1;
   TMenuItem *N4;
   TPopupMenu *popupMain;
   TMenuItem *pitDelete;
   TMenuItem *pitSelectAll;
   TAction *actCompile;
   TMenuItem *mitEdit;
   TMenuItem *itDelete;
   TMenuItem *N3;
   TAction *actSelectNeutral;
   TAction *actSelectOwned;
   TAction *actSelectFriendly;
   TAction *actSelectEnemy;
   TAction *actSelectScreen;
   TAction *actSelectAll;
   TMenuItem *itSelectNeutral;
   TMenuItem *itSelectOwned;
   TMenuItem *itSelectFriendly;
   TMenuItem *itSelectEnemy;
   TMenuItem *N5;
   TMenuItem *itSelectAll;
   TMenuItem *itCancel;
   TImageList *imgList;
   TMenuItem *itSelectScreen;
   TMenuItem *N6;
   TMenuItem *itSoulessEditor;
   TMenuItem *itTileEditor;
   TMenuItem *itAltitude;
   TMenuItem *N7;
   TMenuItem *itSelectStatic;
   TMenuItem *itSelectInfantry;
   TMenuItem *itSelectVehicle;
   TMenuItem *itSelectAir;
   TAction *actSelectStatic;
   TAction *actSelectInfantry;
   TAction *actSelectVehicle;
   TAction *actSelectAir;
   TMenuItem *N8;
   TMenuItem *itPreferences;
   TAction *actPreferences;
   TAction *actObjectProperties;
   TAction *actGroupOrders;
   TMenuItem *N9;
   TMenuItem *itObjectProperties;
   TMenuItem *itUnitsEditor;
   TMenuItem *itTest;
   TAction *actCenterView;
   TTimer *timerMain;
   TMenuItem *itMissionDatas;
   TMenuItem *N10;
   TMenuItem *N11;
   TMenuItem *itZoomIn;
   TMenuItem *itZoomOut;
   TMenuItem *pitCancel;
   TMenuItem *N2;
   TAction *actDelete;
        TMenuItem *ControlScript1;
        TMenuItem *BattleGroupScript1;
        TMenuItem *Politics1;
        TMenuItem *MapGenerator1;
        TMenuItem *OpenRecent1;
        TMenuItem *N12;

   void __fastcall FormCreate(TObject *Sender);

   void __fastcall itOpenClick(TObject *Sender);
   void __fastcall itNewClick(TObject *Sender);
   void __fastcall itSaveClick(TObject *Sender);
   void __fastcall itSaveAsClick(TObject *Sender);
   void __fastcall itExitClick(TObject *Sender);
   void __fastcall itRefreshTreeClick(TObject *Sender);

   void __fastcall itInfoClick(TObject *Sender);
   void __fastcall itPreviewClick(TObject *Sender);
   void __fastcall itToolClick(TObject *Sender);
   void __fastcall itRadarClick(TObject *Sender);
   void __fastcall itTreeClick(TObject *Sender);

   void __fastcall itSaveDesktopClick(TObject *Sender);
   void __fastcall itLoadDesktopClick(TObject *Sender);

   void __fastcall actCancelExecute(TObject *Sender);
   void __fastcall actScrollLeftExecute(TObject *Sender);
   void __fastcall actScrollRightExecute(TObject *Sender);
   void __fastcall actScrollUpExecute(TObject *Sender);
   void __fastcall actScrollDownExecute(TObject *Sender);

   void __fastcall FormMouseMove(TObject *Sender, TShiftState Shift, int X,
          int Y);
   void __fastcall FormMouseDown(TObject *Sender, TMouseButton Button,
          TShiftState Shift, int X, int Y);
   void __fastcall FormMouseUp(TObject *Sender, TMouseButton Button,
          TShiftState Shift, int X, int Y);

   void __fastcall FormPaint(TObject *Sender);
   void __fastcall FormResize(TObject *Sender);

   void __fastcall scrollHorizChange(TObject *Sender);
   void __fastcall scrollVertChange(TObject *Sender);

   void __fastcall itZoomInClick(TObject *Sender);       //nefolosit
   void __fastcall itZoomOutClick(TObject *Sender);      //nefolosit
   void __fastcall actCompileExecute(TObject *Sender);
   void __fastcall actSelectNeutralExecute(TObject *Sender);
   void __fastcall actSelectOwnedExecute(TObject *Sender);
   void __fastcall actSelectFriendlyExecute(TObject *Sender);
   void __fastcall actSelectEnemyExecute(TObject *Sender);
   void __fastcall actDeleteExecute(TObject *Sender);
   void __fastcall actSelectScreenExecute(TObject *Sender);
   void __fastcall actSelectAllExecute(TObject *Sender);
   void __fastcall itSoulessEditorClick(TObject *Sender);
   void __fastcall itTileEditorClick(TObject *Sender);
   void __fastcall itAboutClick(TObject *Sender);
   void __fastcall actSelectStaticExecute(TObject *Sender);
   void __fastcall actSelectInfantryExecute(TObject *Sender);
   void __fastcall actSelectAirExecute(TObject *Sender);
   void __fastcall actSelectVehicleExecute(TObject *Sender);
   void __fastcall itAltitudeClick(TObject *Sender);
   void __fastcall actPreferencesExecute(TObject *Sender);
   void __fastcall actObjectPropertiesExecute(TObject *Sender);
   void __fastcall actGroupOrdersExecute(TObject *Sender);
   void __fastcall itUnitsEditorClick(TObject *Sender);
   void __fastcall itTestClick(TObject *Sender);
   void __fastcall actCenterViewExecute(TObject *Sender);
   void __fastcall timerMainTimer(TObject *Sender);
   void __fastcall FormShow(TObject *Sender);
   void __fastcall FormClose(TObject *Sender, TCloseAction &Action);
   void __fastcall FormDestroy(TObject *Sender);
   void __fastcall itEventManagerClick(TObject *Sender);
   void __fastcall itMissionDatasClick(TObject *Sender);
        void __fastcall ControlScript1Click(TObject *Sender);
        void __fastcall BattleGroupScript1Click(TObject *Sender);
        void __fastcall Politics1Click(TObject *Sender);
        void __fastcall MapGenerator1Click(TObject *Sender);
    void __fastcall FormMouseWheel(TObject *Sender, TShiftState Shift,
          int WheelDelta, Types::TPoint &MousePos, bool &Handled);      //nefolosit

private:  // config and miscellaneous
  CPPME_Desktop mDesktop;
                             
  Types::TRect   TRectConv(CPRect _v);
  CPRect  CPRectConv(Types::TRect _v);

private: // initalizations
   void InitMapEditorSession();

   void InitLog();
   void SetLogDetail(CCfgTxt *Config);

   bool InitGraphics(CCfgTxt *Config);
   bool InitSound(CCfgTxt *Config);
   void InitMask();

   bool InitMission(CCfgTxt *Config);
   bool InitPathServer(CCfgTxt *Config);

   void UpdateVisibility(int _value);

public:  // User declarations
   CPGISprite   *mSpriteFloat, *mSpriteMask;
   CPGISprite   *mTest;
   CPFont*      mAltitudeFont;
   CFileHistory* mFileHistory;
   //bool doubleBuffered;          //false => back=primary si nu se face flip

   AnsiString mapName;           //asa ca fapt divers

   bool isModified, isCompiled;  //arata ce zice

   bool isCaptured;              //nefolosit inca

   //Selection section
   TTreeItem *selectedItem;      //obiectul de pus in harta
   TMask itemMask;               //tine minte masca obiectului de pus in harta

   bool isSelecting;             //arata daca s-a inceput o operatie de selectie
   int selectX, selectY;         //punctul de inceput al selectie
   int selectEndX, selectEndY;   //le folosesc ca sa stiu atunci care a fost selectia
   TColor selectionColor;
   ////////////////////////////////

   //Dims
   int mapWidth, mapHeight;      //ca sa nu ma mai complic;
                                 //sunt folosite pentru setScrollBounds
                                 //in coord reale
                                 
   int altitudeLeft, altitudeTop;//brush (left, top)

   int viewLeft, viewTop, viewWidth, viewHeight;
                                 //portiunea care se vede din harta
                                 //adica are rol de masca
   Types::TRect screenZone;    //adica zona de pe ecran unde se tipareste
   ////////////////////////////////

   int floatLeft, floatTop, floatWidth, floatHeight;
   bool isFloatVisible;                //folosit pentru a arata sau nu float image
   ////////////////////////////////

   int zoomLevel;
   int currentBGroup;

   bool isApplicationClosing, isApplicationBeginning;

   //CMission *theMission;

   __fastcall TFormMain(TComponent* Owner);
   void __fastcall WndProc(Messages::TMessage &Message);
   void __fastcall displayHint(TObject *Sender);
   void __fastcall HistoryClick(TObject *Sender);

   void NoteObject(TTreeItem *treeitem, TMask mask, int x, int y, int _bg, int _id, int _scriptId);

   void SaveDesktop();
   void LoadDesktop();

   void setZoomLevel(int zoomlevel);
   void setMapName(AnsiString mapname);
   void setCompiled(bool iscompiled);
   void setModified(bool ismodified);
   void SaveMap(AnsiString mapname);

   void setScrollBounds(int mapwidth, int mapheight);
   void setViewPortDim();
   void setViewPortCoord(int viewleft, int viewtop);
   void centerViewPort(int x, int y);
   void setScreenZone();

   void Flip();
   void paintFloat();
   void paintMap();
   void paintAltitudeMap();
   void paintGrid();
   void paintMaskMap();
   void paintCanvas();
   void createNewMap(int mapwidth, int mapheight, AnsiString landscape);
   void MakeClean();
   void RecomputeSelection();

   void setSelectedItem(TTreeItem *selecteditem);

   void beginSelection(int xbegin, int ybegin);
   void endSelection(int xend, int yend);
   void cancelSelection();
   void deleteSelection();

   void beginAltitude();
   void endAltitude();
   void setAltitude();

   void setCurrentBGroup(int _bg);
};
//---------------------------------------------------------------------------
extern PACKAGE TFormMain *FormMain;
//---------------------------------------------------------------------------
#endif
 
 
 
