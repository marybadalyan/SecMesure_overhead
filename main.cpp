#include <iostream>
#include <chrono>

void tightLoop() {
    for (int i = 0; i < 100000; ++i) {
        char buffer[100];  // Stack allocation
        strcpy(buffer, "This can overflow");  // Risky
    }
}

void dummy() {
    volatile int x = 0;  // prevent optimization
    x++;
}

void callDummy() {
    for (int i = 0; i < 100000000; ++i) {
        dummy();
    }
}

void warmup() {
    for (int i = 0; i < 100000; ++i) {
        dummy(); // Warm up the CPU cache
    }
}

int main() {
    warmup();
    std::chrono::high_resolution_clock::time_point start = std::chrono::high_resolution_clock::now();
    tightLoop(); // Call the tight loop to measure its overhead
    std::cout << "Tight loop executed." << std::endl;
    std::chrono::high_resolution_clock::time_point end = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double, std::milli> duration = end - start;
    std::cout << "Execution time: " << duration.count() << " ms" << std::endl;
    start = std::chrono::high_resolution_clock::now();

    callDummy();
    end = std::chrono::high_resolution_clock::now();
    duration = end - start;
    std::cout << "Execution time: " << duration.count() << " ms" << std::endl;
    return 0;
}
