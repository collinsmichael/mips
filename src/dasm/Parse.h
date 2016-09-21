//
// File Name : Parse.h
// File Date : 19 March 2012
// Author(s) : Valendian
//

#ifndef PARSE_HEADER
#define PARSE_HEADER

int DoCommand(table *Table);
int DoOpen(table *Table);
int DoLink(table *Table);
int DoClose(table *Table);
int DoRegister(table *Table);
int DoPrint(table *Table);
int DoShow(table *Table);
int DoHide(table *Table);
int DoDebug(table *Table);
int DoDasm(table *Table);
int Parse(table *Table);

#endif
