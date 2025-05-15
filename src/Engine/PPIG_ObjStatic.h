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

//-----------------------------------------------------------------------------
//  Unit              : Static object
//
//  Version           :
//
//  Description       :
//    * group a class of objects that do not move - buildings, props and so on
//    * as instances of this class may share the same static data (struct _StaticObject)
//      thus taking up lots of memory, it could be possible to share the data,
//      and, in the same time, provide a copy-on-write mechanism
//
//  History           :
//    [30.05.2000] - [grabX] - unit created
//    [15.10.2004] - [Karg]  - added visual subsystem
//-----------------------------------------------------------------------------

//---------------------------------------------------------------------------
#ifndef PPIG_ObjStaticH
#define PPIG_ObjStaticH

#include "PPIG_ObjReal.h"
#include "PPIG_EngineDef.h"
#include "Statics.h"
#include "ZCsl.hpp"
#include "PPIG_Doctrine.h"
#include "PPIG_SubSysVisual.h"
#include <vector>
//-----------------------------------------------------------------------------

class CIGSubSysBody;
//-----------------------------------------------------------------------------

class CIGStaticLayer
{
  public: // ctor-dtor
    CIGStaticLayer();
    virtual ~CIGStaticLayer() {}

  public: // ops
    virtual void Update();
    virtual void Paint(CPPoint ptViewPort, CPPoint ptWindow, int Width, int Height, int zoomLevel = 1);

  public: // get-set
    virtual CPSprite * GetSprite ();
    virtual void       SetSprite (CPSprite * value);

    virtual bool GetVisible();
    void SetVisible(double _v) { mAlpha = _v; }
    void SetVisible(bool _v) { mAlpha = (double)_v; }

    void SetCoord(CPPoint _v) { mCoord = _v; }
    CPPoint GetCoord() { return mCoord; }

    void SetEnabled(bool _v) { mEnabled = _v; }
    bool GetEnabled() { return mEnabled; }

    void SetAnimCounter(int _v);

    virtual void SetFrameUpdateTime(int _v) { mFrameUpdateTime = _v; }
    virtual int  GetFrameUpdateTime() { return mFrameUpdateTime; }

    virtual void SetFrameUpdateDelay(int _v) { mFrameUpdateDelay = _v; }
    virtual int  GetFrameUpdateDelay() { return mFrameUpdateDelay; }

    virtual void SetCycleUpdateTime(int _v) { mCycleUpdateTime = _v; }
    virtual int  GetCycleUpdateTime() { return mCycleUpdateTime; }

    virtual void SetCycleUpdateDelay(int _v) { mCycleUpdateDelay = _v; }
    virtual int  GetCycleUpdateDelay() { return mCycleUpdateDelay; }

  public: // serialization - deserialization
    virtual void Serialize(PP::Stream& _a);
    virtual unsigned __int32 DataSize();
    virtual void DeSerialize(PP::Stream& _a);

  protected:

  private:
    CPPoint    mCoord;       // real coordinates
    CPSprite   *mSprite;     // the sprite
    CPCounter  mAnimCounter; // animation
    float      mAlpha;       // visibility
    int        mFrameUpdateTime,
               mFrameUpdateDelay;
    int        mCycleUpdateTime,
               mCycleUpdateDelay;
    bool       mEnabled;
};
//-----------------------------------------------------------------------------

class CIGStaticObject : public CIGRealObject
{
  public:
    CIGStaticObject();
    virtual ~CIGStaticObject();

    virtual CPString ClassName () {return IGCN_STATICOBJECT;}
    virtual bool IsDerivedFrom (CPString className);
    virtual void Update ();
    virtual void Paint(CPPoint ptViewPort, CPPoint ptWindow, int Width, int Height, int zoomLevel = 1);

    virtual void NoteInLogicMap();
    virtual void NoteInLogicMap(CPPoint Point) {}
    virtual void RemoveFromLogicMap();

    CPPoint GetClosestPointTo(CPPoint targetPoint);
    virtual void NoteStimul(CIGObjectStimul &stimul);

    //virtual void SetActionMask (__int32 value);
    //virtual __int32 GetActionMask ();

