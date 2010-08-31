/* 
 * File:   QueryTree.hpp
 * Author: andrei
 *
 * Created on June 26, 2010, 1:20 PM
 */

#ifndef QUERYTREE_HPP
#define	QUERYTREE_HPP

#include "QtNode.hpp"

#include "QtCartesianProduct.hpp"
#include "QtString.hpp"
#include "QtDataSource.hpp"
#include "QtDot.hpp"
#include "QtSelectStatement.hpp"
#include "QtColumn.hpp"
#include "QtEncodeArray.hpp"
#include "QtPostGisFunction.hpp"
#include "QtBinaryOperation.hpp"
#include "QtConst.hpp"
#include "QtRasqlInterval.hpp"
#include "QtTrimOperation.hpp"
#include "QtSqlFunction.hpp"

class HqlTable;

/* Singleton class for accessing the entire query tree. */
class QueryTree
{
public:
    /* Return (and initialize if needed) the singleton instance of HqlMain. */
    static QueryTree& getInstance();

    /* Load the whole query tree and prepare for execution. */
    void load(QtSelectStatement *newroot);

    /* Return the query tree root. */
    QtSelectStatement* getRoot();

    /* Execute the query and return the result. */
    void execute();

    /* Save the Rasdaman objects referenced in a HqlTable to files on disk. */
    void saveRasdamanObjectsToDisk(HqlTable *table);

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
