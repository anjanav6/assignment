#include <stdio.h>
#include <stdint.h>

#define BUFFER_SIZE 8
#define SUCCESS 0
#define FAILURE -1

typedef struct
{
    uint8_t buffer[BUFFER_SIZE];
    uint8_t head;
    uint8_t tail;
    uint8_t count;

} RingBuffer;

/* Function Prototypes */
void rb_init(RingBuffer *rb);
int rb_write(RingBuffer *rb, uint8_t data);
int rb_read(RingBuffer *rb, uint8_t *data);
int rb_is_full(RingBuffer *rb);
int rb_is_empty(RingBuffer *rb);
uint8_t rb_get_count(RingBuffer *rb);

/* Function Definitions */

void rb_init(RingBuffer *rb)
{
    rb->head = 0;
    rb->tail = 0;
    rb->count = 0;
}

int rb_is_empty(RingBuffer *rb)
{
    return (rb->count == 0);
}

int rb_is_full(RingBuffer *rb)
{
    return (rb->count == BUFFER_SIZE);
}

uint8_t rb_get_count(RingBuffer *rb)
{
    return rb->count;
}

int rb_write(RingBuffer *rb, uint8_t data)
{
    if(rb_is_full(rb))
    {
        return FAILURE;
    }

    rb->buffer[rb->head] = data;

    rb->head = (rb->head + 1) % BUFFER_SIZE;

    rb->count++;

    return SUCCESS;
}

int rb_read(RingBuffer *rb, uint8_t *data)
{
    if(rb_is_empty(rb))
    {
        return FAILURE;
    }

    *data = rb->buffer[rb->tail];

    rb->tail = (rb->tail + 1) % BUFFER_SIZE;

    rb->count--;

    return SUCCESS;
}

int main()
{
    RingBuffer rb;
    uint8_t value;

    rb_init(&rb);

    /* Step 1: Write 0x41 - 0x48 */

    for(uint8_t data = 0x41; data <= 0x48; data++)
    {
        if(rb_write(&rb, data) == SUCCESS)
        {
            printf("[WRITE] 0x%02X -> OK (count=%u)",
                   data,
                   rb_get_count(&rb));

            if(rb_is_full(&rb))
            {
                printf(" FULL");
            }

            printf("\n");
        }
    }

    /* Step 2: Attempt extra write */

    if(rb_write(&rb, 0x99) == FAILURE)
    {
        printf("[WRITE] 0x99 -> FAIL (buffer full)\n");
    }

    /* Step 3: Read 3 bytes */

    for(int i = 0; i < 3; i++)
    {
        if(rb_read(&rb, &value) == SUCCESS)
        {
            printf("[READ] -> 0x%02X (count=%u)\n",
                   value,
                   rb_get_count(&rb));
        }
    }

    /* Step 4: Write 0x49, 0x4A, 0x4B */

    uint8_t new_data[] = {0x49, 0x4A, 0x4B};

    for(int i = 0; i < 3; i++)
    {
        if(rb_write(&rb, new_data[i]) == SUCCESS)
        {
            printf("[WRITE] 0x%02X -> OK (count=%u)\n",
                   new_data[i],
                   rb_get_count(&rb));
        }
    }

    /* Step 5: Read all remaining bytes */

    while(!rb_is_empty(&rb))
    {
        rb_read(&rb, &value);

        printf("[READ] -> 0x%02X (count=%u)\n",
               value,
               rb_get_count(&rb));
    }

    /* Step 6: Attempt read from empty buffer */

    if(rb_read(&rb, &value) == FAILURE)
    {
        printf("[READ] (empty) -> FAIL (buffer empty)\n");
    }

    return 0;
}
