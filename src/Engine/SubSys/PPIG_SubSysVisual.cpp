//---------------------------------------------------------------------------
#include <math.h>
#include <stdlib.h>
#pragma hdrstop

#include "PPIG_SubSysVisual.h"
#include "PPIG_ObjReal.h"
#include "PPIG_Mission.h"
#include "PPIG_Map.h"
#include "basic3d.h"
//---------------------------------------------------------------------------

CIGSubSysVisual::CIGSubSysVisual(CIGRealObject* _parent) : CIGSubSysBase(_parent)
{
    // look variables
    mMaxScanRange = 12;
    mMinScanRange = 6;
    mMinScanAngle = 0;
    mMaxScanAngle = 360;
    mScanRange    = mMinScanRange;
    mFOV          = 140;
    mScanAngle    = 0;//gDirAngle[mPropulsion->GetSenseCounter()] * 180.0 / PI;
    mLookAt       = UL_NOTHING;
    mLookObj      = 0;
    mScanRangeCounter = CPCounter(16);

    // provide default values for the mount points (relative to the top-left)
    for(int i = 0; i < 8; i++)
    {
        SetMountPoint(i, CPPoint(0, 0));
    }

    mUnitsInView = new CIGBaseObjectList();//new CPList<CIGBaseObject>(2);
}
//---------------------------------------------------------------------------

CIGSubSysVisual::~CIGSubSysVisual()
{
    mLookAt       = UL_NOTHING;
    mLookObj      = 0;

    delete mUnitsInView;
}
//---------------------------------------------------------------------------

void CIGSubSysVisual::SetLookObj(CIGBaseObject* _v)
{
    mLookObj = _v;
}
//---------------------------------------------------------------------------

void CIGSubSysVisual::Update()
{
    if (mLookAt == UL_OBJECT && mLookObj && mLookObj->GetIsDead())
    {
        mLookAt = UL_NOTHING;
        mLookObj = NULL;
    }

    CIGRealObject* lParent = (CIGRealObject*)GetParent();

    // timer to limit the increase of the scan range
    mScanRangeCounter.Inc();

    if (mScanRangeCounter.GetMaxReached())
    {
        if (mScanRange < mMaxScanRange)
        {
            mScanRange++;
        }

        mScanRangeCounter.Reset();
    }

    CPPoint lPoint;

    // keep looking angle
    switch(mLookAt)
    {
        case UL_POINT:
            mScanAngle = CenterAngle(mLookPoint.x - lParent->GetLogicCoord().x, mLookPoint.y - lParent->GetLogicCoord().y);
            //mScanRange = 6;
            break;

        case UL_OBJECT:
            //lPoint = mLookObj->GetLogicCoord();
            lPoint = mLookObj->GetClosestPointTo(lParent->GetLogicCoord());

            if (InVisualRange(lPoint.x, lPoint.y))
            {
                mScanAngle = CenterAngle(lPoint.x - lParent->GetLogicCoord().x, lPoint.y - lParent->GetLogicCoord().y);
                //mScanRange = 6;
            }
            else
            {
                mLookAt = UL_NOTHING;
            }

            break;

        default:
            break;
    }

    //mUnitsInView->Clear();
}
//---------------------------------------------------------------------------

void CIGSubSysVisual::SetLookPoint(CPPoint _v)
{
    CIGRealObject* lParent = (CIGRealObject*)GetParent();

    mLookPoint = _v;
    mScanAngle = CenterAngle(mLookPoint.x - lParent->GetLogicCoord().x, mLookPoint.y - lParent->GetLogicCoord().y);
}
//---------------------------------------------------------------------------

void CIGSubSysVisual::SetFOV(int _v)
{
    if (_v < 60)
    {
        return;
    }

    mFOV = _v % 360;
}
//---------------------------------------------------------------------------

void CIGSubSysVisual::SetScanRange(int _v)
{
    if (_v < mMaxScanRange)
    {
        mScanRange = _v;
    }
}
//---------------------------------------------------------------------------

void CIGSubSysVisual::SetMinScanRange(int _v)
{
    if (_v > 0)
    {
        if (_v > mMaxScanRange)
        {
            _v = mMaxScanRange;
        }

        mMinScanRange = _v;
        mScanRange    = mMinScanRange;
    }
}
//---------------------------------------------------------------------------

