/* 
 * File:   QtRasqlFunction.hpp
 * Author: andrei
 *
 * Created on August 4, 2010, 1:47 PM
 */

#ifndef QTRASQLFUNCTION_HPP
#define	QTRASQLFUNCTION_HPP

#include "QtNode.hpp"

class QtRasqlFunction : public QtNode
{
public:
    // Constructors for one parameter functions
    QtRasqlFunction(std::string name, std::string s);
    QtRasqlFunction(std::string name, QtNode* node);

    // Constructors for two parameter functions
    QtRasqlFunction(std::string name, std::string s1, std::string s2);
    QtRasqlFunction(std::string name, QtNode* node, std::string str);
    QtRasqlFunction(std::string name, QtNode* node1, QtNode* node2);

    // Destructor
    virtual ~QtRasqlFunction();

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
    // function name
    std::string fname;
    // children nodes
    QtNode *child1, *child2;
    // children count
    int nodeCount;
    // String nodes
    std::string str1, str2;
    // String count
    int stringCount;
};

#endif	/* QTRASQLFUNCTION_HPP */

