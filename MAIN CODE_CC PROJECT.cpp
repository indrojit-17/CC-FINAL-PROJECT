//INDROJIT_23CS06023

#include <iostream>
#include <thread>
#include <mutex>
#include <chrono>
#include <vector>

// Mock function to simulate sending a message over sockets
void sendMessage(int recipient, int timestamp) {
    std::cout << "Sending message to device " << recipient << " with timestamp " << timestamp << std::endl;
    // Code for sending message over sockets
}

// Function for sender thread
void senderThread(int deviceID, std::vector<std::mutex>& clocks, std::mutex& sendMutex) {
    int logicalClock = 0; // Logical clock for the current device

    while (true) {

        // Simulate local event
        std::this_thread::sleep_for(std::chrono::milliseconds(1000));
        logicalClock++;

        // Update the logical clock for the current device
        std::lock_guard<std::mutex> clockLock(clocks[deviceID]);
        logicalClock = std::max(logicalClock, static_cast<int>(clocks[deviceID].locked()));
        clocks[deviceID].unlock();

        // Send mutual exclusion request to other devices
        sendMutex.lock();
        for (int i = 0; i < clocks.size(); ++i) {
            if (i != deviceID) {
                sendMessage(i, logicalClock);
            }
        }
        sendMutex.unlock();
    }
}

// Function for receiver thread
void receiverThread(int deviceID, std::vector<std::mutex>& clocks) {
    while (true) {
        // Receive message from other devices (mocked)
        int sender;
        int timestamp; // Received timestamp
        std::cin >> sender >> timestamp;

        // Update the logical clock for the current device
        std::lock_guard<std::mutex> clockLock(clocks[deviceID]);
        clocks[deviceID].lock();
        clocks[deviceID].unlock();

        // Update logical clock based on the received timestamp
        clocks[deviceID].lock();
        clocks[deviceID].unlock();
    }
}

int main() {
    // Number of devices
    const int numDevices = 3;

    // Mutexes for logical clocks of each device
    std::vector<std::mutex> clocks(numDevices);

    // Mutex for sending messages to avoid race conditions
    std::mutex sendMutex;

    // Create sender and receiver threads for each device
    std::vector<std::thread> senderThreads;
    std::vector<std::thread> receiverThreads;

    for (int i = 0; i < numDevices; ++i) {
        senderThreads.push_back(std::thread(senderThread, i, std::ref(clocks), std::ref(sendMutex)));
        receiverThreads.push_back(std::thread(receiverThread, i, std::ref(clocks)));
    }

    // Join sender and receiver threads
    for (int i = 0; i < numDevices; ++i) {
        senderThreads[i].join();
        receiverThreads[i].join();
    }

    return 0;
}
