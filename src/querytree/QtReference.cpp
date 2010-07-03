/* 
 * File:   QtReference.cpp
 * Author: andrei
 * 
 * Created on June 26, 2010, 1:29 PM
 */

#include "QtReference.hpp"

QtReference::QtReference(QtNode* b): base(b), alias(NULL)
{
    TRACE << "Initializing QtReference: " << base->toString();
}

QtReference::QtReference(QtNode* b, char* al) : base(b), alias(al)
{
    TRACE << "Initializing QtReference: " << base->toString() << " as " << alias;
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
    return base->setupDbSource();
}