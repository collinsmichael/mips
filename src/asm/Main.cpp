#include <malloc.h>
#include <memory.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>

#include "Lexer.h"
#include "Symbol.h"
#include "Parser.h"

#pragma warning(disable:4996)

char *FileData;
int   FileSize;

int FileOpen(char *FilePath)
{
	FILE *File;
	File = fopen(FilePath, "rb");
	if (!File) {
		printf("Error : Failed to open file! '%s'\r\n", FilePath);
		return false;
	}
	fseek(File, 0, SEEK_END);
	FileSize = ftell(File);
	fseek(File, 0, SEEK_SET);
	FileData = (char*)malloc(FileSize+1);
	if (!FileData) {
		fclose(File);
		File = 0;
		FileSize = 0;
		printf("Error : Failed to allocate %u bytes of memory for file! '%s'\r\n", FileSize, FilePath);
		return false;
	}
	memset(FileData, 0, FileSize+1);
	if (fread(FileData, FileSize, 1, File) != 1) {
		fclose(File);
		File = 0;
		free(FileData);
		FileData = 0;
		FileSize = 0;
		printf("Error : Failed to read %u bytes of data from file! '%s'\r\n", FileSize, FilePath);
		return false;
	}
	fclose(File);
	File = 0;
	printf("File Opened : '%s'\r\n", FilePath);
	return true;
}

int Process(char *FilePath, char *FileData, int FileSize)
{
	if (SymbolInit()) {
		Lexer(FileData, FileSize);
		Parser(FilePath);
	}
	SymbolFree();
	return true;
}

int CommandLine(int argc, char *argv[])
{
	for (int i = 1; i < argc; i++) {
		if (FileOpen(argv[i])) {
			Process(argv[i], FileData, FileSize);
		}
	}
	return true;
}

int main(int argc, char *argv[])
{
	CommandLine(argc, argv);
	return 0;
}
