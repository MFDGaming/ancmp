#include "ancmp_rng.h"

#ifdef _WIN32

#include <windows.h>
#include <stdlib.h>
#include <time.h>

static HANDLE ancmp_rng_thread = NULL;
static char ancmp_rng_is_running = 1;

static DWORD WINAPI ancmp_rng_task(LPVOID lpParameter) {
    int rng_buffer[1024] = {0};
    DWORD written = 0;
    HANDLE rng_pipe = CreateNamedPipeA(
        "\\\\.\\pipe\\ancmp\\random", PIPE_ACCESS_OUTBOUND,
        PIPE_TYPE_BYTE | PIPE_WAIT, PIPE_UNLIMITED_INSTANCES,
        sizeof(rng_buffer), sizeof(rng_buffer), 0, NULL
    );
    if (rng_pipe == INVALID_HANDLE_VALUE) {
        return -1;
    }
    if (ConnectNamedPipe(rng_pipe, NULL) || GetLastError() == ERROR_PIPE_CONNECTED) {
        while (ancmp_rng_is_running) {
            int i;
            srand(time(NULL));
            for (i = 0; i < 1024; ++i) {
                rng_buffer[i] = rand();
            }
            WriteFile(rng_pipe, rng_buffer, sizeof(rng_buffer), &written, NULL);
        }
        CloseHandle(rng_pipe);
    }
    return 0;
}

int ancmp_init_rng() {
    ancmp_rng_thread = CreateThread(NULL, 0, ancmp_rng_task, NULL, 0, NULL);
    if (!ancmp_rng_thread) {
        return 0;
    }
    return 1;
}

void ancmp_stop_rng() {
    ancmp_rng_is_running = 0;
    WaitForSingleObject(ancmp_rng_thread, INFINITE);
    CloseHandle(ancmp_rng_thread);
}


#endif