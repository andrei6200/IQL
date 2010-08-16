/*
 * File:   QtSelect.cpp
 * Author: andrei
 *
 * Created on June 26, 2010, 1:36 PM
 */

#include "QtSelectStatement.hpp"
#include "QtString.hpp"
#include <vector>
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
        product->setName(from->getId());
        TRACE << "Cartesian product: " << product;
        /* Insert the result into Postgres, to be available for QtColumn nodes. */
        HqlMain::getInstance().getSqlDataSource().insertData(product, from->getId());
        
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
    switch (db_source)
    {
    case POSTGRES:
        cout << RESPONSE_PROMPT << "Executing postgres query ..." << endl;
        result = what->addResults();
        break;
    case RASDAMAN:
        cout << RESPONSE_PROMPT << "Executing rasdaman query ..." << endl;
        TRACE << " WHAT:  " << what->toString();
        TRACE << " FROM:  " << from->toString();

        // Allow simple (but very useful) queries like: SELECT * FROM mr
        if (what->length() == 1 && from->length() == 1 && what->toString() == "*")
        {
            QtDataSource *src = (QtDataSource*) from->get(0);
            delete what;
            QtString *str = new QtString(src->toString());
            QtDataSourceRef *ref = new QtDataSourceRef(str);
            vector<QtNode*> vec;
            vec.push_back(ref);
            what = new QtList(vec);
            setupDbSource();
        }

//        TRACE << "Executing rasdaman query: " << toString();
//        result = HqlMain::getInstance().runRasqlQuery(toString());
        result = what->addResults();
        break;
    case MIXED:
        cout << RESPONSE_PROMPT << "Executing mixed query ..." << endl;
        result = what->addResults();
        break;
    default:
        cout << RESPONSE_PROMPT << "Cannot execute query, unknown data source. " << endl;
        result = NULL;
        break;
    }

    /* Post-processing: parse the result table for references to Rasdaman objects,
     * and create the corresponding files on disk */
    /*
     * FIXME
     */


    if (result)
        INFO << result;

    return result;
}


std::string QtSelectStatement::toString()
{
    string out = string("SELECT ") + what->toString() +
                 string(" FROM ") + from->toString();
    return out;
}

/* Returns true if the current Hql query contains a simple mixture of RaSQl and SQL.
 In case this is not a mixed query, this function returns false. */
//bool QtSelectStatement::mixedQueryIsSimple()
//{
//    if (db_source != MIXED)
//        return false;
//    for (int i = 0; i < from->length(); i++)
//    {
//        DbEnum db = from->get(i)->setupDbSource();
//        if (db == MIXED || db == UNKNOWN_DB || db == DB_NOT_INITIALIZED)
//            return false;
//    }
//    return true;
//}

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