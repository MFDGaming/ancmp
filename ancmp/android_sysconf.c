#include "android_sysconf.h"

long android_sysconf(int name) {
    if (name == ANDROID_SC_PAGESIZE) {
        return 4096;
    } else if (name == ANDROID_SC_NPROCESSORS_ONLN) {
        return 2;
    }
    return -1;
}