#ifndef CIGTRANSPORTRULES_H
#define CIGTRANSPORTRULES_H

class CIGBaseObject;
class CIGBaseObjectList;

class CIGTransportRules
{
    public:
        CIGTransportRules();
        virtual ~CIGTransportRules();

    public:
        bool CanLoad(CIGBaseObject* target, CIGBaseObject* source);
        bool CanLoadAny(CIGBaseObject* target, CIGBaseObjectList* source);

    protected:

    private:
};

#endif // CIGTRANSPORTRULES_H
