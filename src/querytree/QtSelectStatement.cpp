/*
 * File:   QtSelect.cpp
 * Author: andrei
 *
 * Created on June 26, 2010, 1:36 PM
 */

#include "QtSelectStatement.hpp"
#include "QtString.hpp"
#include <vector>
#include <map>
#include <typeinfo>
#include "HqlMain.hpp"
#include "logger.hpp"

using namespace std;

QtSelectStatement::QtSelectStatement()
    : product(NULL), what(NULL), from(NULL)
{
}

QtSelectStatement::QtSelectStatement(QtList* refs, QtList* sources)
    : what(refs), from(sources), product(NULL)
{
}

QtSelectStatement::~QtSelectStatement()
{
    if (from)
    {
        delete from;
        from = NULL;
    }
    if (what)
    {
        delete what;
        what = NULL;
    }
}

DbEnum QtSelectStatement::setupDbSource()
{
    string s = toString();
    db_source = from->setupDbSource();
    what->setupDbSource();

    TRACE << "Source tables: " << db_source << endl;
    TRACE << "Selected tables: " << what->getDbSource() << endl;
    return db_source;
}

HqlTable* QtSelectStatement::getCartesianProduct()
{
    if (product == NULL)
    {
        /* Evaluate the cartesian product. */
        TRACE << "Evaluating cartesian product between given tables...";
        product = from->multiplyResults();
        string tname = product->getName();
        delete product;
        string q = "SELECT * FROM " + tname;
        product = HqlMain::getInstance().runSqlQuery(q);
        product->setName(tname);
        TRACE << "Cartesian product: " << product;
    }
    return product;
}

HqlTable* QtSelectStatement::execute()
{
    INFO << "ID: " << getId();

    /* First compute the cartesian product between the available tables. */
    getCartesianProduct();

    /* Pre-processing */
    setupDbSource();

    /* Query evaluation */
    HqlTable* result = NULL;
    result = what->addResults();
    switch (db_source)
    {
    case POSTGRES:
        cout << RESPONSE_PROMPT << "Executing postgres query ..." << endl;
        break;
    case RASDAMAN:
        cout << RESPONSE_PROMPT << "Executing rasdaman query ..." << endl;
        break;
    case MIXED:
        cout << RESPONSE_PROMPT << "Executing mixed query ..." << endl;
        break;
    default:
        cout << RESPONSE_PROMPT << "Cannot execute query, unknown data source. " << endl;
        result = NULL;
        break;
    }

    

    if (result)
        INFO << result;

    return result;
}


std::string QtSelectStatement::toString()
{
    string s = what->toString();
    string f = from->toString();
    string out = string("SELECT ") + s + string(" FROM ") + f;
    return out;
}

void QtSelectStatement::print(ostream &o, std::string indent)
{
    o << endl;
    o << indent << "QtSelectStatement (" << id << ") : " << toString() << endl;
    o << indent << QTINDENT << "what: " << endl;
    what->print(o, indent + QTINDENT + QTINDENT);
    o << indent << QTINDENT << "from: " << endl;
    from->print(o, indent + QTINDENT + QTINDENT);
}

QtList* QtSelectStatement::getSourceTables()
{
    return from;
}
