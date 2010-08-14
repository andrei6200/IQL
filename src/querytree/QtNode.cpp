/* 
 * File:   QtNode.cpp
 * Author: andrei
 * 
 * Created on June 26, 2010, 12:31 PM
 */

#include <string>
#include "QtNode.hpp"
#include "HqlMain.hpp"

using namespace std;

long QtNode::idCounter = 0;

QtNode::QtNode() : db_source(DB_NOT_INITIALIZED), query(NULL)
{
    /* Generate ID for this node. */
    id = QtNode::generateId();
}

QtNode::~QtNode()
{
}

DbEnum QtNode::getDbSource()
{
    return db_source;
}

HqlTable* QtNode::execute()
{
    return NULL;
}

char* QtNode::toCString()
{
    return strdup((char*) this->toString().c_str());
}

string QtNode::getId()
{
    return id;
}

string QtNode::generateId()
{
    stringstream s;
    s << "id_" << HqlMain::getInstance().getId() << "_" << ++ QtNode::idCounter;
    return s.str();
}
