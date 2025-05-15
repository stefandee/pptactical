//---------------------------------------------------------------------------


#pragma hdrstop

#include "WeaponRulesLib.h"
#include "ZCsl.hpp"
#include "PPIG_WeaponRules.h"

//#ifndef _MSC_VER
//#include <strstream.h>
#ifdef PPT_COMPILER_MICROSOFT
#include <strstream>
#else
#include <sstream>
#endif
//#else
//#include <strstrea.h>	// TMN: Note use of REALLY OLD header !!!
//#endif

using namespace std;

//---------------------------------------------------------------------------

#ifdef PPT_COMPILER_BORLAND
#pragma package(smart_init)
#endif

WeaponRulesLib::WeaponRulesLib(ZCsl* _Parent, const ZString& _Name/*, ZLibrary* _Prev*/) : ZLibrary(_Parent, _Name/*, _Prev*/)
{
  //mNative = true;
}
//---------------------------------------------------------------------------

void WeaponRulesLib::InitLibrary()
{
   ZString iModule("WeaponRulesLib");
   // this is native library - no external dependencies
   ZString init(ZString(
      "const weaponRulesVersion = '1.0';\n"
      "const weaponRulesCompiler = '") + PPT_COMPILER +"';\n"
      "const weaponRulesLibtype = 'NATIVE';\n"
      "const weaponRulesBuilt = '"+ZString(__DATE__)+" "+__TIME__+"';\n"
      );

#ifdef PPT_COMPILER_MICROSOFT
   std::istrstream str((char*)init);
#else
   std::istringstream str((char *) init);
#endif

   GetParent()->loadScript(iModule, &str);

   // handler assignment
   mHandleCanFireObject    .set(this, &WeaponRulesLib::mcCanFireObject);
   mHandleCanFireGround    .set(this, &WeaponRulesLib::mcCanFireGround);

   // add functions
   GetParent()->addFunc(iModule, "CanFireObject(const attackerId, const targetId, const weaponId)",          mHandleCanFireObject);
   GetParent()->addFunc(iModule, "CanFireGround(const attackerId, const weaponId)",          mHandleCanFireGround);
}
//---------------------------------------------------------------------------

ZString WeaponRulesLib::mcCanFireObject(ZCsl* csl)
{
  int lId1(csl->get("attackerId").asInt());
  int lId2(csl->get("targetId").asInt());
  int lWeaponId(csl->get("weaponId").asInt());

  return (int)(CIGWeaponRules().CanFireObject(lId1, lWeaponId, lId2));
}
//---------------------------------------------------------------------------

ZString WeaponRulesLib::mcCanFireGround(ZCsl* csl)
{
  int lId(csl->get("attackerId").asInt());
  int lWeaponId(csl->get("weaponId").asInt());

  return (int)(CIGWeaponRules().CanFireGround(lId, lWeaponId));
}
//---------------------------------------------------------------------------

