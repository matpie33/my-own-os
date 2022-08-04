#include <string.h>
#include <kernel/string_utils.h>
#include <stdint.h>

void join(char *string1, char *string2, char buffer[])
{
    size_t size1 = strlen(string1);
    size_t size2 = strlen(string2);

    for (uint32_t i = 0; i < size1; i++)
    {
        buffer[i] = string1[i];
    }
    for (uint32_t i = 0; i < size2; i++)
    {
        buffer[size1 + i] = string2[i];
    }
    buffer[size1 + size2] = '\0';
}

char *int_to_string(long long integer_to_convert, char buffer[])
{
    char const digit[] = "0123456789";
    char *p = buffer;
    if (integer_to_convert < 0)
    {
        *p++ = '-';
        integer_to_convert *= -1;
    }
    long long shifter = integer_to_convert;
    do
    {
        ++p;
        shifter = shifter / 10;
    } while (shifter);
    *p = '\0';
    do
    {
        *--p = digit[integer_to_convert % 10];
        integer_to_convert = integer_to_convert / 10;
    } while (integer_to_convert);
    return buffer;
}