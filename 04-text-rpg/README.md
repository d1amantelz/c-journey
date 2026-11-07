# ⚔️ Project 04: Text RPG — "Moonlit Echoes"

> Console-based fantasy role-playing game written in pure C (`C99`), featuring a connected location graph (FSM), dynamic heap-managed inventory with memory shifting, NPC shop trading, puzzle chests, and a turn-based combat system with multi-tier enemies and boss mechanics.

---

## 🎮 Game Architecture & Core Mechanics

```
                  ┌──────────────────────┐
                  │       Деревня        │◄───────────────────┐
                  │  (Хаб, Магазин NPC)  │                    │
                  └──────────┬───────────┘                    │
                             │                                │
            ┌────────────────┼────────────────┐               │ (Побег /
            ▼                ▼                ▼               │  Гибель)
     ┌─────────────┐  ┌─────────────┐  ┌─────────────┐        │
     │ Темный лес  │  │  Побережье  │  │   Склеп     │        │
     │   (Гоблин)  │  │  (Сундук)   │  │(Скелет-босс)│        │
     └──────┬──────┘  └─────────────┘  └──────┬──────┘        │
            │                                 │               │
            └─────────────────────────────────┴───────────────┘
```

### 1. State Management & Location Graph (FSM)
* Graph-like movement between locations: `Village` (safe zone/hub), `Dark Forest` (hostile), `Shore` (exploration/secrets), and `Crypt` (dungeon boss chamber).
* Dynamic event actions tailored to player state (combat initiation, NPC dialogues, chest puzzles).

### 2. Player Inventory & Memory Management
* Fixed/dynamic inventory buffer with strict string-safe bounds.
* **Item Consumption & Deletion:** Using memory shifting `memmove` / array compacting to remove used items (`Зелье лечения`) without leaving holes or leaking memory.
* **Equipment System:** Distinguishing consumable items from weapons (`Ржавый меч`, `Стальной меч`). Equipping a weapon dynamically recalculates total player attack:
  ```c
  total_attack = base_attack + weapon_attack_bonus;
  ```

### 3. Economy & Shop Trading (NPC)
* Merchant inventory with gold verification.
* Atomic purchase transactions: validating player gold balance → deducting cost → pushing item into inventory.

### 4. Turn-Based Combat Loop
* Enemy structure: `name`, `hp`, `maxHp`, `attack`, `goldReward`.
* Combat choices on each turn:
  1. **Атаковать:** Calculate damage based on total attack power.
  2. **Использовать предмет:** Open combat bag to drink health potions mid-battle.
  3. **Сбежать:** Retreat mechanism that saves the player from death at the cost of returning to the village.
* Post-boss progression: Defeating the "Хранитель склепа" permanently grants `+2` to permanent `base_attack`.

### 5. Typewriter Atmospheric Text Engine (`typePrintf`)
* Formatted string output with variable arguments via `<stdarg.h>` (`va_list`, `va_start`, `va_end`) and microsecond delays (`usleep`), creating a retro text-quest aesthetic with auto-flush (`fflush(stdout)`).

### 6. Persistence & Serialization (`Save / Load`)
* Custom structured text format (`save.txt`) storing full game state across sessions:
  * **Player stats:** name, location ID, HP, Max HP, base attack, total attack, gold.
  * **World & Quest progress:** persistent state flags (`chestOpened`, `cryptUnlocked`, `bossDefeated`).
  * **Dynamic inventory heap:** dynamically serialized and deserialized with string-safe scansets (`%95[^|]`) and memory reallocation (`malloc` / `free`).
* Auto-save on graceful exit, in-game manual save, and start menu load with fallback to New Game.

---

## 🛠️ Build & Run

```bash
cd 04-text-rpg
gcc -Wall -Wextra main.c -o main
./main
```

---

## 📦 Version History
* **v1.2.0** — Added persistent Save/Load system (`save.txt`), quest flags persistence, and dynamic inventory deserialization.
* **v1.1.0** — Added locked crypt location, skeleton crypt boss fight, and permanent base attack scaling reward.
* **v1.0.0** — Added turn-based combat system, Goblin enemy, and Dark Forest.
* **v0.3.0** — Added NPC merchant shop and shoreline chest riddle.
* **v0.2.0** — Added inventory management, potion usage, and weapon equipment.
* **v0.1.0** — Initial FSM location skeleton and typewriter CLI engine.
