# CLAUDE.md

This file provides guidance to Claude Code (claude.ai/code) when working with code in this repository.

## Overview

This is the server-side source code for **Jade Dynasty** (ZX), a Chinese MMORPG. It is a Linux-targeted C++ project compiled with GCC. The repo contains three main subsystems:

- **`znet/`** — Network layer: daemons for delivery (`gdeliveryd`), gated access (`ggated`), linking (`glinkd`), game database (`gamedbd`), authentication (`gauthd`), and logging (`logservice`/`logclient`). Also contains the RPC code generator (`rpcgen`, a Perl script using `rpcalls.xml`).
- **`zgame/`** — Game server (`gs`): player logic, AI, skills, combat, items, tasks, zones. Links against shared libraries built from `znet`.
- **`zskill/`** — Skill system compiled as a shared library (`libskill.so`). Loaded at runtime by the game server (`zgame/gs/libskill.so` is a symlink). Built independently with its own `makefile`.

## Build System

**All builds run on Linux** (the build root is `/root/zx_source/`). The `setup.sh` script must be placed in the root directory with execute permissions (`chmod 777 setup.sh`).

### Build commands

```bash
# Full build (all components)
./setup.sh all

# First-time build (builds once-only libs like libgsio.a, libTrace.a, then all)
./setup.sh first

# Rebuild deliver-side daemons only (gdeliveryd, glinkd, gamedbd, etc.)
./setup.sh del

# Rebuild game server and its libs (skips skill)
./setup.sh libgs

# Rebuild game server binary only
./setup.sh gs

# Rebuild game libraries only (without rpcgen)
./setup.sh gslib

# Build dumpitem tool
./setup.sh dumpitem
```

Build output is logged to `/root/zx_source/build.log`.

### Build order (for `all`)
1. `znet/rpcgen` — generates RPC stubs from `rpcalls.xml`
2. `znet/logclient`, `znet/gamed` (lib), `znet/gdbclient` (lib + dbtool) — network libs
3. `zgame/libgs/` — aggregated object library
4. `zgame/gs/task/` — task lib
5. `zgame/gs/` — main game server binary

### Skill library

```bash
cd zskill/skill
make        # builds libskill.so
make clean
```

`libskill.so` is symlinked into `zgame/gs/libskill.so` and loaded at runtime. The skill makefile uses `-fPIC --shared` and compiles with `-D_SKILL_SERVER`.

## Architecture

### RPC System (`znet/rpc/`)
RPCs are defined in `rpcalls.xml` and transformed by the Perl `rpcgen` script (uses `rpcalls.xsl`) into `.hpp` stub files in daemon directories (e.g., `znet/gamed/*.hpp`, `znet/gdeliveryd/*.hpp`). Do not hand-edit generated `.hpp` files — modify `rpcalls.xml` and re-run `./rpcgen` from inside `znet/`.

### Skill System (`zskill/`)
- `skill/skillwrapper.h/.cpp` — `GNET::SkillWrapper`: manages a player's skill set (learn/forget, upgrade, cooldowns, persistent data). This is the primary interface the game server uses.
- `skill/playerwrapper.h/.cpp` — `GNET::PlayerWrapper`: extends `SkillWrapper` with player-specific state.
- `skill/skill.h/.cpp` — `SKILL::Skill`: individual skill definition and execution logic.
- `skill/skillfilter.h/.cpp` — filter/conditional logic applied during skill use.
- `skill/skillfunction.h/.cpp` — skill effect functions.
- `header/include/common/base_wrapper.h` — defines `archive` (alias for `base_wrapper`), the serialization interface used throughout for `Store`/`Load` methods.
- `header/include/common/types.h` — shared types including `A3DVECTOR`, `XID`, `object_interface`, `attack_msg`.

### iozlib (generated at setup time)
`setup.sh` creates an `iozlib/` directory at the root with symlinked headers from `znet/` and symlinked `.a` libs. `zgame/Rules.make` references `IOPATH` pointing to this directory. This must exist before building `zgame`.

### Compile flags
- Game server: `-DLINUX -D_DEBUG -D__THREAD_SPIN_LOCK__`, `-O0 -g -ggdb`
- Skill lib: `-D_SKILL_SERVER`, `-O0 -g -ggdb -fPIC -shared`
- Threading: `-D_REENTRANT -D_THREAD_SAFE -pthread`

## Key Notes

- Source files contain Chinese comments (GB2312/GBK encoding) — this is expected.
- Skill log output goes to `/root/1378/libskill.log` (hardcoded in `skillwrapper.cpp`).
- `zskill/skill/` contains a `Makefilelib` (alternative makefile) alongside the primary `makefile`.
- `zgame/Rules.make` has hardcoded paths (`IOPATH`, `BASEPATH`) that `setup.sh` patches via `sed` on each build.

