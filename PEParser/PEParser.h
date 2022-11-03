#pragma once

#include <iostream>
#include <fstream>
#include <vector>

class PEParser {
private:
	class DataDir {
	private:
		unsigned int addr;
		unsigned int size;
	public:
		DataDir(unsigned int addr, unsigned int size);
	};

	class SectionHeader {
	public:
		std::string name;
		unsigned int size;
		unsigned int addr;
		unsigned int rawSize;
		unsigned int rawAddr;
		unsigned int ptrRelocations;
		unsigned int ptrLineNums;
		unsigned short numRelocations;
		unsigned short numLineNums;
		unsigned int characteristics;
	};

	const std::fstream& file;
	unsigned int signatureOffset;
	std::string signature;
	unsigned short machine;
	unsigned short numSections;
	unsigned int timestamp;
	unsigned int ptrSymbolTable;
	unsigned int numSymbols;
	unsigned short optHeaderSz;
	unsigned short characteristics;
	// Optional Header
	// Standard
	unsigned short magic;
	bool plusMode;
	unsigned char linkerMajor;
	unsigned char linkerMinor;
	unsigned int codeSize;
	unsigned int initSize;
	unsigned int uninitSize;
	unsigned int entryPoint;
	unsigned int codeStart;
	unsigned int dataStart;
	// Windows-specific
	unsigned long long imageBase;
	unsigned int sectionAlignment;
	unsigned int fileAlignment;
	unsigned short osMajor;
	unsigned short osMinor;
	unsigned short imMajor;
	unsigned short imMinor;
	unsigned short ssMajor;
	unsigned short ssMinor;
	unsigned int win32Version;
	unsigned int imageSize;
	unsigned int headersSize;
	unsigned int checkSum;
	unsigned short subsystem;
	unsigned short dllCharacteristics;
	unsigned long long stackReserveSize;
	unsigned long long stackCommitSize;
	unsigned long long heapReserveSize;
	unsigned long long heapCommitSize;
	unsigned int loaderFlags;
	unsigned int numRva;
	std::vector<DataDir> dataDirs;
	std::vector<SectionHeader> sectionHeaders;
public:
	PEParser(std::fstream& file);
};