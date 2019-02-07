#include "stdafx.h"

#ifdef _WIN32
#ifdef MEMORY_LEAKS_MONITOR
#include <windows.h>
#include "leaks.h"
#include <stdio.h>

void DumpUnfreed()
{
	DWORD totalSize = 0;
	char buf[1024];

	if(!g_LeakList)
		return;

	ALLOC_INFO *pCurrItem = g_LeakList;
	while (pCurrItem)
	{
		sprintf(buf, "%s(%d): ADDRESS %d\t%d unfreed\n", pCurrItem->file, pCurrItem->line, pCurrItem->address, pCurrItem->size);
		OutputDebugStringA(buf);
		totalSize += pCurrItem->size;
		pCurrItem = pCurrItem->pNext;
	}
	sprintf(buf, "-----------------------------------------------------------\n");
	OutputDebugStringA(buf);
	sprintf(buf, "Total Unfreed: %d bytes\n", totalSize);
	OutputDebugStringA(buf);
};
#endif
#endif