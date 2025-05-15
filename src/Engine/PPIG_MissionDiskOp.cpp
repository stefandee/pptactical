//---------------------------------------------------------------------------
#pragma hdrstop

#include "Platform.h"

#ifdef PPT_PLATFORM_WIN
#include <dos.h>
#endif

#ifdef PPT_FAMILY_UNIX
#include <unistd.h>
#endif

#include "Stream_PhysFS.h"
#include "PPIG_MissionDiskOp.h"
#include "PPIG_Mission.h"
#include "PP_VfsSystem.h"
#include "Paths.h"
//---------------------------------------------------------------------------
#ifdef __BORLANDC__
#pragma package(smart_init)
#endif

bool CIGMissionDiskOp::Save(CPString fileName)
{
  // make sure the "saves" folder exists
  if (!Vfs()->EnsureWriteDir(PATH_SAVEDGAMES))
  {
      return false;
  }

  try
  {
    //PP::Stream_ANSI_C lA(fileName, true);
    PP::Stream_PhysFS lA(fileName, true);
    GetMissionInstance()->Serialize(lA);

    XmlSerializer out;
    GetMissionInstance()->Serialize(out);

    out.GetDoc().SaveFile((Vfs()->GetWriteDir() + PATH_SAVEDGAMES + "test.xml").c_str());
  }
  catch(...)
  {
    LOG(ERROR) << "CIGMissionDiskOp::Save - unable to save the mission!!!";
    return false;
  }

  return true;
}
//---------------------------------------------------------------------------

bool CIGMissionDiskOp::Load(CPString fileName)
{
  try
  {
    PP::Stream_PhysFS lA(fileName);
    //PP::Stream_ANSI_C lA(fileName);
    GetMissionInstance()->DeSerialize(lA);
  }
  catch(...)
  {
    LOG(ERROR) << "CIGMissionDiskOp::Load - unable to load the mission!!!";
    return false;
  }

  return true;
}
//---------------------------------------------------------------------------

bool CIGMissionDiskOp::Delete(CPString fileName)
{
    return Vfs()->Delete(fileName);
}
//---------------------------------------------------------------------------

