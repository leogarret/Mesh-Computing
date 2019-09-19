#pragma once
#include "main.h"
#include "openMesh.h"
#include "vcg.h"

#include <tchar.h>
#include <strsafe.h>

typedef struct MyData {
	int val1;
	int val2;
} MYDATA, *PMYDATA;

DWORD WINAPI start_vcg(LPVOID lpParam)
{
	DWORD error;

	/* Le HANDLE ACTUEL EST INVALIDE (CODE 6) */
	if (!SetThreadPriority(GetCurrentThread(), THREAD_PRIORITY_HIGHEST)) {
		error = GetLastError();
		if (error == ERROR_THREAD_MODE_ALREADY_BACKGROUND)
		{
			std::cout << "This thread is arleady in this mode.\n" << std::endl;
		}
		else
		{
			std::cout << "Failed to enter background mode (" << HRESULT_FROM_WIN32(error) << ")" << std::endl;
		}
	}

	Mesh mesh;
	vcg::tri::io::ImporterOBJ<Mesh>::Info mesh_info;

	vc::open_mesh(mesh, "../../obj/M5.obj");
	return 0;
}

int main(void)
{
	PMYDATA pData;
	DWORD   dwThreadId;
	HANDLE  hThread;

	pData = (PMYDATA)HeapAlloc(GetProcessHeap(), HEAP_ZERO_MEMORY, sizeof(MYDATA));

	if (pData == NULL)
		ExitProcess(2);

	pData->val1 = 0;
	pData->val2 = 100;

	hThread = CreateThread(NULL, 0, start_vcg, pData, 0, &dwThreadId);

	if (hThread == NULL)
		ExitProcess(3);

	WaitForSingleObject(hThread, INFINITE);

	CloseHandle(hThread);
	if (pData != NULL)
	{
		HeapFree(GetProcessHeap(), 0, pData);
		pData = NULL;
	}

	return 0;
}