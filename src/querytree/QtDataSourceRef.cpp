/* 
 * File:   QtDataSourceRef.cpp
 * Author: andrei
 * 
 * Created on June 26, 2010, 1:29 PM
 */

#include <typeinfo>

#include "QtDataSourceRef.hpp"
#include "QtDot.hpp"

QtDataSourceRef::QtDataSourceRef(QtNode* b): base(b), alias(NULL), all(false)
{
    TRACE << "Initializing QtDataSourceRef: " << base->toString();
    TRACE << "  Using base class: " << typeid(*b).name();
    if (b->toString() == "*")
        all = true;
}

QtDataSourceRef::QtDataSourceRef(QtNode* b, char* al) : base(b), alias(al), all(false)
{
    TRACE << "Initializing QtDataSourceRef: " << base->toString() << " as " << alias;
    TRACE << "  Using base class: " << typeid(*b).name();
}

QtDataSourceRef::~QtDataSourceRef()
{
    if (base)
        delete base;
    if (alias)
        delete alias;
}

HqlTable* QtDataSourceRef::execute()
{
    return NULL;
}

string QtDataSourceRef::toString()
{
    
    string out(base->toString());
    if (alias)
        out += string(" AS ") + string(alias);
    TRACE << "QtDataSourceRef :: toString() : " << out;
    return out;
}

char* QtDataSourceRef::toCString()
{
    return (char*) this->toString().c_str();
}

DbEnum QtDataSourceRef::setupDbSource()
{
    db_source = base->setupDbSource();
    return db_source;
}

bool QtDataSourceRef::selectsAll()
{
    return all;
}