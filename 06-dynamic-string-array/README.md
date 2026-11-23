# 📚 Project 06: Dynamic 2D String Array (`char **`)

> High-performance dynamic string vector implemented in pure C (`C99`) demonstrating true two-level dynamic heap allocation, geometric capacity growth, $O(1)$ in-place pointer swapping, memory shift compaction, and leak-free two-stage teardown.

---

## 🎯 Architecture & Memory Layout

In C, working with text dynamically requires managing two distinct memory levels on the heap:
1. **The Index Array (The Shelf):** A contiguous buffer of pointers (`char **lines`), where each 8-byte cell stores the memory address of an independent string.
2. **The String Buffers:** Individual heap allocations (`char *newLine`), sized precisely to `strlen(str) + 1` to accommodate characters and the mandatory null-terminator `\0`.

```text
Stack (StringArray)           Heap (Level 1: Pointers)              Heap (Level 2: Characters)
+------------------+          +------------------------+
| lines: char **   | -------> | lines[0]: char * (8 B) | ---------> [ 'A' | 'l' | 'p' | 'h' | 'a' | '\0' ]
| count: 5         |          | lines[1]: char * (8 B) | ---------> [ 'B' | 'r' | 'a' | 'v' | 'o' | '\0' ]
| capacity: 8      |          | lines[2]: char * (8 B) | ---------> [ 'C' | 'h' | 'a' | 'r' | 'l' | 'i' | 'e' | '\0' ]
+------------------+          | lines[3]: char * (8 B) | ---------> [ 'D' | 'e' | 'l' | 't' | 'a' | '\0' ]
                              | lines[4]: char * (8 B) | ---------> [ 'E' | 'c' | 'h' | 'o' | '\0' ]
                              | lines[5..7]: Unused    |
                              +------------------------+
```

---

## ⚡ Core Modules & Algorithmic Design

### 1. `initArray(StringArray *arr)`
* **Memory Strategy:** Preallocates an initial pointer capacity (`capacity = 4`), requiring $4 \times \text{sizeof(char *)} = 32\text{ bytes}$.
* **Safety:** Verifies allocation return code; initializes `count = 0`. Time complexity: $O(1)$.

### 2. `appendLine(StringArray *arr, const char *str)`
* **Geometric Reallocation:** When `count == capacity`, doubles capacity (`capacity * 2`) using `realloc` with an isolated temporary pointer `char **temp`. This guarantees that if memory reallocation fails, the existing array is not leaked or corrupted.
* **Exact Allocation:** Computes length via `strlen(str)` and allocates exactly `strLength + 1` bytes. Copies characters including `\0` via a single pass `for (size_t i = 0; i <= strLength; i++)`.
* **Complexity:** Amortized $O(1)$ time per append.

### 3. `sortLines(StringArray *arr)`
* **Technique:** $O(1)$ In-Place Pointer Swapping (Bubble Sort).
* **Zero Character Copying:** Instead of moving kilobytes of characters using `strcpy`, sorting only swaps 64-bit address pointers in `arr->lines`:
  ```c
  if (strcmp(arr->lines[j], arr->lines[j + 1]) > 0) {
      char *temp = arr->lines[j + 1];
      arr->lines[j + 1] = arr->lines[j];
      arr->lines[j] = temp;
  }
  ```
* **Guard Clause:** Includes `if (arr->count < 2) return;` to prevent unsigned `size_t` underflow when sorting empty or single-element arrays.

### 4. `removeLine(StringArray *arr, size_t index)`
* **Bounds Checking:** Rejects any `index >= arr->count` to prevent out-of-bounds dereferencing.
* **Surgical Deallocation:** Explicitly calls `free(arr->lines[index])` on the targeted heap string.
* **Pointer Shift:** Compacts the pointer array by shifting subsequent pointers left: `lines[i] = lines[i + 1]`. Decrements `count--`. Time complexity: $O(N)$ pointer movements, 0 character movements.

### 5. `freeArray(StringArray *arr)`
* **Two-Stage Teardown:**
  1. **Stage 1 (Leaves):** Iterates $0 \le i < \text{count}$ and frees each individual string buffer `free(arr->lines[i])`.
  2. **Stage 2 (Trunk):** Frees the primary pointer array `free(arr->lines)`.
* **Defensive Cleanup:** Sets `arr->lines = NULL`, `arr->count = 0`, `arr->capacity = 0` to prevent dangling pointer bugs.

---

## 🛠️ Build & Verification

```bash
# Compile with strict flags
make

# Execute test suite
make run

# Clean binary
make clean
```

### Memory Safety Verification (AddressSanitizer)
```bash
gcc -Wall -Wextra -fsanitize=address main.c -o main
./main
# Result: 0 memory leaks, 0 heap buffer overflows, clean exit code 0
```

---

## 👤 Author
* **GitHub:** [@d1amantelz](https://github.com/d1amantelz)
* **Part of:** [C Journey Roadmap](../README.md)
