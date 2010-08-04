/* 
 * File:   RasdamanDS.cpp
 * Author: andrei
 * 
 * Created on August 4, 2010, 2:15 PM
 */




#include "RasdamanDS.hpp"
#include "PostgresDS.hpp"
#include "config.hpp"
#include "utils/logger.hpp"


using namespace std;
using namespace pqxx;


RasdamanDS::RasdamanDS() : db(NULL), tr(NULL)
{
}

void RasdamanDS::connect()
{
    if (isConnected())
    {
        WARN << "RasdamanDS: Already connected. ";
        return;
    }
    
    try
    {
        /* Init RMAN database connections */
        TRACE << "Connecting to Rasdaman host " << RASDAMAN_SERVER
                << " with port " << RASDAMAN_PORT
                << " using user/password " << RASDAMAN_USERNAME << "/"
                << RASDAMAN_PASSWORD
                << " and opening database " << RASDAMAN_DATABASE;
        db = new r_Database();
        db->set_servername(RASDAMAN_SERVER, RASDAMAN_PORT);
        db->set_useridentification(RASDAMAN_USERNAME, RASDAMAN_PASSWORD);
        db->open(RASDAMAN_DATABASE);

        tr = new r_Transaction();
        tr->begin();
    }
    catch (r_Error e)
    {
        FATAL << "Could not connect to Rasdaman: \n" << e.what();
        cout << "Could not connect to Rasdaman: \n" << e.what() << endl;
        throw e;
    }
}

void RasdamanDS::disconnect()
{
    if (tr)
    {
        tr->abort();
        delete tr;
        tr = NULL;
        TRACE << "RasdamanDS: Aborted the database transaction ...";
    }
    if (db)
    {
        db->close();
        delete db;
        db = NULL;
        TRACE << "RasdamanDS: Closed the database connection ...";
    }
}

RasdamanDS::~RasdamanDS()
{
    disconnect();
    TRACE << "Rasdaman Data Source was now destroyed.";
}

bool RasdamanDS::isConnected()
{
    bool result = false;
    if (db)
        result = db->get_status() != r_Database::not_open;
    result = result && (tr != NULL);
    return result;
}

vector<string> RasdamanDS::getObjectNames()
{
    string queryStr = "select mddcollname from RAS_MDDCOLLNAMES;";

    string opts("dbname=");
    opts += RASDAMAN_DATABASE;
    PostgresDS pg(opts);
    
    HqlTable *table = pg.query(queryStr);
    return table->getColumn(0);
}

void RasdamanDS::abort()
{
    if (tr)
    {
        TRACE << "Aborting current Rasdaman transaction ...";
        tr->abort();
        delete tr;
        tr = NULL;
        TRACE << "Rasdaman transaction aborted.";
    }
}

void RasdamanDS::commit()
{
    if (tr)
    {
        TRACE << "Committing current Rasdaman transaction ...";
        tr->commit();
        delete tr;
        tr = NULL;
        TRACE << "Rasdaman transaction closed.";
    }
}

HqlTable* RasdamanDS::query(string queryString)
{
    connect();

    DEBUG << "Executing RaSQL query: " << queryString;
    r_OQL_Query query(queryString.c_str());

    r_Set< r_Ref_Any > result_set;
    r_Ref< r_GMarray > image;
    r_Iterator< r_Ref_Any > iter;

    /* Execute the actual query. */
    try
    {
        TRACE << "Executing RaSQL ...";
        r_oql_execute(query, result_set);
        TRACE << "RaSQL execution ended. ";
        DEBUG << "Result has " << result_set.cardinality() << " objects... ";
        TRACE << "Finished executing RaSQL.";
    }
    catch (r_Error &e)
    {
        abort();
        throw e;
    }

    HqlTable *table = new HqlTable();
    table->importFromRasql(&result_set);

    /* Close the transaction (after processing of data) */
//    commit();

    /* Return the data. */
    return table;
}


//FIXME: implement
void RasdamanDS::insertData()
{

}