void CIGSubSysVisual::SetMaxScanRange(int _v)
{
    if (_v > 0)
    {
        mMaxScanRange = _v;
        mScanRange = mMaxScanRange / 2;
    }
}
//---------------------------------------------------------------------------

void CIGSubSysVisual::SetMinScanAngle(int _v)
{
    if (_v > mMaxScanAngle)
    {
        _v = mMaxScanAngle;
        mScanAngle = _v;
    }

    mMinScanAngle = _v;
}
//---------------------------------------------------------------------------

void CIGSubSysVisual::SetMaxScanAngle(int _v)
{
    if (_v < mMinScanAngle)
    {
        _v = mMinScanAngle;
        mScanAngle = _v;
    }

    mMaxScanAngle = _v;
}
//---------------------------------------------------------------------------

int  CIGSubSysVisual::GetUnitsInViewId(int _index)
{
    return mUnitsInView->GetObject(_index)->GetId();
}
//---------------------------------------------------------------------------

bool CIGSubSysVisual::InVisualRange(int _x, int _y)
{
    CIGRealObject* lParent = (CIGRealObject*)GetParent();
    int lMin1 = mScanAngle - mFOV/2, lMin2 = mScanAngle, lMax1 = mScanAngle, lMax2 = mScanAngle + mFOV/2;

    if ((_x < 0) || (_y < 0) ||
            (_x > GetMissionInstance()->GetMap()->GetLogicWidth()) ||
            (_y > GetMissionInstance()->GetMap()->GetLogicHeight()))
    {
        return false;
    }

    if (lMin1 < 0)
    {
        lMin1 = 360 + lMin1;
        lMax1 = 359;
        lMin2 = 0;
        lMax2 = mScanAngle + mFOV/2;
    }

    if (lMax2 > 360)
    {
        lMax2 = lMax2 - 360;
        lMin2 = 0;
        lMax1 = 359;
        lMin1 = mScanAngle - mFOV/2;
    }

    // if the point is not in the scan range, then return false
    CPPoint lObsPoint = CPPoint(
                            lParent->GetLogicCoord().x + GetMountPoint(0).x / MAPCELLX,
                            lParent->GetLogicCoord().y + GetMountPoint(0).y / MAPCELLY
                        );

    if (gfGetDist2D(CPPoint(_x, _y), lObsPoint) > mScanRange)
    {
        return false;
    }

    // check if the point is in scan angle aperture (the view "pie")
    float lDir = CenterAngle(_x - lObsPoint.x, _y - lObsPoint.y);

    if ((lDir < lMin1 || lDir > lMax1) && (lDir < lMin2 || lDir > lMax2))
    {
        return false;
    }

    // now scan a line from view point to target point and check altitudes
    // if a higher altitude than that of the source point is interposed, then
    // the target point is not in visual range

    // prepare to scan the line  - no Bresenham :)
    float x1 = lObsPoint.x;
    float y1 = lObsPoint.y;
    float x2 = _x;
    float y2 = _y;

    float lM;
    float lReferenceAltitude = GetMissionInstance()->GetMap()->GetAltitude(x1, y1);

    if (abs(y2 - y1) > abs(x2 - x1))
    {
        lM = (float)(x2 - x1) / (y2 - y1);

        int   y = y1;
        float x = x1;
        int   s = SIGN(y2 - y1);
        lM *= s;

        while (y != (int)y2)
        {
            if (GetMissionInstance()->GetMap()->GetAltitude(x, y) > lReferenceAltitude)
            {
                return false;
            }

            y += s;
            x += lM;
        }
    }
    else
    {
        lM = (float)(y2 - y1) / (x2 - x1);

        float y = y1;
        int   x = x1;
        int   s = SIGN(x2 - x1);
        lM *= s;

        while (x != (int)x2)
        {
            if (GetMissionInstance()->GetMap()->GetAltitude(x, y) > lReferenceAltitude)
            {
                return false;
            }

            x += s;
            y += lM;
        }
    }

    // the point is visible
    return true;
}
//---------------------------------------------------------------------------

int  CIGSubSysVisual::GetUnitsInViewCount()
{
    return mUnitsInView->Count();
}
//---------------------------------------------------------------------------

