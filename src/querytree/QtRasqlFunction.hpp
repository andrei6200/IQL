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
    /** Constructors for one parameter functions. Reduce operations should change
     * the boolean parameter, to let the system know that the output will be of
     * scalar type.  */
    QtRasqlFunction(std::string name, QtNode* node, bool reduceOperation = false);

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

    /** Execute the operation of this node and return a IqlTable result. */
    IqlTable* execute();

    virtual void print(ostream &o, std::string indent = "");

private:

    /** Compute the output datatype of the current operation. */
    std::string computeDatatype();

    /** function name */
    std::string fname;
    /** children nodes */
    QtNode *child1, *child2;
    /** children count */
    int nodeCount;
    /** Is the output of this operation of scalar type ? */
    bool scalarOutput;
};

#endif	/* QTRASQLFUNCTION_HPP */

