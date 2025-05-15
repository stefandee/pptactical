//-----------------------------------------------------------------------------
// This file is part of PPTactical Engine.                                   //
//                                                                           //
// PPTactical Engine - engine for tactical/strategy games                    //
// Copyright (C) 1998-2004 Stefan Dicu                                       //
//                                                                           //
// PPTactical Engine is free software; you can redistribute it               //
// and/or modify it under the terms of the GNU Lesser General Public License //
// as published by the Free Software Foundation; either version 2.1 of the   //
// License, or (at your option) any later version.                           //
//                                                                           //
// PPTactical Engine is distributed in the hope that it will be useful, but  //
// WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY//
// or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU Lesser General Public   //
// License for more details.                                                 //
//                                                                           //
// You should have received a copy of the GNU Lesser General Public License  //
// along with PPTactical Engine; if not, write to the Free Software          //
// Foundation Inc. 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA   //
//-----------------------------------------------------------------------------
#pragma hdrstop

#include "PPIG_ObjectCreator.h"
#include "PPIG_ObjEffect.h"
#include "PPIG_ObjInfantry.h"
#include "PPIG_ObjStatic.h"
#include "PPIG_ObjFactory.h"
#include "PPIG_ObjDropship.h"
#include "PPIG_ObjTank.h"
#include "PPIG_ObjRecon.h"
#include "PPIG_ObjTran.h"
#include "PPIG_ObjChopper.h"
#include "PPIG_ObjFighter.h"
#include "PPIG_ObjArtillery.h"
#include "PPIG_ObjRocket.h"
#include "PPIG_ObjLaser.h"
#include "PPIG_ObjBullet.h"
#include "PPIG_ObjBallistic.h"
#include "PPIG_ObjDamage.h"
#include "PPIG_MapData.h"
//---------------------------------------------------------------------------

CIGBaseObject* CIGObjectCreator::CreateObjectFromId(int _id)
{
  CIGBaseObject* lObject = 0;

  switch(_id)
  {
     case OBJT_STATIC:
        VLOG(2) << "..Creating static object ...";
        try
        {
          lObject = new CIGStaticObject();
        }
        catch(...)
        {
          LOG(FATAL) << "UNABLE TO ALLOCATE MEMORY FOR STATIC OBJECT !!!!!!!!!!";
          throw CErrorGeneric();
        }
        break;

     case OBJT_STATIC_FACTORY:
        VLOG(2) << "..Creating static factory object ...";
        try
        {
          lObject = new CIGFactoryObject();
        }
        catch(...)
        {
          LOG(FATAL) << "UNABLE TO ALLOCATE MEMORY FOR STATIC FACTORY OBJECT !!!!!!!!!!";
          throw CErrorGeneric();
        }
        break;

     case OBJT_INFANTRY:
        VLOG(2) << "..Creating infantry unit ...";
        try
        {
           lObject = new CIGInfantryUnit();
        }
        catch(...)
        {
          LOG(FATAL) << "UNABLE TO ALLOCATE MEMORY FOR INFANTRY OBJECT !!!!!!!!!!";
          throw CErrorGeneric();
        }
        break;

     case OBJT_TANK:
        VLOG(2) << "..Creating tank unit ...";
        try
        {
           lObject = new CIGTankUnit();
        }
        catch(...)
        {
          LOG(FATAL) << "UNABLE TO ALLOCATE MEMORY FOR TANK OBJECT !!!!!!!!!!";
          throw CErrorGeneric();
        }
        break;

     case OBJT_TRAN:
        VLOG(2) << "..Creating transport unit ...";
        try
        {
           lObject = new CIGTranUnit();
        }
        catch(...)
        {
          LOG(FATAL) << "UNABLE TO ALLOCATE MEMORY FOR TRAN OBJECT !!!!!!!!!!";
          throw CErrorGeneric();
        }
        break;

     case OBJT_RECON:
        VLOG(2) << "..Creating recon unit ...";
        try
        {
           lObject = new CIGReconUnit();
        }
        catch(...)
        {
          LOG(FATAL) << "UNABLE TO ALLOCATE MEMORY FOR RECON OBJECT !!!!!!!!!!";
          throw CErrorGeneric();
        }
        break;

     case OBJT_ARTILLERY:
        VLOG(2) << "..Creating artillery unit ...";
        try
        {
           lObject = new CIGArtilleryUnit();
        }
        catch(...)
        {
          LOG(FATAL) << "UNABLE TO ALLOCATE MEMORY FOR ARTILLERY OBJECT !!!!!!!!!!";
          throw CErrorGeneric();
        }
        break;

     case OBJT_CHOPPER:
        VLOG(2) << "..Creating chopper unit ...";
        try
        {
           lObject = new CIGChopperUnit();
        }
        catch(...)
        {
          LOG(FATAL) << "UNABLE TO ALLOCATE MEMORY FOR CHOPPER OBJECT !!!!!!!!!!";
          throw CErrorGeneric();
        }
        break;

     case OBJT_FIGHTER:
        VLOG(2) << "..Creating fighter unit ...";
        try
        {
           lObject = new CIGFighterUnit();
        }
        catch(...)
        {
          LOG(FATAL) << "UNABLE TO ALLOCATE MEMORY FOR FIGHTER OBJECT !!!!!!!!!!";
          throw CErrorGeneric();
        }
        break;

     case OBJT_DROPSHIP:
        VLOG(2) << "..Creating dropship unit ...";
        try
        {
           lObject = new CIGDropship();
        }
        catch(...)
        {
          LOG(FATAL) << "UNABLE TO ALLOCATE MEMORY FOR DROPSHIP OBJECT !!!!!!!!!!";
          throw CErrorGeneric();
        }
        break;

     case OBJT_ROCKET:
        VLOG(2) << "..Creating rocket object ...";
        try
        {
          lObject = new CIGRocketObject();
        }
        catch(...)
        {
          LOG(FATAL) << "UNABLE TO ALLOCATE MEMORY FOR ROCKET OBJECT !!!!!!!!!!";
          throw CErrorGeneric();
        }
        break;

     case OBJT_EFFECT:
        VLOG(2) << "..Creating effect object ...";
        try
        {
          lObject = new CIGEffectObject();
        }
        catch(...)
        {
          LOG(FATAL) << "UNABLE TO ALLOCATE MEMORY FOR EFFECT OBJECT !!!!!!!!!!";
          throw CErrorGeneric();
        }
        break;

     case OBJT_LASER:
        VLOG(2) << "..Creating laser object ...";
        try
        {
          lObject = new CIGLaserObject();
        }
        catch(...)
        {
          LOG(FATAL) << "UNABLE TO ALLOCATE MEMORY FOR LASER OBJECT !!!!!!!!!!";
          throw CErrorGeneric();
        }
        break;

     case OBJT_BULLET:
        VLOG(2) << "..Creating bullet object ...";
        try
        {
          lObject = new CIGBulletObject();
        }
        catch(...)
        {
          LOG(FATAL) << "UNABLE TO ALLOCATE MEMORY FOR BULLET OBJECT !!!!!!!!!!";
          throw CErrorGeneric();
        }
        break;

     case OBJT_BALLISTIC:
        VLOG(2) << "..Creating ballistic object ...";
        try
        {
          lObject = new CIGBallisticObject();
        }
        catch(...)
        {
          LOG(FATAL) << "UNABLE TO ALLOCATE MEMORY FOR BALLISTIC OBJECT !!!!!!!!!!";
          throw CErrorGeneric();
        }
        break;

     default:
        // invalid id, probably a bad map
        LOG(FATAL) << "UNKNOWN UNIT ID";
        throw CErrorGeneric();
  }

  return lObject;
}
//---------------------------------------------------------------------------

