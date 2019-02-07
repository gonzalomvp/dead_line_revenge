#ifndef _____LEAKS_CONTROL____
#define _____LEAKS_CONTROL____

#pragma warning(disable: 4100)
#pragma warning(disable: 4996)
#include <stdlib.h>

template<class T>
using ptr = T*;

#ifdef MEMORY_LEAKS_MONITOR

struct ALLOC_INFO 
{
	unsigned int address;
	unsigned int size;
	char         file[_MAX_PATH];
	unsigned int line;
	ALLOC_INFO*  pNext;
};

template<typename T>
ptr<T> AddTrack(ptr<T> addr, unsigned int asize, const char *fname, unsigned int lnum) {
	ALLOC_INFO *info;

	info = (ALLOC_INFO*)malloc(sizeof(ALLOC_INFO));
	info->address = (unsigned int) addr;
	strncpy(info->file, fname, _MAX_PATH - 1);
	info->line = lnum;
	info->size = sizeof(T);
	info->pNext = g_LeakList;
	g_LeakList = info;

	return addr;
}

template<typename T>
void RemoveTrack(ptr<T> addr) {
	ALLOC_INFO *pLastItem = 0;
	ALLOC_INFO *pCurrItem = g_LeakList;
		while (pCurrItem) {
		if (pCurrItem->address == (unsigned int) addr) {
			if (pLastItem)
				pLastItem->pNext = pCurrItem->pNext;
			else
				g_LeakList = pCurrItem->pNext;
			free(pCurrItem);
			break;
		}
		else {
			pLastItem = pCurrItem;
			pCurrItem = pCurrItem->pNext;
		}
	}
}

void DumpUnfreed();

extern ALLOC_INFO *g_LeakList;

#define NEW(type, ...) AddTrack<type>(new type(__VA_ARGS__), sizeof(type), __FILE__, __LINE__)
#define NEW_ARRAY(type, num) AddTrack<type>(new type[num], sizeof(type) * num, __FILE__, __LINE__)
#define DELETE(ptr) { RemoveTrack(ptr); delete ptr; }
#define DELETE_ARRAY(ptr) { RemoveTrack(ptr); delete[] ptr; }
#define DUMP_UNFREED DumpUnfreed()

#else

#define NEW(type, ...) new type(__VA_ARGS__)
#define NEW_ARRAY(type, num) new type[num]
#define DELETE(ptr) delete ptr;
#define DELETE_ARRAY(ptr) delete[] ptr;
#define DUMP_UNFREED

#endif

#endif


