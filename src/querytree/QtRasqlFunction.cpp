/* 
 * File:   QtRasqlFunction.cpp
 * Author: andrei
 * 
 * Created on August 4, 2010, 1:47 PM
 */

#include <algorithm>
#include "datasources/PostgresDS.hpp"
#include "QtRasqlFunction.hpp"
#include "datasources/RasdamanDS.hpp"
#include "IqlApp.hpp"

using namespace std;


QtRasqlFunction::QtRasqlFunction(string name, QtNode *node, bool reduceOperation)
    : fname(name), nodeCount(1), child1(node), child2(NULL), scalarOutput(reduceOperation)
{
    transform(fname.begin(), fname.end(), fname.begin(), ::tolower);
}

QtRasqlFunction::QtRasqlFunction(string name, QtNode* node1, QtNode* node2)
    : fname(name), nodeCount(2), child1(node1), child2(node2), scalarOutput(false)
{
    transform(fname.begin(), fname.end(), fname.begin(), ::tolower);
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
    string out(fname);
    out += "(";
    if (child1 == NULL)
        out += "error";
    else
        out += child1->toString();
    if (child2 != NULL)
        out += string(", ") + child2->toString();
    out += ")";
    return out;
}

DbEnum QtRasqlFunction::setupDbSource()
{
    db_source = RASDAMAN;
    if (scalarOutput)
        db_source = POSTGRES;
    return db_source;
}

IqlTable* QtRasqlFunction::execute()
{
    RasdamanDS &rman = IqlApp::getInstance().getRasqlDataSource();
    PostgresDS &pg = IqlApp::getInstance().getSqlDataSource();
    
    IqlTable* result = NULL, *tmp = NULL;
    string query, mddName;
    vector<string> names;
    string collName;
    vector<vector<string> > rows;
    vector<vector<string> >::iterator it;
    string suffix = IQL_TBL_COL_SEP + "oid";

    tmp = child1->execute();
    mddName = tmp->getName();
    delete tmp;

    switch (nodeCount)
    {
        case 1:
            // The function call looks like this: Function(mdd)
            // We issue a single Rasql query
            
            if (scalarOutput)   
            {
                // output is scalar, do not store it in Rasdaman
                query = "SELECT " + fname + " ( mdd ) FROM " + mddName +
                        " AS mdd";
                result = rman.query(query);
                result->setName(this->id);
                string datatype = this->computeDatatype();
                result->setDataType(datatype);

                /* Delete the first column("oid") and name the second one ("filename") properly. */
                result->setColumnName(1, fname);
                result->deleteColumn(0);

                pg.insertData(result, this->id);
                pg.insertHqlIdToTable(this->id);
				pg.addTempTable(this->id);
            }
            else    
            {
                // output is an MDD expression, and can be stored in Rasdaman
                query = "SELECT " + fname + " ( mdd ) INTO " + this->id +
                    " FROM " + mddName + " AS mdd";
                rman.updateQuery(query);

                // Store the new OIDs into Postgresql
                result = rman.getCollection(this->id, false, false);
                result->setName(this->id);

                pg.insertData(result, this->id);
                pg.insertHqlIdToTable(this->id);
				pg.addTempTable(this->id);
            }
            
            break;
        case 2:
            // The function call looks like this: Function(mdd, options)
            // We will issue many Rasql queries because the options may change from row to row
            tmp = child2->execute();
            query = "SELECT * FROM " + mddName + " JOIN " + tmp->getName() +
                    " USING (" + HQL_COL + ")";
            delete tmp;
            tmp = pg.query(query);

            // Retrieve the Rasdaman collection name
            names = tmp->getQualifiedColumnNames();
            collName = names[1];
            
            if (collName.rfind(suffix) == string::npos)
                throw string("Could not determine Rasdaman collection name.");
            collName = collName.substr(0, collName.rfind(suffix));
            TRACE << "Found collection name: " << collName;

            /* Build intermediate queries and execute them. */
            rows = tmp->getData();
            for (it = rows.begin(); it != rows.end(); it++)
            {
                string funcCall = fname + "( mdd";

                vector<string> row = *it;
                vector<string>::iterator col = row.begin();
                // Skip the first column, as that contains the HQL_COL internal ID
                col++;
                // The next two columns contain the Rasdaman OID and filename
                // Retrieve the collection oid
                string oid = *col;
                col+= 2;
                // Now the next column contains the options
                funcCall += ", " + *col;
                funcCall += " ) ";

                query = "SELECT " + funcCall + " INTO " + this->id +
                        " FROM " + collName + " AS mdd " +
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
        default:
            throw string("Invalid number of arguments for the Rasdaman function.");
    }
    

    return result;
}

void QtRasqlFunction::print(ostream& o, std::string indent)
{
    o << indent << "QtRasqlFunction (" << id << "): " << fname;
    if (scalarOutput)
        o << ", Scalar Output";
    else
        o << ", MDD Output";
    o << endl;
    if (child1)
        child1->print(o, indent + QTINDENT);
    if (child2)
        child2->print(o, indent + QTINDENT);
}

string QtRasqlFunction::computeDatatype()
{
    string type = "VARCHAR";
    if (scalarOutput)
    {
        type = "INTEGER";
        if (fname == "avg_cells")
            type = "FLOAT";
    }
    return type;
}
