/* 
 * File:   QtRasqlFunction.hpp
 * Author: andrei
 *
 * Created on August 4, 2010, 1:47 PM
 *
 * Corresponds to a Rasql function with one or two parameters. The first parameter
 * contains an MDD expression, and the second parameter contains options.
 */

#ifndef QTRASQLFUNCTION_HPP
#define	QTRASQLFUNCTION_HPP

#include "QtNode.hpp"

/** Represents a Rasdaman function.
 * Note that sdom() is not supported. 
 */
class QtRasqlFunction : public QtNode
{
public:
    /** Constructors for one parameter functions */
    QtRasqlFunction(std::string name, QtNode* node);

    /** Constructors for two parameter functions */
    QtRasqlFunction(std::string name, QtNode* node1, QtNode* node2);

    /** Destructor */
    virtual ~QtRasqlFunction();

    /** String representation */
    std::string toString();

    /** Recursively computes the value of the "db_source" field.
     * - If all the child nodes belong to the same DB, then this node also belongs to it
     * - If some child nodes belong to different DB, then this node will be "mixed"
     */
    DbEnum setupDbSource();

    /** Get the source system that can handle this node. */
    DbEnum getDbSource();

    /** Execute the operation of this node and return a IqlTable result. */
    IqlTable* execute();

    virtual void print(ostream &o, std::string indent = "");

private:
    /** function name */
    std::string fname;
    /** children nodes */
    QtNode *child1, *child2;
    /** children count */
    int nodeCount;
};

#endif	/* QTRASQLFUNCTION_HPP */

