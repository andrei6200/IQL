/* 
 * File:   QtRasqlFunction.cpp
 * Author: andrei
 * 
 * Created on August 4, 2010, 1:47 PM
 */

#include "QtRasqlFunction.hpp"

using namespace std;


QtRasqlFunction::QtRasqlFunction(string name, string s)
    : fname(name), stringCount(1), nodeCount(0), str1(s),
    child1(NULL), child2(NULL)
{
}

QtRasqlFunction::QtRasqlFunction(string name, QtNode *node)
    : fname(name), stringCount(0), nodeCount(1), child1(node), child2(NULL)
{
}

QtRasqlFunction::QtRasqlFunction(string name, string s1, string s2)
    : fname(name), stringCount(2), nodeCount(0), str1(s1), str2(s2),
    child1(NULL), child2(NULL)
{
}

QtRasqlFunction::QtRasqlFunction(string name, QtNode* node, string s2)
    : fname(name), stringCount(1), nodeCount(1), str1(s2), 
    child1(node), child2(NULL)
{
}

QtRasqlFunction::QtRasqlFunction(string name, QtNode* node1, QtNode* node2)
    : fname(name), stringCount(0), nodeCount(2),
    child1(node1), child2(node2)
{
}


QtRasqlFunction::~QtRasqlFunction()
{
    if (child1)
    {
        delete child1;
        child1 = NULL;
    }
    if (child2)
    {
        delete child2;
        child2 = NULL;
    }
}

string QtRasqlFunction::toString()
{
    bool error = false;
    string out(fname);
    out += "(";
    switch (nodeCount)
    {
        case 2:
            switch (stringCount)
            {
                case 2:
                case 1:
                    error = true;
                    break;
                case 0:
                    out += child1->toString() + ", " + child2->toString();
            }
            break;
        case 1:
            switch (stringCount)
            {
                case 2:
                    error = true;
                    break;
                case 1:
                    out += child1->toString() + ", " + str1;
                    break;
                case 0:
                    out += child1->toString();
                    break;
            }
            break;
        case 0:
            switch (stringCount)
            {
                case 2:
                    out += str1 + ", " + str2;
                    break;
                case 1:
                    out += str1;
                    break;
                case 0:
                    // should be impossible 
                    error = true;
            }
    }
    out += ")";
    return out;
}

DbEnum QtRasqlFunction::getDbSource()
{
    return RASDAMAN;
}

DbEnum QtRasqlFunction::setupDbSource()
{
    db_source = RASDAMAN;
    return db_source;
}

HqlTable* QtRasqlFunction::execute()
{
    // FIXME: implement
    return NULL;
}
