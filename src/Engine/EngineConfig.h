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
#ifndef ENGINECONFIG_H
#define ENGINECONFIG_H

#include "XmlSerializable.h"
//#include "pmvcpp.h"

class EngineConfig : protected Serializable2/*, public PureMVC::Proxy*/
{
public:
    //static const std::string NAME;

    EngineConfig();
    virtual ~EngineConfig();

    void onRegister() {}
	void onRemove() {}

	bool Load(CPString fileName);

public:
    bool GetSelectNeutralUnits()
    {
        return mSelectNeutralUnits;
    }

    bool GetNeutralUnitHint()
    {
        return mNeutralUnitHint;
    }

    int GetVerboseLogLevel()
    {
        return mVerboseLogLevel;
    }

    int GetResolutionX()
    {
        return mResolutionX;
    }

    int GetResolutionY()
    {
        return mResolutionY;
    }

    std::string GetTestMap()
    {
        return mTestMap;
    }

    int GetTimerUpdatePeriod()
    {
        return mTimerUpdatePeriod;
    }

    std::string GetWindowTitle()
    {
        return mWindowTitle;
    }

    bool GetFullscreen()
    {
        return mFullscreen;
    }

protected:
    void Serialize(BaseSerializer& out) {};
    void DeSerialize(BaseDeSerializer& in);

private:
    bool mSelectNeutralUnits;
    bool mNeutralUnitHint;
    int mVerboseLogLevel;
    int mResolutionX;
    int mResolutionY;
    std::string mTestMap;
    int mTimerUpdatePeriod;
    std::string mWindowTitle;
    bool mFullscreen;
};

#endif // ENGINECONFIG_H