## Bug Fixes Applied (zskill)

This section documents bugs that were found and fixed in the skill library. Keep this updated when further fixes are made.

### 1. `SkillStub::GetMap()` returned by value — all stubs silently lost
**File:** `zskill/skill/skill.h` and `zskill/skill/skill.cpp`

**Symptom:** Every skill returned a null `SkillStub*`. Calling any stub method (`GetOccupation`, `ItemCondition`, etc.) crashed the game server (gs) immediately when a player tried to use a skill.

**Root cause:** `SkillStub::GetMap()` was declared and implemented returning `std::map<...>` by value. Every `SkillStub` constructor called `GetMap().insert(...)` — but this inserted into a temporary copy of the map that was discarded immediately. The real static singleton map inside the function was never populated. `SkillStub::GetStub(id)` always returned null, so `Skill::Skill(uint i)` always set `this->stub = null`.

**Fix:** Changed return type to `std::map<...>&` (reference) in both the header declaration and the `.cpp` implementation.

---

### 2. `GetSkillLevel()` had inverted iterator check
**File:** `zskill/skill/skillwrapper.cpp`

**Symptom:** `GetSkillLevel` returned 0 for skills that existed in the map, and tried to dereference `map.end()` for skills that did not — undefined behaviour / crash.

**Root cause:** The `if`/`else` branches were swapped: the found-iterator branch returned 0, and the not-found branch tried to read `it->second.reallevel`.

**Fix:** Swapped branches — return 0 when `it == map.end()`, return `it->second.reallevel` otherwise.

---

### 3. `LoadDatabase` never wrote `reallevel` for regular and system skills
**File:** `zskill/skill/skillwrapper.cpp`

**Symptom:** All class skills showed in the skill window after LoadDatabase, but using any of them produced "Invalid Skill" on the client. `GetSkillData` read `reallevel` from the map and got 0, so `StartSkill` saw level ≤ 0 and returned -1.

**Root cause:** The regular-skill loop assigned `data.reallevel = data.baselevel` locally but never wrote it to `map[id].reallevel`. The same omission existed for the four hardcoded system skills (389, 2354, 2356, 4934).

**Fix:** Added `map[id].reallevel = data.reallevel;` in the regular-skill loop and `map[389/2354/2356/4934].reallevel = data.reallevel;` for each system skill block.

---

### 4. `SetXPSkill` only wrote `baselevel` to the map entry
**File:** `zskill/skill/skillwrapper.cpp`

**Symptom:** The XP skill did not appear in the skill window after loading the database.

**Root cause:** `SetXPSkill` wrote the local `PersistentData data` struct but only assigned `map[id].baselevel`. The other four fields (`reallevel`, `actilevel`, `mask`, `cooltime`) were left at their zero-initialised defaults.

**Fix:** Added assignments for all five fields from `data` to `map[id]`.

---

### 5. `SetSkillTalent` dereferenced null stub
**File:** `zskill/skill/skillwrapper.cpp`

