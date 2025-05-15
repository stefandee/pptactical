//-----------------------------------------------------------------------------
// This file is part of PPTactical Engine.                                   //
//                                                                           //
// PPTactical Engine - engine for tactical/strategy games                    //
// Copyright (C) 1998-2017 Stefan Dicu                                       //
//                                                                           //
// PPTactical Engine is free software; you can redistribute it               //
// and/or modify it under the terms of the GNU Lesser General Public License //
// as published by the Free Software Foundation; either version 2.1 of the   //
// License, or (at your option) any later version.                           //
//                                                                           //
// PPTactical Engine is distributed in the hope that it will be useful, but  //
// WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY//
// or FITNESS FOR A PARTICULAR PURPOSE. See the GNU Lesser General Public    //
// License for more details.                                                 //
//                                                                           //
// You should have received a copy of the GNU Lesser General Public License  //
// along with PPTactical Engine; if not, write to the Free Software          //
// Foundation Inc. 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA   //
//-----------------------------------------------------------------------------

#include <string>
#include "EngineConfig.h"
#include "PP_StdLib.h"
#include "easylogging++.h"

//const std::string EngineConfig::NAME = "EngineConfigProxy";

EngineConfig::EngineConfig() /*: Proxy(NAME)*/
{
    mSelectNeutralUnits = false;
    mNeutralUnitHint = false;
    mVerboseLogLevel = 0;
    mResolutionX = 1024;
    mResolutionY = 768;
    mTestMap = "";
    mTimerUpdatePeriod = 20;
    mWindowTitle = "Pure Power Tactical Engine";
    mFullscreen = false;
}
//---------------------------------------------------------------------------

EngineConfig::~EngineConfig()
{
    //dtor
}
//---------------------------------------------------------------------------

bool EngineConfig::Load(CPString fileName)
{
    tinyxml2::XMLDocument xmlDoc;

    if (xmlDoc.LoadFile(fileName.c_str()) != tinyxml2::XML_SUCCESS)
    {
        LOG(ERROR) << "Could not load/parse engine config! " << xmlDoc.ErrorStr();
        return false;
    }

    XmlDeSerializer in(xmlDoc);

    DeSerialize(in);

    return true;
}
//---------------------------------------------------------------------------

void EngineConfig::DeSerialize(BaseDeSerializer& in)
{
    in.beginElement("document");

    if (in.hasElement("VerboseLogLevel"))
    {
        in.beginElement("VerboseLogLevel");
        //in.readElementAttribute("VerboseLogLevel", "value", mVerboseLogLevel, 0);
        in.readAttribute("value", mVerboseLogLevel, 1024);
        mVerboseLogLevel = clamp(mResolutionY, 0, 9);
        in.endElement();
    }

    if (in.hasElement("ResolutionWidth"))
    {
        in.beginElement("ResolutionWidth");
        in.readAttribute("value", mResolutionX, 1024);
        mResolutionX = std::max(640, mResolutionX);
        in.endElement();
    }

    if (in.hasElement("ResolutionHeight"))
    {
        in.beginElement("ResolutionHeight");
        in.readAttribute("value", mResolutionY, 768);
        mResolutionY = std::max(480, mResolutionY);
        in.endElement();
    }

    if (in.hasElement("Fullscreen"))
    {
        in.beginElement("Fullscreen");
        in.readAttribute("value", mFullscreen, false);
        in.endElement();
    }

    if (in.hasElement("TimerUpdatePeriod"))
    {
        in.beginElement("TimerUpdatePeriod");
        in.readAttribute("value", mTimerUpdatePeriod, 20);
        in.endElement();
    }

    if (in.hasElement("TestMap"))
    {
        in.beginElement("TestMap");
        in.readAttribute("value", mTestMap);
        in.endElement();
    }

    if (in.hasElement("WindowTitle"))
    {
        in.beginElement("WindowTitle");
        in.readAttribute("value", mWindowTitle, "Pure Power Tactical Engine");
        in.endElement();
    }

    if (in.hasElement("SelectNeutralUnits"))
    {
        in.beginElement("SelectNeutralUnits");
        in.readAttribute("value", mSelectNeutralUnits, false);
        in.endElement();
    }

    if (in.hasElement("NeutralUnitHint"))
    {
        in.beginElement("NeutralUnitHint");
        in.readAttribute("value", mNeutralUnitHint, false);
        in.endElement();
    }

    in.endElement(); /**< document */
}
//---------------------------------------------------------------------------
