// defDLL.cpp : 定义 DLL 的导出函数。
//

#include "defDLL.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#pragma warning(disable : 4996)

int plus(int x, int y) {
	return x + y;
}

int dim(int x, int y) {
	return x - y;
}

int func1() {
	return 1;
}
int func2() {
	return 2;
}
int func3() {
	return 3;
}

//PEpointer函数

int* ptre_lfanew(char* fbuffer) {
	//先获取节表起始地址，即e_lfanew + 24 + SizeOfOptionalHeader
	//e_lfanew
	int* pe_lfanew;
	pe_lfanew = (int*)(fbuffer + 60);
	//int e_lfanew;
	//e_lfanew = *pe_lfanew;
	//printf("e_lfanew = %x\n", e_lfanew);

	return pe_lfanew;
}

char* ptrPE(char* fbuffer) {
	char* ret;

	int e_lfanew;
	e_lfanew = *ptre_lfanew(fbuffer);

	//printf("%x\n", e_lfanew);
	ret = fbuffer + e_lfanew + 4;

	return ret;
}

char* ptrOptionPE(char* fbuffer) {
	char* ret;

	int e_lfanew;
	e_lfanew = *ptre_lfanew(fbuffer);
	///printf("%x\n", e_lfanew);
	ret = fbuffer + e_lfanew + 24;

	//printf("ptrOptionPE = %x\n", *(ret));

	return ret;

}

int* ptrAddressOfEntryPoint(char* fbuffer) {
	int* pAddressOfEntryPoint;
	pAddressOfEntryPoint = (int*)(ptrOptionPE(fbuffer) + 16);

	return pAddressOfEntryPoint;
}

int* ptrSizeOfImage(char* fbuffer) {
	int* pSizeOfImage;
	//首先获取image在内存中的大小，即扩展PE头里面偏移56个字节的SizeOfImage(4byte)
	pSizeOfImage = (int*)(ptrOptionPE(fbuffer) + 56);

	//printf("function in ptrSizeOfImage()%x   %x\n", *((int*)( ptrOptionPE(fbuffer) + 56 )),1);
	//printf("function in ptrSizeOfImage()%x   %x\n", *pSizeOfImage,1);

	return pSizeOfImage;
}

int* ptrSizeOfHeaders(char* fbuffer) {
	//先拷贝头文件+节表，即获取扩展PE头偏移60个字节的SizeOfHeaders(4 byte)里面的值
	int* pSizeOfHeaders;
	pSizeOfHeaders = (int*)(ptrOptionPE(fbuffer) + 60);
	//int SizeOfHeaders;
	//SizeOfHeaders = *pSizeOfHeaders;
	//printf("SizeOfHeaders = %x\n", SizeOfHeaders);

	return pSizeOfHeaders;
}

short* ptrSizeOfOptionalHeader(char* fbuffer) {
	//PE标记起始地址，即e_lfanew
	int e_lfanew;
	e_lfanew = *ptre_lfanew(fbuffer);

	//SizeOfOptionalHeader
	short* pSizeOfOptionalHeader;
	pSizeOfOptionalHeader = (short*)(fbuffer + e_lfanew + 20);
	//short SizeOfOptionalHeader;
	//SizeOfOptionalHeader = *pSizeOfOptionalHeader;
	//printf("SizeOfOptionalHeader = %x\n", SizeOfOptionalHeader);

	return pSizeOfOptionalHeader;
}

char* ptrSection(char* fbuffer) {
	char* ret;

	ret = fbuffer + *ptre_lfanew(fbuffer) + 24 + *ptrSizeOfOptionalHeader(fbuffer);

	return ret;
}

int SectionOffset(char* fbuffer) {
	int sectionOffset;

	int e_lfanew;
	e_lfanew = *ptre_lfanew(fbuffer);

	short SizeOfOptionalHeader;
	SizeOfOptionalHeader = *ptrSizeOfOptionalHeader(fbuffer);

	//按照下述公式求得节表的起始地址
	sectionOffset = e_lfanew + 24 + SizeOfOptionalHeader;
	return sectionOffset;

}

int* ptrVirtualAddress(char* ptrSection) {
	int* pVirtualAddress;
	pVirtualAddress = (int*)(ptrSection + 12);

	return pVirtualAddress;
}

int* ptrSizeOfRawData(char* ptrSection) {
	int* pSizeOfRawData;
	pSizeOfRawData = (int*)(ptrSection + 16);

	return pSizeOfRawData;
}

int* ptrPhysicalAddress(char* ptrSection) {
	int* pPhysicalAddress;
	pPhysicalAddress = (int*)(ptrSection + 8);

	return pPhysicalAddress;
}

int* ptrPointerToRawData(char* ptrSection) {
	int* pPointerToRawData;
	pPointerToRawData = (int*)(ptrSection + 20);

	return pPointerToRawData;
}

