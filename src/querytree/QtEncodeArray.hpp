/* 
 * File:   QtEncodeArray.hpp
 * Author: andrei
 *
 * Created on August 4, 2010, 1:47 PM
 */

#ifndef QTENCODEARRAY_HPP
#define	QTENCODEARRAY_HPP

//#include <string>
#include "QtNode.hpp"

class QtEncodeArray : public QtNode
{
public:
    // public constructor
    QtEncodeArray(QtNode *node, std::string fmt);

    QtEncodeArray(QtNode *node, std::string fmt, std::string opt);

    // public destructor
    virtual ~QtEncodeArray();

    /* String representation */
    std::string toString() = 0;

    /* Recursively computes the value of the "db_source" field.
     * - If all the child nodes belong to the same DB, then this node also belongs to it
     * - If some child nodes belong to different DB, then this node will be "mixed"
     */
    DbEnum setupDbSource() = 0;

    /* Get the source system that can handle this node. */
    DbEnum getDbSource();

    /* Execute the operation of this node and return a HqlTable result. */
    HqlTable* execute();

private:
    std::string format;
    std::string options;
    QtNode *child;
};

#endif	/* QTENCODEARRAY_HPP */
