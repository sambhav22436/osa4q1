#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#define NUM_PHILOSOPHERS 5

pthread_mutex_t forks[NUM_PHILOSOPHERS];
pthread_mutex_t bowls[2];
pthread_cond_t cond[NUM_PHILOSOPHERS];

void eating(int philosopher_id) {
    printf("Philosopher %d is eating\n", philosopher_id);
    // Simulate eating by sleeping for a random time
    sleep(rand() % 3 + 1);
}

void thinking(int philosopher_id) {
    printf("Philosopher %d is thinking\n", philosopher_id);
    // Simulate thinking by sleeping for a random time
    sleep(rand() % 3 + 1);
}

void* philosopher(void* arg) {
    int philosopher_id = *(int*)arg;

    while (1) {
        thinking(philosopher_id);

        pthread_mutex_lock(&forks[philosopher_id]);
        pthread_mutex_lock(&forks[(philosopher_id + 1) % NUM_PHILOSOPHERS]);
        pthread_mutex_lock(&bowls[0]);
        pthread_mutex_lock(&bowls[1]);

        eating(philosopher_id);

        pthread_mutex_unlock(&forks[philosopher_id]);
        pthread_mutex_unlock(&forks[(philosopher_id + 1) % NUM_PHILOSOPHERS]);
        pthread_mutex_unlock(&bowls[0]);
        pthread_mutex_unlock(&bowls[1]);

        // Sleep before the next iteration to allow other philosophers to access the forks
        sleep(rand() % 2);
    }

    pthread_exit(NULL);
}

int main() {
    pthread_t philosophers[NUM_PHILOSOPHERS];
    int philosopher_ids[NUM_PHILOSOPHERS];

    // Initialize mutexes and condition variables
    for (int i = 0; i < NUM_PHILOSOPHERS; ++i) {
        pthread_mutex_init(&forks[i], NULL);
        pthread_cond_init(&cond[i], NULL);
    }
    for (int i = 0; i < 2; ++i) {
        pthread_mutex_init(&bowls[i], NULL);
    }

    // Create philosopher threads
    for (int i = 0; i < NUM_PHILOSOPHERS; ++i) {
        philosopher_ids[i] = i;
        pthread_create(&philosophers[i], NULL, philosopher, &philosopher_ids[i]);
    }

    // Join philosopher threads
    for (int i = 0; i < NUM_PHILOSOPHERS; ++i) {
        pthread_join(philosophers[i], NULL);
    }

    // Destroy mutexes and condition variables
    for (int i = 0; i < NUM_PHILOSOPHERS; ++i) {
        pthread_mutex_destroy(&forks[i]);
        pthread_cond_destroy(&cond[i]);
    }
    for (int i = 0; i < 2; ++i) {
        pthread_mutex_destroy(&bowls[i]);
    }

    return 0;
}