**Symptom:** gs crashed inside `SetSkillTalent` when a skill whose stub was null was passed in (before fix #1 was applied; also a defensive guard for any future stub-less skill).

**Root cause:** `int n = stub->talent_size` was reached when `stub == nullptr`.

**Fix:** Added a null guard immediately after retrieving the stub — if null, call `SetTalent(6, 0)` and `SetTalent(7, 0)` and return early.

---

### 6. `Stub->ItemCondition` null dereference in `StartSkill` and `Run`
**File:** `zskill/skill/skillwrapper.cpp`

**Symptom:** gs crashed on `Stub->ItemCondition(skill)` when `Stub` was null.

**Root cause:** The condition was written as `if (Stub->ItemCondition(skill))` without a null check.

**Fix:** Changed to `if (!Stub || Stub->ItemCondition(skill))` so a null stub is treated as "no item condition required".

---

### 7. `IsCycle` operator-precedence bug — every non-cycle skill returned "Invalid Skill"
**File:** `zskill/skill/skillwrapper.cpp`, `StartSkill(XID)` overload

**Symptom:** After all other fixes, every skill that is not a cycle skill still returned "Invalid Skill" to the client. Occupation check, Condition, and TestCoolDown all passed.

**Root cause:** The guard expression:
```cpp
if ( (skill->IsCycle() && player.IsInCircleOfDoom())
    || (skill->IsCycle() && player.CircleOfDoomPrepare(...)) == 0)
```
Because `==` has higher precedence than `||`, this parsed as:
```cpp
... || ((skill->IsCycle() && CircleOfDoomPrepare(...)) == 0)
```
When `IsCycle()` is false, `false && anything` evaluates to `0`, and `0 == 0` is `true`, so the whole `if` always fired and returned -1 for non-cycle skills.

**Fix:** Moved `== 0` inside the inner parentheses so it applies only to the return value of `CircleOfDoomPrepare`:
```cpp
if ( (skill->IsCycle() && player.IsInCircleOfDoom())
    || (skill->IsCycle() && player.CircleOfDoomPrepare(...) == 0))
```

---

### 9. `EQUIPMENT_ESSENCE` missing 16 fields — version 158 struct size mismatch
**File:** `zgame/gs/template/exptypes.h`

**Symptom:** gs crashed immediately on startup when loading `elements.data` version 158 (`ELEMENTDATA_VERSION = 0x1000009e`). The debug logger reported `FAILED equipment_essence_array` — the very first array load after the basic equipment type arrays.

**Root cause:** The `array<T>::load()` function validates `sizeof(T)` against the value stored in the file header. Version 158 added 16 new `int` fields to `EQUIPMENT_ESSENCE` that were never added to the server-side struct, making it 64 bytes smaller than what the file expected.

Two groups of missing fields, identified by cross-referencing the editor's column definition against the server struct:

**Group 1 — 14 model path fields** (between `file_model_female_taihao` and `file_matter`):
`file_model_back_female_fash`, `file_model_female_taihao_fash`, `file_model_big_male_sword`, `file_model_big_female_sword`, `file_model_big_male_scythe`, `file_model_big_female_scythe`, `file_model_big_male_bow`, `file_model_big_female_bow`, and 6 reserved slots.

**Group 2 — 2 dye fields** (between `color` and `soul_level`):
`fixed_color` (Dyed_Fixed_Color) and `dyed` (Dyed).

**Fix:** Added all 16 `int` fields in the correct positions in `EQUIPMENT_ESSENCE`.

---

### 14. `TRANSCRIPTION_CONFIG` missing `difficulty` field — version 158 struct size mismatch
**File:** `zgame/gs/template/exptypes.h`

**Symptom:** gs crashed on startup with `FAILED transcription_config_array stored_sizeof=444 expected_sizeof=440 diff=4`.

**Root cause:** The editor definition has `Difficulty` (int32) as the last field, but the server struct ended after `strategy[10]` without it.

**Fix:** Added `int difficulty;` at the end of `TRANSCRIPTION_CONFIG`.

---

### 13. `SPECIAL_ID_CONFIG` missing 8 fields — version 158 struct size mismatch
**File:** `zgame/gs/template/exptypes.h`

**Symptom:** gs crashed on startup with `FAILED special_id_config_array stored_sizeof=484 expected_sizeof=452 diff=32`.

**Root cause:** Version 158 added 8 int32 fields across two locations that were never added to the server struct:

**Group 1 — 6 cross-server speaker fields** (between `id_speaker_special_anony[3]` and `id_change_name[3]`):
The editor defines `ID_Cross_Speaker_Special_1/2/3` and `ID_Cross_Speaker_Special_Anony_1/2/3`. The server jumped directly from `id_speaker_special_anony` to `id_change_name`.

**Group 2 — 2 fields at end of struct** (after `id_rune2013_merge_extra_num[2]`):
`ID_Matrix_Card_Break` and `Fee_Vehicle_Enhance` were missing entirely.

**Fix:** Added `int id_cross_speaker_special[3]` and `int id_cross_speaker_special_anony[3]` after `id_speaker_special_anony`, and `int id_matrix_card_break` and `int fee_vehicle_enhance` at the end of the struct.

---

### 12. `VEHICLE_ESSENCE` missing 2 fields — version 158 struct size mismatch
**File:** `zgame/gs/template/exptypes.h`

**Symptom:** gs crashed on startup with `FAILED vehicle_essence_array stored_sizeof=324 expected_sizeof=316 diff=8`.

**Root cause:** Version 158 added 2 fields that were never added to the server struct:
- `Multi_Ride_Limit` (int32) between `multi_ride_mode` and `speed`
- `Max_Speed` (float) between `speed` and `height`

**Fix:** Added `unsigned int multi_ride_limit;` after `multi_ride_mode`, and `float max_speed;` after `speed`.

---

### 10. `MONSTER_ESSENCE` missing `resist_valid` field — version 158 struct size mismatch
**File:** `zgame/gs/template/exptypes.h`

**Symptom:** gs crashed on startup when loading `elements.data` version 158. The debug logger reported `FAILED monster_essence_array stored_sizeof=800 expected_sizeof=796 diff=4`.

**Root cause:** The `array<T>::load()` function validates `sizeof(T)` against the value stored in the file header. Version 158 added 1 new `int` field (`Resist_Valid`) to `MONSTER_ESSENCE` that was never added to the server-side struct, making it 4 bytes smaller than what the file expected.

The missing field sits between `show_damage` and `level` in the editor column definition (field 19, `Resist_Valid`, type `int32`). The server struct jumped directly from `show_damage` to `level`.

**Fix:** Added `unsigned int resist_valid;` between `show_damage` and `level` in `MONSTER_ESSENCE`.

---

### 11. `NPC_ESSENCE` missing 7 fields — version 158 struct size mismatch
**File:** `zgame/gs/template/exptypes.h`

**Symptom:** gs crashed on startup with `FAILED npc_essence_array stored_sizeof=856 expected_sizeof=828 diff=28`.

**Root cause:** Version 158 added 7 int32 fields across two groups that were never added to the server struct:

**Group 1 — 3 service combo mask fields** (between `combined_services3` and `has_pkvalue_service`):
The editor defines `Special_Mode_2`, `Special_Mode_3`, `Special_Mode_4` (int32 each) after `combined_services3`. The server had only 3 combined_services fields but the file expects 6.

**Group 2 — 4 instance ID fields** (after `id_transcription`):
The editor defines `Instance_1` through `Instance_5` (5 int32s). The server had only one `id_transcription` field (Instance_1). Four more were missing.

**Fix:** Added `combined_services4`, `combined_services5`, `combined_services6` after `combined_services3`, and `id_transcription2` through `id_transcription5` after `id_transcription`.

---

### 15. `CreateProviderInstance` ASSERTed for unregistered service IDs — crash during world load
**Files:** `zgame/gs/serviceprovider.h`, `zgame/gs/servicenpc.h`

**Symptom:** gs crashed during world loading (NPC initialization) with:
```
assert failed:"false && "can not find provider"" in file serviceprovider.h:170
Segmentation fault (core dumped)
```

**Root cause:** `npcgenerator.cpp` calls `service_manager::CreateProviderInstance()` for service IDs 28 (`service_hatch_pet`), 29 (`service_recover_pet`), 96 (`service_equipment_upgrade2`), and 100 (`service_astrology_identify`). None of these IDs have registered providers in `serviceprovider.cpp` (28 and 29 were never added; 96 and 100 are commented out because the provider classes don't exist). `CreateProviderInstance` had a hard `ASSERT(false && "can not find provider")` when lookup failed, which killed the process. Additionally, `servicenpc.h::AddProvider` called `provider->Init()` without a null check, so a NULL return from `CreateProviderInstance` would also crash even if the ASSERT were removed.

**Fix:**
- `serviceprovider.h`: Replaced `ASSERT(false && "can not find provider")` with `fprintf(stderr, "WARNING: ...")` so the function returns NULL gracefully instead of crashing.
- `servicenpc.h::AddProvider`: Added `if(!provider) return;` guard before `provider->Init()` so NPCs with unimplemented services are silently skipped rather than crashing. The NPC simply does not gain that service capability.

---

### 16. "can not init vendor goods 65620" — `BABY_FOOD_ESSENCE` type unknown to server
**Files:** `zgame/gs/template/exptypes.h`, `zgame/gs/template/itemdataman.cpp`

**Symptom:** gs logs `can not init vendor goods 65620` during vendor NPC initialization. Item 65620 belongs to list type 208 (`BABY_FOOD_ESSENCE` in the editor), which has no corresponding `DT_BABY_FOOD_ESSENCE` enum entry and no struct in the server code. Non-fatal: the item slot is set to NULL in the vendor list.

**Root cause:** `itemdataman::generate_item_for_sell()` iterates all items in `ID_SPACE_ESSENCE` and uses a switch on `DATA_TYPE` to register each item. `BABY_FOOD_ESSENCE` is not in the switch (no `DT_BABY_FOOD_ESSENCE` enum value exists), so the item falls to `default: ret = -1` and is never added to `sale_item_id_index_map`. Vendors that have item 65620 in their goods list then cannot look it up.

**Fix needed:** Requires the editor column definition for list 208 to determine field layout and size. Once known: add `BABY_FOOD_ESSENCE` struct to `exptypes.h`, add `DT_BABY_FOOD_ESSENCE` to the `DATA_TYPE` enum, and add `CASE_GEN_NORMAL_ITEM(BABY_FOOD_ESSENCE)` to `generate_item_for_sell()`. Currently left as a non-fatal warning.

---

### 8. `Learn()` consumed a skill point without learning the skill on first learn
**File:** `zskill/skill/skillwrapper.cpp`

**Symptom:** Learning a skill for the first time (skill not yet in the map) consumed a skill point but the skill was not added. A second learn attempt worked correctly.

**Root cause:** When `it == map.end()` (first-time learn), the code inserted a blank `PersistentData` entry and then immediately `return -1`. The game server treated -1 as failure and did not restore the skill point, but the client also received no `SendClientLearnSkill` notification.

**Fix:** Removed the early `return -1`. After inserting the blank entry, execution now falls through to the shared completion path: writes `baselevel`/`reallevel`, calls `SendClientLearnSkill`, calls `OnTalentChange`, and returns `level` (success) — identical to the existing-entry path.
