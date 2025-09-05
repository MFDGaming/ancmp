#include "ancmp_rng.h"

#ifdef _WIN32

#include <windows.h>
#include <io.h>
#include <fcntl.h>
#include <stdlib.h>
#include <time.h>

static DWORD WINAPI ancmp_rng_task(LPVOID lpParameter) {
    HANDLE hw = (HANDLE)lpParameter;
    int rng_buffer[1024] = {0};
    DWORD written = 0;
    int i;
    for (;;) {
        srand((unsigned)time(NULL) ^ GetCurrentThreadId());
        for (i = 0; i < 1024; ++i) {
            rng_buffer[i] = rand();
        }
        if (!WriteFile(hw, rng_buffer, sizeof(rng_buffer), &written, NULL)) {
            break;
        }
    }
    CloseHandle(hw);
    return 0;
}

int ancmp_open_rng() {
    HANDLE hr, hw, t;
    if (!CreatePipe(&hr, &hw, NULL, 0)) {
        return -1;
    }
    t = CreateThread(NULL, 0, ancmp_rng_task, hw, 0, NULL);
    if (t) {
        CloseHandle(t);        
    }
    return _open_osfhandle((intptr_t)hr, _O_RDONLY);
}

#endif