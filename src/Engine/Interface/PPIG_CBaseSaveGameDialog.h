#ifndef PPIG_CBaseSaveGameDialogH
#define PPIG_CBaseSaveGameDialogH

#include "PP_CCommonDialog.h"

class CPILabel;
class CPIListBox;
class CPIEdit;
class CPP_CSLinkMenu;
class CPP_CMove;
class CPIImage;
class CPIImage;

const CPString SAVE_GAME_EXTENSION = CPString(".sav");

const CPString DEFAULT_SAVEGAME_NAME = CPString("SAVEGAME");

class CPP_CBaseSaveGameDialog : public CPICommonDialog
{
  public:
    CPP_CBaseSaveGameDialog(CPIWinControl *Parent = NULL);

    virtual void ShowModal(CPIControl *parent);
    void SetTitle(CPString _v);

    // TODO: a file utility lib should exist
    bool FileExists();

    virtual ~CPP_CBaseSaveGameDialog();

  protected:
    virtual void SaveListClick(CPIObject *Sender, int X, int Y);

  protected:
    int             mBtnTag;
    CPString        mTitle;

    CPP_CSLinkMenu* mMenu;
    CPP_CMove*      mScreen;
    CPILabel*       mLabelTitle, *mLabelTimeDate;
    CPIImage*       mImgDialog;
    CPIListBox*     mSaveGamesList;
    CPIImage*       mImgThumb;

    void SetupInfo(CPString shortName);

  protected:
    // handlere interne pentru atasat componentelor
    void DefaultBtnHandle(CPIObject *Sender);
    void CloseStopMoveHandle(CPIObject *Sender);
    void ShowStopMoveHandle(CPIObject *Sender);
    void FillSaveGamesList();
};


#endif // PPIG_CBaseSaveGameDialogH
