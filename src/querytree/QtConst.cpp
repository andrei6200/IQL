/* 
 * File:   QtConst.cpp
 * Author: andrei
 * 
 * Created on August 14, 2010, 12:04 PM
 */

#include "QtConst.hpp"
#include "HqlMain.hpp"
#include "QueryTree.hpp"

QtConst::QtConst(char* str, char* type)
    : value(str), type(type)
{
}

QtConst::QtConst(char* str)
    : value(str), type("")
{
}

QtConst::~QtConst()
{
}

HqlTable* QtConst::execute()
{
    PostgresDS &pg = HqlMain::getInstance().getSqlDataSource();
    RasdamanDS &rman = HqlMain::getInstance().getRasqlDataSource();
    HqlTable *result = NULL, *prod = NULL;

    prod = QueryTree::getInstance().getRoot()->getCartesianProduct();

    string q = "SELECT " + toString() + ", " + HQL_COL + " INTO "
            + this->id + " FROM " + prod->getName();
    
    result = pg.query(q);
    pg.addTempTable(this->id);
    result->setName(this->id);

    return result;
}

string QtConst::toString()
{
    string str(value);
    if (type != "")
        str += string(" AS ") + type;
    return str;
}

DbEnum QtConst::setupDbSource()
{
    return POSTGRES;
}

void QtConst::print(ostream &o, std::string indent)
{
    o << indent << "QtConst (" << id << "): '" << value << "' AS '" << type <<"'" << endl;
}