  public:
    virtual void SetStaticData(struct _StaticObject& value);
    virtual struct _StaticObject& GetStaticData();

    void                  SetScriptObserver(CPPIG_ScriptObserver* _obs);
    CPPIG_ScriptObserver* GetScriptObserver() { return mScriptObserver; };

  public: // serialization - deserialization
    virtual void Serialize(PP::Stream& _a);
    virtual unsigned __int32 DataSize();
    virtual void DeSerialize(PP::Stream& _a);

  public: // mediating with subsystems
    void        UpdateView(int _value);

  protected: // script compiler
    ZCsl*                 mCompiler;
    ZString               mModuleName;
    bool                  mValidCsl;
    CPPIG_ScriptObserver* mScriptObserver;

    void InitCsl();
    void InitHandlers();

  protected: // script handlers & script methods
    void addModuleFunc(const char* szMemberName, const ZCsl_callback_t& callback);

    ZCsl_callback_t  mHandlePostMessage;

    // layers
    ZCsl_callback_t  mHandleLayerSetEnabled;
    ZCsl_callback_t  mHandleLayerGetEnabled;
    ZCsl_callback_t  mHandleLayerSetVisible;
    ZCsl_callback_t  mHandleLayerGetVisible;
    ZCsl_callback_t  mHandleLayerGetX;
    ZCsl_callback_t  mHandleLayerGetY;
    ZCsl_callback_t  mHandleLayerSetX;
    ZCsl_callback_t  mHandleLayerSetY;
    ZCsl_callback_t  mHandleLayerSetCycleDelay;
    ZCsl_callback_t  mHandleLayerGetCycleDelay;
    ZCsl_callback_t  mHandleLayerSetFrameDelay;
    ZCsl_callback_t  mHandleLayerGetFrameDelay;
    // static
    ZCsl_callback_t  mHandleMyX;
    ZCsl_callback_t  mHandleMyY;
    ZCsl_callback_t  mHandleGetMask;
    ZCsl_callback_t  mHandleSetMask;
    ZCsl_callback_t  mHandleGetAltitude;
    ZCsl_callback_t  mHandleSetAltitude;
    ZCsl_callback_t  mHandleMyHp;
    ZCsl_callback_t  mHandleMyId;
    ZCsl_callback_t  mHandleCanFire;
    // implementations of callbacks
    ZString mcPostMessage(ZCsl* csl);
    ZString mcLayerSetEnabled(ZCsl* csl);
    ZString mcLayerGetEnabled(ZCsl* csl);
    ZString mcLayerSetVisible(ZCsl* csl);
    ZString mcLayerGetVisible(ZCsl* csl);
    ZString mcLayerGetX(ZCsl* csl);
    ZString mcLayerGetY(ZCsl* csl);
    ZString mcLayerSetX(ZCsl* csl);
    ZString mcLayerSetY(ZCsl* csl);
    ZString mcLayerSetCycleDelay(ZCsl* csl);
    ZString mcLayerGetCycleDelay(ZCsl* csl);
    ZString mcLayerSetFrameDelay(ZCsl* csl);
    ZString mcLayerGetFrameDelay(ZCsl* csl);
    ZString mcMyX(ZCsl* csl);
    ZString mcMyY(ZCsl* csl);
    ZString mcGetMask(ZCsl* csl);
    ZString mcSetMask(ZCsl* csl);
    ZString mcGetAltitude(ZCsl* csl);
    ZString mcSetAltitude(ZCsl* csl);
    ZString mcMyHp(ZCsl* csl);
    ZString mcMyId(ZCsl* csl);
    ZString mcCanFire(ZCsl* csl);

  protected:
    virtual void ClearVisuals();
    virtual bool InVisualRange(int _x, int _y);

  protected:
    struct _StaticObject          mStaticData;

    CIGSubSysBody                *mBody;
    CPList<CIGStaticLayer>*       mLayers;
    std::vector<CIGSubSysVisual*> mVisuals;

    virtual bool CanBePlaced(CPPoint _point);        // check the object mask against the map and
                                                     // returns true if the object may be placed on the map
};
//---------------------------------------------------------------------------

#endif
