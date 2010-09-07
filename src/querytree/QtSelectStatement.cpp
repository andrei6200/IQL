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
    : product(NULL), what(NULL), from(NULL), where(NULL)
{
}

QtSelectStatement::QtSelectStatement(QtList* refs, QtList* sources, QtWhere *condition)
    : what(refs), from(sources), product(NULL), where(condition)
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
    PostgresDS &pg = HqlMain::getInstance().getSqlDataSource();
    HqlTable* result = NULL;
    INFO << "ID: " << getId();

    /* First compute the cartesian product between the available tables. */
    getCartesianProduct();

    /* Evaluate the WHERE clause, if it exists. */
    if (where)
    {
        /* The product table is already in main-memory. */
        HqlTable *tmp = where->execute();
        /* And filter the cartesian product according to the condition. */
        string newName = this->id + "filtered";
        string q = "SELECT prod.* INTO " + newName +
                " FROM " + product->getName() + " AS prod " +
                " JOIN " + tmp->getName() + " USING (" + HQL_COL + ")";
        HqlTable *tmp2 = pg.query(q);
        pg.addTempTable(newName);
        delete tmp;
        delete tmp2;
        delete product;
        
        q = "SELECT * FROM " + newName;
        product = pg.query(q);
        product->setName(newName);
    }

    result = product;

    if (product->rowCount() > 0)
    {
        /* Pre-processing */
        setupDbSource();

        /* Query evaluation */
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
    }

    if (result)
        INFO << result;

    return result;
}


std::string QtSelectStatement::toString()
{
    string s = what->toString();
    
    string out = string("SELECT ") + s;
    if (from != NULL)
    {
        string f = from->toString();
        out += string(" FROM ") + f;
    }
    return out;
}

void QtSelectStatement::print(ostream &o, std::string indent)
{
    o << endl;
    o << indent << "QtSelectStatement (" << id << ") : " << toString() << endl;
    o << indent << QTINDENT << "what: " << endl;
    what->print(o, indent + QTINDENT + QTINDENT);
    if (from != NULL)
    {
        o << indent << QTINDENT << "from: " << endl;
        from->print(o, indent + QTINDENT + QTINDENT);
    }
    if (where != NULL)
    {
        o << indent << QTINDENT << "where: " << endl;
        where->print(o, indent + QTINDENT + QTINDENT);
    }
}

QtList* QtSelectStatement::getSourceTables()
{
    return from;
}

void QtSelectStatement::switchContextForJoin(HqlTable* newContext)
{
    product = newContext;
}