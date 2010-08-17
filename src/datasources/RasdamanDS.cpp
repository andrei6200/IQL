/* 
 * File:   RasdamanDS.cpp
 * Author: andrei
 * 
 * Created on August 4, 2010, 2:15 PM
 */


#include <pqxx/pqxx>

#include "RasdamanDS.hpp"
#include "config.hpp"
#include "logger.hpp"


// Filename pattern, for Rasdaman coverages stored on disk
#define DEFAULT_OUTFILE_MASK "hql_%d"


using namespace std;
using namespace pqxx;


RasdamanDS::RasdamanDS() : db(NULL), tr(NULL)
{
    tempTables.clear();
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
        if (db == NULL)
        {
            TRACE << "Connecting to Rasdaman host " << RASDAMAN_SERVER
                    << " with port " << RASDAMAN_PORT
                    << " using user/password " << RASDAMAN_USERNAME << "/"
                    << RASDAMAN_PASSWORD
                    << " and opening database " << RASDAMAN_DATABASE;
            db = new r_Database();
            db->set_servername(RASDAMAN_SERVER, RASDAMAN_PORT);
            db->set_useridentification(RASDAMAN_USERNAME, RASDAMAN_PASSWORD);
            db->open(RASDAMAN_DATABASE);
        }
        /* And Init RMAN database transaction */
        openTa();

        TRACE << "Successfully connected to Rasdaman";
        TRACE << "Server: " << RASDAMAN_SERVER;
        TRACE << "Port: " << RASDAMAN_PORT;
        TRACE << "Database: " << RASDAMAN_DATABASE;
        TRACE << "Username: " << RASDAMAN_USERNAME;
        TRACE << "Password: " << RASDAMAN_PASSWORD;
    }
    catch (r_Error e)
    {
        FATAL << "Could not connect to Rasdaman: \n" << e.what();
        cout << "Could not connect to Rasdaman: \n" << e.what() << endl;

        disconnect();

        throw e;
    }
}

void RasdamanDS::openTa()
{
    if (tr != NULL)
    {
        tr->abort();
        tr = NULL;
    }
    
    tr = new r_Transaction();
    tr->begin();
}

void RasdamanDS::disconnect()
{
    abortTa();
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
    removeTempTables();
    disconnect();
    TRACE << "Rasdaman Data Source was now destroyed.";
}

bool RasdamanDS::isConnected()
{
    bool result = false;
    if (db == NULL || tr == NULL)
        result = false;
    else
        result = (db->get_status() != r_Database::not_open) &&
                 (tr->get_status() == r_Transaction::active);
    return result;
}

vector<string> RasdamanDS::getObjectNames()
{
    string queryStr = "select mddcollname from RAS_MDDCOLLNAMES;";
    HqlTable *table = new HqlTable();
    connection *sqlconn = NULL;
    work *sqltr = NULL;

    string opts("dbname=");
    opts += RASDAMAN_DATABASE;

    try
    {
        sqlconn = new connection(opts);
        sqltr = new work(*sqlconn, string("Hql-Rasdaman-Conn"));
        result R(sqltr->exec(queryStr));
        table->importFromSql(R);
        TRACE << table << endl;
        sqltr->abort();
        sqlconn->disconnect();
    }
    catch (...)
    {
        FATAL << "Could not get object names from Rasdaman underlying database.";
        delete sqltr;
        delete sqlconn;
        delete table;
        return vector<string>();
    }

    delete sqltr;
    delete sqlconn;

    return table->getColumn("mddcollname");
}

void RasdamanDS::abortTa()
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

void RasdamanDS::commitTa()
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
    
    // By default, do not write files to disk
    HqlTable* result = this->query(queryString, false);
    return result;
}

HqlTable* RasdamanDS::query(string queryString, bool storeOnDisk)
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
        abortTa();
        disconnect();
        throw e;
    }

    /* Return the data. */
    HqlTable *table = new HqlTable();
    table->importFromRasql(&result_set, storeOnDisk);

    /* Close the transaction (after processing of data) */
    commitTa();

    return table;
}

