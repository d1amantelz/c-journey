# 💻 C Journey: From Bare Metal to Clean Architecture

> Systematic, hands-on learning roadmap of low-level systems programming in C (`C99`/`C11`), focusing on memory management, pointer arithmetic, data structures, and CLI software design.

---

## 🚀 Projects Overview

| # | Project | Key Concepts | Status |
|---|---|---|:---:|
| `01` | [**CLI To-Do List**](./01-todo-list) | Fixed-size 2D arrays, strings (`fgets`, `strcspn`), CLI state loop, pass-by-pointer | ✅ Completed |
| `02` | [**CLI Phonebook**](./02-phonebook) | Structures (`struct`), record search by substring (`strstr`), record deletion & memory shifting | ✅ Completed |
| `03` | [**CLI Expense Tracker**](./03-expense-tracker) | Dynamic arrays on heap (`malloc`, safe `realloc(temp)`, `free`), `enum` categories, CSV tokenizer (`strtok`), Snapshot State Dump persistence | ✅ Completed |
| `04` | [**Text RPG: Moonlit Echoes**](./04-text-rpg) | Finite State Machine (FSM), dynamic player inventory on heap (`malloc`/`realloc`), modular architecture (`.h`/`.c`), formatted item bonuses | 🚧 In Progress |

---

## 🛠️ Tech Stack & Core Competencies
* **Language:** Pure C (`C99` / `C11`)
* **Compilers:** `gcc` (with `-Wall -Wextra -g`)
* **Memory Model:** Stack vs Heap, `malloc`, `realloc`, `free`, memory safety and leak prevention
* **Data Structures:** Dynamic vectors, nested structures, enums with `COUNT` sentinels
* **I/O & Persistence:** Robust user input handlers (`getInt`, `getString`), CSV serialization, file streams (`fopen`, `fclose`, `fprintf`, `fgets`)
* **Architectures:** Finite State Machine (FSM), modular multi-file structure

---

## ⚙️ Quick Start

### Build and Run any project:
```bash
# Example: Running the Expense Tracker
cd 03-expense-tracker
gcc -Wall -Wextra main.c -o main
./main
```

---

## 👤 Author
* **GitHub:** [@d1amantelz](https://github.com/d1amantelz)
* **Goal:** Building solid low-level foundations toward Embedded Systems & Robotics.
