// cap03/02-fread-parser.cpp
// Custom integer parser using fread — fastest stdin reading in contest code
// Reads all of stdin in one syscall, then parses integers from the buffer.
// Compile (LLVM): clang++-23 -std=c++23 -stdlib=libc++ -O2 -Wall -Wextra -o fread_parser 02-fread-parser.cpp
// Compile (MSVC): cl /std:c++latest /O2 /EHsc /W4 /permissive- /Zc:__cplusplus /Fefread_parser.exe 02-fread-parser.cpp
// Test:    echo "10 1 2 3 4 5 6 7 8 9 10" | ./fread_parser

#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <print>

// ---- Global read buffer ----
// 1 << 22 = 4 MB: fits most competitive programming input files.
// Increase to 1 << 25 (32 MB) if the problem allows up to 10^7 integers.
static constexpr int BUF_SIZE = 1 << 22;  // 4 MB
static char buf[BUF_SIZE];
static int  buf_pos = 0;
static int  buf_len = 0;

// Load all of stdin into buf in one fread call. O(n) where n = input size in bytes.
void load_input() {
    buf_len = static_cast<int>(fread(buf, 1, BUF_SIZE, stdin));
    buf_pos = 0;
}

// Read one non-negative integer from the buffer. O(digits).
// Skips whitespace (space, '\n', '\r', '\t') before the number.
// Returns 0 and leaves buf_pos unchanged if no more integers remain.
int read_int() {
    while (buf_pos < buf_len && (buf[buf_pos] < '0' || buf[buf_pos] > '9'))
        ++buf_pos;                      // skip whitespace
    int val = 0;
    while (buf_pos < buf_len && buf[buf_pos] >= '0' && buf[buf_pos] <= '9')
        val = val * 10 + (buf[buf_pos++] - '0');
    return val;
}

// Read one signed integer from the buffer. O(digits).
int read_int_signed() {
    while (buf_pos < buf_len &&
           buf[buf_pos] != '-' && (buf[buf_pos] < '0' || buf[buf_pos] > '9'))
        ++buf_pos;                      // skip whitespace
    int sign = 1;
    if (buf_pos < buf_len && buf[buf_pos] == '-') { sign = -1; ++buf_pos; }
    int val = 0;
    while (buf_pos < buf_len && buf[buf_pos] >= '0' && buf[buf_pos] <= '9')
        val = val * 10 + (buf[buf_pos++] - '0');
    return sign * val;
}

// ---- Output: putchar_unlocked is faster than printf/cout for single chars ----
// putchar_unlocked skips the per-call mutex that putchar acquires.
// Use only in single-threaded programs (all competitive programs are single-threaded).

static char out_buf[BUF_SIZE];
static int  out_pos = 0;

void write_char(char c) {
    out_buf[out_pos++] = c;
    if (out_pos == BUF_SIZE) {          // flush when buffer is full
        fwrite(out_buf, 1, out_pos, stdout);
        out_pos = 0;
    }
}

void write_int(long long val) {
    if (val < 0) { write_char('-'); val = -val; }
    if (val == 0) { write_char('0'); return; }
    char tmp[20];
    int  len = 0;
    while (val > 0) { tmp[len++] = static_cast<char>('0' + val % 10); val /= 10; }
    for (int i = len - 1; i >= 0; --i) write_char(tmp[i]);
}

void flush_output() {
    if (out_pos > 0) {
        fwrite(out_buf, 1, out_pos, stdout);
        out_pos = 0;
    }
}

// ---- Example: read n integers and output their sum ----
int main() {
    load_input();                        // one syscall reads all stdin

    int n = read_int();
    long long sum = 0;
    for (int i = 0; i < n; ++i)
        sum += read_int_signed();

    write_int(sum);
    write_char('\n');
    flush_output();
    return 0;
}
