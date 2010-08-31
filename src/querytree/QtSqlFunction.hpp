/* 
 * File:   QtSqlFunction.hpp
 * Author: andrei
 *
 * Created on August 4, 2010, 1:45 PM
 */

#ifndef QTSQLFUNCTION_HPP
#define	QTSQLFUNCTION_HPP

#include "QtNode.hpp"
#include "QtList.hpp"

class QtSqlFunction : public QtNode
{
public:
    QtSqlFunction(char* fname, QtList *args);
    ~QtSqlFunction();

    HqlTable* execute();
    std::string toString();
    void print(ostream &o, std::string indent);

    DbEnum setupDbSource();
private:
    string name;
    QtList* arguments;
};

#endif	/* QTSQLFUNCTION_HPP */
