#include "PPIG_TransportRules.h"
#include "PPIG_ObjList.h"
#include "PPIG_ObjBase.h"

CIGTransportRules::CIGTransportRules()
{
    //ctor
}
//---------------------------------------------------------------------------

CIGTransportRules::~CIGTransportRules()
{
    //dtor
}
//---------------------------------------------------------------------------

bool CIGTransportRules::CanLoad(CIGBaseObject* target, CIGBaseObject* source)
{
    if (!target || target->GetIsDead() || !target->Can(CAN_LOAD))
    {
        return false;
    }

    if (!source || source->GetIsDead())
    {
        return false;
    }

    CPString lTargetClassName = target->ClassName();

    if (lTargetClassName != CPString(IGCN_TRANUNIT) && lTargetClassName != CPString(IGCN_DROPSHIPUNIT))
    {
        return false;
    }

    // dropship must be on ground to be able to load
    if (lTargetClassName == CPString(IGCN_DROPSHIPUNIT) && target->GetCoordZ() > 0)
    {
        return false;
    }

    return ( (source->ClassName() == CPString(IGCN_INFANTRYUNIT)) && target->Can(CAN_CARRY_INFANTRY) ) ||
           ( (source->IsDerivedFrom(CPString(IGCN_VEHICLEUNIT))) && target->Can(CAN_CARRY_VEHICLES));
}
//---------------------------------------------------------------------------

bool CIGTransportRules::CanLoadAny(CIGBaseObject* target, CIGBaseObjectList* source)
{
    // cannot load if invalid target
    if (!target || target->GetIsDead())
    {
        return false;
    }

    // can't load if nothing is in the source object list
    if (source->Count() == 0)
    {
        return false;
    }

    // cannot load into itself
    if (source->ContainsOnly(target))
    {
        return false;
    }

    // bail if the target doesn't have the load capability
    if (!target->Can(CAN_LOAD))
    {
        return false;
    }

    // for safety, check if the target object is a transporter or a dropship
    CPString lTargetClassName = target->ClassName();

    if (lTargetClassName != CPString(IGCN_TRANUNIT) && lTargetClassName != CPString(IGCN_DROPSHIPUNIT))
    {
        return false;
    }

    // dropship must be on ground to be able to load
    if (lTargetClassName == CPString(IGCN_DROPSHIPUNIT) && target->GetCoordZ() > 0)
    {
        return false;
    }

    // iterate through the source units, checking their type
    const int lSourceCount = source->Count();

    for (int i = 0; (unsigned)i < lSourceCount; i++)
    {
        CIGBaseObject* lSourceObject = source->GetObject(i);

        if ( (lSourceObject->ClassName() == CPString(IGCN_INFANTRYUNIT)) &&
                target->Can(CAN_CARRY_INFANTRY)
           )
        {
            return true;
        }

        if ( (lSourceObject->IsDerivedFrom(CPString(IGCN_VEHICLEUNIT)))  &&
                target->Can(CAN_CARRY_VEHICLES)
           )
        {
            return true;
        }
    }

    return false;
}
//---------------------------------------------------------------------------

