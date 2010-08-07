/* 
 * File:   QtNode.cpp
 * Author: andrei
 * 
 * Created on June 26, 2010, 12:31 PM
 */

#include <string>
#include "QtNode.hpp"
#include "utils/logger.hpp"

using namespace std;

QtNode::QtNode() : db_source(DB_NOT_INITIALIZED), query(NULL)
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
