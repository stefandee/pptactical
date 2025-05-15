//---------------------------------------------------------------------------
#include <dirent.h>

#include <sys\stat.h>
#include <time.h>
#include <stdio.h>
#pragma hdrstop

#include "PPIG_CBaseSaveGameDialog.h"
#include "Paths.h"
#include "PP_CListBox.h"
#include "PP_CLabel.h"
#include "PP_CSLinkMenu.h"
#include "PP_CEdit.h"
#include "PP_Stdlib.h"
#include "PP_VfsSystem.h"
//---------------------------------------------------------------------------

#ifdef __BORLANDC__
#pragma package(smart_init)
#endif

CPP_CBaseSaveGameDialog::CPP_CBaseSaveGameDialog(CPIWinControl *Parent) : CPICommonDialog(Parent)
{
}
//---------------------------------------------------------------------------

void CPP_CBaseSaveGameDialog::ShowModal(CPIControl *parent)
{
  // setup the savegames list
  FillSaveGamesList();

  mScreen->SetTop(-mImgDialog->GetHeight() - mMenu->GetWidth());
  mScreen->SetTopDestination(0);
  mScreen->SetHandleOnStopMove(std::bind(&CPP_CBaseSaveGameDialog::ShowStopMoveHandle, this, std::placeholders::_1));

  CPIForm::ShowModal(parent);
}
//---------------------------------------------------------------------------

void CPP_CBaseSaveGameDialog::SetTitle(CPString _v)
{
  mTitle = _v;
  mLabelTitle->SetText(mTitle);
}
//---------------------------------------------------------------------------

void CPP_CBaseSaveGameDialog::DefaultBtnHandle(CPIObject *Sender)
{
    mScreen->SetTop(mScreen->GetTop());
    mScreen->SetTopDestination(-mImgDialog->GetHeight() - mMenu->GetWidth());
    mScreen->SetHandleOnStopMove(std::bind(&CPP_CBaseSaveGameDialog::CloseStopMoveHandle, this, std::placeholders::_1));

    mBtnTag = Sender->GetTag();
}
//---------------------------------------------------------------------------

void CPP_CBaseSaveGameDialog::ShowStopMoveHandle(CPIObject *Sender)
{
}

void CPP_CBaseSaveGameDialog::CloseStopMoveHandle(CPIObject *Sender)
{
  if (mScreen->GetTop() < 0)
  {
    SetModalResult(mBtnTag);
    OnClose();
  }
}
//---------------------------------------------------------------------------

void CPP_CBaseSaveGameDialog::FillSaveGamesList()
{
  DIR *dir;
  struct dirent *ent;
  CPString outPath = Vfs()->GetWriteDir() + PATH_SAVEDGAMES;

  (mSaveGamesList->Items()).clear();

  if ((dir = opendir(outPath.c_str())) == NULL)
  {
    LOG(ERROR) << "CPP_CLoadGameDialog::FillSaveGamesList - unable to open dir";
    return;
  }

  struct stat statBuf;

  while ((ent = readdir(dir)) != NULL)
  {
    CPString fileName = CPString(ent->d_name);
    CPString filePath = outPath + fileName;

    if (stat(filePath.c_str(), &statBuf) != 0)
    {
      continue;
    }

    CPString fileExt = fileName.SubString(fileName.Length() - 4, 4);
    fileExt.ToLowerCase();

    if ((statBuf.st_mode & S_IFREG) != 0 && (fileExt == SAVE_GAME_EXTENSION))
    {
        CPString shortFileName = fileName.SubString(0, fileName.Length() - 4);
        shortFileName.ToLowerCase();
        (mSaveGamesList->Items()).push_back(shortFileName);
    }
  }

  if (mSaveGamesList->Items().size() > 0)
  {
    mLabelTimeDate->SetVisible(true);
    mImgThumb->SetVisible(true);
    mSaveGamesList->SetItemIndex(0);
    SetupInfo(mSaveGamesList->Items().at(0));
  }
  else
  {
    mLabelTimeDate->SetVisible(false);
    mImgThumb->SetVisible(false);
    mSaveGamesList->SetItemIndex(-1);
  }

  if (closedir(dir) != 0)
  {
    LOG(ERROR) << "CPP_CBaseSaveGameDialog::FillSaveGamesList - unable to close dir";
  }
}
//---------------------------------------------------------------------------

void CPP_CBaseSaveGameDialog::SaveListClick(CPIObject *Sender, int X, int Y)
{
  int itemIndex = mSaveGamesList->ItemAtPos(X, Y);

  if (itemIndex == -1)
  {
    return;
  }

  SetupInfo(mSaveGamesList->Items().at(itemIndex));
  mSaveGamesList->SetItemIndex(itemIndex);
}
//---------------------------------------------------------------------------

void CPP_CBaseSaveGameDialog::SetupInfo(CPString shortName)
{
  CPString longName = Vfs()->GetWriteDir() + PATH_SAVEDGAMES + shortName + SAVE_GAME_EXTENSION;

  struct stat statBuf;

  // get the date and time informations to display
  if (stat(longName.c_str(), &statBuf) != 0)
  {
    return;
  }

  // reformat the output of ctime for custom displaying - cant find any c lib to do it :|
  char weekday[4], month[4];
  int  day, year, hour, min, sec;

  sscanf(ctime(&statBuf.st_mtime), "%s %s %d %d:%d:%d %d", weekday, month, &day, &hour, &min, &sec, &year);

  weekday[3] = '\0';
  month[3]   = '\0';

  CPString text = CPString(month) + " " + CPString(day) + ", " + padZero(CPString(hour), 2) + ":" + padZero(CPString(min), 2) + "H";

  mLabelTimeDate->SetText(text);

  // TODO retrieve the thumbnail associated to the savegame
  mImgThumb->LoadFromFile(PATH_MISSION_THUMBNAILS + "test.png");
}
//---------------------------------------------------------------------------

bool CPP_CBaseSaveGameDialog::FileExists()
{
  struct stat statBuf;

  // TODO: check for 0 items in the list
  CPString fileName = Vfs()->GetWriteDir() + PATH_SAVEDGAMES + mSaveGamesList->Items().at(mSaveGamesList->GetItemIndex()) + SAVE_GAME_EXTENSION;

  if (stat(fileName.c_str(), &statBuf) == 0)
  {
    return true;
  }

  return false;
}
//---------------------------------------------------------------------------

CPP_CBaseSaveGameDialog::~CPP_CBaseSaveGameDialog()
{
}
//---------------------------------------------------------------------------
