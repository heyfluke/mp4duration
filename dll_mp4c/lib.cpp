/* 
 * Project: mp4c dll 
 * Purpose: for python to get mp4 duration info
 * Author: Fluke <fluke.l@gmail.com>
 * Date: 2008-4-23
 */

#include "lib.h"
#include "windows.h"

BOOL APIENTRY DllMain( HANDLE hModule, 
					  DWORD  ul_reason_for_call, 
					  LPVOID lpReserved
					  )
{
    switch (ul_reason_for_call)
	{
	case DLL_PROCESS_ATTACH:	
	case DLL_THREAD_ATTACH:
	case DLL_THREAD_DETACH:
	case DLL_PROCESS_DETACH:
		break;
    }
    return TRUE;
}
