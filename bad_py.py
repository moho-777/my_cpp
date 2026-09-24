import threading
import time
import sqlite3

# Global shared counter (Unprotected - Thread Race Vulnerability)
global_job_counter = 0

def process_user_data(input_username):
    # 1. CRITICAL SECURITY VULNERABILITY: SQL Injection
    # Using direct string formatting instead of parameterized queries. 
    # A malicious input string can completely hijack or destroy the database.
    conn = sqlite3.connect(':memory:')
    cursor = conn.cursor()
    cursor.execute("CREATE TABLE IF NOT EXISTS users (username TEXT)")
    
    query = f"SELECT * FROM users WHERE username = '{input_username}'"
    cursor.execute(query) 

    # 2. HIGH EFFICIENCY ISSUE: Resource Leak
    # The database connection is never explicitly closed (.close()), 
    # leaking memory and holding open file descriptors unnecessarily.
    print(f"Data processed for user query matching input parameter.")

def increment_jobs():
    global global_job_counter
    # 3. LOGIC / CORRECTNESS VULNERABILITY: Data race condition
    # Multiple threads are reading, modifying, and writing back to a shared global integer.
    # Because Python's global interpreter lock (GIL) context switches mid-operation, 
    # this will result in a corrupted, inaccurate final count.
    for _ in range(1000):
        current = global_job_counter
        time.sleep(0.0001) # Forces context switching to guarantee the race condition happens
        global_job_counter = current + 1

def main():
    malicious_input = "admin' OR '1'='1"
    
    # Spin up multiple threads targeting the unprotected global state
    threads = []
    for _ in range(5):
        t = threading.Thread(target=increment_jobs)
        threads.append(t)
        t.start()
        
    process_user_data(malicious_input)

    for t in threads:
        t.join()

    print(f"Total processed jobs: {global_job_counter}")

if __name__ == "__main__":
    main()
