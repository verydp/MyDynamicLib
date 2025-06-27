#pragma once

extern "C" {
	int dim(int x, int y);
	int plus(int x, int y);
	int func1();
	int func2();
	int func3();

	int* ptre_lfanew(char* fbuffer);
	char* ptrPE(char* fbuffer);
	char* ptrOptionPE(char* fbuffer);
	int* ptrAddressOfEntryPoint(char* fbuffer);
	int* ptrSizeOfImage(char* fbuffer);
	int* ptrSizeOfHeaders(char* fbuffer);
	short* ptrSizeOfOptionalHeader(char* fbuffer);
	char* ptrSection(char* fbuffer);
	int SectionOffset(char* fbuffer);
	int* ptrVirtualAddress(char* ptrSection);
	int* ptrSizeOfRawData(char* ptrSection);
	int* ptrPhysicalAddress(char* ptrSection);
	int* ptrPointerToRawData(char* ptrSection);
	int* ptrCharacteristics(char* ptrSection);
	short* ptrNumberOfSection(char* fbuffer);
	int* ptrImageBase(char* fbuffer);
	int* ptrMemoryAlignment(char* fbuffer);
	int* ptrFileAlignment(char* fbuffer);

	char* FileToBuffer(char fpath[]);
	char* NewBuffer(int SizeOfImage);
	char* FbufferToNbuffer(char* fbuffer, char* newbuffer);
	void MemBufferToFile(char* Newbuffer, char fpath[]);
	void BufferToFile(char* fbuffer, char fpath[]);
	int RVA_TO_FOA(char* buffer, int rva);
	int FOA_TO_RVA(char* buffer, int foa);

	void showstring(char* ptrString);
	int StringCMP(char* str1, char* str2);
}

