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
        HqlMain::getInstance().getSqlDataSource().commit();
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

    /* In the end, remove temporary tables. */
    HqlMain::getInstance().getSqlDataSource().removeTempTables();

    /* And display the query execution status*/
    cout << RESPONSE_PROMPT << status << endl;
}


QtSelectStatement* QueryTree::getRoot()
{
    return root;
}