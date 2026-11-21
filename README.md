# Subfolder Alone (`suba`) *Work in Progress*

**suba** is a fast, lightweight C++ CLI tool built to do one thing well: jump directly to any subfolder in your project or root directory using just its name—with **zero learning phase**, **zero guessing**, and **zero external dependencies**.

> **Project Status:** Core directory explorer, `fchash` compact key generator, 1MB timestamp-sharded flat-file store (`.sa`), and binary offset indexer (`suba_fl.bin`) are complete. Path resolver (`getpath`) and shell wrappers (`sa` / `s`) are currently in active development.

---

## Why `suba` Exists

Traditional directory jumpers (`zoxide`, `autojump`, `fasd`) rely on **habit learning**—you have to manually visit folders repeatedly before the tool becomes useful. Worse, when two different folders share the exact same name (e.g., `example/test` and `internet/test`), frequency-based tools silently guess and drop you into whichever one you visited more often.

**`suba` takes the opposite approach:**
* **Explore, Don't Learn:** Point `suba` at a project or root directory once. It immediately explores the entire tree and indexes everything—ready to jump on day one.
* **No Silent Guessing:** When multiple folders share the same name, `suba` never picks for you blindly. It either prompts you with a clean numbered list (**Collision Suggest**) or jumps straight there if you pass parent context (e.g., `sa example/test`).
* **Strict Unix Philosophy:** `suba` does not try to replace your shell or claim to do everything. Its sole job is instant subfolder navigation (`sa <folder>` or `s <folder>`). Everything else stays with `cd` and your standard CLI tools.

---

## How It Works: The Sharded Two-Store Architecture

Instead of dumping everything into one bloated database file or scanning thousands of paths on every jump, `suba` splits storage into **two decoupled stores** with automatic file sharding:

1. **The Heavy Store (1MB-Capped Timestamped `.sa` Shards):** Stores the actual full folder paths (the "cartons"). Each `.sa` file is named using its creation timestamp (e.g., `2026-11-20_17-18-50-296773.sa`) and capped at a **1MB size limit**—once a file hits 1MB, `suba` automatically rolls over into a new timestamped `.sa` shard to keep individual file operations fast and lightweight.
2. **The Lightweight Index (`suba_fl.bin`):** Uses **`fchash`** to compress any folder name—no matter how long—into a compact `double` key mapped to a `PathLocation` structure (`<sa_filename, vector<streamoff>>`). This tracks **both which `.sa` shard file(s) contain the match and the exact byte offsets inside each shard**.

### Visual Flow

```text
  [ Lightweight Index: suba_fl.bin ]                    [ 1MB-Capped Heavy Shards ]
  ┌──────────────────────────────────────────┐          ┌────────────────────────────────────┐
  │ fchash("test") ─► 10.20                  │    ┌───►│ 2026-11-20_17-18-50-296773.sa      │
  │   ├─ "2026-11-20_17-18-50-296773.sa"     │    │     │   Byte 42: "example/test"          │
  │   │   └─► [42] ──────────────────────────┼────┘     └────────────────────────────────────┘
  │   └─ "2026-11-20_17-18-52-814091.sa"     │          ┌────────────────────────────────────┐
  │       └─► [108] ─────────────────────────┼────────► │ 2026-11-20_17-18-52-814091.sa      │
  └──────────────────────────────────────────┘          │   Byte 108: "internet/test"        │
                                                        └────────────────────────────────────┘
```

### Handling Collisions & Selective Lookup Across Shards
* **Unique Match (`sa inside_pro1`):** `fchash` maps to a single `.sa` shard and a single byte offset. `suba` opens only that shard, seeks directly to that byte offset, reads the single path, and jumps immediately.
* **Collision Suggest (`sa test`):** Even if two folders named `test` were scanned across a large tree and landed in different 1MB timestamped shards, `PathLocation` groups both shard names and their exact offsets under the same `fchash` key (`10.20`). `suba` seeks straight to those exact positions and lists them for you to pick:
  ```text
  1: example/test
  2: internet/test
  ```
* **Direct Context Jump (`sa internet/test`):** Already know which one you want? Provide partial parent info (`internet/test`), and `suba` fetches only the colliding offsets across the relevant shards, filters for `internet/test`, and jumps straight there without prompting.

*(Planned Optimization: Per-entry compression inside the 1MB `.sa` shards so only the exact target path requested by the user is ever decompressed on disk.)*

---

## `suba` vs. `zoxide` & Similar Tools

| Feature | `suba` (Subfolder Alone) | `zoxide` / `autojump` / `fasd` |
| :--- | :--- | :--- |
| **Setup Behavior** | **Immediate tree exploration** (works instantly) | **Habit learning** (must visit folders over time) |
| **Duplicate Folder Names** | **Collision Suggest** list + direct `parent/child` jump | Silently jumps to highest-ranked frequency match |
| **Storage & Lookup** | `fchash` + 1MB timestamped `.sa` shards + O(1) `seekg` | Single database query / string scoring across history |
| **Long Folder Names** | Fixed-size `double` hash key via `fchash` | Stores full strings in index |
| **Target Platform** | Linux / Bash / **Android (Termux)** *(multi-platform planned based on community interest)* | Cross-platform |

---

## Roadmap

- [x] Recursive directory explorer (`scan`)
- [x] Compact `double` folder hashing (`fchash`)
- [x] 1MB-capped timestamped `.sa` path shards
- [x] Custom binary serializer & multi-shard offset indexer (`iobs`)
- [ ] Direct lookup, context filter & Collision Suggest resolver (`getpath`)
- [ ] Shell integration (`sa` & `s` commands for Bash / Termux)
- [ ] Per-record compression inside `.sa` shards
- [ ] Multi-shell & cross-platform support (Zsh, Fish, PowerShell, Windows/macOS)

---

## Support & Contributions

`suba` is built from scratch in pure C++17 with no external libraries—originally crafted for fast terminal workflows on Linux and Android (Termux).

If you like the architecture or want multi-platform/multi-shell support prioritized, drop a ⭐ **Star** on the repo to show support!
