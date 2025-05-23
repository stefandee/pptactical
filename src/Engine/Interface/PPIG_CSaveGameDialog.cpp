//---------------------------------------------------------------------------
// doesnt natively exists on msvc
#include <dirent.h>

#include <sys\stat.h>
#include <time.h>
#include <stdio.h>
#pragma hdrstop

#include "PPIG_CSaveGameDialog.h"
#include "Paths.h"
#include "PP_CListBox.h"
#include "PP_CLabel.h"
#include "PP_CSLinkMenu.h"
#include "PP_CEdit.h"
#include "PP_VfsSystem.h"
//---------------------------------------------------------------------------

#ifdef __BORLANDC__
#pragma package(smart_init)
#endif

CPP_CSaveGameDialog::CPP_CSaveGameDialog(CPIWinControl *Parent) : CPP_CBaseSaveGameDialog(Parent)
{
  // membrii
  mTitle    = CPString("TITLE");

  // controalele
  mScreen = new CPP_CMove(this);
  mScreen->SetLeft(0);
  mScreen->SetTime(0.25);
  AddControl(mScreen);

  mImgDialog = new CPIImage(mScreen);
  mImgDialog->SetLeft(0);
  mImgDialog->SetTop(0);
  mImgDialog->SetAutoSizeToSprite(true);
  mImgDialog->LoadFromFile(CPString(PATH_GUI_GFX) + "dialog_save.png");
  mScreen->AddControl(mImgDialog);

  // title label
#if PPT_USE_SDL
  CPFont* lFont2 = new CPFont(PATH_FONTS + FILE_FONT_BIG, 20, true);
#else
  CPFont* lFont2 = new CPFont(PATH_FONTS + FILE_FONT_BIG);
  lFont2->SetSize(20);
#endif // PPT_USE_SDL

  mLabelTitle = new CPILabel(mScreen);
  mLabelTitle->SetLeft(340);
  mLabelTitle->SetTop(203);
  mLabelTitle->SetWidth(100);
  mLabelTitle->SetHeight(32);
  mLabelTitle->SetText(mTitle);
  mLabelTitle->SetFont(lFont2);
  mLabelTitle->SetColor(0xE3E0E0);
  mScreen->AddControl(mLabelTitle);
  mLabelTitle->BringToFront();

  // si din nou ecranul
  mScreen->SetWidth(mImgDialog->GetWidth());

  SetWidth(mScreen->GetWidth());
  SetLeft(400 - mScreen->GetWidth() / 2);
  SetTop(0);
  SetHeight(600);

  // the link menu
  mMenu = new CPP_CSLinkMenu(3, mScreen);
  mMenu->SetBtnCaption(0, "SAVE");
  mMenu->SetBtnTag(0, PMR_YES);
  mMenu->SetBtnHandle(0, std::bind(&CPP_CSaveGameDialog::DefaultBtnHandle, this, std::placeholders::_1));

  mMenu->SetBtnCaption(1, "DELETE");
  mMenu->SetBtnTag(1, PMR_NO);
  mMenu->SetBtnHandle(1, std::bind(&CPP_CSaveGameDialog::DefaultBtnHandle, this, std::placeholders::_1));

  mMenu->SetBtnCaption(2, "CANCEL");
  mMenu->SetBtnTag(2, PMR_CANCEL);
  mMenu->SetBtnHandle(2, std::bind(&CPP_CSaveGameDialog::DefaultBtnHandle, this, std::placeholders::_1));

  mMenu->SetWidth(214);
  mMenu->SetHeight(120);
  mMenu->SetMenuWidth(150);
  mMenu->SetLeft(mImgDialog->GetWidth() - 214 - 80);
  mMenu->SetLeftDestination(mImgDialog->GetWidth() - 214 - 80);
  mMenu->SetTop(mImgDialog->GetHeight() - 3);
  mMenu->SetTopDestination(mImgDialog->GetHeight() - 3);
  mMenu->SetBtnOrientation(SBO_RIGHT);
  mMenu->SetLinkOrientation(SLMO_UP);
  mMenu->SendToBack();

  // the save game list
  mSaveGamesList = new CPIListBox(mScreen);
  mSaveGamesList->SetTop(238);
  mSaveGamesList->SetLeft(50);
  mSaveGamesList->SetWidth(200);
  mSaveGamesList->SetHeight(115);
  mSaveGamesList->SetStyle(LBS_STD);
  mSaveGamesList->SetAlign(LA_LEFT);
  mSaveGamesList->SetBackColor(0x101030);
  mScreen->AddControl(mSaveGamesList);
  mSaveGamesList->BringToFront();

#if PPT_USE_SDL
  lFont2 = new CPFont(PATH_FONTS + FILE_FONT_SMALL, 14, true);
#else
  lFont2 = new CPFont(PATH_FONTS + FILE_FONT_SMALL);
  lFont2->SetSize(14);
#endif // PPT_USE_SDL

  mSaveGamesList->SetFont(lFont2);

  // set the handler for left click
  mSaveGamesList->SetHandleOnLeftMouseDown(std::bind(&CPP_CSaveGameDialog::SaveListClick, this, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3));

  // the edit box
  mEdSaveName = new CPIEdit(mScreen);
  mEdSaveName->SetTop(358);
  mEdSaveName->SetLeft(50);
  mEdSaveName->SetWidth(200);
  mEdSaveName->SetHeight(18);
  mEdSaveName->SetColor(0xFFFFFF);
  //mEdSaveName->SetRepeatDelay(100);
  //mEdSaveName->SetRepeatRate(100);
  mEdSaveName->SetText(DEFAULT_SAVEGAME_NAME);
  mScreen->AddControl(mEdSaveName);
  mEdSaveName->BringToFront();

#if PPT_USE_SDL
  lFont2 = new CPFont(PATH_FONTS + FILE_FONT_SMALL, 14, true);
#else
  lFont2 = new CPFont(PATH_FONTS + FILE_FONT_SMALL);
  lFont2->SetSize(14);
#endif // PPT_USE_SDL

  mEdSaveName->SetFont(lFont2);

  // the savegame info box
#if PPT_USE_SDL
  lFont2 = new CPFont(PATH_FONTS + FILE_FONT_SMALL, 14, true);
#else
  lFont2 = new CPFont(PATH_FONTS + FILE_FONT_SMALL);
  lFont2->SetSize(14);
#endif // PPT_USE_SDL

  mLabelTimeDate = new CPILabel(mScreen);
  mLabelTimeDate->SetLeft(265);
  mLabelTimeDate->SetTop(360);
  mLabelTimeDate->SetWidth(100);
  mLabelTimeDate->SetHeight(32);
  mLabelTimeDate->SetFont(lFont2);
  mLabelTimeDate->SetAutoSize(true);
  mLabelTimeDate->SetText("");
  mLabelTimeDate->SetColor(0xE3E0E0);
  mScreen->AddControl(mLabelTimeDate);
  mLabelTimeDate->BringToFront();

  // TODO: thumb - not functional, just display a static image
  mImgThumb = new CPIImage(mScreen);
  mImgThumb->SetLeft(265);
  mImgThumb->SetTop(238);
  mImgThumb->SetWidth(160);
  mImgThumb->SetHeight(120);
  mScreen->AddControl(mImgThumb);
  mImgThumb->BringToFront();

  // more screen setup
  mScreen->SetHeight(mImgDialog->GetHeight() + mMenu->GetHeight());
  mScreen->AddControl(mMenu);
  mScreen->BringControlToFront(mMenu);
}
//---------------------------------------------------------------------------

