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
