#ifndef _____LEAKS_CONTROL____
#define _____LEAKS_CONTROL____

#ifdef _DEBUG
#pragma warning(disable: 4100)
#pragma warning(disable: 4996)
#include <stdlib.h>

struct ALLOC_INFO 
{
	  unsigned int	address;
	  unsigned int	size;
	  char	file[_MAX_PATH];
	  unsigned int	line;
		ALLOC_INFO *pNext;
};

void RemoveTrack(unsigned int addr);
void AddTrack(unsigned int addr,  unsigned int asize,  const char *fname, unsigned int lnum);

void DumpUnfreed();

extern ALLOC_INFO *g_LeakList;

#ifdef MEMORY_LEAKS_MONITOR
inline void * __cdecl operator new(unsigned int  size, const char *file, int line)
{
	void *ptr = (void *)malloc(size);
	AddTrack((unsigned int)ptr, size, file, line);
	return(ptr);
};

inline void __cdecl operator delete(void *p)
{
	RemoveTrack((unsigned int)p);
	free(p);
};

inline void * __cdecl operator new[](unsigned int size, const char *file, int line)
{
	void *ptr = (void *)malloc(size);
	AddTrack((unsigned int)ptr, size, file, line);
	return(ptr);
};

inline void __cdecl operator delete[](void *p)
{
	RemoveTrack((unsigned int)p);
	free(p);
}

#define NEW(type, ...) createPtr<type>(__FILE__, __LINE__, __VA_ARGS__)
#define NEW_ARRAY new(__FILE__, __LINE__)
#define DELETE delete
#define DELETE_ARRAY delete[]
#define DUMP_UNFREED DumpUnfreed();

#endif

#endif

#ifndef NEW
#define NEW new
#endif
#ifndef NEW_ARRAY
#define NEW_ARRAY new
#endif
#ifndef DELETE
#define DELETE delete
#endif
#ifndef DELETE_ARRAY
#define DELETE_ARRAY delete[]
#endif
#ifndef DUMP_UNFREED
#define DUMP_UNFREED
#endif

#endif


