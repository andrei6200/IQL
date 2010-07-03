/* 
 * File:   QtDot.cpp
 * Author: andrei
 * 
 * Created on July 1, 2010, 6:33 AM
 */

#include "QtDot.hpp"

using namespace std;

QtDot::QtDot(QtNode *n, string e): base(n), exp(e)
{
    TRACE << "Initialized QtDot with base '" << base->toString() << "' and attr: " << exp;
}

QtDot::~QtDot()
{
    if (base)
    {
        delete base;
        base = NULL;
    }
}

string QtDot::toString()
{
    return base->toString() + string(".") + exp;
}

char* QtDot::toCString()
{
    TRACE << "QtDot :: toString() : " << this->toString();
    TRACE << "QtDot :: toCString() : " << (char*) this->toString().c_str();
    return (char*) this->toString().c_str();
}

DbEnum QtDot::setupDbSource()
{
    db_source = base->setupDbSource();
    TRACE << "QtDot::setupDbSource: " << db_source;
    return db_source;
}

HqlTable* QtDot::execute()
{
    return NULL;
}