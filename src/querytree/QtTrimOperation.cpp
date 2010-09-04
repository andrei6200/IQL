/* 
 * File:   QtTrimOperation.cpp
 * Author: andrei
 * 
 * Created on August 31, 2010, 12:16 PM
 */

#include "QtTrimOperation.hpp"
#include "datasources/PostgresDS.hpp"
#include "datasources/RasdamanDS.hpp"
#include "HqlMain.hpp"

QtTrimOperation::QtTrimOperation(QtNode *baseExpr, QtNode *limits)
    : mdd(baseExpr), intervals(limits)
{
}

QtTrimOperation::~QtTrimOperation()
{
    if (mdd)
    {
        delete mdd;
        mdd = NULL;
    }
    if (intervals)
    {
        delete intervals;
        intervals = NULL;
    }
}

HqlTable* QtTrimOperation::execute()
{
    RasdamanDS &rman = HqlMain::getInstance().getRasqlDataSource();
    PostgresDS &pg = HqlMain::getInstance().getSqlDataSource();

    HqlTable* result = NULL, *tmp = NULL;
    string query, mddTable, intervalTable;
    
    tmp = mdd->execute();
    mddTable = tmp->getName();
    delete tmp;
    tmp = intervals->execute();
    intervalTable = tmp->getName();
    delete tmp;

    query = "SELECT * FROM " + mddTable + " JOIN " + intervalTable +
            " USING (" + HQL_COL + ")";
    tmp = pg.query(query);
    TRACE << tmp;
    vector<vector<string> > rows = tmp->getData();

    /* Retrieve the Rasdaman collection name */
    vector<string> names = tmp->getQualifiedColumnNames();
    if (names.size() < 2)
        throw string("A Trim operation needs non-empty interval limits. ");
    string collName = names[1];
    if (collName.rfind("_oid") == string::npos)
        throw string("Could not determine Rasdaman collection name.");
    collName = collName.substr(0, collName.rfind("_oid"));
    TRACE << "Found collection name: " << collName;

    /* Build intermediate queries and execute them. */
    vector<vector<string> >::iterator it;
    
    for (it = rows.begin(); it != rows.end(); it++)
    {
        string trim ="mdd [ ";
        
        vector<string> row = *it;
        vector<string>::iterator col = row.begin();
        // Skip the first column, as that contains the HQL_COL internal ID
        col++;
        // The next two columns contain the Rasdaman OID and filename
        // Retrieve the collection oid
        string oid = *col;
        col+= 2;
        // Now each two columns represent the low and high limits of an interval
        trim += *col + ":";
        col ++;
        trim += *col;

        for (col++; col != row.end(); col++)
        {
            trim += ", " + *col + ":";
            col ++;
            trim += *col;
        }
        trim += " ] ";

        query = "SELECT " + trim + " INTO " + this->id + 
                " FROM " + collName + " AS mdd " +
                "WHERE oid(mdd) = <\"" + oid + "\">";
        rman.updateQuery(query);
    }

    // Retrieve the new OIDs
    tmp = rman.getCollection(this->id, false);
    tmp->setName(this->id, true);
    rman.addTempTable(this->id);

    // And store the OID table in PostgreSQL as well
    pg.insertData(tmp, this->id);
    pg.addTempTable(this->id);
    pg.insertHqlIdToTable(this->id);

    return tmp;
}

DbEnum QtTrimOperation::setupDbSource()
{
    return RASDAMAN;
}

void QtTrimOperation::print(std::ostream& o, std::string indent)
{
    o << indent << "QtTrimOperation (" << id << "): " << endl;
    mdd->print(o, indent + QTINDENT);
    intervals->print(o, indent + QTINDENT);
}

string QtTrimOperation::toString()
{
    string r("trim");
    r += string("( ") + mdd->toString();
    r += string(", ") + intervals->toString() + " )";
    return r;
}