void CIGSubSysVisual::CreateViewPoints(std::vector<CPPoint>& lPoints)
{
    CIGMap* lMap = GetMissionInstance()->GetMap();
    CIGRealObject* lParent = (CIGRealObject*)GetParent();

    int parentX = lParent->GetLogicCoord().x, parentY = lParent->GetLogicCoord().y, parentDimX = lParent->GetLogicDimX(), parentDimY = lParent->GetLogicDimY();

    // add tiles under the object to the view list
    for(int x = parentX; x < parentX + parentDimX; x++)
    {
        for(int y = parentY; y < parentY + parentDimY; y++)
        {
            lPoints.push_back(CPPoint(x, y));
        }
    }

    if (mScanRange > 1)
    {
        // the reference altitude is the altitude of the tile the object is sitting on
        // to the coordinates will also add the relative position of the observation point
        // TODO: find a way to extract orientation data from parent
        int mountX = parentX + GetMountPoint(0).x / MAPCELLX;
        int mountY = parentY + GetMountPoint(0).y / MAPCELLY;
        float lReferenceAltitude = lMap->GetAltitude(mountX, mountY);

        float startAngle = (mScanAngle - mFOV / 2) * PI / 180.0f;
        float endAngle = (mScanAngle + mFOV / 2) * PI / 180.0f;

        // experimental value, adjust depending on how large the scan range is
        float deltaAngle = PI / (4.0f * mScanRange);

        do
        {
            int x2 = mountX + mScanRange * cos(startAngle);
            int y2 = mountY + mScanRange * sin(startAngle);
            startAngle += deltaAngle;

            float lM;

            if (abs(y2 - mountY) > abs(x2 - mountX))
            {
                lM = (float)(x2 - mountX) / (y2 - mountY);

                int   y = mountY;
                float x = mountX;

                int   s = SIGN(y2 - mountY);
                lM *= s;

                do
                {
                    y += s;
                    x += lM;
                    lPoints.push_back(CPPoint(x, y));
                }
                while (y != y2 && lMap->GetAltitude(x, y) <= lReferenceAltitude);
            }
            else
            {
                lM = (float)(y2 - mountY) / (x2 - mountX);

                float y = mountY;
                int   x = mountX;
                int   s = SIGN(x2 - mountX);

                lM *= s;

                do
                {
                    x += s;
                    y += lM;
                    lPoints.push_back(CPPoint(x, y));
                }
                while (x != x2 && lMap->GetAltitude(x, y) <= lReferenceAltitude);
            }
        }
        while(startAngle < endAngle);
    }
}
//---------------------------------------------------------------------------

void CIGSubSysVisual::UpdateView(int _value)
{
    std::vector<CPPoint> lPoints;

    CreateViewPoints(lPoints);

    CIGMap* lMap = GetMissionInstance()->GetMap();
    CIGRealObject* lParent = (CIGRealObject*)GetParent();

    lMap->NotifyVisibilityMap(lParent->GetBattleGroupId(), lPoints.data(), lPoints.size(), _value);
}
//---------------------------------------------------------------------------

void CIGSubSysVisual::UnitsInView()
{
    CIGRealObject* lParent = (CIGRealObject*)GetParent();
    CIGMap* lMap = GetMissionInstance()->GetMap();

    // TODO: add a counter here, to limit the scans/frame
    // in any case, this is limited by the updatetimeperiod parameter of
    // each object (infantry, tank, artillery) - check the Update method of
    // CIGInfantry, CIGTankUnit
    mUnitsInView->RemoveAll();

    std::vector<CPPoint> lPoints;

    CreateViewPoints(lPoints);

    for (std::vector<CPPoint>::iterator it = lPoints.begin(); it != lPoints.end(); ++it)
    {
        CIGBaseObjectList* objList = lMap->GetObjectsAt((*it).x, (*it).y);

        if (objList)
        {
            mUnitsInView->AddUnique(objList);
        }
    }

    // remove itself from the list
    mUnitsInView->Remove(lParent);
}
//---------------------------------------------------------------------------

void CIGSubSysVisual::ResetScan()
{
    mScanRange = mMinScanRange;
}
//---------------------------------------------------------------------------

