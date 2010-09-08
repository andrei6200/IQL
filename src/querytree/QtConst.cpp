/* 
 * File:   QtConst.cpp
 * Author: andrei
 * 
 * Created on August 14, 2010, 12:04 PM
 */

#include "QtConst.hpp"
#include "IqlApp.hpp"
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

IqlTable* QtConst::execute()
{
    PostgresDS &pg = IqlApp::getInstance().getSqlDataSource();
    IqlTable *result = NULL, *prod = NULL;

    prod = QueryTree::getInstance().getRoot()->getCartesianProduct();

    string val = toString();
    if (type != "")
        val += " AS " + type + this->id;
    string q = "SELECT " + val + ", " + HQL_COL + " INTO "
            + this->id + " FROM " + prod->getName();
    
    result = pg.query(q);
    pg.addTempTable(this->id);
    result->setName(this->id);

    return result;
}

string QtConst::toString()
{
    string str(value);
    if (type == "string")
        str = string("'") + value + "'::text";
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
