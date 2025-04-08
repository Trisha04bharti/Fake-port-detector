#include <iostream>
#include <pthread.h>
#include <semaphore.h>
#include <unistd.h>

using namespace std;

const int BUFFER_SIZE = 5; // Size of the shared buffer
int buffer[BUFFER_SIZE];    // Shared buffer
int in = 0, out = 0;        // Pointers to track buffer index

sem_t emptySlots; // Tracks empty slots in the buffer
sem_t fullSlots;  // Tracks filled slots in the buffer
pthread_mutex_t mutex; // Mutex to protect critical section

void* producer(void* arg) {
    int item = 1;
    while (true) {
        sem_wait(&emptySlots); // Wait for an empty slot
        pthread_mutex_lock(&mutex); // Lock the buffer

        // Produce an item
        buffer[in] = item;
        cout << "Producer produced: " << item << endl;
        in = (in + 1) % BUFFER_SIZE;
        item++; // Increment item number

        pthread_mutex_unlock(&mutex); // Unlock the buffer
        sem_post(&fullSlots); // Signal that a slot is filled

        sleep(1); // Simulate time taken to produce an item
    }
}

void* consumer(void* arg) {
    while (true) {
        sem_wait(&fullSlots); // Wait for a filled slot
        pthread_mutex_lock(&mutex); // Lock the buffer

        // Consume an item
        int item = buffer[out];
        cout << "Consumer consumed: " << item << endl;
        out = (out + 1) % BUFFER_SIZE;

        pthread_mutex_unlock(&mutex); // Unlock the buffer
        sem_post(&emptySlots); // Signal that a slot is emptied

        sleep(2); // Simulate time taken to consume an item
    }
}

int main() {
    pthread_t producerThread, consumerThread;

    // Initialize semaphores
    sem_init(&emptySlots, 0, BUFFER_SIZE); // Initially, all slots are empty
    sem_init(&fullSlots, 0, 0); // Initially, no slots are filled
    pthread_mutex_init(&mutex, NULL); // Initialize the mutex

    // Create producer and consumer threads
    pthread_create(&producerThread, NULL, producer, NULL);
    pthread_create(&consumerThread, NULL, consumer, NULL);

    // Join threads (though they run infinitely)
    pthread_join(producerThread, NULL);
    pthread_join(consumerThread, NULL);

    // Destroy semaphores and mutex (Not reached in this infinite loop example)
    sem_destroy(&emptySlots);
    sem_destroy(&fullSlots);
    pthread_mutex_destroy(&mutex);

    return 0;
}
