# Subfolder Alone (`suba`) — Work in Progress

**Suba** is a lightweight C++ tool for navigating folders by name.

It explores the directory you choose once, indexes the folder names and locations, and lets you jump to a matching folder without typing or searching the full path every time.

Suba does not learn from your navigation history. It starts from the directory you give it and is ready to find folders immediately.

## What Suba does

- Explore a project or directory tree once.
- Find a folder using only its name.
- Use extra path context to narrow down a match.
- Show a list when the same folder name exists in more than one place.
- Keep jump data separated from the original project files.
- Skip missing or damaged shard files without stopping the whole lookup instance.

Example:

```text
sa inside_pro1

sa assets/styles

sa styles
```

A unique folder can return directly:

```text
project/project1/inside_pro1
```

A repeated folder name can produce a collision suggestion:

```text
1: project/project1/same_one
2: project/project2/same_one
3: project/project3/same_one
```

If you provide context such as `assets/styles`, Suba can find `project/assets/styles` instead of listing every folder named `styles`.

## How it works

Suba has two main stores:

1. **Path shards**

   Folder paths are stored in timestamped `.sa` files. These files are written in small chunks, currently capped around 1 MB each, so Suba does not keep every path in one large unstructured file.

2. **Binary offset index**

   `suba_fl.bin` stores compact `fchash` keys and points to the `.sa` file and byte offset for each matching path.

The lookup flow is:

```text
folder name or path context
        |
        v
  find the fchash key
        |
        v
  get matching shard and offset records
        |
        v
  read only those path records
        |
        v
  return one match or collision suggestions
```

The code already checks whether the requested shard exists and whether an offset can be read. A missing or damaged shard reports an error and is skipped instead of ending the complete Suba instance.

## `suba` and tools like `zoxide`

Zoxide and similar tools learn which directories you visit. They build a ranked history, while Suba explores the directory you specify and builds a folder-location index.

| Behavior | Suba | Zoxide and similar tools |
|---|---|---|
| Knowledge source | Directory scan you choose | Directory visits and history |
| First use | Finds folders immediately after indexing | Results improve as folders are visited |
| Folder-name collision | Suggests matching paths or uses context | Usually chooses the highest-ranked match |
| Direct path context | Supported by `getpath()` | Matching rules and ranking still apply |
| Storage | Hash keys, shard names and offsets | Ranked directory records |
| Main goal | Find a known folder without guessing | Rank frequently used folders |

Both tools solve directory navigation, but they do it in different ways. Suba is built for explicit folder lookup inside selected projects or trees; not for replacing every shell command or replacing `cd`.

## Project status

Working now:

- Recursive directory scanning.
- Flexible `fchash` lookup keys.
- Timestamped `.sa` path shards.
- Binary shard-and-offset index.
- Direct path and folder-name lookup.
- Parent/path-context filtering.
- Collision suggestions.
- Missing-shard and damaged-offset error handling.

Still in progress:

- Shell integration for `sa` and `s` commands.
- Per-record compression inside path shards.
- More platform and shell support.
- Documentation and installation instructions.

Suba is currently developed and tested on Android through Termux. It uses C++ filesystem and path APIs, so portability will be addressed as the core becomes stable.

## Build

The project is still under active development. The source can be compiled manually for testing:

```bash
g++ suba.cpp ../src/iobs.cpp ../src/suba.cpp -o run
```

Run paths and setup may change while `getpath()` and shell integration are still being completed.

## About contributions

The core is being built manually. Feedback about the architecture, lookup behavior and multi-platform support is welcome.

If Suba's approach interests you, star or follow the repository:

https://github.com/777Tu/suba