int* ptrCharacteristics(char* ptrSection) {
	int* pCharacteristics;
	pCharacteristics = (int*)(ptrSection + 36);

	return pCharacteristics;
}

short* ptrNumberOfSection(char* fbuffer) {
	//根据PE头获取节的数量，即PE头偏移6个字节的NumberOfSection(4)
	int e_lfanew;
	e_lfanew = *ptre_lfanew(fbuffer);

	short* pNumberOfSection;
	pNumberOfSection = (short*)(fbuffer + e_lfanew + 6);
	//short NumberOfSection;
	//NumberOfSection = *pNumberOfSection;
	//printf("NumberOfSection = %d\n", NumberOfSection);

	return pNumberOfSection;
}

int* ptrImageBase(char* fbuffer) {
	char* pOPE = ptrOptionPE(fbuffer);

	int* pImageBase = (int*)(pOPE + 28);

	return pImageBase;
}

int* ptrMemoryAlignment(char* fbuffer) {
	char* pOPE = ptrOptionPE(fbuffer);

	int* pMemoryAlignment = (int*)(pOPE + 32);

	return pMemoryAlignment;
}


int* ptrFileAlignment(char* fbuffer) {
	char* pOPE = ptrOptionPE(fbuffer);

	int* pFileAlignment = (int*)(pOPE + 36);

	return pFileAlignment;
}

//编写一个函数能自动将文件读到一块内存中，并且返回该块内存的指针
char* FileToBuffer(char fpath[]) {
	FILE* pfile;
	long lSize;
	char* fbuffer;

	pfile = fopen(fpath, "rb");

	fseek(pfile, 0, SEEK_END);
	lSize = ftell(pfile);

	rewind(pfile);

	fbuffer = (char*)malloc(lSize);

	if (fbuffer != NULL) {
		fread(fbuffer, 1, lSize, pfile);
		fclose(pfile);
		return fbuffer;
	}
	else {
		printf("Malloc defeat~");
		fclose(pfile);
		return NULL;
	}
}

//新申请内存的函数
char* NewBuffer(int SizeOfImage) {
	//根据传入的SizeOfImage确认需要新分配的内存的大小
	char* newbuffer;
	newbuffer = (char*)malloc(SizeOfImage);

	if (newbuffer != NULL) {
		return newbuffer;
	}
	else {
		return NULL;
	}
}

char* FbufferToNbuffer(char* fbuffer, char* newbuffer) {
	//头大小
	int SizeOfHeaders;
	SizeOfHeaders = *ptrSizeOfHeaders(fbuffer);

	//节数量
	short NumberOfSection;
	NumberOfSection = *ptrNumberOfSection(fbuffer);

	//节表起始地址偏移
	int sectionoffset;
	sectionoffset = SectionOffset(fbuffer);

	//映像基址
	int ImageBase;
	ImageBase = *ptrImageBase(fbuffer);

	printf("头大小 = %x\t节数量 = %d\t节表起始地址偏移量 = %x\t映像基址 = %x\n", SizeOfHeaders, NumberOfSection, sectionoffset, ImageBase);


	if (newbuffer != NULL && fbuffer != NULL) {
		//首先将头+节表复制到newbuffer中
		memcpy(newbuffer, fbuffer, SizeOfHeaders);

		//将节循环复制到newbuffer中

		int VirtualAddress;
		int SizeOfRawData;
		int PointerToRawData;

		for (int i = 0; i < NumberOfSection; i++) {
			//pSection + 40 * i;
			VirtualAddress = *ptrVirtualAddress(ptrSection(fbuffer) + 40 * i);
			SizeOfRawData = *ptrSizeOfRawData(ptrSection(fbuffer) + 40 * i);
			PointerToRawData = *ptrPointerToRawData(ptrSection(fbuffer) + 40 * i);

			memcpy(newbuffer + VirtualAddress, fbuffer + PointerToRawData, SizeOfRawData);


		}

	}
	else {
		printf("newbuffer defeat~");
		return NULL;
	}

	return newbuffer;
}

//2、将内存运行中的文件重新写到某个文件中
void MemBufferToFile(char* Newbuffer, char fpath[]) {
	FILE* pfile;
	pfile = fopen(fpath, "wb");

	//获取文件头的大小
	int SizeOfHeaders;
	SizeOfHeaders = *ptrSizeOfHeaders(Newbuffer);
	//向文件写入PE头
	fwrite(Newbuffer, SizeOfHeaders, 1, pfile);

	//节数量
	short NumberOfSection;
	NumberOfSection = *ptrNumberOfSection(Newbuffer);
	//节表起始地址
	char* pSection = ptrSection(Newbuffer);

	int VirtualAddress;
	int SizeOfRawData;
	int PointerToRawData;
	//循环遍历节的内存地址，然后开始向文件写入
	for (int i = 0; i < NumberOfSection; i++) {
		VirtualAddress = *ptrVirtualAddress(ptrSection(Newbuffer) + 40 * i);
		SizeOfRawData = *ptrSizeOfRawData(ptrSection(Newbuffer) + 40 * i);
		PointerToRawData = *ptrPointerToRawData(ptrSection(Newbuffer) + 40 * i);

		fseek(pfile, PointerToRawData, SEEK_SET);
		fwrite(Newbuffer + VirtualAddress, SizeOfRawData, 1, pfile);
	}
}