/*
void CIGSubSysVisual::UnitsInView()
{
  CIGRealObject* lParent = (CIGRealObject*)GetParent();
  CIGBaseObject* lObj;

  // TODO: add a counter here, to limit the scans/second
  mUnitsInView->Clear();

  // the whole process should be optimized; now it's a trial-and-error alg.
  // basically this is an (circle) sector filling algorithm, but I'm damn
  // too lazy to implement this :)

  // init some intervals
  int lMin1 = mScanAngle - mFOV/2, lMin2 = mScanAngle, lMax1 = mScanAngle, lMax2 = mScanAngle + mFOV/2;

  if (lMin1 < 0)
  {
    lMin1 = 360 + lMin1;
    lMax1 = 359;
    lMin2 = 0;
    lMax2 = mScanAngle + mFOV/2;
  }

  if (lMax2 > 360)
  {
    lMax2 = lMax2 - 360;
    lMin2 = 0;
    lMax1 = 359;
    lMin1 = mScanAngle - mFOV/2;
  }

  // do the scan
  CPPoint lCoord = lParent->GetLogicCoord();
  for(int x = lCoord.x - mScanRange; x < lCoord.x + mScanRange; x++)
  {
    for(int y = lCoord.y - mScanRange; y < lCoord.y + mScanRange; y++)
    {
      if ((x - lCoord.x) * (x - lCoord.x) + (y - lCoord.y) * (y - lCoord.y) <= mScanRange * mScanRange)
      {
        float lDir = CenterAngle(x - lCoord.x, -y + lCoord.y);

        if ((lDir >= lMin1 && lDir <= lMax1) || (lDir >= lMin2 && lDir <= lMax2))
        {
          // is there a unit in view?
          lObj = GetMissionInstance()->GetMap()->GetGroundObject(x, y);

          // add it if valid
          if (lObj)
          {
            if (!mUnitsInView->Contain(lObj))
            {
              mUnitsInView->Add(lObj);
            }
          }
        }
      }
    }
  }
}
//---------------------------------------------------------------------------
*/

//---------------------------------------------------------------------------
//
// Class serialize/deserialize section
//
//---------------------------------------------------------------------------

void CIGSubSysVisual::Serialize(PP::Stream& _a)
{
    CSerPoint  lLookPoint(mLookPoint);
    int        lLookObjId = -1, lInViewCount;
    int        lLookAt = (int)mLookAt;

    // base class ser
    CIGSubSysBase::Serialize(_a);

    // setup
    if (mLookObj)
    {
        lLookObjId = mLookObj->GetId();
    }

    lInViewCount = mUnitsInView->Count();

    _a.writeInt(&mScanAngle );
    _a.writeInt(&mFOV );
    _a.writeInt(&mScanRange );
    _a.writeInt(&mMinScanRange );
    _a.writeInt(&mMaxScanRange );
    _a.writeInt(&mMinScanAngle );
    _a.writeInt(&mMaxScanAngle );
    _a.writeInt(&lLookAt );
    lLookPoint.Serialize(_a);
    _a.writeInt(&lLookObjId);

    _a.writeInt(&lInViewCount);
    for(int i = 0; i < lInViewCount; i++)
    {
        int lId = mUnitsInView->GetObject(i)->GetId();

        _a.writeInt(&lId);
    }
}
//---------------------------------------------------------------------------

unsigned __int32 CIGSubSysVisual::DataSize()
{
    return 0;
}
//---------------------------------------------------------------------------

void CIGSubSysVisual::DeSerialize(PP::Stream& _a)
{
    CSerPoint  lLookPoint;
    int        lLookObjId, lInViewCount, lLookAt;

    // base class deser
    CIGSubSysBase::DeSerialize(_a);

    // cleanup
    mUnitsInView->RemoveAll();

    // parameters deser
    _a.readInt(&mScanAngle );
    _a.readInt(&mFOV );
    _a.readInt(&mScanRange );
    _a.readInt(&mMinScanRange );
    _a.readInt(&mMaxScanRange );
    _a.readInt(&mMinScanAngle );
    _a.readInt(&mMaxScanAngle );
    _a.readInt(&lLookAt );
    lLookPoint.DeSerialize(_a);
    _a.readInt(&lLookObjId );

    _a.readInt(&lInViewCount );
    for(int i = 0; i < lInViewCount; i++)
    {
        int lId;
        _a.readInt(&lId );

        CIGBaseObject *lObj = GetMissionInstance()->GetObjectById(lId);

        if (lObj)
        {
            mUnitsInView->Add(lObj);
        }
    }

    // setup
    mLookPoint  = lLookPoint.Get();
    mLookObj    = GetMissionInstance()->GetObjectById(lLookObjId);
    mLookAt     = (TUnitLook)lLookAt;
}
//---------------------------------------------------------------------------

#ifdef __BORLANDC__
#pragma package(smart_init)
#endif
