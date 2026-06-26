# CSC 308 – Operating Systems | Week 8: Synchronization & IPC

## Overview
Practical implementations covering mutex locks, semaphores, and shared memory IPC in C on Linux.

## Sessions

| Session | Topic | File |
|---------|-------|------|
| 1 | Mutex Lock Demonstration | session1_mutex/mutex_demo.c |
| 2 | Producer-Consumer Simulation | session2_producer_consumer/producer_consumer.c |
| 3 | Semaphore Implementation in C | session3_semaphores/semaphore_compare.c |
| 4 | Shared Memory Programming | session4_shared_memory/writer.c + reader.c |

## How to Compile & Run

### Session 1 – Mutex
```bash
gcc -o mutex_demo session1_mutex/mutex_demo.c -lpthread
./mutex_demo
```

### Session 2 – Producer-Consumer
```bash
gcc -o prod_cons session2_producer_consumer/producer_consumer.c -lpthread
./prod_cons
```

### Session 3 – Semaphores
```bash
gcc -o sem_compare session3_semaphores/semaphore_compare.c -lpthread
./sem_compare
```

### Session 4 – Shared Memory (run writer first, then reader in a second terminal)
```bash
gcc -o writer session4_shared_memory/writer.c
gcc -o reader session4_shared_memory/reader.c
./writer        # Terminal 1
./reader        # Terminal 2
```

## Requirements
- Linux OS (Ubuntu recommended)
- GCC compiler
- POSIX threads (`-lpthread` flag)

## Course
CSC 308 – Operating Systems | Week 8
