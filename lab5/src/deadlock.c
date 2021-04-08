#include <errno.h>
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>

void thing_A(int *);
void thing_B(int *);
void do_wrap_up(int);
int common = 0; /* A shared variable for two threads */
int r1 = 0, r2 = 0, r3 = 0;
pthread_mutex_t mut1, mut2 = PTHREAD_MUTEX_INITIALIZER;

int main() {
  pthread_t thread1, thread2;

  if (pthread_create(&thread1, NULL, (void *)thing_A,
			  (void *)&common) != 0) {
    perror("pthread_create");
    exit(1);
  }

  if (pthread_create(&thread2, NULL, (void *)thing_B,
                     (void *)&common) != 0) {
    perror("pthread_create");
    exit(1);
  }

  if (pthread_join(thread1, NULL) != 0) {
    perror("pthread_join");
    exit(1);
  }

  if (pthread_join(thread2, NULL) != 0) {
    perror("pthread_join");
    exit(1);
  }

  do_wrap_up(common);

  return 0;
}

void thing_A(int *pnum_times) {
  int i, j, x;
  unsigned long k;
  int work;
  for (i = 0; i < 50; i++) {
    pthread_mutex_lock(&mut1);
        printf("doing one thing\n");
        work = *pnum_times;
        printf("counter = %d\n", work);
        work++;
        for (k = 0; k < 500000; k++)
        ;
        pthread_mutex_lock(&mut2);
        *pnum_times = work;
        pthread_mutex_unlock(&mut2);
	pthread_mutex_unlock(&mut1);
  }
}

void thing_B(int *pnum_times) {
  int i, j, x;
  unsigned long k;
  int work;
  for (i = 0; i < 50; i++) {
    pthread_mutex_lock(&mut2);
        printf("doing another thing\n");
        work = *pnum_times;
        printf("counter = %d\n", work);
        work++;
        for (k = 0; k < 500000; k++)
        ;
        pthread_mutex_lock(&mut1);
        *pnum_times = work;
        pthread_mutex_unlock(&mut1);
    pthread_mutex_unlock(&mut2);
  }
}

void do_wrap_up(int counter) {
  int total;
  printf("All done, counter = %d\n", counter);
}