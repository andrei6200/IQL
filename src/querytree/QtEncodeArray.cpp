/* 
 * File:   QtEncodeArray.cpp
 * Author: andrei
 * 
 * Created on August 4, 2010, 1:47 PM
 */

#include "QtEncodeArray.hpp"

using namespace std;

QtEncodeArray::QtEncodeArray(QtNode *node, string fmt)
    : child(node), format(fmt), options("")
{
}

QtEncodeArray::QtEncodeArray(QtNode *node, string fmt, string opt)
    : child(node), format(fmt), options(opt)
{
}

QtEncodeArray::~QtEncodeArray()
{
    if (child)
    {
        delete child;
        child = NULL;
    }
}

string QtEncodeArray::toString()
{
    string repr(format);
    repr += "(";
    repr += child->toString();
    if (options != "")
    {
        repr += ",";
        repr += options;
    }
    repr += ")";
}

DbEnum QtEncodeArray::setupDbSource()
{
    db_source = RASDAMAN;
    return db_source;
}

HqlTable* QtEncodeArray::execute()
{
    // FIXME: implement
    return NULL;
}