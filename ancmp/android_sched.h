#pragma once

#define ANDROID_SCHED_NORMAL            0
#define ANDROID_SCHED_OTHER             0
#define ANDROID_SCHED_FIFO              1
#define ANDROID_SCHED_RR                2

struct android_sched_param {
    int sched_priority;
};