/* 
 * File:   QtRasqlInterval.cpp
 * Author: andrei
 * 
 * Created on August 31, 2010, 10:07 AM
 */

#include "querytree/QtRasqlInterval.hpp"
#include "HqlMain.hpp"
#include "QueryTree.hpp"

#include <map>
#include <string>

using namespace std;

QtRasqlInterval::QtRasqlInterval(char *low, char *high)
    : clow(NULL), chigh(NULL), slow(low), shigh(high), type(1)
{
}

QtRasqlInterval::QtRasqlInterval(QtNode *low, char *high)
    : clow(low), chigh(NULL), slow(""), shigh(high), type(2)
{
}

QtRasqlInterval::QtRasqlInterval(char *low, QtNode *high)
    : clow(NULL), chigh(high), slow(low), shigh(""), type(3)
{
}

QtRasqlInterval::QtRasqlInterval(QtNode *low, QtNode *high)
    : clow(low), chigh(high), slow(""), shigh(""), type(4)
{
}

QtRasqlInterval::~QtRasqlInterval()
{
    if (clow != NULL)
    {
        delete clow;
        clow = NULL;
    }

    if (chigh != NULL)
    {
        delete chigh;
        chigh = NULL;
    }
}

HqlTable* QtRasqlInterval::execute()
{
    PostgresDS &pg = HqlMain::getInstance().getSqlDataSource();
    HqlTable *tmp = NULL, *result = NULL, *prod = NULL;
    prod = QueryTree::getInstance().getRoot()->getCartesianProduct();
    string query, colname, tblname;

    switch (type)
    {
        case 1:
            // stringConst : stringConst
            query = "SELECT '" + slow + "'::text AS lo" + this->id + ", '" +
                                shigh + "'::text AS hi" + this->id + ", " + HQL_COL +
                    " INTO " + this->id + " FROM " + prod->getName();
            result = pg.query(query);
            
            break;
        case 2:
            // childNode : stringConst
            if (clow)
            {
                tmp = clow->execute();
                tblname = tmp->getName();
                query = "SELECT * FROM " + tblname + " LIMIT 1";
                delete tmp;
                tmp = pg.query(query);
                colname = tmp->getQualifiedColumnNames().at(0);
                delete tmp;
                query = "SELECT " + colname + ", '" + shigh + "', " + HQL_COL +
                        " INTO " + this->id + " FROM " + tblname;
                result = pg.query(query);
            }
            else
                throw string("RasqlInterval type 2 and data contents do not match ");
            break;

        case 3:
            // stringConst : childNode
            if (chigh)
            {
                tmp = chigh->execute();
                tblname = tmp->getName();
                query = "SELECT * FROM " + tblname + " LIMIT 1";
                delete tmp;
                tmp = pg.query(query);
                colname = tmp->getQualifiedColumnNames().at(0);
                delete tmp;
                query = "SELECT '" + slow + "', " + colname + ", " + HQL_COL +
                        " INTO " + this->id + " FROM " + tblname;
                result = pg.query(query);
            }
            else
                throw string("RasqlInterval type 3 and data contents do not match ");
            break;

        case 4:
            // childNode : childNode
            if (clow && chigh)
            {
                tmp = clow->execute();
                tblname = tmp->getName();
                query = "SELECT * FROM " + tblname + " LIMIT 1";
                delete tmp;
                tmp = pg.query(query);
                colname = tmp->getQualifiedColumnNames().at(0);
                delete tmp;

                tmp = chigh->execute();
                string tblname2 = tmp->getName();
                query = "SELECT * FROM " + tblname2 + " LIMIT 1";
                delete tmp;
                tmp = pg.query(query);
                string colname2 = tmp->getQualifiedColumnNames().at(0);
                delete tmp;

                query = "SELECT " + tblname + "." + colname + ", " +
                        tblname2 + "." + colname2 + ", " + HQL_COL + 
                        " INTO " + this->id +
                        " FROM " + tblname + " JOIN " + tblname2 + " USING (" + HQL_COL + ")";
                TRACE << query;
                result = pg.query(query);
            }
            else
                throw string("RasqlInterval type 4 and data contents do not match ");
            break;
    }

    pg.addTempTable(this->id);
    result->setName(this->id);
    return result;
}

string QtRasqlInterval::toString()
{
    string result("");
    if (clow)
        result += clow->toString();
    else
        result += slow;
    if (chigh)
        result += chigh->toString();
    else
        result += shigh;
    
    return result;
}

DbEnum QtRasqlInterval::setupDbSource()
{
    db_source = RASDAMAN;

    if (clow != NULL)
    {
        switch (clow->setupDbSource())
        {
            case POSTGRES:
            case MIXED:
                db_source = MIXED;
                break;
            case RASDAMAN:
                break;
            case UNKNOWN_DB:
                db_source = UNKNOWN_DB;
                break;
        }
    }
    if (chigh != NULL)
    {
        switch (chigh->setupDbSource())
        {
            case POSTGRES:
            case MIXED:
                db_source = MIXED;
                break;
            case RASDAMAN:
                break;
            case UNKNOWN_DB:
                db_source = UNKNOWN_DB;
                break;
        }
    }
    
    return db_source;

}

void QtRasqlInterval::print(ostream &o, std::string indent)
{
    o << indent << "QtRasqlInterval (" << id << "): " << endl;
    if (clow)
        clow->print(o, indent + QTINDENT);
    else
        o << indent << QTINDENT << slow << endl;
    if (chigh)
        chigh->print(o, indent + QTINDENT);
    else
        o << indent << QTINDENT << shigh << endl;
}