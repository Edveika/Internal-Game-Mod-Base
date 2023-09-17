#pragma once
#include <Windows.h>
#include <vector>
#include <Psapi.h>

#define STR_MERGE_IMPL(a, b) a##b
#define STR_MERGE(a, b) STR_MERGE_IMPL(a, b)
#define MAKE_PAD(size) STR_MERGE(_pad, __COUNTER__)[size]
#define DEFINE_MEMBER_N(type, name, offset) struct {unsigned char MAKE_PAD(offset); type name;}

namespace Utils
{
	void nop(BYTE* dst, unsigned int size);
	void patch(BYTE* dst, BYTE* src, unsigned int size);
	void write_to_memory(DWORD addressToWrite, const char* valueToWrite, int byteNum);
	DWORD find_dma_address(DWORD ptr, std::vector<unsigned int> offsets);
	DWORD find_signature(const char* module, const char* pattern, const char* mask);
}