r_Ref<r_GMarray> RasdamanDS::queryByOid(std::string queryString)
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
        if (result_set.cardinality() != 1)
            throw string("Query by OID did not fetch exactly one MDD object. ");
        TRACE << "Finished executing RaSQL.";
    }
    catch (r_Error &e)
    {
        abortTa();
        disconnect();
        throw e;
    }

    iter = result_set.create_iterator();
    // iter.not_done() seems to behave wrongly on empty set, therefore this additional check -- PB 2003-aug-16
    image = r_Ref<r_GMarray> (*iter);
    return image;
}

HqlTable* RasdamanDS::getCollection(string name, bool storeOnDisk)
{
    string query = "SELECT " + name + " FROM " + name;
    HqlTable* out = this->query(query, storeOnDisk);
    out->names[1] = name + "_" + out->names[1];
    out->names[2] = name + "_" + out->names[2];
    out->setName(name);
    return out;
}

void RasdamanDS::insertData(HqlTable* table, string tableName)
{
    ERROR << "Inserting temporary tables into Rasdaman is not supported !";
    throw string("Inserting temporary tables into Rasdaman is not supported !");
}

void RasdamanDS::removeTempTables()
{
    TRACE << "Removing " << tempTables.size() << " temporary tables";
    /* Drop created tables. */
    string q;
    map<string,bool>::iterator it;
    for (it = tempTables.begin(); it != tempTables.end(); it++)
    {
        q = "DROP COLLECTION " + it->first;
        try
        {
            connect();
            updateQuery(q);
            commitTa();
        }
        catch (...)
        {
            WARN << "Got error while deleting Rasdaman collection: " << it->first;
        }
    }

    /* Clean up */
    tempTables.clear();
}


string RasdamanDS::saveRasdamanMddToFile(r_Ref<r_GMarray> mdd, bool storeOnDisk, int index)
{
    string result("");

    char defFileName[FILENAME_MAX];
    (void) snprintf(defFileName, sizeof (defFileName) - 1, DEFAULT_OUTFILE_MASK, index);

    // special treatment only for DEFs
    r_Data_Format mafmt = mdd->get_current_format();
    switch (mafmt)
    {
        case r_TIFF:
            strcat(defFileName, ".tif");
            break;
        case r_JPEG:
            strcat(defFileName, ".jpg");
            break;
        case r_HDF:
            strcat(defFileName, ".hdf");
            break;
        case r_PNG:
            strcat(defFileName, ".png");
            break;
        case r_BMP:
            strcat(defFileName, ".bmp");
            break;
        case r_VFF:
            strcat(defFileName, ".vff");
            break;
        default:
            strcat(defFileName, ".unknown");
            break;
    }

    if (storeOnDisk)
    {
        DEBUG << "  MDD Result object " << index << ": going into file " << defFileName << "..." << flush;
        FILE *tfile = fopen(defFileName, "wb");
        char* output = mdd->get_array();
        size_t size = mdd->get_array_size();
        if (size == fwrite((void*) output, sizeof(char), size, tfile))
            TRACE << "ok." << endl;
        else
            TRACE << "could not write data into file !" << flush;
        fclose(tfile);

        result = defFileName;
    }

    return result;
}

void RasdamanDS::addTempTable(std::string name)
{
    tempTables[name] = true;
    TRACE << "Temporary tables: " << tempTables.size();
    map<string,bool>::iterator it;
    for (it = tempTables.begin(); it != tempTables.end(); it ++)
        TRACE << " Collection " << it->first;
    TRACE;
}

void RasdamanDS::updateQuery(std::string queryString)
{
    connect();

    DEBUG << "Executing RaSQL update query: " << queryString;
    r_OQL_Query query(queryString.c_str());

    /* Execute the actual query. */
    try
    {
        TRACE << "Executing RaSQL update ...";
        r_oql_execute(query);
        TRACE << "RaSQL update ended. ";
    }
    catch (r_Error &e)
    {
        abortTa();
        disconnect();
        throw e;
    }

    /* Close the transaction (after processing of data) */
    commitTa();
}