//2-1、将内存中一块数据写到某个文件中
void BufferToFile(char* fbuffer, char fpath[]) {
	FILE* pfile;
	pfile = fopen(fpath, "wb");

	//获取最后一节数据，然后确认文件的大小
	int iSize;
	int NumberOfSection;
	int SizeOfRawData;
	int PointerToRawData;
	NumberOfSection = *ptrNumberOfSection(fbuffer);
	SizeOfRawData = *ptrSizeOfRawData(ptrSection(fbuffer) + 40 * (NumberOfSection - 1));
	PointerToRawData = *ptrPointerToRawData(ptrSection(fbuffer) + 40 * (NumberOfSection - 1));
	iSize = PointerToRawData + SizeOfRawData;

	printf("%x\n", PointerToRawData);
	//向文件写入PE头
	fwrite(fbuffer, iSize, 1, pfile);


}

//3、RVA to FOA（将内存中的地址转换到文件中的地址）
int RVA_TO_FOA(char* buffer, int rva) {
	int FOA;
	// 用内存地址 - ImageBase = 虚拟内存地址（RVA）
	int virtualaddress;
	virtualaddress = rva;

	//节表起始地址
	char* pSection = ptrSection(buffer);

	//节数量
	short NumberOfSection;
	NumberOfSection = *ptrNumberOfSection(buffer);

	int VirtualAddress;
	int SizeOfRawData;
	int PointerToRawData;
	//通过遍历节表信息，获取每一个节的虚拟地址范围（VirtualAddress ~ VirtualAddress + SizeRawData）
	for (int i = 0; i < NumberOfSection; i++) {
		VirtualAddress = *ptrVirtualAddress(ptrSection(buffer) + 40 * i);
		SizeOfRawData = *ptrSizeOfRawData(ptrSection(buffer) + 40 * i);
		PointerToRawData = *ptrPointerToRawData(ptrSection(buffer) + 40 * i);
		//判断RVA是否在节范围里面
		if (VirtualAddress <= virtualaddress && virtualaddress < VirtualAddress + SizeOfRawData) {
			//用RVA - VirtualAddress + PointerToData = FOA
			FOA = virtualaddress - VirtualAddress + PointerToRawData;
			//printf("%x", FOA);
			return FOA;
		}
	}
	printf("Your Arg %x in RVA_TO_FOA() Address change defeat~\n", rva);
	return NULL;
}

int FOA_TO_RVA(char* buffer, int foa) {
	int RVA;
	//节表起始地址
	char* pSection = ptrSection(buffer);

	//节数量
	short NumberOfSection;
	NumberOfSection = *ptrNumberOfSection(buffer);

	int VirtualAddress;
	int SizeOfRawData;
	int PointerToRawData;
	//循环遍历节表，确认foa所在的节
	for (int i = 0; i < NumberOfSection; i++) {
		VirtualAddress = *ptrVirtualAddress(ptrSection(buffer) + 40 * i);
		SizeOfRawData = *ptrSizeOfRawData(ptrSection(buffer) + 40 * i);
		PointerToRawData = *ptrPointerToRawData(ptrSection(buffer) + 40 * i);

		if (PointerToRawData <= foa && foa < PointerToRawData + SizeOfRawData) {
			//用RVA - VirtualAddress + PointerToData = FOA
			RVA = foa - PointerToRawData + VirtualAddress;
			//printf("%x", FOA);
			return RVA;
		}
	}
	printf("Your Arg %x in FOA_TO_RVA() Address change defeat~\n");
	return NULL;
}

//传入字符串指针，打印该字符串
void showstring(char* ptrString) {
	for (int x = 0; *(ptrString + x) != 0; x++) {
		printf("%c", *(ptrString + x));
	}
	printf("\n");
}

//判断两个字符串是否相等
int StringCMP(char* str1, char* str2) {
	int x;
	x = 0;
	//开始判断函数名和传入的两个参数是否相等。
	//showstring(str1);
	if (strlen(str1) == strlen(str2)) {
		while (x < strlen(str1)) {
			if (*(str1 + x) == *(str2 + x)) {
				x++;
			}
			else {
				return 0;
			}
		}
		return 1;
	}
	else {
		//printf("name missed\n");
		return 0;
	}
}


