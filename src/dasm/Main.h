//
// File Name : Main.h
// File Date : 19 March 2012
// Author(s) : Valendian
//

#ifndef MAIN_HEADER
#define MAIN_HEADER
#include <stdint.h>
#include <stdio.h>

typedef struct binary {
	FILE    *File;
	void    *Data;
	void    *Copy;
	void    *Code;
	uint32_t Size;
	char     Path[0x1000];
	char     Drive[0x10];
	char     Dir[0xDF0];
	char     Fname[0x100];
	char     Ext[0x100];
} binary;

int main(int argc, char *argv[]);

#endif
