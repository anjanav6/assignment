

## Ring Buffer (Circular Buffer) Implementation

### Description

This project implements a fixed-size Ring Buffer (Circular Buffer) for `uint8_t` data in C.

The buffer supports:

* Initialization
* Write operation
* Read operation
* Full buffer detection
* Empty buffer detection
* Element count tracking
* Circular wrap-around behavior

### Buffer Size

```c
#define BUFFER_SIZE 8
```

### Features

* FIFO (First In First Out) behavior
* Fixed memory usage
* No dynamic memory allocation
* Error handling for full and empty conditions
* Demonstrates wrap-around functionality

### Files

* `ringbuf.c` - Ring Buffer implementation and demonstration program

### Build Instructions

```bash
gcc -Wall -std=c99 ringbuf.c -o ringbuf
```

### Run

```bash
./ringbuf
```

### Expected Demonstration

1. Write bytes `0x41` to `0x48`
2. Verify buffer becomes full
3. Attempt write of `0x99` (fails)
4. Read first three bytes
5. Write `0x49`, `0x4A`, `0x4B`
6. Read remaining bytes
7. Verify buffer becomes empty
8. Attempt read from empty buffer (fails)

### Concepts Demonstrated

* Circular Buffer / Ring Buffer
* FIFO Queue
* Structures
* Pointers
* Fixed-width integer types (`uint8_t`)
* Buffer management
* Embedded C programming fundamentals

### Author

Anjana V
