/* 
 * File:   QtString.cpp
 * Author: andrei
 * 
 * Created on July 2, 2010, 7:16 PM
 */

#include "querytree/QtString.hpp"
#include "HqlMain.hpp"

#include <map>
#include <string>

using namespace std;

QtString::QtString(string s): str(s)
{
}

QtString::QtString(char* s)
{
    if (s == NULL)
        s = (char*) "(null)";
    TRACE << "Initializing QtString with: " << s;
    str = string(s);
    TRACE << "Initialization ended: " << str;
}

QtString::~QtString()
{
}

HqlTable* QtString::execute()
{
    return NULL;
}

string QtString::toString()
{
    return str;
}

char* QtString::toCString()
{
    char* cstr = (char*) str.c_str();
    TRACE << "QtDot :: toCString() : " << cstr;
    return cstr;
}

DbEnum QtString::setupDbSource()
{
    /* Only Postgres understands SELECT * FROM table ... */
    if (str == string("*"))
        return POSTGRES;
    /* Else try to use the global data source dictionary. */
    map<string, DbEnum> tables = HqlMain::getInstance().tableMap;
    switch (tables.count(str))
    {
    case 0:
        return UNKNOWN_DB;
    case 1:
        DEBUG << "QtString::setupDbSource(). Found String " << str << " as a table.";
        DEBUG << "\tDatabase source: " << tables[str];
        return tables[str];
    default:
        return UNKNOWN_DB;
    }

}