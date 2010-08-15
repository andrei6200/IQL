/* 
 * File:   QtDot.cpp
 * Author: andrei
 * 
 * Created on July 1, 2010, 6:33 AM
 */

#include "QtDot.hpp"
#include "HqlMain.hpp"

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
    setupDbSource();

    HqlTable *result = NULL, *tmp = NULL;
    string query;

    TRACE << "starting execution... ";
    
    switch (db_source)
    {
        case POSTGRES:
            tmp = base->execute();
            TRACE << "Child table: " << endl << tmp;
            query = "SELECT " + exp + " INTO " + this->id
                         + " FROM " + tmp->getName();
            delete tmp; 
            result = HqlMain::getInstance().runSqlQuery(query);
            result->setName(this->id);
            TRACE << "Current node table" << endl << result;
            break;
            
        case RASDAMAN:
            WARN << "Executing rasdaman query ... ";
            break;
            
        case MIXED:
        default:
            WARN << "Executing mixed query ... ";
    }

    TRACE << "execution finished... ";

    return result;
}

void QtDot::print(ostream &o, std::string indent)
{
    o << indent << "QtDot (" << id << "): " << base->toString() << "." << exp << endl;
    base->print(o, indent + QTINDENT);
}