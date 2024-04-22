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
