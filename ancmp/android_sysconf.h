#pragma once

#define ANDROID_SC_ARG_MAX             0x0000
#define ANDROID_SC_BC_BASE_MAX         0x0001
#define ANDROID_SC_BC_DIM_MAX          0x0002
#define ANDROID_SC_BC_SCALE_MAX        0x0003
#define ANDROID_SC_BC_STRING_MAX       0x0004
#define ANDROID_SC_CHILD_MAX           0x0005
#define ANDROID_SC_CLK_TCK             0x0006
#define ANDROID_SC_COLL_WEIGHTS_MAX    0x0007
#define ANDROID_SC_EXPR_NEST_MAX       0x0008
#define ANDROID_SC_LINE_MAX            0x0009
#define ANDROID_SC_NGROUPS_MAX         0x000a
#define ANDROID_SC_OPEN_MAX            0x000b
#define ANDROID_SC_PASS_MAX            0x000c
#define ANDROID_SC_2_C_BIND            0x000d
#define ANDROID_SC_2_C_DEV             0x000e
#define ANDROID_SC_2_C_VERSION         0x000f
#define ANDROID_SC_2_CHAR_TERM         0x0010
#define ANDROID_SC_2_FORT_DEV          0x0011
#define ANDROID_SC_2_FORT_RUN          0x0012
#define ANDROID_SC_2_LOCALEDEF         0x0013
#define ANDROID_SC_2_SW_DEV            0x0014
#define ANDROID_SC_2_UPE               0x0015
#define ANDROID_SC_2_VERSION           0x0016
#define ANDROID_SC_JOB_CONTROL         0x0017
#define ANDROID_SC_SAVED_IDS           0x0018
#define ANDROID_SC_VERSION             0x0019
#define ANDROID_SC_RE_DUP_MAX          0x001a
#define ANDROID_SC_STREAM_MAX          0x001b
#define ANDROID_SC_TZNAME_MAX          0x001c
#define ANDROID_SC_XOPEN_CRYPT         0x001d
#define ANDROID_SC_XOPEN_ENH_I18N      0x001e
#define ANDROID_SC_XOPEN_SHM           0x001f
#define ANDROID_SC_XOPEN_VERSION       0x0020
#define ANDROID_SC_XOPEN_XCU_VERSION   0x0021
#define ANDROID_SC_XOPEN_REALTIME      0x0022
#define ANDROID_SC_XOPEN_REALTIME_THREADS  0x0023
#define ANDROID_SC_XOPEN_LEGACY        0x0024
#define ANDROID_SC_ATEXIT_MAX          0x0025
#define ANDROID_SC_IOV_MAX             0x0026
#define ANDROID_SC_PAGESIZE            0x0027
#define ANDROID_SC_PAGE_SIZE           0x0028
#define ANDROID_SC_XOPEN_UNIX          0x0029
#define ANDROID_SC_XBS5_ILP32_OFF32    0x002a
#define ANDROID_SC_XBS5_ILP32_OFFBIG   0x002b
#define ANDROID_SC_XBS5_LP64_OFF64     0x002c
#define ANDROID_SC_XBS5_LPBIG_OFFBIG   0x002d
#define ANDROID_SC_AIO_LISTIO_MAX      0x002e
#define ANDROID_SC_AIO_MAX             0x002f
#define ANDROID_SC_AIO_PRIO_DELTA_MAX  0x0030
#define ANDROID_SC_DELAYTIMER_MAX      0x0031
#define ANDROID_SC_MQ_OPEN_MAX         0x0032
#define ANDROID_SC_MQ_PRIO_MAX         0x0033
#define ANDROID_SC_RTSIG_MAX           0x0034
#define ANDROID_SC_SEM_NSEMS_MAX       0x0035
#define ANDROID_SC_SEM_VALUE_MAX       0x0036
#define ANDROID_SC_SIGQUEUE_MAX        0x0037
#define ANDROID_SC_TIMER_MAX           0x0038
#define ANDROID_SC_ASYNCHRONOUS_IO     0x0039
#define ANDROID_SC_FSYNC               0x003a
#define ANDROID_SC_MAPPED_FILES        0x003b
#define ANDROID_SC_MEMLOCK             0x003c
#define ANDROID_SC_MEMLOCK_RANGE       0x003d
#define ANDROID_SC_MEMORY_PROTECTION   0x003e
#define ANDROID_SC_MESSAGE_PASSING     0x003f
#define ANDROID_SC_PRIORITIZED_IO      0x0040
#define ANDROID_SC_PRIORITY_SCHEDULING 0x0041
#define ANDROID_SC_REALTIME_SIGNALS    0x0042
#define ANDROID_SC_SEMAPHORES          0x0043
#define ANDROID_SC_SHARED_MEMORY_OBJECTS  0x0044
#define ANDROID_SC_SYNCHRONIZED_IO     0x0045
#define ANDROID_SC_TIMERS              0x0046
#define ANDROID_SC_GETGR_R_SIZE_MAX    0x0047
#define ANDROID_SC_GETPW_R_SIZE_MAX    0x0048
#define ANDROID_SC_LOGIN_NAME_MAX      0x0049
#define ANDROID_SC_THREAD_DESTRUCTOR_ITERATIONS  0x004a
#define ANDROID_SC_THREAD_KEYS_MAX     0x004b
#define ANDROID_SC_THREAD_STACK_MIN    0x004c
#define ANDROID_SC_THREAD_THREADS_MAX  0x004d
#define ANDROID_SC_TTY_NAME_MAX        0x004e

#define ANDROID_SC_THREADS                     0x004f
#define ANDROID_SC_THREAD_ATTR_STACKADDR       0x0050
#define ANDROID_SC_THREAD_ATTR_STACKSIZE       0x0051
#define ANDROID_SC_THREAD_PRIORITY_SCHEDULING  0x0052
#define ANDROID_SC_THREAD_PRIO_INHERIT         0x0053
#define ANDROID_SC_THREAD_PRIO_PROTECT         0x0054
#define ANDROID_SC_THREAD_SAFE_FUNCTIONS       0x0055

#define ANDROID_SC_NPROCESSORS_CONF            0x0060
#define ANDROID_SC_NPROCESSORS_ONLN            0x0061
#define ANDROID_SC_PHYS_PAGES                  0x0062
#define ANDROID_SC_AVPHYS_PAGES                0x0063

long android_sysconf(int name);