#include <stdio.h>
#include <pthread.h>

#define NUM_THREADS 7

int x1 = 1, x2 = 2, x3 = 3, x4 = 4, x5 = 5, x6 = 6;
int w = 0, v = 0, y = 0, z = 0;
int ans;

pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t cond = PTHREAD_COND_INITIALIZER;

void *thread_a(void *arg) {
    pthread_mutex_lock(&mutex);
    printf("Thread a was running!\n");
    w = x1 * x2;
    pthread_cond_broadcast(&cond);
    pthread_mutex_unlock(&mutex);
    return NULL;
}

void *thread_b(void *arg) {
    pthread_mutex_lock(&mutex);
    printf("Thread b was running!\n");
    v = x3 * x4;
    pthread_cond_broadcast(&cond);
    pthread_mutex_unlock(&mutex);
    return NULL;
}

void *thread_c(void *arg) {
    pthread_mutex_lock(&mutex);
    while (v == 0) pthread_cond_wait(&cond, &mutex);
    printf("Thread c was running!\n");
    y = v * x5;
    pthread_cond_broadcast(&cond);
    pthread_mutex_unlock(&mutex);
    return NULL;
}

void *thread_d(void *arg) {
    pthread_mutex_lock(&mutex);
    while (v == 0) pthread_cond_wait(&cond, &mutex);
    printf("Thread d was running!\n");
    z = v * x6;
    pthread_cond_broadcast(&cond);
    pthread_mutex_unlock(&mutex);
    return NULL;
}

void *thread_e(void *arg) {
    pthread_mutex_lock(&mutex);
    while (w == 0 || y == 0) pthread_cond_wait(&cond, &mutex);
    printf("Thread e was running!\n");
    y = w * y;
    pthread_cond_broadcast(&cond);
    pthread_mutex_unlock(&mutex);
    return NULL;
}

void *thread_f(void *arg) {
    pthread_mutex_lock(&mutex);
    while (w == 0 || z == 0) pthread_cond_wait(&cond, &mutex);
    printf("Thread f was running!\n");
    z = w * z;
    pthread_cond_broadcast(&cond);
    pthread_mutex_unlock(&mutex);
    return NULL;
}

void *thread_g(void *arg) {
    pthread_mutex_lock(&mutex);
    while (z == 0 || y == 0) pthread_cond_wait(&cond, &mutex);
    printf("Thread g was running!\n");
    ans = y + z;
    pthread_mutex_unlock(&mutex);
    printf("ans = %d\n", ans);
    return NULL;
}

int main() {
    int i;
    pthread_t threads[NUM_THREADS];

    // Create threads
    pthread_create(&threads[0], NULL, thread_a, NULL);
    pthread_create(&threads[1], NULL, thread_b, NULL);
    pthread_create(&threads[2], NULL, thread_c, NULL);
    pthread_create(&threads[3], NULL, thread_d, NULL);
    pthread_create(&threads[4], NULL, thread_e, NULL);
    pthread_create(&threads[5], NULL, thread_f, NULL);
    pthread_create(&threads[6], NULL, thread_g, NULL);

    // Wait for threads to finish
    for (i = 0; i < NUM_THREADS; i++) {
        pthread_join(threads[i], NULL);
    }

    return 0;
}
