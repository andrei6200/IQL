/* 
 * File:   QtBinaryOperation.cpp
 * Author: andrei
 * 
 * Created on August 14, 2010, 12:04 PM
 */

#include "QtBinaryOperation.hpp"
#include "HqlMain.hpp"
#include "QueryTree.hpp"

QtBinaryOperation::QtBinaryOperation(QtNode *node1, char* op1, QtNode *node2)
    : child1(node1), child2(node2), op(op1), opname("binaryop")
{
    if (node1 == NULL || node2 == NULL)
        throw string("Operand of the BinaryOperation must be non-null.");
    if (op == "+")
        opname = "plus";
    if (op == "-")
        opname = "minus";
    if (op == "*")
        opname = "times";
    if (op == "/")
        opname = "divide";
    if (op == "%")
        opname = "mod";
    if (op == "^")
        opname = "power";
}

QtBinaryOperation::~QtBinaryOperation()
{
    if (child1)
    {
        delete child1;
        child1 = NULL;
    }
    if (child2)
    {
        delete child2;
        child2 = NULL;
    }
}

HqlTable* QtBinaryOperation::execute()
{
    PostgresDS &pg = HqlMain::getInstance().getSqlDataSource();
    RasdamanDS &rman = HqlMain::getInstance().getRasqlDataSource();
    HqlTable *result = NULL, *tmp1 = NULL, *tmp2 = NULL;

    tmp1 = child1->execute();
    tmp2 = child2->execute();
    string t1 = tmp1->getName();
    string t2 = tmp2->getName();
    delete tmp1;
    delete tmp2;
    tmp1 = pg.query("SELECT * FROM " + t1);
    tmp2 = pg.query("SELECT * FROM " + t2);
    string col1 = tmp1->getColumnNames().at(0);
    string col2 = tmp2->getColumnNames().at(0);
    delete tmp1;
    delete tmp2;
    string q = "SELECT t1." + col1 + " " + op + " t2." + col2 +
            " AS " + opname + ", t1." + HQL_COL + " INTO " + this->id +
            " FROM " + t1 + " AS t1 JOIN " + t2 + " AS t2 ON t1." + HQL_COL + "=t2." + HQL_COL + ";";
    
    result = pg.query(q);
    pg.addTempTable(this->id);
    result->setName(this->id);

    return result;
}

string QtBinaryOperation::toString()
{
    string str =  child1->toString() + " " + op + " " + child2->toString();
    return str;
}

DbEnum QtBinaryOperation::setupDbSource()
{
    
    return db_source;
}

void QtBinaryOperation::print(ostream &o, std::string indent)
{
    o << indent << "QtBinaryOperation (" << id << "): " << op << endl;
    child1->print(o, indent + QTINDENT);
    child2->print(o, indent + QTINDENT);
}
