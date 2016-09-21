//
// File Name : Command.cpp
// File Date : 19 March 2012
// Author(s) : Valendian
//

#include <stdio.h>
#include "Lexer.h"
#include "Parse.h"
#include "Command.h"

extern volatile bool DebugCommand;

int Command(char *Source, char *Text)
{
	bool Status = false;
	if (!Source) return Status;
	if (!Text) return Status;

	if (DebugCommand) printf("%s\n", Source);
	table *Table = NewTokenTable(Source);
	if (Table) {
		if (Lexer(Table, Text)) {
			if (Parse(Table)) Status = true;
		}
	}
	DeleteTokenTable(Table);
	return Status;
}
