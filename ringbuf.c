
#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>

#define BUFFER_SIZE 8U

/* Ring Buffer Status Codes */
typedef enum
{
    RB_SUCCESS = 0,
    RB_FAILURE = -1

} RB_Status_t;

/* Ring Buffer Structure */
typedef struct
{
    uint8_t buffer[BUFFER_SIZE];
    uint8_t head;
    uint8_t tail;
    uint8_t count;

} RingBuffer;

/* Ring Buffer API Prototypes */
void rb_init(RingBuffer *rb);
RB_Status_t rb_write(RingBuffer *rb, uint8_t data);
RB_Status_t rb_read(RingBuffer *rb, uint8_t *data);
bool rb_is_full(RingBuffer *rb);
bool rb_is_empty(RingBuffer *rb);
uint8_t rb_get_count(RingBuffer *rb);

/* Initialize buffer to empty state */
void rb_init(RingBuffer *rb)
{
    rb->head = 0U;
    rb->tail = 0U;
    rb->count = 0U;
}

/* Check whether buffer is empty */
bool rb_is_empty(RingBuffer *rb)
{
    return (rb->count == 0U);
}

/* Check whether buffer is full */
bool rb_is_full(RingBuffer *rb)
{
    return (rb->count == BUFFER_SIZE);
}

/* Return current number of stored bytes */
uint8_t rb_get_count(RingBuffer *rb)
{
    return rb->count;
}

/* Write one byte into the ring buffer */
RB_Status_t rb_write(RingBuffer *rb, uint8_t data)
{
    if (rb_is_full(rb))
    {
        return RB_FAILURE;
    }

    rb->buffer[rb->head] = data;

    /* BUFFER_SIZE is 8 (power of 2).
     * Using modulo (%) wraps the index to 0 when
     * the end of the buffer is reached.
     *
     * Bonus optimization:
     * rb->head = (rb->head + 1U) & (BUFFER_SIZE - 1U);
     *
     * Bitwise AND is faster than modulo on MCUs
     * without a hardware divider.
     */
    rb->head = (rb->head + 1U) % BUFFER_SIZE;

    rb->count++;

    return RB_SUCCESS;
}

/* Read one byte from the ring buffer */
RB_Status_t rb_read(RingBuffer *rb, uint8_t *data)
{
    if (rb_is_empty(rb))
    {
        return RB_FAILURE;
    }

    *data = rb->buffer[rb->tail];

    /* BUFFER_SIZE is 8 (power of 2).
     * Using modulo (%) wraps the index to 0 when
     * the end of the buffer is reached.
     *
     * Bonus optimization:
     * rb->tail = (rb->tail + 1U) & (BUFFER_SIZE - 1U);
     */
    rb->tail = (rb->tail + 1U) % BUFFER_SIZE;

    rb->count--;

    return RB_SUCCESS;
}

int main(void)
{
    RingBuffer rb;
    uint8_t value;

    rb_init(&rb);

    /* Fill the buffer with 8 bytes (0x41 - 0x48) */
    for (uint8_t data = 0x41U; data <= 0x48U; data++)
    {
        if (rb_write(&rb, data) == RB_SUCCESS)
        {
            printf("[WRITE] 0x%02X -> OK (count=%u)",
                   data,
                   rb_get_count(&rb));

            if (rb_is_full(&rb))
            {
                printf(" FULL");
            }

            printf("\n");
        }
    }

    /* Verify write failure when buffer is full */
    if (rb_write(&rb, 0x99U) == RB_FAILURE)
    {
        printf("[WRITE] 0x99 -> FAIL (buffer full)\n");
    }

    /* Read first three bytes from the buffer */
    for (uint8_t i = 0U; i < 3U; i++)
    {
        if (rb_read(&rb, &value) == RB_SUCCESS)
        {
            printf("[READ] -> 0x%02X (count=%u)\n",
                   value,
                   rb_get_count(&rb));
        }
    }

    /* Reuse freed locations to demonstrate wrap-around */
    uint8_t new_data[] = {0x49U, 0x4AU, 0x4BU};

    for (uint8_t i = 0U; i < 3U; i++)
    {
        if (rb_write(&rb, new_data[i]) == RB_SUCCESS)
        {
            printf("[WRITE] 0x%02X -> OK (count=%u)\n",
                   new_data[i],
                   rb_get_count(&rb));
        }
    }

    /* Drain the buffer completely */
    while (!rb_is_empty(&rb))
    {
        (void)rb_read(&rb, &value);

        printf("[READ] -> 0x%02X (count=%u)\n",
               value,
               rb_get_count(&rb));
    }

    /* Verify read failure when buffer is empty */
    if (rb_read(&rb, &value) == RB_FAILURE)
    {
        printf("[READ] (empty) -> FAIL (buffer empty)\n");
    }

    return 0;
}

