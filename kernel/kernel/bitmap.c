
#include <stdint.h>
#include <stdbool.h>
#include <kernel/bitmap.h>

extern inline void bitmap_set_bit(int bit_number, uint32_t *bitmap)
{
    bitmap[bit_number / 32] |= (1 << (bit_number % 32));
}

extern inline void bitmap_unset_bit(int bit_number, uint32_t *bitmap)
{
    bitmap[bit_number / 32] &= ~(1 << (bit_number % 32));
}

extern inline bool bitmap_is_bit_set(int bit_number, uint32_t *bitmap)
{

    return bitmap[bit_number / 32] & (1 << (bit_number % 32));
}

extern inline bool is_bit_set_in_array_element(uint32_t bit_index, uint32_t array_index, uint32_t *bitmap)
{
    return bitmap[array_index] & bit_index;
}

int32_t get_first_free_block_index(uint32_t *bitmap, uint32_t total_blocks)
{

    for (uint32_t i = 0; i < total_blocks / BLOCKS_PER_INT; i++)
        if (bitmap[i] != ALL_BITS_IN_INT_ARE_TAKEN)
        {
            for (int32_t j = 0; j < BLOCKS_PER_INT; j++)
            {

                int32_t bit = 1 << j;
                if (!is_bit_set_in_array_element(bit, i, bitmap))
                    return i * BLOCKS_PER_INT + j;
            }
        }

    return -1;
}

int32_t get_multiple_contiguous_free_blocks(uint32_t number_of_blocks, uint32_t total_blocks, uint32_t *bitmap)
{

    int32_t starting_index = -1;
    int32_t last_index_found = -1;
    int32_t blocks_left_to_find = number_of_blocks;
    for (uint32_t i = 0; i < total_blocks / BLOCKS_PER_INT; i++)
        if (bitmap[i] != ALL_BITS_IN_INT_ARE_TAKEN)
        {
            for (int32_t j = 0; j < BLOCKS_PER_INT; j++)
            {

                int32_t bit = 1 << j;
                int32_t current_frame = i * BLOCKS_PER_INT + j;
                if (!is_bit_set_in_array_element(bit, i, bitmap))
                {
                    if (starting_index == -1 || last_index_found + 1 != current_frame)
                    {
                        starting_index = current_frame;
                        blocks_left_to_find = number_of_blocks;
                    }
                    blocks_left_to_find--;
                    last_index_found = current_frame;
                    if (blocks_left_to_find == 0)
                    {
                        return starting_index;
                    }
                }
            }
        }

    return -1;
}