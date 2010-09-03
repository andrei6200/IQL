/* 
 * File:   QtSqlFunction.cpp
 * Author: andrei
 * 
 * Created on August 4, 2010, 1:45 PM
 */

#include "QtSqlFunction.hpp"
#include "datasources/PostgresDS.hpp"
#include "HqlMain.hpp"
#include "QueryTree.hpp"

QtSqlFunction::QtSqlFunction(char *fname, QtList *args)
    : name(strdup(fname)), arguments(args)
{
    TRACE << "Name: " << name;
}

QtSqlFunction::~QtSqlFunction()
{
    if (arguments != NULL)
    {
        delete arguments;
        arguments = NULL;
    }
}

void QtSqlFunction::print(ostream& o, std::string indent)
{
    o << indent << "QtSqlFunction (" << id << "): " << name << endl;
    if (arguments)
    {
        o << indent << QTINDENT << "arguments: " << endl;
        arguments->print(o, indent + QTINDENT + QTINDENT);
    }
}

DbEnum QtSqlFunction::setupDbSource()
{
    db_source = POSTGRES;

    if (arguments != NULL)
    {
        db_source = arguments->setupDbSource();
        switch (db_source)
        {
            case POSTGRES:
            case UNKNOWN_DB:
            case MIXED:
                break;
            case RASDAMAN:
                db_source = MIXED;
                break;
        }
    }
    
    return db_source;
}

HqlTable* QtSqlFunction::execute()
{
    PostgresDS &pg = HqlMain::getInstance().getSqlDataSource();
    HqlTable *result = NULL, *args = NULL;
    string q, sep = "";

    if (arguments != NULL)
    {
        // Function call with arguments
        args = arguments->addResults();
        string argsTableName = args->getName();
        // Get column names
        q = "SELECT * FROM " + argsTableName;
        delete args;
        args = pg.query(q);
        vector<string> cols = args->getColumnNames();
        q = "SELECT " + name + "( ";
        // Cols[0] should be the HQL id (HQL_COL), and it should not be a function argument
        for (int i = 0; i < cols.size(); i++)
            if (cols[i] != HQL_COL)
            {
                q += sep + cols[i];
                sep = ", ";
            }
        q += string(" ), ") + HQL_COL + " INTO " + this->id + " FROM " + argsTableName;
    }
    else
    {
        string prodName = QueryTree::getInstance().getRoot()->getCartesianProduct()->getName();
        // Just Function Call, no arguments
        q = "SELECT " + name + " (), " + HQL_COL + " INTO " + this->id + " FROM " + prodName;
    }

    result = pg.query(q);
    pg.addTempTable(this->id);
    result->setName(this->id);

    return result;
}

string QtSqlFunction::toString()
{
    TRACE << "Function: " << name;
    string str(name);
    if (arguments != NULL && arguments->length() > 0)
        str += string("(") + arguments->toString() + ")";
    else
        str += "()";
    return str;
}
