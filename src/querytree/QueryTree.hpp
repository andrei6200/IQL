/* 
 * File:   QueryTree.hpp
 * Author: andrei
 *
 * Created on June 26, 2010, 1:20 PM
 */

#ifndef QUERYTREE_HPP
#define	QUERYTREE_HPP

#include "utils/HqlTable.hpp"

#include "QtString.hpp"
#include "QtNode.hpp"
#include "QtDataSource.hpp"
#include "QtDataSourceRef.hpp"
#include "QtDot.hpp"
#include "QtSelectStatement.hpp"

/* Singleton class for accessing the entire query tree. */
class QueryTree
{
public:
    /* Return (and initialize if needed) the singleton instance of HqlMain. */
    static QueryTree& getInstance();

    /* Load the whole query tree and prepare for execution. */
    void load(QtSelectStatement *newroot);

    /* Execute the query and return the result. */
    void execute();

    /* Destructor */
    ~QueryTree();

private:
    /* Private constructor */
    QueryTree();

    /* Parses the query tree and attaches a reference to the QtSource nodes
     whenever it is needed. */
    void attachDataSourceNodes();

    /* The query tree root node*/
    QtSelectStatement *root;

    /* Singleton instance. */
    static QueryTree instance;
};


#endif	/* QUERYTREE_HPP */