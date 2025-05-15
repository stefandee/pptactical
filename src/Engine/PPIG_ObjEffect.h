//-----------------------------------------------------------------------------
// This file is part of PPTactical Engine.                                   //
//                                                                           //
// PPTactical Engine - engine for tactical/strategy games                    // 
// Copyright (C) 1998, 1999, 2000, 2001 Stefan Dicu & Tudor Girba            //
//                                                                           //
// PPTactical Engine is free software; you can redistribute it               //
// and/or modify it under the terms of the GNU Lesser General Public License as     // 
// published by the Free Software Foundation; either version 2.1 of the        //
// License, or (at your option) any later version.                           //
//                                                                           //  
// PPTactical Engine is distributed in the hope that it will be useful, but  //
// WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY// 
// or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU Lesser General Public License  //
// for more details.                                                         //
//                                                                           // 
// You should have received a copy of the GNU Lesser General Public License         //
// along with PPTactical Engine; if not, write to the Free Software          //
// Foundation Inc. 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA   // 
//-----------------------------------------------------------------------------
/*---------------------------------------------------------------------------
 application: Pure Power In Game

 description: Class In Game - Effect Object
 last modify: 21 05 00
        by grabX
---------------------------------------------------------------------------*/
//---------------------------------------------------------------------------
#ifndef PPIG_ObjEffectH
#define PPIG_ObjEffectH

#include "PPIG_ObjBase.h"

class CIGEffectObject : public CIGBaseObject  //## Inherits: <unnamed>%3916EEC40064
{
  public:
      CIGEffectObject();
      virtual ~CIGEffectObject();

      void Paint(CPPoint ptViewport, CPPoint ptWindow, int Width, int Height, int zoomLevel);
      void Update ();

  public:
      void OnActionDie();

    //## Get and Set Operations for Class Attributes (generated)
      virtual void SetSprite(CPSprite * value);

      virtual int GetAnimCyclesCount ();
      virtual void SetAnimCyclesCount (int value);

  public: // generic
    virtual CPString ClassName () { return IGCN_EFFECTOBJECT; }
    virtual bool IsDerivedFrom (CPString className);

  public: // serialization - deserialization
    virtual void Serialize(PP::Stream& _a);
    virtual unsigned __int32 DataSize();
    virtual void DeSerialize(PP::Stream& _a);
    
  protected:
    virtual bool CanBePlaced(CPPoint _point) { return true; }

  private: //## implementation
    // Data Members for Class Attributes
      int mAnimCyclesCount;
      CPCounter mAnimCounter;
      CPCounter mActionStepCounter;
      int mAnimType;
};

/////////////////////////////////////////////////////////////////////////////
//GET-SET SECTION
/////////////////////////////////////////////////////////////////////////////
/*---------------------------------------------------------------------------
 description: Sprite 
---------------------------------------------------------------------------*/
inline void CIGEffectObject::SetSprite(CPSprite * value)
{
  CIGBaseObject::SetSprite(value);

  if (value != NULL)
  {
    mAnimCounter.GetThis()->SetMaxValue(GetSprite()->GetAnimCount()-1);
    mActionStepCounter.GetThis()->SetMaxValue(value->GetAnimTypes()-1);
  }  
}

/*---------------------------------------------------------------------------
 description: Anim Cycles Count
---------------------------------------------------------------------------*/
inline int CIGEffectObject::GetAnimCyclesCount ()
{
  return mAnimCyclesCount;
}

inline void CIGEffectObject::SetAnimCyclesCount (int value)
{
  mAnimCyclesCount = value ;
}

//---------------------------------------------------------------------------
#endif

