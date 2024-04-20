#include "../h/printf.hpp"


void printf(char const *format, void *argument)
{

    if (format == 0)
    {
        char *str = ((char *)argument);
        while (*str != '\0')
        {
            __putc(*str);
            str++;
        }
        return;
    }

    while (*format != '\0')
    {
        if (*format == '%')
        {
            format++;

            switch (*format)
            {
            case 'd':
            {
                int num = *((int *)argument);
                if (num < 0)
                {
                    __putc('-');
                    num = -num;
                }
                int divisor = 1;
                while (num / divisor >= 10)
                {
                    divisor *= 10;
                }
                while (divisor > 0)
                {
                    __putc('0' + (num / divisor));
                    num %= divisor;
                    divisor /= 10;
                }
                break;
            }
            case 's':
            {
                char *str = ((char *)argument);
                while (*str != '\0')
                {
                    __putc(*str);
                    str++;
                }
                break;
            }
            case 'c':
            {
                char ch = *((char *)argument);
                __putc(ch);
                break;
            }
            case 'x':
            {
                unsigned int num = *((unsigned int *)argument);
                char buffer[20];
                int index = 0;

                if (num == 0)
                {
                    __putc('0');
                }
                else
                {
                    while (num != 0)
                    {
                        int remainder = num % 16;
                        buffer[index++] = (remainder < 10) ? ('0' + remainder) : ('a' + (remainder - 10));
                        num /= 16;
                    }
                    for (int i = index - 1; i >= 0; i--)
                    {
                        __putc(buffer[i]);
                    }
                }
                break;
            }
            case 'l':
            {
                long int num = *((long int *)argument);
                if (num < 0)
                {
                    __putc('-');
                    num = -num;
                }
                long int divisor = 1;
                while (num / divisor >= 10)
                {
                    divisor *= 10;
                }
                while (divisor > 0)
                {
                    __putc('0' + (num / divisor));
                    num %= divisor;
                    divisor /= 10;
                }
                break;
            }

            default:
                __putc('%');
                __putc(*format);
                break;
            }
        }
        else
        {
            __putc(*format);
        }
        format++;
    }
}