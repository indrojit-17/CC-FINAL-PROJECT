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
