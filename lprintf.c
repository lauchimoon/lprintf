// Copyright (c) 2024 Laurentino Luna <laurentino.luna06@gmail.com>

// MIT License
// 
// Copyright (c) 2024 Laurentino Luna
// 
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:
// 
// The above copyright notice and this permission notice shall be included in all
// copies or substantial portions of the Software.
// 
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
// SOFTWARE.

#include <stdarg.h>
#include <unistd.h>
#include <string.h>

#define MAX_BUF 2048

ssize_t lputc(char c);
ssize_t lputbuf(char *s, size_t n);

char lprintf_buffer[MAX_BUF] = { 0 };
int lprintf_idx = 0;

int lprintf(const char *fmt, ...)
{
#define STDOUT 1
    size_t fmt_size = strlen(fmt);
    ssize_t written_size = 0;

    va_list ap;
    va_start(ap, fmt);

    for (int i = 0; i < fmt_size; i++) {
        if (fmt[i] == '%') {
            i++;
            switch (fmt[i]) {
                // %
                case '%':
                    written_size += lputc('%');
                    break;
                // Signed integer
                case 'd':
                case 'i':
#define NUM_BUFFER 256
                    // Using Apple's method to print integers.
                    // lauchimoon: It's a bit modified; a bit more understandable for my peanut brain...
                    // https://opensource.apple.com/source/xnu/xnu-201/osfmk/kern/printf.c.auto.html
                    int n = va_arg(ap, int);
                    char buffer[NUM_BUFFER] = { 0 };

                    int i = NUM_BUFFER - 1;

                    do {
                        buffer[i] = n%10 + '0';
                        i--;
                        n /= 10;
                    } while (n != 0);

                    for (int j = i; j < NUM_BUFFER; j++)
                        written_size += lputc(buffer[j]);

                    break;
                // Unsigned integer
                case 'u':
                    unsigned int u = va_arg(ap, unsigned int);
                    break;
                // Signed double
                case 'f':
                case 'g':
                    double f = va_arg(ap, double);
                    break;
                // Character
                case 'c':
                    char c = (char)va_arg(ap, int);
                    written_size += lputc(c);
                    break;
                // String
                case 's':
                    char *s = va_arg(ap, char *);
                    size_t slen = strlen(s);
                    written_size += lputbuf(s, slen);

                    break;
                // Octal representation
                case 'o':
                    break;
                // Hex representation, lowercase
                case 'x':
                    break;
                // Hex representation, uppercase
                case 'X':
                    break;
                // Binary representation
                case 'b':
                    break;
                default:
                    break;
            }
        } else {
            written_size += lputc(fmt[i]);
        }
    }

    va_end(ap);
    return written_size;
}

inline ssize_t lputc(char c)
{
    lprintf_buffer[lprintf_idx] = c;
    lprintf_idx++;

    if (lprintf_idx == MAX_BUF - 1 || c == '\n') {
        lprintf_idx = 0;
        return write(STDOUT, lprintf_buffer, MAX_BUF);
    }

    return sizeof(char);
}

ssize_t lputbuf(char *s, size_t n)
{
    write(STDOUT, lprintf_buffer, lprintf_idx);
    lprintf_idx = 0;
    return write(STDOUT, s, n);
}