int CIGObjectCreator::ObjectClassNameToId (const CPString& _className)
{
  int lObjType = -1;

  // TODO: these values should be put into a map
  if (_className == CPString(IGCN_FACTORYOBJECT))
  {
    lObjType = OBJT_STATIC_FACTORY;
  }

  if (_className == CPString(IGCN_STATICOBJECT))
  {
    lObjType = OBJT_STATIC;
  }

  if (_className == CPString(IGCN_INFANTRYUNIT))
  {
    lObjType = OBJT_INFANTRY;
  }

  if (_className == CPString(IGCN_TANKUNIT))
  {
    lObjType = OBJT_TANK;
  }

  if (_className == CPString(IGCN_TRANUNIT))
  {
    lObjType = OBJT_TRAN;
  }

  if (_className == CPString(IGCN_RECONUNIT))
  {
    lObjType = OBJT_RECON;
  }

  if (_className == CPString(IGCN_ARTILLERYUNIT))
  {
    lObjType = OBJT_ARTILLERY;
  }

  if (_className == CPString(IGCN_CHOPPERUNIT))
  {
    lObjType = OBJT_CHOPPER;
  }

  if (_className == CPString(IGCN_FIGHTERUNIT))
  {
    lObjType = OBJT_FIGHTER;
  }

  if (_className == CPString(IGCN_BOMBERUNIT))
  {
    lObjType = OBJT_BOMBER;
  }

  if (_className == CPString(IGCN_DROPSHIPUNIT))
  {
    lObjType = OBJT_DROPSHIP;
  }

  if (_className == CPString(IGCN_ROCKETOBJECT))
  {
    lObjType = OBJT_ROCKET;
  }

  if (_className == CPString(IGCN_EFFECTOBJECT))
  {
    lObjType = OBJT_EFFECT;
  }

  if (_className == CPString(IGCN_LASEROBJECT))
  {
    lObjType = OBJT_LASER;
  }

  if (_className == CPString(IGCN_BULLETOBJECT))
  {
    lObjType = OBJT_BULLET;
  }

  if (_className == CPString(IGCN_BALLISTICOBJECT))
  {
    lObjType = OBJT_BALLISTIC;
  }

  return lObjType;
}
//---------------------------------------------------------------------------

#ifdef __BORLANDC__
#pragma package(smart_init)
#endif
