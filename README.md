# Smart EV Charging Network

A terminal-based C++ simulation of a smart electric vehicle charging station network, built as a DSA semester project. The entire network is modelled as a **singly linked list** — every charging station is a node, and all operations traverse or modify that list in real time.

**Institution:** ITM Skills University — B.Tech CSE 2025–29  
**Subject:** Data Structures & Algorithms, Semester II  
**Language:** C++17  
**Interface:** Full-screen terminal dashboard with ANSI colors  

---

## What It Does

The program simulates a network control dashboard for EV charging stations. An operator can install new stations, remove them, update their status as cars plug in and out, search for a specific station, and track how many cars are queued at a busy station — all from a live-redrawing terminal UI.

---

## Data Structure — Singly Linked List

Each charging station is a **node** in a singly linked list. The list grows and shrinks dynamically as stations are added or removed, with no fixed upper limit on network size.

```
HEAD
 │
 ▼
┌─────────────────────────┐     ┌─────────────────────────┐
│ id        : ST001       │     │ id        : ST002       │
│ location  : Kharghar    │     │ location  : Vashi       │
│ capacity  : 50 kW       │────▶│ capacity  : 100 kW      │────▶ NULL
│ status    : Available   │     │ status    : Occupied    │
│ waiting   : 0           │     │ waiting   : 2           │
└─────────────────────────┘     └─────────────────────────┘
```

**Why a linked list over an array?**  
Arrays have a fixed size and require O(n) shifting on every insert or delete. A linked list is dynamic — adding or removing a station only rewires two pointers. In a real network where stations are frequently installed or decommissioned, this matters.

---

## Node Structure

Defined in `station.h`:

```cpp
struct ChargingStation {
    string id;            // unique station identifier (e.g. "ST001")
    string location;      // human-readable location name
    int    capacity;      // charging power in kW
    string status;        // "Available" | "Occupied" | "Maintenance"
    int    waitingCount;  // number of cars queued at this station
    ChargingStation* next; // pointer to the next node in the list
};
```

---

## File Structure

```
ev_charging/
├── station.h      — ChargingStation struct (node definition)
├── network.h      — ChargingNetwork class declaration
├── network.cpp    — all linked list method implementations
├── ui.h           — ANSI color codes, screen helpers, trim(), padding
├── main.cpp       — full-screen dashboard loop and input handling
├── bugSheet.md    — log of all reported bugs and their status
└── README.md      — this file
```

---

## Operations

| # | Operation | What It Does | Real-World Meaning | Time Complexity |
|---|---|---|---|---|
| 1 | `addStation()` | Insert node at tail, reject duplicate IDs | New station installed in the network | O(n) |
| 2 | `removeStation()` | Find node by ID, rewire pointers, free memory | Station taken offline | O(n) |
| 3 | `updateStatus()` | Traverse to node, update status field | Car plugs in / unplugs / maintenance flagged | O(n) |
| 4 | `displayAll()` | Traverse head → NULL, render full dashboard | Live network overview | O(n) |
| 5 | `searchStation()` | Find node by ID, highlight in dashboard | Driver or operator looks up a station | O(n) |
| 6 | `incrementWaiting()` | Find node, increment waitingCount | Car joins the queue at a busy station | O(n) |

All operations are O(n) because the list must be traversed from `head` — there is no index or hash map. This is a known limitation and a natural discussion point for V2.

---

## Build & Run

**Requirements:** `g++` with C++17 support (standard on macOS/Linux).

```bash
# compile
g++ -std=c++17 -o ev main.cpp network.cpp

# run
./ev
```

The terminal must support ANSI escape codes for colors and full-screen redraws (any modern macOS Terminal, iTerm2, or Linux terminal works).

---

## Terminal UI

The dashboard redraws the entire screen on every action — no scrolling log. Each frame shows:

- A live table of all stations with color-coded status dots
- A stats footer (`⚡ N stations  ● Available  ● Occupied  ● Maintenance`)
- The result of the last action as a status message
- A compact color-coded menu

