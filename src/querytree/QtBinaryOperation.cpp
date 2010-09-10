/* 
 * File:   QtBinaryOperation.cpp
 * Author: andrei
 * 
 * Created on August 14, 2010, 12:04 PM
 */

#include "QtBinaryOperation.hpp"
#include "IqlApp.hpp"
#include "QueryTree.hpp"

QtBinaryOperation::QtBinaryOperation(QtNode *node1, char* op1, QtNode *node2)
    : child1(node1), child2(node2), op(op1), opname("binaryop")
{
    if (node1 == NULL || node2 == NULL)
        throw string("Operand of the BinaryOperation must be non-null.");
    if (op == "+")
        opname = "plus";
    else
    if (op == "-")
        opname = "minus";
    else
    if (op == "*")
        opname = "times";
    else
    if (op == "/")
        opname = "divide";
    else
    if (op == "%")
        opname = "mod";
    else
    if (op == "^")
        opname = "power";
    else
    if (op == "<")
        opname = "less";
    else
    if (op == "<=")
        opname = "lessorequal";
    else
    if (op == ">")
        opname = "greater";
    else
    if (op == ">=")
        opname = "greaterorequal";
    else
    if (op == "=")
        opname = "equal";
    else
    if (op == "!=")
        opname = "notequal";
    else
    if (op == "and" || op == "or" || op == "xor" || op == "not" || op == "like")
        opname = op;
    else
    if (op == "not like")
        opname = "notlike";
    else
        opname = "op";
}

QtBinaryOperation::~QtBinaryOperation()
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

