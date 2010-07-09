/* 
 * File:   QtReference.cpp
 * Author: andrei
 * 
 * Created on June 26, 2010, 1:29 PM
 */

#include <typeinfo>

#include "QtReference.hpp"
#include "QtDot.hpp"

QtReference::QtReference(QtNode* b): base(b), alias(NULL)
{
    TRACE << "Initializing QtReference: " << base->toString();
    TRACE << "  Using base class: " << typeid(*b).name();
}

QtReference::QtReference(QtNode* b, char* al) : base(b), alias(al)
{
    TRACE << "Initializing QtReference: " << base->toString() << " as " << alias;
    TRACE << "  Using base class: " << typeid(*b).name();
}

QtReference::~QtReference()
{
    if (base)
        delete base;
    if (alias)
        delete alias;
}

HqlTable* QtReference::execute()
{
    return NULL;
}

string QtReference::toString()
{
    
    string out(base->toString());
    if (alias)
        out += string(" AS ") + string(alias);
    TRACE << "QtReference :: toString() : " << out;
    return out;
}

char* QtReference::toCString()
{
    return (char*) this->toString().c_str();
}

DbEnum QtReference::setupDbSource()
{
    db_source = base->setupDbSource();
    return db_source;
}
