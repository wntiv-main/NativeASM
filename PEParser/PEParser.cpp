#include "PEParser.h"

PEParser::PEParser(std::fstream& file) : file(file) {
	if(!file.is_open()) throw std::runtime_error("File is not opened");
	file.seekg(0x3c);
	file.read((char*)&signatureOffset, 4);
	std::cout << std::hex << signatureOffset << std::endl;
	file.seekg(signatureOffset);
	char sig[5] = {0};
	file.read(sig, 4);
	signature = sig;
	std::cout << signature << std::endl;
	if(signature != "PE") throw std::runtime_error("Unexpected signature");
	file.read((char*)&machine, 2);
	file.read((char*)&numSections, 2);
	file.read((char*)&timestamp, 4);
	file.read((char*)&ptrSymbolTable, 4);
	file.read((char*)&numSymbols, 4);
	file.read((char*)&optHeaderSz, 2);
	file.read((char*)&characteristics, 2);
	std::cout << numSections << std::endl;
	std::cout << characteristics << std::endl;
	if(optHeaderSz > 0) {
		// Standard
		file.read((char*)&magic, 2);
		if(magic == 0x10b) plusMode = false;
		else if(magic == 0x10b) plusMode = true;
		else throw std::runtime_error("Unexpected magic");
		file.read((char*)&linkerMajor, 1);
		file.read((char*)&linkerMinor, 1);
		file.read((char*)&codeSize, 4);
		file.read((char*)&initSize, 4);
		file.read((char*)&uninitSize, 4);
		file.read((char*)&entryPoint, 4);
		file.read((char*)&codeStart, 4);
		if(!plusMode) file.read((char*)&dataStart, 4);
		// Windows-specific
		file.read((char*)&imageBase, plusMode ? 8 : 4);
		file.read((char*)&sectionAlignment, 4);
		file.read((char*)&fileAlignment, 4);
		file.read((char*)&osMajor, 2);
		file.read((char*)&osMinor, 2);
		file.read((char*)&imMajor, 2);
		file.read((char*)&imMinor, 2);
		file.read((char*)&ssMajor, 2);
		file.read((char*)&ssMinor, 2);
		file.read((char*)&win32Version, 4);
		file.read((char*)&imageSize, 4);
		file.read((char*)&headersSize, 4);
		file.read((char*)&checkSum, 4);
		file.read((char*)&subsystem, 2);
		file.read((char*)&dllCharacteristics, 2);
		file.read((char*)&stackReserveSize, plusMode ? 8 : 4);
		file.read((char*)&stackCommitSize, plusMode ? 8 : 4);
		file.read((char*)&heapReserveSize, plusMode ? 8 : 4);
		file.read((char*)&heapCommitSize, plusMode ? 8 : 4);
		file.read((char*)&loaderFlags, 4);
		file.read((char*)&numRva, 4);
		// Data Dirs
		for(int i = 0; i < numRva; i++) {
			unsigned int addr = 0;
			unsigned int size = 0;
			file.read((char*)&addr, 4);
			file.read((char*)&size, 4);
			dataDirs.push_back(DataDir(addr, size));
		}
		// Section Headers
		for(int i = 0; i < numSections; i++) {
			SectionHeader section;
			char name[9] = {0};
			file.read(name, 8);
			section.name = name;
			file.read((char*)&section.size, 4);
			file.read((char*)&section.addr, 4);
			file.read((char*)&section.rawSize, 4);
			file.read((char*)&section.rawAddr, 4);
			file.read((char*)&section.ptrRelocations, 4);
			file.read((char*)&section.ptrLineNums, 4);
			file.read((char*)&section.numRelocations, 2);
			file.read((char*)&section.numLineNums, 2);
			file.read((char*)&section.characteristics, 4);
			std::cout << name << " " << section.characteristics << std::endl;
			if(section.characteristics & 0x20000000) {
				long cur = file.tellg();
				unsigned int newCharacteristics = section.characteristics;
				newCharacteristics |= 0x80000000;
				std::cout << "EXECUTABLE" << std::endl;
				file.seekp(cur - 4L);
				file.write((char*)&newCharacteristics, 4);
				file.seekg(cur);
			}
			sectionHeaders.push_back(section);
		}
	}
}

PEParser::DataDir::DataDir(unsigned int addr, unsigned int size)
	: addr(addr), size(size) {}
