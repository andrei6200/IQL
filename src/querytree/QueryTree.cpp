#include "QueryTree.hpp"
#include "IqlApp.hpp"

using namespace std;


QueryTree::QueryTree(): root(NULL), encode("")
{
    
}

QueryTree::~QueryTree()
{
    if (root)
        delete root;
    root = NULL;
}

void QueryTree::load(QtSelectStatement* newroot)
{
    if (root)
        delete root;
    root = newroot;
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
        IqlTable* table = root->execute();
        /* Read the result table from postgres. */
        string name = table->getName();
        string queryString = "SELECT * FROM " + name;
        delete table;
        table = IqlApp::getInstance().getSqlDataSource().query(queryString);
        table->setName(name);
        /* Save required files to disk. */
        saveRasdamanObjectsToDisk(table);
        /* Remove traces of execution */
//        IqlApp::getInstance().getSqlDataSource().removeTempTables();
        IqlApp::getInstance().getSqlDataSource().commit();
        IqlApp::getInstance().getRasqlDataSource().removeTempTables();
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
//    IqlApp::getInstance().getSqlDataSource().removeTempTables();
//    IqlApp::getInstance().getRasqlDataSource().removeTempTables();

    /* And display the query execution status*/
    cout << RESPONSE_PROMPT << status << endl;
}


QtSelectStatement* QueryTree::getRoot()
{
    return root;
}

void QueryTree::saveRasdamanObjectsToDisk(IqlTable *table)
{
    /* Post-processing: parse the result table for references to Rasdaman objects,
     * and create the corresponding files on disk */
    RasdamanDS &rman = IqlApp::getInstance().getRasqlDataSource();
    vector<string> names = table->getQualifiedColumnNames();
    map<string, string> oid2fname = map<string, string>();
    int fileIndex = 0;
    for (int col = 0; col < names.size() - 1; col++)
    {
        string::size_type pos = names[col].rfind("_oid");
        string::size_type pos2 = names[col+1].rfind("_filename");
        if (pos != string::npos && pos == names[col].size() - 4  &&  // end with "_oid"
            pos2 != string::npos && pos2 == names[col+1].size() - 9) // next col end with "_filename"
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
                    /* Perform optional format encoding. */
                    string expr = "var";
                    if (encode != "")
                        expr = encode + "(var)";
                    /* Build the RaSQL query. */
                    string q = "SELECT " + expr + " FROM " + collName + " AS var " +
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

void QueryTree::setEncodingFormat(string format)
{
    if (encode != "")
        throw string("Queries with multiple format encoders are not supported at the moment.");
    encode = format;
}