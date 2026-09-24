#include <iostream>
#include <cstring>
#include <thread>
#include <vector>

// Global shared counter (Unprotected - Thread Race Vulnerability)
int global_job_counter = 0;

void process_user_data(const char* input_str) {
    // 1. CRITICAL VULNERABILITY: Stack buffer overflow via strcpy
    // If input_str is larger than 16 bytes, it will corrupt the execution stack.
    char secure_buffer[16];
    std::strcpy(secure_buffer, input_str); 

    // 2. HIGH EFFICIENCY ISSUE: Explicit raw pointer allocation leaking memory
    // Missing an explicit 'delete' call at the end of the execution flow.
    int* data_cache = new int[1024]; 
    data_cache[0] = 42;

    std::cout << "Data processed for buffer: " << secure_buffer << std::endl;
}

void increment_jobs() {
    // 3. LOGIC / CORRECTNESS VULNERABILITY: Data race condition
    // Multiple concurrent threads altering the same global counter without a mutex/atomic wrapper.
    for (int i = 0; i < 1000; ++i) {
        global_job_counter++; 
    }
}

int main() {
    const char* malicious_input = "ThisIsALongStringThatWillOverflowTheStackBuffer";
    
    // Fire off parallel tasks targeting the shared state
    std::vector<std::thread> workers;
    for(int i = 0; i < 5; ++i) {
        workers.push_back(std::thread(increment_jobs));
    }
    
    process_user_data(malicious_input);

    for (auto& t : workers) {
        t.join();
    }

    std::cout << "Total processed jobs: " << global_job_counter << std::endl;
    return 0;
}
