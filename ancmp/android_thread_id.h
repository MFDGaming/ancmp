#ifndef ANCMP_ANDROID_THREAD_ID
#define ANCMP_ANDROID_THREAD_ID

int android_thread_id_init(void);

int android_thread_id_alloc(int thread_id);

void android_thread_id_free(int thread_id);

int android_thread_id_find_free(void);

int android_thread_id_acquire(void);

int android_thread_id_get_current(void);

#endif
