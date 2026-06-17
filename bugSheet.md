# Bug Sheet — EV Charging Network

---

## BUG-001 · Duplicate Station IDs allowed
**Reported:** Session 1  
**Status:** ✅ Fixed

**Description:** Calling Add Station with an ID that already existed in the list silently inserted a second node with the same ID, corrupting the list.

**Fix:** Added a traversal check at the top of `addStation()` in `network.cpp`. If the ID already exists, the insert is aborted and an error message is returned.

---

## BUG-002 · Lowercase status input breaks color and overview counter
**Reported:** Session 2  
**Status:** ✅ Fixed

**Description:** Typing `occupied` (or any non-exact-case variant) instead of `Occupied` caused two failures:
- The status cell in the table lost its color (showed white instead of red).
- The stats footer (`⚡ N stations … ● N Occupied`) did not count the station, showing wrong totals.

**Root cause:** `statusColor()` and the stats counter in `displayAll()` both do exact string matching against `"Available"`, `"Occupied"`, `"Maintenance"`. A lowercase input stored as-is never matched.

**Fix:** Added `canonicalStatus()` in `network.cpp` that lowercases the input and maps it to the canonical cased form before storing. Input is now case-insensitive.

---

## BUG-003 · Trailing (or leading) spaces in input not stripped
**Reported:** Session 2  
**Status:** ✅ Fixed

**Description:** Typing `maintenance ` (with a trailing space) stored the string with the space, so it failed the canonical match, showed in the table unstyled, and was not counted in the overview.

**Root cause:** `getline()` captures the full raw line including surrounding whitespace. C++ has no built-in `strip()`.

**Fix:** Added `trim()` to `ui.h` using `find_first_not_of` / `find_last_not_of`. The `ask()` helper in `main.cpp` now calls `trim()` on every line read before returning it, so all inputs are silently stripped.

---

## BUG-004 · Queue counter has no behavior — decrement never happens
**Reported:** Session 3  
**Status:** 🔲 Open (deferred to V2)

**Description:** Queue+ increments `waitingCount` on a station, but changing that station's status back to `Available` does nothing to the counter. The number just sits there — no car is "assigned", the status doesn't flip back to `Occupied`, and the counter never decrements.

**Expected behavior:** When a station transitions `Occupied → Available` and `waitingCount > 0`, the system should auto-assign the next waiting car: decrement the counter by 1 and flip the status back to `Occupied`.

**Planned fix (V2):** Inside `updateStatus()`, after storing the new status, check — if `newStatus == "Available"` and `waitingCount > 0`, decrement `waitingCount` and set status back to `"Occupied"`. Return a message indicating a queued car was assigned.

---

*Add new bugs below this line as they are reported.*
