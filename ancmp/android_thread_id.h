#ifndef ANCMP_ANDROID_THREAD_ID
#define ANCMP_ANDROID_THREAD_ID

int android_thread_id_init(void);

int android_thread_id_acquire(void);

int android_reusable_thread_id_push(int thread_id);

int android_thread_id_get_current(void);

#endif