IqlTable* QtBinaryOperation::execute()
{
    PostgresDS &pg = IqlApp::getInstance().getSqlDataSource();
    RasdamanDS &rman = IqlApp::getInstance().getRasqlDataSource();
    IqlTable *result = NULL, *tmp1 = NULL, *tmp2 = NULL, *tmp = NULL;

    string t1, t2, col1, col2, query, col[2];
    string mddName, scalarName, operation;
    int count = 0, oidIndex = -1, scalarIndex = -1;
    vector<string> names;
    vector<vector<string> > rows;
    vector<vector<string> >::iterator it;

    setupDbSource();

    switch (db_source)
    {
        case POSTGRES:
            /* Both operands are relational tables, so we can write a simple
             SQL query that computes the result. */
            tmp1 = child1->execute();
            tmp2 = child2->execute();
            t1 = tmp1->getName();
            t2 = tmp2->getName();
            delete tmp1;
            delete tmp2;
            tmp1 = pg.query("SELECT * FROM " + t1 + " LIMIT 1");
            tmp2 = pg.query("SELECT * FROM " + t2 + " LIMIT 1");
            col1 = tmp1->getQualifiedColumnNames().at(0);
            col2 = tmp2->getQualifiedColumnNames().at(0);
            delete tmp1;
            delete tmp2;
            query = "SELECT t1." + col1 + " " + op + " t2." + col2 +
                    " AS " + opname + ", t1." + HQL_COL + " INTO " + this->id +
                    " FROM " + t1 + " AS t1 JOIN " + t2 + " AS t2 ON t1." + HQL_COL + "=t2." + HQL_COL + ";";

            result = pg.query(query);
            pg.addTempTable(this->id);
            result->setName(this->id);
            break;

        case MIXED:
            /* Since at least one operand is an array, we need to generate RaSQL
             * queries, one query for each row of the source tables */
            tmp1 = child1->execute();
            tmp2 = child2->execute();
            t1 = tmp1->getName();
            t2 = tmp2->getName();
            query = "SELECT * FROM " + t1 + " JOIN " + t2 + " USING (" + HQL_COL + ")";
            delete tmp1;
            delete tmp2;
            tmp = pg.query(query);

            if (child1->getDbSource() == RASDAMAN)
            {
                // Retrieve the Rasdaman collection name
                names = tmp->getQualifiedColumnNames();
                for (int i = 0; i < names.size(); i ++)
                    if (names[i] != HQL_COL && names[i].rfind("_oid") != string::npos)
                    {
                        count ++;
                        col1 = names[i];
                        oidIndex = i;
                    }
                if (count != 1)
                    throw string("Could not determine Rasdaman collection name.");
                mddName = col1.substr(0, col1.rfind("_oid"));
                TRACE << "Found Rasdaman collection name for first operand: " << mddName;
            }
            else
            {
                tmp1 = pg.query("SELECT * FROM " + t1 + " LIMIT 1");
                names = tmp1->getQualifiedColumnNames();
                scalarName = names.at(0);
                col1 = scalarName;
                scalarIndex = 1;
                TRACE << "Found column name for first operand: " << scalarName;
                delete tmp1;
            }

            if (child2->getDbSource() == RASDAMAN)
            {
                // Retrieve the Rasdaman collection name
                names = tmp->getQualifiedColumnNames();
                for (int i = 0; i < names.size(); i ++)
                    if (names[i] != HQL_COL && names[i].rfind("_oid") != string::npos)
                    {
                        count ++;
                        col2 = names[i];
                        oidIndex = i;
                    }
                if (count != 1)
                    throw string("Could not determine Rasdaman collection name.");
                mddName = col2.substr(0, col2.rfind("_oid"));
                TRACE << "Found Rasdaman collection name for second operand: " << mddName;
            }
            else
            {
                tmp2 = pg.query("SELECT * FROM " + t2 + " LIMIT 1");
                names = tmp2->getQualifiedColumnNames();
                scalarName = names.at(0);
                col2 = scalarName;
                scalarIndex = 3;
                TRACE << "Found column name for second operand: " << scalarName;
                delete tmp2;
            }

            /* Build intermediate queries and execute them. */
            rows = tmp->getData();
            
            for (it = rows.begin(); it != rows.end(); it++)
            {
                vector<string> row = *it;
                string oid = row.at(oidIndex);
                string scalar = row.at(scalarIndex);
                if (child1->getDbSource() == RASDAMAN)
                    operation = "mdd " + this->op + " " + scalar;
                else
                    operation = scalar + " " + this->op + " mdd";
                
                query = "SELECT " + operation + " INTO " + this->id +
                        " FROM " + mddName + " AS mdd " +
                        "WHERE oid(mdd) = <\"" + oid + "\">";
                rman.updateQuery(query);
            }
            delete tmp;

            // Retrieve the new OIDs
            result = rman.getCollection(this->id, false);
            result->setName(this->id, true);
            rman.addTempTable(this->id);

            // And store the OID table in PostgreSQL as well
            pg.insertData(result, this->id);
            pg.addTempTable(this->id);
            pg.insertHqlIdToTable(this->id);

            break;

        case RASDAMAN:
            /* Since at both operands are arrays, we need to generate RaSQL
             * queries, one query for each row of the source tables */
            tmp1 = child1->execute();
            tmp2 = child2->execute();
            t1 = tmp1->getName();
            t2 = tmp2->getName();
            query = "SELECT * FROM " + t1 + " JOIN " + t2 + " USING (" + HQL_COL + ")";
            delete tmp1;
            delete tmp2;
            tmp = pg.query(query);

            // Retrieve the Rasdaman collection names for the operands
            names = tmp->getQualifiedColumnNames();
            for (int i = 0; i < names.size(); i ++)
                if (names[i] != HQL_COL && names[i].rfind("_oid") != string::npos)
                {
                    if (count < 2)
                        col [ count ] = names[i];
                    count ++;
                }
            if (count != 2)
                throw string("Could not determine Rasdaman collection names for all operands.");
            col1 = col[0].substr(0, col[0].rfind("_oid"));
            col2 = col[1].substr(0, col[1].rfind("_oid"));
            TRACE << "Found Rasdaman collection name for first operand: " << col1;
            TRACE << "Found Rasdaman collection name for second operand: " << col2;

            /* Build intermediate queries and execute them. */
            rows = tmp->getData();

            for (it = rows.begin(); it != rows.end(); it++)
            {
                vector<string> row = *it;
                string oid1 = row.at(1);
                string oid2 = row.at(3);
                operation = "mdd1 " + op + " mdd2";
                query = "SELECT " + operation + " INTO " + this->id +
                        " FROM " + col1 + " AS mdd1, " + col2 + " AS mdd2 " +
                        "WHERE oid(mdd1) = <\"" + oid1 + "\">" +
                        "  and oid(mdd2) = <\"" + oid2 + "\">";
                rman.updateQuery(query);
            }
            delete tmp;

            // Retrieve the new OIDs
            result = rman.getCollection(this->id, false);
            result->setName(this->id, true);
            rman.addTempTable(this->id);

            // And store the OID table in PostgreSQL as well
            pg.insertData(result, this->id);
            pg.addTempTable(this->id);
            pg.insertHqlIdToTable(this->id);
            break;

        default:
            throw string("Internal Error: Invalid data source system.");
    }

    return result;
}

string QtBinaryOperation::toString()
{
    string str =  child1->toString() + " " + op + " " + child2->toString();
    return str;
}

DbEnum QtBinaryOperation::setupDbSource()
{
    DbEnum db1 = child1->setupDbSource();
    DbEnum db2 = child2->setupDbSource();
    if (db1 == UNKNOWN_DB || db2 == UNKNOWN_DB)
        db_source = UNKNOWN_DB;
    if (db1 == DB_NOT_INITIALIZED || db2 == DB_NOT_INITIALIZED)
        db_source = DB_NOT_INITIALIZED;
    if (db1 == db2)
        db_source = db1;
    else
        db_source = MIXED;
    return db_source;
}

void QtBinaryOperation::print(ostream &o, std::string indent)
{
    o << indent << "QtBinaryOperation (" << id << "): " << op << endl;
    child1->print(o, indent + QTINDENT);
    child2->print(o, indent + QTINDENT);
}
