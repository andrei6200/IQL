/* 
 * File:   QtReference.cpp
 * Author: andrei
 * 
 * Created on June 26, 2010, 1:29 PM
 */

#include "QtReference.hpp"

QtReference::QtReference(char* str): name(str), alias(NULL)
{
    TRACE << "Initializing QtReference: " << name;
}

QtReference::QtReference(char* str, char* al) : name(str), alias(al)
{
    TRACE << "Initializing QtReference: " << name << " as " << alias;
}

QtReference::~QtReference()
{
    if (name)
        delete name;
    if (alias)
        delete alias;
}

HqlTable* QtReference::execute()
{
    return NULL;
}

string QtReference::toString()
{
    TRACE << "QtReference :: toString()";
    string out(name);
    if (alias)
        out += string(" AS ") + string(alias);
    return out;
}

DbEnum QtReference::setupDbSource()
{
    return UNKNOWN_DB;
}