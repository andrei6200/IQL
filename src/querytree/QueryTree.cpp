#include "QueryTree.hpp"

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

void QueryTree::load(QtNode* newroot)
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
    INFO << "Received SELECT structure: ";
    DEBUG << root->toString();
    string status;

    try
    {
        /* Analyze data sources */
        root->setupDbSource();
        /* Execute the hybrid query, based on information about data sources. */
        HqlTable* table = root->execute();
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
    catch (exception e)
    {
        ERROR << "Query execution exception: " << e.what();
        status = string("failed ... ") + e.what();
    }

    /* And display the query execution status*/
    cout << RESPONSE_PROMPT << status << endl;
}