**Status colors:**
- `●` Green — Available
- `●` Red — Occupied  
- `●` Yellow — Maintenance
- Waiting count turns orange when > 0
- Search highlights the matched row in bold cyan with a `◀ found` marker

---

## Version History

| Version | Description |
|---|---|
| `v1.0.0` | Plain terminal dashboard, scrolling menu, all 6 operations, duplicate ID check |
| `v1.1.0` | Full-screen colored UI, ANSI 256-color palette, live redraw, stats footer |
| `v1.1.3` | Bug fixes: case-insensitive status input, whitespace trimming on all inputs, bugSheet added |

---

## Known Bugs / Open Issues

See [`bugSheet.md`](bugSheet.md) for full details.

| ID | Description | Status |
|---|---|---|
| BUG-001 | Duplicate station IDs were allowed | ✅ Fixed in v1.0.0 |
| BUG-002 | Lowercase status input broke color and overview counter | ✅ Fixed in v1.1.3 |
| BUG-003 | Trailing/leading spaces in input were not stripped | ✅ Fixed in v1.1.3 |
| BUG-004 | Queue counter never decrements when station becomes Available | 🔲 Deferred to V2 |

---

## Future Scope

### V2 — Simulation Mode
- Auto-generate 80+ station nodes with randomised data
- Spawn cars at random intervals using a timer or `rand()`
- Car checks list for the first `Available` station; if none found, joins the queue
- **Fix BUG-004:** when a station flips `Occupied → Available` and `waitingCount > 0`, auto-assign the next queued car — decrement counter, flip status back to `Occupied`
- After a random charge duration, station returns to `Available` and the next car is assigned
- Log search time per car (nodes traversed) to measure O(n) cost live
- Print utilisation stats at end of simulation
- **Viva argument:** at 80 nodes, linear search is measurably slow — justify replacing the list traversal with a HashMap or BST index for O(1) / O(log n) lookup

### V3 — Web App
- Port core logic to Python (FastAPI backend)
- REST endpoints: `GET /stations`, `POST /station`, `DELETE /station/{id}`, `PATCH /station/{id}/status`
- Frontend with Leaflet.js + OpenStreetMap tiles
- Plot stations on real Navi Mumbai / Kharghar coordinates
- Color-coded map markers mirroring the terminal status colors
- Live status refresh via polling or WebSocket
- Deploy with Cloudflare Tunnel

---

## Key Concepts for Viva

**Q: Why Linked List over Array?**  
Arrays have fixed size and O(n) shifting on insert/delete. Linked lists are dynamic — insert and delete only require pointer rewiring. Ideal for a network where stations are added and removed frequently.

**Q: What is the time complexity of deletion?**  
O(n) to traverse and find the node, O(1) for the actual pointer rewiring.

**Q: What happens when you delete a node?**  
The previous node's `next` pointer skips the deleted node and points to the one after it. The deleted node's memory is freed with `delete`.

**Q: How do you handle deletion of the head node?**  
Move `head` to `head→next`, then free the old head. This is a special case handled separately before the general traversal.

**Q: What is the head pointer?**  
The entry point to the entire list. It always points to the first node. If `head` is `nullptr`, the list is empty.

**Q: What is the space complexity?**  
O(n) — one node per station, memory scales linearly with network size.

**Q: What's the difference between singly and doubly linked list?**  
Singly: each node has one pointer (`next`). Doubly: each node has two pointers (`next` + `prev`), allowing backward traversal. This project uses singly linked.

**Q: How would you improve search performance at scale?**  
At 80+ nodes, O(n) linear search becomes a bottleneck. A HashMap (unordered_map) would give O(1) average lookup by ID. A BST would give O(log n) with sorted traversal. Both would be layered on top of the linked list in V2.

---

*Built by AASHPAW — ITM Skills University — DSA Sem II*
