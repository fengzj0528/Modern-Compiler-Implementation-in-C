#ifndef INTERP_H
#define INTERP_H

#include<stdio.h>
#include"util.h"
#include"slp.h"

typedef struct table *Table_;
struct table{
    string id;
    int value;
    Table_ tail;
};
Table_ Table(string id, int value, Table_ tail);

typedef struct intAndTable *IntAndTable_;
struct intAndTable{
    int i;
    Table_ t;
};
IntAndTable_ IntAndTable(int i, Table_ t);

Table_ interpStm(A_stm s, Table_ t);
IntAndTable_ interpExp(A_exp e, Table_ t);
IntAndTable_ interpExpList(A_expList expList, Table_ t);

int lookup(Table_ t, string key);
Table_ update(Table_ t, string id, int value);
#endif