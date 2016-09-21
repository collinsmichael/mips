//
// File Name : Main.h
// File Date : 19 March 2012
// Author(s) : Valendian
//

#ifndef MAIN_HEADER
#define MAIN_HEADER

#ifndef int8
typedef signed __int8 int8;
#endif
#ifndef uint8
typedef unsigned __int8 uint8;
#endif
#ifndef int16
typedef signed __int16 int16;
#endif
#ifndef uint16
typedef unsigned __int16 uint16;
#endif
#ifndef int32
typedef signed __int32 int32;
#endif
#ifndef uint32
typedef unsigned __int32 uint32;
#endif
#ifndef int64
typedef signed __int64 int64;
#endif
#ifndef uint64
typedef unsigned __int64 uint64;
#endif

typedef struct binary {
	FILE  *File;
	void  *Data;
	void  *Copy;
	void  *Code;
	uint32 Size;
	char   Path[0x1000];
	char   Drive[0x10];
	char   Dir[0xDF0];
	char   Fname[0x100];
	char   Ext[0x100];
} binary;

int main(int argc, char *argv[]);

#endif
