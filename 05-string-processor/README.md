# 🔤 Project 05: Low-Level CLI String Processor

> A high-performance, zero-dependency Unix command-line text processing utility implemented in pure C (`C99`) purely on direct pointer arithmetic, two-pointer in-place algorithms, and finite state machines, **completely without standard `<string.h>` functions**.

---

## 🎯 Architecture & Design Philosophy

The primary objective of this project is to demystify string manipulation and work with memory at the lowest hardware level. In standard C development, `<string.h>` abstracts away the underlying byte traversals. Here, every byte is inspected, shifted, and tracked manually using pointer offsets and address arithmetic.

```
Pointer Arithmetic Model:
Memory:     [ 'H' ] [ 'e' ] [ 'l' ] [ 'l' ] [ 'o' ] [ '\0' ]
Address:     0x100   0x101   0x102   0x103   0x104   0x105
Pointers:      ^                                       ^
             left (start)                            right (end - 1)
```

---

## ⚡ Core Modules & Algorithms

### 1. `stringLength(const char *str)`
* **Technique:** Pointer subtraction (`p - str`).
* Traverses forward until hitting the null terminator `\0`. Returns the absolute distance in bytes between the end pointer and the base address. Time complexity: $O(N)$, Space: $O(1)$.

### 2. `reverseString(char *str)`
* **Technique:** Symmetrical Two-Pointer Swap (In-Place).
* Places `left` at the start and `right` at `str + length - 1`. Swaps values in place and advances pointers towards each other until `left >= right`. Zero heap allocations.

### 3. `trimString(char *str)`
* **Technique:** Two-Stage In-Place Compaction.
* **Stage 1 (Leading Spaces):** Advances `src` past all leading spaces, then streams bytes to `dst` in place, appending a clean `\0`.
* **Stage 2 (Trailing Spaces):** Steps `end` backwards with short-circuit evaluation:
  ```c
  while (end >= str && *end == ' ') {
      *end = '\0';
      end--;
  }
  ```
  The guard `end >= str` prevents stack buffer underflows on strings consisting entirely of spaces.

### 4. `findSubstring(const char *haystack, const char *needle)`
* **Technique:** Dual-Scout Pattern Matching (Manual `strstr`).
* For every candidate character in `haystack`, deploys a temporary scout pointer `h_scout` alongside `n`. If the full pattern matches (`*n == '\0'`), returns the match address. Calculating the match index is achieved via pointer difference: `result - argv[2]`.

### 5. `countStats(const char *str, size_t *chars, size_t *words, size_t *lines)`
* **Technique:** Single-Pass Finite State Machine (FSM).
* Uses an `inWord` state flag. Transition from whitespace/newline (`' '`, `'\t'`, `'\n'`) to printable character triggers word increment. Modifies caller variables via pointer dereferencing (`size_t *`).

### 6. `stringEquals(const char *s1, const char *s2)`
* **Technique:** Strict Character Walk Comparison (Manual `strcmp`).
* Validates matching character streams until reaching `\0` simultaneously on both operands.

---

## 🚀 CLI Usage & Flags

Built as a true POSIX-style command-line utility with argument count (`argc`) and vector (`argv`) validation:

```bash
# Compile with Makefile
make

# Measure length
./main --len "Hello, world!"
# Output: Длина строки: 13

# In-place string reversal
./main --reverse "Antigravity Engine"
# Output: Перевернутая строка: enignE ytivargitnA

# Strip leading and trailing whitespace
./main --trim "   Clean code is clean art   "
# Output: Очищенная строка: Clean code is clean art

# Search substring index
./main --find "The quick brown fox" "brown"
# Output: Первое вхождение шаблона на индексе: 10

# Single-pass text analysis (chars, words, lines)
./main --stats "First line of text
Second line with more words
Final line."
```

---

## 🛠️ Build & Verification

Compiled with strict pedantic compiler flags:
```bash
gcc -Wall -Wextra -Werror main.c -o main
```

Clean build artifacts:
```bash
make clean
```