CPString CPP_CSaveGameDialog::GetFileName()
{
  return mEdSaveName->GetText() + SAVE_GAME_EXTENSION;
}
//---------------------------------------------------------------------------

void CPP_CSaveGameDialog::SetFileName(CPString fileName)
{
  mEdSaveName->SetText(fileName + SAVE_GAME_EXTENSION);
}
//---------------------------------------------------------------------------

void CPP_CSaveGameDialog::SaveListClick(CPIObject *Sender, int X, int Y)
{
  int itemIndex = mSaveGamesList->ItemAtPos(X, Y);

  if (itemIndex == -1)
  {
    return;
  }

  mSaveGamesList->SetItemIndex(itemIndex);

  CPString shortFileName = mSaveGamesList->Items().at(itemIndex);
  SetupInfo(shortFileName);
  mEdSaveName->SetText(shortFileName);
}

CPP_CSaveGameDialog::~CPP_CSaveGameDialog()
{
}
//---------------------------------------------------------------------------

bool CPP_CSaveGameDialog::FileExists()
{
  struct stat statBuf;

  CPString fileName = Vfs()->GetWriteDir() + PATH_SAVEDGAMES + mEdSaveName->GetText() + SAVE_GAME_EXTENSION;

  if (stat(fileName.c_str(), &statBuf) == 0)
  {
    return true;
  }

  return false;
}
