// Safe Gergis
// Dining Philosophers using monitors

#include <iostream>
#include <thread>
#include <mutex>
#include <condition_variable>
#include <vector>
#include <chrono>
#include <cstdlib>

#define NUM_PHILOSOPHERS 5

class DiningPhilosophers {
public:
    DiningPhilosophers() : state(NUM_PHILOSOPHERS, THINKING), chopsticks(NUM_PHILOSOPHERS, true) {}

    void wants_to_eat(int id) {
        std::unique_lock<std::mutex> lock(mtx);
        // Wait until both chopsticks are available
        cv.wait(lock, [&]() {
            return chopsticks[id] && chopsticks[(id + 1) % NUM_PHILOSOPHERS];
        });
        // Pick up chopsticks
        chopsticks[id] = false;
        chopsticks[(id + 1) % NUM_PHILOSOPHERS] = false;
        std::cout << "P#" << id << " picked up left chopstick.\n";
        std::cout << "P#" << id << " picked up right chopstick.\n";
        std::cout << "P#" << id << " EATING.\n";
    }

    void done_eating(int id) {
        std::unique_lock<std::mutex> lock(mtx);
        // Put down chopsticks
        chopsticks[id] = true;
        chopsticks[(id + 1) % NUM_PHILOSOPHERS] = true;
        std::cout << "P#" << id << " put down left chopstick.\n";
        std::cout << "P#" << id << " put down right chopstick.\n";
        std::cout << "P#" << id << " finished eating and is thinking again.\n";
        // Notify all waiting philosophers
        cv.notify_all();
    }

private:
    enum { THINKING, HUNGRY, EATING } ;
    std::vector<int> state;
    std::vector<bool> chopsticks;
    std::mutex mtx;
    std::condition_variable cv;
};

// Philosopher function
void philosopher(int id, DiningPhilosophers& dp) {
    while (true) {
        // Thinking
        std::cout << "P#" << id << " THINKING.\n";
        std::this_thread::sleep_for(std::chrono::milliseconds(rand() % 100));

        // Hungry and try to eat
        dp.wants_to_eat(id);

        // Eating
        std::this_thread::sleep_for(std::chrono::milliseconds(rand() % 100));

        // Done eating
        dp.done_eating(id);
    }
}

int main() {
    srand(time(NULL));
    DiningPhilosophers dp;
    std::vector<std::thread> philosophers_threads;

    // Create philosopher threads
    for (int i = 0; i < NUM_PHILOSOPHERS; i++) {
        philosophers_threads.emplace_back(philosopher, i, std::ref(dp));
    }

    // Join threads (this example runs indefinitely)
    for (auto& th : philosophers_threads) {
        th.join();
    }

    return 0;
}