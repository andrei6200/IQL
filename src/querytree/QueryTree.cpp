#include "QueryTree.hpp"
#include "HqlMain.hpp"

using namespace std;


QueryTree::QueryTree(): root(NULL)
{
    
}

QueryTree::~QueryTree()
{
    if (root)
        delete root;
    root = NULL;
}

void QueryTree::attachDataSourceNodes()
{
    /* TODO */
}

void QueryTree::load(QtSelectStatement* newroot)
{
    if (root)
        delete root;
    root = newroot;
    attachDataSourceNodes();
}

QueryTree QueryTree::instance;

QueryTree& QueryTree::getInstance()
{
    return instance;
}

void QueryTree::execute()
{
    DEBUG << "Query tree: ";
    DEBUG << endl << root << endl;
    string status;

    try
    {
        /* Analyze data sources */
        root->setupDbSource();
        /* Execute the hybrid query, based on information about data sources. */
        HqlTable* table = root->execute();
        /* Read the result table from postgres. */
        string name = table->getName();
        string queryString = "SELECT * FROM " + name;
        delete table;
        table = HqlMain::getInstance().getSqlDataSource().query(queryString);
        table->setName(name);
        /* Save required files to disk. */
        saveRasdamanObjectsToDisk(table);
        /* Remove traces of execution */
//        HqlMain::getInstance().getSqlDataSource().removeTempTables();
        HqlMain::getInstance().getSqlDataSource().commit();
        HqlMain::getInstance().getRasqlDataSource().removeTempTables();
        /* Print output */
        status = "ok";
        if (table)
            table->print(cout);
        else
            throw string("NULL result table.");
    }
    /* Error handling */
    catch (string str)
    {
        ERROR << str;
        status = string("failed ... ") + str;
    }
    catch (pqxx::failure &e)
    {
        ERROR << "Query execution exception: " << e.what();
        status = string("failed ... ") + e.what();
    }
    catch (pqxx::usage_error &e)
    {
        ERROR << "Libpqxx usage exception: " << e.what();
        status = string("failed ... ") + e.what();
    }
    catch (exception &e)
    {
        ERROR << "Query execution exception: " << e.what();
        status = string("failed ... ") + e.what();
    }

//    /* In the end, remove temporary tables. */
//    HqlMain::getInstance().getSqlDataSource().removeTempTables();
//    HqlMain::getInstance().getRasqlDataSource().removeTempTables();

    /* And display the query execution status*/
    cout << RESPONSE_PROMPT << status << endl;
}


QtSelectStatement* QueryTree::getRoot()
{
    return root;
}

void QueryTree::saveRasdamanObjectsToDisk(HqlTable *table)
{
    /* Post-processing: parse the result table for references to Rasdaman objects,
     * and create the corresponding files on disk */
    RasdamanDS &rman = HqlMain::getInstance().getRasqlDataSource();
    vector<string> names = table->getColumnNames();
    map<string, string> oid2fname = map<string, string>();
    int fileIndex = 0;
    for (int col = 0; col < names.size(); col++)
    {
        string::size_type pos = names[col].rfind("_oid");
        if (pos != string::npos && pos == names[col].size() - 4)  // end with "_oid"
        {
            string collName = names[col].substr(0, pos);
            TRACE << "Fetching MDDs from collection: " << collName;
            /* This column contains Rasdaman OIDs. Need to save the referenced
             * objects to disk. */
            vector<string> oids = table->getColumn(col);
            vector<string> fnameCol(oids);
            for (int row = 0; row < oids.size(); row ++)
            {
                string oid = oids[row];
                string filename = "";
                if (oid2fname.count(oid) == 0)
                {
                    string q = "SELECT var FROM " + collName + " AS var " +
                            "WHERE oid(var) = <\"" + oid + "\">";
                    r_Ref<r_GMarray> mdd = rman.queryByOid(q);
                    filename = rman.saveRasdamanMddToFile(mdd, true, ++fileIndex);

                    // cache this oid->file link for later use
                    oid2fname[oid] = filename;
                }
                else
                    filename = oid2fname[oid];

                fnameCol[row] = filename;
            }

            TRACE << "Built map from Rasdaman OIDs to filenames:";
            map<string,string>::iterator it;
            for (it = oid2fname.begin(); it != oid2fname.end(); it ++)
                TRACE << " filename[" << it->first << "] = " << it->second;
            TRACE << endl;

            table->setFilenames(fnameCol, col+1);
            col++;
        }
    }
}