#include <mutex>
#include <thread>
#include <iostream>

std::mutex resourceX;
std::mutex resourceY;
 
void threadAFunc()
{
    unsigned counter = 0;
    while(true)
    {
        std::unique_lock<std::mutex> lockX(resourceX);
        std::this_thread::yield(); // encourage the livelock
 
        std::unique_lock<std::mutex> lockY(resourceY, std::defer_lock);
        if (not lockY.try_lock())
            continue;
 
        std::cout << "threadA working: " << ++counter << "\n";
        std::this_thread::sleep_for(std::chrono::milliseconds(50));
    }
}
 
void threadBFunc()
{
    unsigned counter = 0;
    while(true)
    {
        std::unique_lock<std::mutex> lockY(resourceY);
        std::this_thread::yield(); // encourage the livelock
 
        std::unique_lock<std::mutex> lockX(resourceX, std::defer_lock);
        if (not lockX.try_lock())
            continue;
 
        std::cout << "threadB working: " << ++counter << "\n";
        std::this_thread::sleep_for(std::chrono::milliseconds(50));
    }
}

int main() {
    std::thread tA(threadAFunc);
    std::thread tB(threadAFunc);
    tA.join();
    tB.join();
    return 0;
}