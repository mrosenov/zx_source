# Task System Analysis — `zgame/gs/task/`

## Overview

The task (quest) system in Jade Dynasty (ZX) is a self-contained subsystem living under `zgame/gs/task/`. It is compiled as a set of `.o` object files that link directly into the main game server binary (`zgame/gs/gs`). An optional `libtask.so` shared library target also exists but is not used by the primary server build. The subsystem is shared between server and client via preprocessor guards (`#ifdef _TASK_CLIENT` / `#ifndef _TASK_CLIENT`), so the same source files compile for both targets — this document focuses entirely on the server side.

Entry point from the rest of the game server: `InitQuestSystem()` in `taskman.cpp`, called during startup with four file paths (static task pack, dynamic task pack, forbidden-task list, NPC info pack) plus a pointer to the element data manager. It loads templates, then registers a periodic timer (every 5 × 20 ticks × 10 interval = roughly every 1000 ticks) that calls `OnTaskCheckAllTimeLimits()` to expire timed-out tasks.

---

## File Inventory

| File | Lines | Role |
|---|---|---|
| `TaskInterface.h` | 638 | Pure-virtual `TaskInterface` base class; all constants, error codes, buffer sizes, notification codes, shared structs |
| `taskman.h` | 283 | `PlayerTaskInterface` (concrete server impl) + `PlayerTaskTeamInterface` |
| `taskman.cpp` | 1646 | All `PlayerTaskInterface` method bodies — bridges task system to `gplayer_imp` |
| `TaskTempl.h` | 2765 | `ATaskTemplFixedData` + `ATaskTempl` class; all template data fields, `AWARD_DATA`, `ITEM_WANTED`, `MONSTER_WANTED`, `TEAM_MEM_WANTED`, wire structs, Check/Deliver/Award method declarations |
| `TaskTempl.inl` | 2863 | Inline implementations of all `ATaskTempl` check and process methods |
| `TaskTempl.cpp` | 8834 | Non-inline `ATaskTempl` implementations (load from binary/text, marshal/unmarshal, award delivery) |
| `TaskTemplMan.h` | 176 | `ATaskTemplMan` class — manages all loaded template maps |
| `TaskTemplMan.cpp` | 1610 | Template loading, dynamic task pack handling, storage refresh, death-trigger checks |
| `TaskProcess.h` | 378 | `ActiveTaskEntry`, `ActiveTaskList`, `FinishedTaskList`, `TaskFinishTimeList`, `TaskGlobalData`, `TaskPreservedData` — the runtime state structs |
| `TaskProcess.cpp` | 4497 | `ActiveTaskList` and `FinishedTaskList` method bodies |
| `TaskServer.h` | 147 | Free-function declarations for all server-side event callbacks (kill monster, collect item, etc.) |
| `TaskServer.cpp` | 2179 | Implementations of those callbacks; routing to the correct template methods |
| `TaskAsyncData.h` | 233 | `TaskFamilyAsyncData` and `TaskCircleGroupAsyncData` — serialisation helpers for async DB queries |
| `TaskExpAnalyser.h` | 58 | `TaskExpAnalyser` — expression parser for runtime reward formulae |
| `TaskExpAnalyser.cpp` | 189 | Expression parser implementation (recursive-descent, operates on `TASK_EXPRESSION` arrays) |
| `Makefile` | 30 | Compiles all `.cpp` to `.o` with `-fPIC`; optional `lib` target builds `libtask.so` |

---

## Architecture

```
gplayer_imp
    |
    v  (owns)
PlayerTaskInterface   (implements TaskInterface)
    |
    |-- GetActiveTaskList()   --> ActiveTaskList  (player's live quest state, in player blob)
    |-- GetFinishedTaskList() --> FinishedTaskList
    |-- GetFinishedTimeList() --> TaskFinishTimeList
    |
    v  (calls into)
ATaskTemplMan   (global singleton, GetTaskTemplMan())
    |
    |-- m_TaskTemplMap     hash_map<id, ATaskTempl*>  (top-level tasks only)
    |-- m_AllTemplMap      hash_map<id, ATaskTempl*>  (every node including sub-tasks)
    |-- m_AutoDelvMap      (auto-deliver tasks)
    |-- m_DeathTrigMap     (tasks triggered on player death)
    |-- m_ProtectNPCMap    (NPC protect tasks)
    |-- m_SkillTaskLst     (skill/living-skill tasks)
    |-- m_Storages[9]      (storage pool tasks by slot)
    |-- m_DynTaskMap       (dynamic/live-event tasks loaded from a separate pack)
    |-- m_ForbiddenTaskMap (tasks administratively disabled)
    |-- m_TmLmtChkLst      (templates with active time limits, for periodic sweep)
    |
    v
ATaskTempl : ATaskTemplFixedData
    |
    |-- tree links: m_pParent, m_pFirstChild, m_pPrevSibling, m_pNextSibling
    |-- prerequisite fields (CheckXxx methods)
    |-- completion method + finish type fields
    |-- award data (AWARD_DATA* for success/fail, plus ratio/item/count-scaled variants)
    |-- five talk_proc objects (dialogue trees for deliver/unqualified/item/exec/award)
```

The `ActiveTaskList` and related lists live inside the player's persistent data blob (serialised to the database) rather than being heap-allocated separately. Pointers into the template tree (`m_ulTemplAddr`, `m_ulCapTemplAddr`) are stored in `ActiveTaskEntry` as raw `unsigned long` addresses cast at runtime — template objects must stay at stable addresses after load.

---

## Runtime State Structures (`TaskProcess.h`)

### `ActiveTaskEntry` (32 bytes, `#pragma pack(1)`)

The unit entry for one active task node.

| Field | Type | Meaning |
|---|---|---|
| `m_ID` | `unsigned short` | Task template ID (14-bit range) |
| `m_ParentIndex` | `unsigned char` | Index in `m_TaskEntries[]` of parent, `0xff` = root |
| `m_PrevSblIndex` | `unsigned char` | Left sibling index, `0xff` = none |
| `m_NextSblIndex` | `unsigned char` | Right sibling index, `0xff` = none |
| `m_ChildIndex` | `unsigned char` | First child index, `0xff` = leaf |
| `m_uState` | `unsigned char` | Bitmask of state flags (see below) |
| `m_ulTaskTime` | `unsigned long` | Unix timestamp when task was accepted |
| `m_uCapTaskId` | `unsigned short` | Captain's task ID (for team tasks) |
| `m_ulTemplAddr` | `unsigned long` | Raw pointer to owning `ATaskTempl` |
| `m_ulCapTemplAddr` | `unsigned long` | Raw pointer to captain's `ATaskTempl` (or 0) |
| `m_BufData` / union | 11 bytes | Overlaid with `m_wMonsterNum[3]`, `m_wItemsToGet`, or `m_wInterObjNum[3]` |

State flags (`m_uState`):

| Flag | Value | Meaning |
|---|---|---|
| `TASK_STATE_FINISHED` | 0x01 | Task is done (success or fail) |
| `TASK_STATE_SUCCESS` | 0x02 | Finished successfully |
| `TASK_STATE_GIVEUP` | 0x04 | Player abandoned |
| `TASK_STATE_ERR_REPORTED` | 0x08 | Error sent to client |
| `TASK_STATE_AWARD_NOTIFY_TEAM` | 0x10 | Team members notified of award |
| `TASK_STATE_AWARD_FAMILY` | 0x20 | Family reward dispatched |
| `TASK_STATE_CAN_DELIVER_EXTRA_AWARD` | 0x40 | Extra reward available |
| `TASK_STATE_HAS_DELIVER_EXTRA_AWARD` | 0x80 | Extra reward already delivered |

### `ActiveTaskList`

Fixed-size struct that stores up to `TASK_ACTIVE_LIST_MAX_LEN` (120) entries and 9 storage-slot pools.

| Key Field | Value | Meaning |
|---|---|---|
| `m_uTaskCount` | 0–120 | Number of entries in use |
| `m_uUsedCount` | — | Actual non-empty slots (can differ after alignment) |
| `m_Version` | `unsigned short` | Incremented on changes, synced to client |
| `m_uTopShowTaskCount` | — | Visible top-level tasks |
| `m_uTopHideTaskCount` | — | Hidden top-level tasks |
| `m_Storages[9][10]` | `unsigned short` | Task IDs in each of the 9 storage pools |
| `m_TaskEntries[120]` | `ActiveTaskEntry` | The actual entries |

Important methods: `GetEntry(id)`, `ClearTask()`, `RecursiveClearTask()`, `ClearChildrenOf()`, `RealignTask()`, `UpdateTaskMask()`.

### `FinishedTaskList`

Stores up to `TASK_FINISHED_LIST_MAX_LEN` (2040) completed task IDs as `unsigned short`. The high bit of the stored ID encodes success/failure. Buffer total: 4096 bytes.

### `TaskFinishTimeList`

Up to 1700 entries, each holding task ID, delivery timestamp, finish count, and last-update time. Used for daily/weekly reset logic and repeat-count caps.

Fields per `TaskFinishTimeEntry`:
- `m_uTaskId` — task ID
- `m_ulTimeMark` — delivery timestamp
- `m_uFinishCount` — how many times completed (capped at `0xffff`)
- `m_ulUpdateTime` — last updated timestamp

### `TaskGlobalData` (256-byte blob)

Shared per-task global state stored server-side (in `gamedbd`). At minimum contains:
- `m_ulReceiverNum` — total players who have accepted
- `m_ulRcvUpdateTime` — last receive-count update
- `m_ulFinishCount` — total players who finished
- `m_ulFnshUpdateTime` — last finish-count update

Used for co-op tasks (max receivers), finish-count prerequisites, and extra awards gated on global counts.

### `TaskPreservedData`

Small struct passed with global-data queries to preserve context across the async round-trip (reason, source task ID, receiver num, sub-task ID or kill timestamp).

---

## Template Data (`ATaskTempl` / `ATaskTemplFixedData`)

Every quest is an `ATaskTempl` object. The class inherits `ATaskTemplFixedData` (all the plain-old-data fields) and adds the runtime tree pointers, text strings, and `talk_proc` dialogue objects.

### Identity and Metadata

| Field | Meaning |
|---|---|
| `m_ID` | Unique task ID |
| `m_szName[30]` | Display name (wide chars) |
| `m_ulType` | Task category (0–17; types 14–17 are cross-server) |
| `m_ulRank` | Difficulty rank |
| `m_ulDisplayType` | Display style (0=default, 1=daily, 2=weekly, 3=monthly, 4=support) |
| `m_ulRecommendType` | Recommendation category (on-top, exp, money, cultivation, deity, special, title, equip) |
| `m_bKeyTask` | Whether task appears in the key-task list |
| `m_bHidden` | Hidden task (doesn't appear in task list UI) |
| `m_szFilePath[260]` | Source file path (for editor and debug) |

### Delivery Conditions

| Field | Meaning |
|---|---|
| `m_ulPremise_Lev_Min/Max` | Level range required |
| `m_ulPremItems` / `m_PremItems[]` | Items that must be in inventory |
| `m_ulPremise_Reputation` | Min reputation |
| `m_ulPremise_Deposit` | Required deposit (gold held in escrow) |
| `m_lPremise_Contribution` | Faction contribution required |
| `m_ulPremise_Task_Count` / `m_ulPremise_Tasks[]` | Previous tasks (success or fail flags encoded in ID) |
| `m_ulMutexTaskCount` / `m_ulMutexTasks[]` | Active tasks that block this one |
| `m_ulPremise_Faction` | Faction membership check |
| `m_ulGender` | Gender restriction |
| `m_ulOccupations` / `m_Occupations[]` | Allowed occupations (up to 45) |
| `m_ulPremise_Period` | Required game-world period |
| `m_ulPremise_Cotask` | Co-task (another player's active task) |
| `m_PremTitles[]` | Required titles |
| `m_nSkillLev[]` | Living-skill level requirements (up to 4 skills) |
| `m_bPremise_Spouse` | Must be married |
| `m_bPremise_FactionMaster` | Must be faction master |
| `m_bPremise_GM` | GM only |
| `m_lPKValueMin/Max` | PK value range |
| `m_nPremBattleScoreMin/Max` | Battle score range |
| `m_nTalismanValueMin/Max` | Talisman value range |
| `m_bInFamily` / `m_bFamilyHeader` | Family membership / leader check |
| `m_bMaster` / `m_bPrentice` | Master/apprentice relationship |
| `m_bCheckLifeAgain` | Requires specific reincarnation count/occupations |
| `m_ulPremCult` | Cultivation level mask |
| `m_ulPremAchievementMin/Max` | Achievement point range |
| `m_ulPremCircleGroupMin/Max` | Circle group points range |
| `m_ulPremTerritoryScoreMin/Max` | Territory score range |
| `m_nPremFengshenType` / `m_ulPremFengshenLvlMin/Max` | Fengshen (ascension) level range |
| `m_iPremTransformID/Level/ExpLevel` | Required transform state |
| `m_ulPremNationPositionMask` | Nation/faction position bitmask |
| `m_nPremKingScoreCost/Min/Max` | King score requirements |
| `m_bPremHasKing` | Kingdom must have a king |
| `m_nBuildId` / `m_nBuildLevel` | Faction building level requirement |
| `m_bPremNeedComp` / `m_PremCompExp1/2` | Global key/value expression comparisons |
| `m_ulPremGlobalCount` | Global finish-count prerequisite |
| `m_bScriptOpenTask` | Task opened by script (not player-initiated) |

Time-window availability:
- `m_ulTimetable` — number of time windows
- `m_tmType[12]` — window types (date / per-month / per-week / per-day)
- `m_tmStart[12]` / `m_tmEnd[12]` — start/end `task_tm` structs
- `m_lAvailFrequency` — repeat frequency type (normal / daily / weekly / monthly / every-N-seconds)
- `m_lTimeInterval` — interval in seconds for `enumTAFEverySeconds`

### Task Behaviour Flags

| Field | Default | Meaning |
|---|---|---|
| `m_bCanGiveUp` | true | Player may abandon |
| `m_bCanRedo` | true | Can re-accept after completion |
| `m_bCanRedoAfterFailure` | true | Can re-accept after failure |
| `m_bNeedRecord` | false | Completion is recorded in finished list |
| `m_bFailAsPlayerDie` | false | Player death = task failure |
| `m_bAutoDeliver` | false | Auto-accepted when conditions met |
| `m_bDeliverWindowMode` | false | Auto-deliver mode: 0=normal, 1=window |
| `m_bDeathTrig` | false | Triggered on player death |
| `m_bManualTrig` | false | Manually triggered (client-side) |
| `m_bClearAcquired` | true | Given items removed on abandon/fail |
| `m_bKeyTask` | false | Appears in key-task panel |
| `m_bHidden` | false | Hidden from UI |
| `m_bFaction` | false | Faction task |
| `m_bSharedByFamily` | false | Shared across family members |
| `m_bLifeAgainReset` | false | Reset on reincarnation |
| `m_bFailAfterLogout` | false | Fails if player logs out |
| `m_ulLogoutFailTime` | 0 | Seconds before failure after logout |
| `m_bPrenticeTask` | false | Apprentice-only task |
| `m_bSkillTask` | false | Listed in skill-task pool |
| `m_bRecFinishCount` | false | Record local finish count |
| `m_bRecFinishCountGlobal` | false | Record global finish count |
| `m_bChooseOne` | false | Execute exactly one child (player's choice) |
| `m_bRandOne` | false | Execute one randomly chosen child |
| `m_bExeChildInOrder` | false | Children execute in order (sequential) |
| `m_bParentAlsoFail` | true | Child failure propagates to parent |
| `m_bParentAlsoSucc` | false | Child success propagates to parent |
| `m_bShareWork` | false | Shared work task (team members each contribute) |

### Completion Methods (`m_enumMethod`)

Defines what the player must do:

| Enum | Value | Description |
|---|---|---|
| `enumTMNone` | 0 | No objective (container/structural node) |
| `enumTMKillNumMonster` | 1 | Kill N specific monsters |
| `enumTMCollectNumArticle` | 2 | Collect N specific items |
| `enumTMTalkToNPC` | 3 | Talk to a specific NPC |
| `enumTMReachSite` | 4 | Reach a specific location |
| `enumTMWaitTime` | 5 | Wait for a fixed duration |
| `enumTMAnswerQuestion` | 6 | Answer a multiple-choice question |
| `enumTMTinyGame` | 7 | Complete a mini-game |
| `enumTMProtectNPC` | 8 | Escort/protect an NPC for a duration |
| `enumTMNPCReachSite` | 9 | Guide NPC to a location |
| `enumTMTitle` | 10 | Collect specific titles |
| `enumTMFixedTime` | 11 | Wait until a specific real-world time |
| `enumTMAchievement` | 12 | Reach an achievement threshold |
| `enumTMLeaveSite` | 13 | Leave a specific location |
| `enumTMFriendNum` | 14 | Have N friends in-game |
| `enumTMNPCAction` | 15 | Trigger a specific NPC action |
| `enumTMSpecialTrigger` | 16 | Special trigger (script/server-driven) |
| `enumTMTotalCaseAdd` | 17 | Spend a certain amount of premium currency |
| `enumTMFactionProperty` | 18 | Reach a faction resource threshold |
| `enumTMInterReachSite` | 19 | Bring an interactive item to a location |
| `enumTMInterLeaveSite` | 20 | Take an interactive item away from a location |
| `enumTMCollectNumInterObj` | 21 | Collect N interactive objects |
| `enumTMBuildingToLevel` | 22 | Upgrade a faction building to a level |

Finish trigger (`m_enumFinishType`): `enumTFTDirect` (auto-finishes) or `enumTFTNPC` (player must return to an NPC).

### Objective Data Fields

Monster killing:
- `m_ulMonsterWanted` / `m_MonsterWanted[]` — up to `MAX_MONSTER_WANTED` (3) monsters, each with: template ID, required kill count, drop item ID/count, drop probability, killer-level flag, common-item flag

Item collection (`enumTMCollectNumArticle` and `enumTMInterObj`):
- `m_ulItemsWanted` / `m_ItemsWanted[]` — up to `MAX_ITEM_WANTED` (8) items, each with: template ID, common/task flag, required count, drop probability, bind flag, period, timetable flag, day-of-week/hour/minute, refine condition/level, replacement item ID
- `m_ulGoldWanted` — gold amount to collect
- Faction resource wants: `m_iFactionGoldNoteWanted`, `m_iFactionGrassWanted`, `m_iFactionMineWanted`, `m_iFactionMonsterCoreWanted`, `m_iFactionMonsterFoodWanted`, `m_iFactionMoneyWanted`

Interactive objects:
- `m_ulInterObjWanted` / `m_InterObjWanted[]` — up to 3 interactive objects
- `m_InterReachSiteMin/Max` / `m_ulInterReachSiteId` / `m_iInterReachItemId` — reach zone for interactive item
- `m_InterLeaveSiteMin/Max` / `m_ulInterLeaveSiteId` / `m_iInterLeaveItemId` — leave zone for interactive item

Site objectives:
- `m_ReachSiteMin/Max` — bounding box
- `m_ulReachSiteId` — world/map ID
- `m_ulWaitTime` — seconds to wait at site
- `m_LeaveSiteMin/Max` / `m_ulLeaveSiteId` — leave-site zone

NPC objectives:
- `m_ulDelvNPC` — NPC that delivers (accepts) the task
- `m_ulAwardNPC` — NPC that gives the reward
- `m_ulNPCToProtect` / `m_ulProtectTimeLen` — NPC protect task
- `m_ulNPCMoving` / `m_ulNPCDestSite` — NPC movement task

Given items (handed to player on accept):
- `m_ulGivenItems` / `m_GivenItems[]` — up to `MAX_ITEM_WANTED` (8) items given at delivery

### Award System

Four award types exist for both success (`_S`) and failure (`_F`):

| Field pair | Type | Description |
|---|---|---|
| `m_Award_S` / `m_Award_F` | `AWARD_DATA*` | Standard flat award |
| `m_AwByRatio_S` / `m_AwByRatio_F` | `AWARD_RATIO_SCALE*` | Award scales with time-ratio (how fast task completed) |
| `m_AwByItems_S` / `m_AwByItems_F` | `AWARD_ITEMS_SCALE*` | Award scales with item count collected |
| `m_AwByCount_S` / `m_AwByCount_F` | `AWARD_COUNT_SCALE*` | Award scales with task-finish count |

The active award type is selected by `m_ulAwardType_S` / `m_ulAwardType_F` (enums `enumTATNormal`, `enumTATEach`, `enumTATRatio`, `enumTATItemCount`, `enumTATFinishCount`).

`AWARD_DATA` fields (partial — the most important ones):

| Field | Meaning |
|---|---|
| `m_ulGoldNum` | Gold reward |
| `m_ulExp` / `m_fExpCoef/2/3` | Experience (base + scale coefficients) |
| `m_ulSP` | Spirit Points |
| `m_ulReputation` | Reputation |
| `m_lContribution` | Faction contribution |
| `m_ulNewTask` | New task to deliver on completion |
| `m_ulTerminateTask[8]` | Tasks to forcibly terminate |
| `m_lTitle` | Title to award |
| `m_lPKValue` | PK value change |
| `m_bResetPKValue` | Reset PK value to 0 |
| `m_bDivorce` | Force divorce |
| `m_ulNewPeriod` | Change game period |
| `m_ulNewRelayStation` | Set new relay station |
| `m_ulStorehouseSize` | Expand storehouse |
| `m_lInventorySize` / `m_lPocketSize` | Expand inventory |
| `m_ulPetInventorySize` | Expand pet bag |
| `m_ulMountInventorySize` | Expand mount bag |
| `m_ulFuryULimit` | Set fury upper limit |
| `m_ulNewProfession` | Change profession |
| `m_ulLifeAgainCnt/Faction` | Reincarnation count/faction |
| `m_ulTransWldId` / `m_TransPt` | Teleport to location |
| `m_lBuffId` / `m_lBuffLev` | Apply a buff |
| `m_ulCandItems` / `m_CandItems[]` | Item award candidates (up to 16 groups) |
| `m_ulSpecialAwardType/ID` | Special/event award |
| `m_bOutMaster` / `m_bDeviateMaster` | End master or apprentice relationship |
| `m_lMasterMoral` | Master moral value change |
| `m_bSetCult` / `m_ulSetCult` | Set cultivation |
| `m_ulClearCultSkill` | Clear cultivation skills |
| `m_nParaExpSel` / `m_pszParaExp` | Parametric expression for variable-reward formulae |
| `m_lChangeKeyArr[]` / `m_lChangeKeyValueArr[]` | Modify global key/value variables |
| `m_lMultiGlobalKey` | Multiply a global variable |
| `m_nSJBattleScore` / `m_bResetSJBattleScore` | Arena battle score |
| `m_iTransformID/Duration/Level/ExpLevel` | Apply a transform |
| `m_ulFengshenExp` / `m_bFengshenDujie` | Fengshen experience / trigger tribulation |
| `m_ulBonusNum` | Bonus currency |
| `m_ulBattleScore` | Battle score (general) |
| `m_bOpenSoulEquip` | Unlock soul equipment slot |
| `m_nKingScore` | Nation king score |
| `m_iPhaseCnt` / `m_Phase[8]` | Open/close phase objects |
| `m_iAuctionCnt` / `m_Auction[8]` | Auction item delivery (probability-based) |
| `m_iFactionGoldNote/Grass/Mine/MonsterCore/MonsterFood/Money` | Faction resource awards |
| `m_iTravelItemId/Time/Speed/Path` | Trigger a travel sequence |
| `m_lVariable[3]` | Variables passed to `SendMessage` |
| `m_bCheckGlobalFinishCount` | Gate extra award on global finish count |
| `m_GlobalCompareExpression` | Global expression guard for extra award |
| Extra-award fields (`m_ulExtraCandItems`, `m_ExtraMonCtrl[]`, `m_bSendExtraMsg`, etc.) | Secondary award for special conditions |

`AWARD_ITEMS_CAND` wraps a list of `ITEM_WANTED` entries plus a `m_bRandChoose` flag (choose one randomly vs. give all).

### Time-Limit and Failure Conditions

| Field | Meaning |
|---|---|
| `m_ulTimeLimit` | Seconds to complete (0 = no limit) |
| `m_bAbsTime` | Time limit is absolute (counts from server boot) not relative |
| `m_bFailAsPlayerDie` | Death causes failure |
| `m_bOutZoneFail` | Leaving a zone causes failure |
| `m_ulOutZoneWorldID` / `m_OutZoneMinVert/MaxVert` | The failure zone |
| `m_bEnterZoneFail` | Entering a zone causes failure |
| `m_bKillMonsterFail` / `m_KillFailMonsters[]` | Killing certain monsters causes failure |
| `m_bHaveItemFail` / `m_HaveFailItems[]` | Having certain items causes failure |
| `m_bNotHaveItemFail` / `m_NotHaveFailItems[]` | Not having certain items causes failure |
| `m_bAbsFail` / `m_tmAbsFailTime` | Fails at a specific absolute time |
| `m_bFailAfterLogout` / `m_ulLogoutFailTime` | Fails if offline too long |

---

## Task Tree Structure

Tasks form a tree:

- **Root task** — a top-level task (has no parent). Stored in `m_TaskTemplMap`.
- **Sub-task** — a child node. Stored only in `m_AllTemplMap`.

Tree execution modes (`ATaskTemplFixedData`):
- `m_bExeChildInOrder = true` → children execute sequentially; a child must complete before the next starts.
- `m_bChooseOne = true` → player selects exactly one child to execute.
- `m_bRandOne = true` → server randomly selects one child.
- All-false → all children execute in parallel.

`m_uDepth` is computed recursively at load time; sequential/choose/rand modes take the max child depth; parallel mode sums children depths. This determines how many `ActiveTaskEntry` slots a task tree will consume.

In `ActiveTaskList`, the tree is flattened into `m_TaskEntries[]`. The `m_ParentIndex`, `m_ChildIndex`, `m_PrevSblIndex`, `m_NextSblIndex` fields in each entry reconstruct the tree topology at runtime using array indices rather than pointers.

---

## Prerequisite Check Functions

All defined on `ATaskTempl` (server-side non-inline path via `TaskTempl.inl`). Each returns `0` on success or one of the `TASK_PREREQU_FAIL_*` constants on failure.

The master prerequisite check is `CheckPrerequisite(pTask, pList, ulCurTime, bCheckPrevTask, bCheckTeam, bCheckBudge, bStorageRefresh)`. It calls the individual `CheckXxx()` methods in a defined order:

`CheckFnshLst` → `CheckBudget` → `CheckLevel` → `CheckTalismanValue` → `CheckConsumeTreasure` → `CheckPremTotalTreasure` → `CheckRepu` → `CheckFriendship` → `CheckDeposit` → `CheckContrib` → `CheckItems` → `CheckFaction` → `CheckGender` → `CheckOccupation` → `CheckPeriod` → `CheckTeamTask` → `CheckMutexTask` → `CheckInZone` → `CheckGivenItems` → `CheckLivingSkill` → `CheckPetConAndCiv` → `CheckPreTask` → `CheckTimetable` → `CheckDeliverTime` → `CheckSpecialAward` → `CheckPKValue` → `CheckGM` → `CheckLifeAgain` → `CheckTitle` → `CheckSpouse` → `CheckFamily` → `CheckFinishCount` → `CheckOpenCount` → `CheckPremFinishCount` → `CheckBattleScore` → `CheckSJBattleScore` → `CheckMasterPrentice` → `CheckCultivation` → `CheckAccomp` → `CheckTransform` → `CheckAchievement` → `CheckTerritoryScore` → `CheckFengshen` → `CheckCreateRoleTime` → `CheckNationPosition` → `CheckKingScore` → `CheckHasKing` → `CheckBuildLevel` → `CheckBuildLevelCon` → `CheckFactionTask` → `CheckFactionGoldNote` → `CheckFactionGrass` → `CheckFactionMine` → `CheckFactionMonsterCore` → `CheckFactionMosnterFood` → `CheckFactionMoney` → `CheckInterObj` → `CheckGlobalExpressions`

---

## Notification System

### Server → Client Notifications (`TASK_SVR_NOTIFY_*`)

Sent via `TaskInterface::NotifyClient()` to the player's client connection.

| Code | Value | Meaning |
|---|---|---|
| `TASK_SVR_NOTIFY_NEW` | 1 | New task delivered |
| `TASK_SVR_NOTIFY_COMPLETE` | 2 | Task completed (success/fail determined by sub-state) |
| `TASK_SVR_NOTIFY_GIVE_UP` | 3 | Task abandoned |
| `TASK_SVR_NOTIFY_MONSTER_KILLED` | 4 | Monster kill progress update |
| `TASK_SVR_NOTIFY_FINISHED` | 5 | Task reached finished state (ready to claim award) |
| `TASK_SVR_NOTIFY_ERROR_CODE` | 6 | Prerequisite error code to show player |
| `TASK_SVR_NOTIFY_FORGET_SKILL` | 7 | Living skill forgotten (task impact) |
| `TASK_SVR_NOTIFY_DYN_TIME_MARK` | 8 | Dynamic task time mark |
| `TASK_SVR_NOTIFY_DYN_DATA` | 9 | Dynamic task data chunk |
| `TASK_SVR_NOTIFY_SPECIAL_AWARD` | 10 | Special/event award notification |
| `TASK_SVR_NOTIFY_STORAGE` | 11 | Storage pool refresh |
| `TASK_SVR_NOTIFY_FINISH_COUNT` | 12 | Finish count update |
| `TASK_SVR_NOTIFY_CLEAR_LIFEAGAIN_FIN_REC` | 13 | Clear reincarnation finish records |
| `TASK_SVR_NOTIFY_ITEMS_TO_GET` | 14 | Item collection progress update |
| `TASK_SVR_NOTIFY_CAMERA_MOVE` | 15 | Trigger camera movement |
| `TASK_SVR_NOTIFY_RM_FINISH_TASK` | 16 | Remove a task from finished list |
| `TASK_SVR_NOTIFY_ADD_FINISH_TASK` | 17 | Add a task to finished list |
| `TASK_SVR_NOTIFY_CLEAR_STATE` | 18 | Clear a task's state |
| `TASK_SVR_NOTIFY_INTEROBJ_COLLECT` | 19 | Interactive object collection progress |

### Client → Server Notifications (`TASK_CLT_NOTIFY_*`)

| Code | Value | Meaning |
|---|---|---|
| `TASK_CLT_NOTIFY_CHECK_FINISH` | 1 | Client requests award (player at NPC) |
| `TASK_CLT_NOTIFY_CHECK_GIVEUP` | 2 | Client requests give-up |
| `TASK_CLT_NOTIFY_REACH_SITE` | 3 | Client reached the target site |
| `TASK_CLT_NOTIFY_AUTO_DELV` | 4 | Auto-deliver trigger |
| `TASK_CLT_NOTIFY_MANUAL_TRIG` | 5 | Manual trigger (script or button) |
| `TASK_CLT_NOTIFY_FORCE_GIVEUP` | 6 | Force give-up from server instruction |
| `TASK_CLT_NOTIFY_DYN_TIMEMARK` | 7 | Dynamic task time-mark from client |
| `TASK_CLT_NOTIFY_DYN_DATA` | 8 | Dynamic task data from client |
| `TASK_CLT_NOTIFY_SPECIAL_AWARD` | 9 | Special award trigger |
| `TASK_CLT_NOTIFY_STORAGE` | 10 | Storage pool data |
| `TASK_CLT_NOTIFY_OUT_ZONE` | 11 | Player left the task zone |
| `TASK_CLT_NOTIFY_GIVEUP_AUTO` | 12 | Auto give-up |
| `TASK_CLT_NOTIFY_ENTER_ZONE` | 13 | Player entered a zone |
| `TASK_CLT_NOTIFY_LEAVE_SITE` | 14 | Player left a site |
| `TASK_CLT_NOTIFY_MANUAL_TRIG_STORAGE` | 15 | Manual trigger for storage task |

### Server → Player Notifications (`TASK_PLY_NOTIFY_*`)

Cross-player notifications (e.g. to a team member's `PlayerTaskInterface`):

| Code | Value | Meaning |
|---|---|---|
| `TASK_PLY_NOTIFY_NEW_MEM_TASK` | 1 | New team member task |
| `TASK_PLY_NOTIFY_FORCE_FAIL` | 2 | Force all members to fail |
| `TASK_PLY_NOTIFY_FORCE_SUCC` | 3 | Force all members to succeed |
| `TASK_PLY_NOTIFY_LEAVE_FAMILY` | 4 | Team member left the family |
| `TASK_PLY_NOTIFY_M2P_NEW_TASK` | 5 | Master-to-apprentice new task |
| `TASK_PLY_NOTIFY_P2M_REC_SUCC` | 6 | Apprentice task accepted successfully |
| `TASK_PLY_NOTIFY_P2M_TASK_FAIL` | 7 | Master/apprentice task failed |
| `TASK_PLY_NOTIFY_P2M_LEAVE_TEAM` | 8 | Left master/apprentice team |

---

## Event Callbacks (`TaskServer.h`)

All functions defined in `TaskServer.cpp`. Called by other game systems when events occur:

| Function | Trigger |
|---|---|
| `OnTeamSetup(pTask)` | Player forms a team |
| `OnTeamDisband(pTask)` | Team disbands |
| `OnTeamAddMember(pTask, pInfo)` | Member joins team |
| `OnTeamDismissMember(pTask, ulPlayerId)` | Member kicked |
| `OnTeamMemberLeave(pTask)` | Member leaves voluntarily |
| `OnTeamCaptainChange(pTask)` | Captain changes |
| `OnPlayerNotify(pTask, ulPlayerId, pBuf, sz)` | Cross-player notification received |
| `OnClientNotify(pTask, pBuf, sz)` | Client notification received |
| `OnTaskCheckDeliver(pTask, ulTaskId, ulStorageId)` | Check if task can be delivered |
| `OnNPCDeliverTaskItem(pTask, ulNPCId, ulTaskId)` | NPC gives task item to player |
| `OnTaskCheckAward(pTask, ulTaskId, nChoice)` | Player requests award at NPC |
| `OnTaskPlayerKilled(pTask)` | Player was killed |
| `OnTaskCheckAllTimeLimits(ulCurTime)` | Periodic sweep for expired tasks |
| `OnTaskCollectInterObj(pTask, ulTemplId)` | Player collected interactive object |
| `OnTaskInterReachSite(pTask, ulInterObjId)` | Player+object reached target site |
| `OnTaskInterLeaveSite(pTask, ulInterObjId)` | Player+object left target site |
| `OnTaskKillMonster(pTask, ulTemplId, ulLev, fRand)` | Player killed a monster (solo) |
| `OnTaskTeamKillMonster(pTask, ulTemplId, ulLev, fRand)` | Player killed a monster (team credit) |
| `OnTaskShareKillMonster(pTask, ulTemplId, ulLev, fRand)` | Shared kill credit |
| `OnTaskMining(pTask, ulTaskId)` | Player mined a node |
| `TaskQueryGlobalData(ulTaskId, ulPlayerId, pPreservedData, sz)` | Query global task data from DB |
| `TaskUpdateGlobalData(ulTaskId, pData)` | Write global task data to DB |
| `OnTaskReceivedGlobalData(pTask, ulTaskId, pData, pPreservedData, sz)` | Async response from DB |
| `OnTaskReceiveFamilyData(pTask, pData, nSize, nRetCode)` | Async family data response |
| `OnTaskPlayerJoinFamily(pTask)` | Player joined a family |
| `OnTaskPlayerLeaveFamily(pTask)` | Player left a family |
| `OnForgetLivingSkill(pTask)` | Living skill forgotten |
| `OnTaskPlayerLevelUp(pTask)` | Player levelled up |
| `OnTaskCheckState(pTask)` | Periodic per-player state check |
| `OnTaskReceiveCircleGroupData(pTask, pData, nSize, nRetCode)` | Async circle group data |
| `OnTaskReceiveFriendNumData(pTask, ulTaskId, ulFriendNum)` | Async friend count response |
| `OnTaskProtectNPCNotify(pTask, ulTaskId, nReason)` | NPC protect outcome (success/die/too-far/timeout) |
| `OnUpdateFinishTaskList(pTask, ulTaskId, param1, param2)` | Debug: modify finished list |
| `OnUpdateFinishTaskCount(pTask, ulTaskId, nCount)` | Debug: set finish count |
| `OnDeliverTaskForDebug(pTask, ulTaskId, param)` | Debug: force-deliver a task |
| `OnUpdateClearTaskState(pTask, ulTaskId)` | Debug: clear task state |
| `OnTaskPlayerLeaveFamilyHeader(pTask)` | Player lost family leader position |
| `OnTaskPlayerLeaveFactionHeader(pTask)` | Player lost faction leader position |

---

## Template Manager (`ATaskTemplMan`)

Singleton accessed via `GetTaskTemplMan()`. Manages all template lifetime and provides lookup.

Key maps:

| Map | Key | Contents |
|---|---|---|
| `m_TaskTemplMap` | task ID | Top-level `ATaskTempl*` only |
| `m_AllTemplMap` | task ID | Every `ATaskTempl*` (including sub-tasks) |
| `m_AutoDelvMap` | task ID | Tasks with `m_bAutoDeliver = true` |
| `m_DeathTrigMap` | task ID | Tasks with `m_bDeathTrig = true` |
| `m_ProtectNPCMap` | NPC template ID | Tasks that protect that NPC |
| `m_SkillTaskLst` | — | `vector` of skill/living-skill tasks |
| `m_Storages[9][]` | slot index | Vectors of task IDs in each storage pool |
| `m_StorageTaskMap` | task ID | Reverse lookup: is this a storage task? |
| `m_ForbiddenTaskMap` | task ID | Tasks blocked by the forbidden list |
| `m_DynTaskMap` | task ID | Dynamically loaded live-event tasks |
| `m_TmLmtChkLst` | — | Templates with `m_ulTimeLimit > 0`, for periodic sweeping |

Loading sequence for server:
1. `LoadTasksFromPack(pDataMan, szPackPath, szDynPackPath, szForbiddenTaskPath, szNPCInfoPath)` (in `TaskServer.cpp`)
2. Calls `ATaskTemplMan::LoadTasksFromPack()` for static tasks
3. Calls `ATaskTemplMan::LoadDynTasksFromPack()` for dynamic tasks
4. Calls `ATaskTemplMan::LoadForbiddenTasks()` for the forbidden list
5. Calls `ATaskTemplMan::LoadNPCInfoFromPack()` for NPC position data

Storage pools (9 slots) are sets of tasks from which one is randomly selected per slot and offered to the player. Storage tasks are refreshed periodically via `UpdateStorage()`. The 9th slot can optionally be skipped on refresh (used for Tai Chi tasks).

---

## Async Data Helpers (`TaskAsyncData.h`)

Two classes handle serialisation for async DB round-trips:

### `TaskFamilyAsyncData`

Used when querying/updating family-related task data (shared tasks, skill info, monster records, common values). Serialises as: header struct (family ID, reason code, task ID, task-reason, main key, pair count) followed by N `TaskPairData` (key/value1/value2 triples).

Reason codes (`enumTaskFamilyGetSkillInfo` through `enumTaskFamilyAddCommonValue`): 14 variants covering read/write of skill info, monster info, shared task lists, finished task lists, monster records, and common values.

### `TaskCircleGroupAsyncData`

Used for querying circle group (guild ring) membership data. Fixed-size struct: circle group ID, min/max level, point threshold, task ID, current online count, max member count.

---

## Expression System (`TaskExpAnalyser`)

A lightweight recursive-descent parser (`Analyse()`) that compiles arithmetic expression strings into `TaskExpressionArr` (vector of `TASK_EXPRESSION` tokens). Tokens are either variables (`enumTaskExpVar`), constants (`enumTaskExpConst`), or operators (`enumTaskExpOprt`).

`Run(pTask, aExp, nCalcType)` evaluates a compiled expression using either the player's level (`enumTaskCalcExp`) or a global variable value (`enumTaskCalcGlobalVal`) as the variable. Used for parametric reward formulae (`m_pszParaExp` in `AWARD_DATA`) and global expression conditions.

---

## Key Limits and Constants

| Constant | Value | Meaning |
|---|---|---|
| `TASK_MAX_SIMULTANEOUS_COUT` | 50 | Max active tasks a player can hold |
| `TASK_ACTIVE_LIST_MAX_LEN` | 120 | Entries in `ActiveTaskList` |
| `TASK_FINISHED_LIST_MAX_LEN` | 2040 | Entries in `FinishedTaskList` |
| `TASK_FINISH_TIME_MAX_LEN` | 1700 | Entries in `TaskFinishTimeList` |
| `TASK_ACTIVE_LIST_BUF_SIZE` | ~3900 bytes | Total serialised size of active list |
| `TASK_FINISHED_LIST_BUF_SIZE` | 4096 bytes | Total serialised size of finished list |
| `TASK_GLOBAL_DATA_SIZE` | 256 bytes | Global task data blob |
| `TASK_FINISH_TIME_LIST_BUF_SIZE` | 20480 bytes | Finish time list buffer |
| `TASK_DATA_BUF_MAX_LEN` | 32 bytes | `ActiveTaskEntry` total size |
| `MAX_MONSTER_WANTED` | 3 | Monster objectives per task |
| `MAX_ITEM_WANTED` | 8 | Item objectives per task |
| `MAX_ITEM_AWARD` | 32 | Items in an award list |
| `MAX_AWARD_CANDIDATES` | 16 | Candidate item groups in an award |
| `MAX_AWARD_SCALES` | 5 | Brackets for ratio/item/count awards |
| `MAX_MONSTER_SUMMONED_CAND` | 32 | Summon candidates for prerequisite summoning |
| `MAX_PREM_TASK_COUNT` | 5 | Previous-task prerequisites |
| `MAX_MUTEX_TASK_COUNT` | 5 | Mutually exclusive tasks |
| `MAX_TEAM_MEM_WANTED` | 8 | Team member requirements |
| `MAX_LIVING_SKILLS` | 4 | Living-skill prerequisites |
| `MAX_TITLE_WANTED` | 5 | Title prerequisites |
| `MAX_MPTASK_COUNT` | 8 | Master/apprentice task conditions |
| `MAX_AWARD_SEL_ROLE` | 32 | Roles that can receive a directed award |
| `MAX_PREM_SEL_CULT` | 8 | Cultivation prerequisites |
| `MAX_AWARD_MON_CTRL` | 8 | Monster controller awards |
| `MAX_KILLFAILMONSTERS` | 8 | Monsters whose kill triggers failure |
| `MAX_FAILITEMS` | 16 | Items whose possession triggers failure |
| `MAX_VARIABLE_NUM` | 3 | Variables in award message |
| `MAX_PHASE_COUNT` | 8 | Phase objects per award |
| `MAX_FACTION_TASK_COUNT` | 10 | Faction task count per day |
| `MAX_AUCTION_COUNT` | 8 | Auction award items |
| `TASK_STORAGE_COUNT` | 9 | Storage pool slots |
| `TASK_STORAGE_LEN` | 10 | Tasks per storage slot |
| `TASK_ZONE_FRIENDSHIP_COUNT` | 32 | Zone friendship values |
| `MAX_OCCUPATIONS` | 45 | Max occupation types |
| `STORAGE_TASK_TYPES` | 21 | Types of storage tasks |

---

## Build Output

The Makefile produces `.o` object files that are linked directly into `zgame/gs/gs` by the parent `Makefile`. An optional `lib` target can produce `libtask.so`, but this is not part of the normal server build. All files are compiled with `-fPIC` regardless.

---

## Known Issues / Areas to Investigate

- The `m_ulTemplAddr` and `m_ulCapTemplAddr` in `ActiveTaskEntry` are raw pointers cast to `unsigned long`. On 64-bit Linux this is a latent bug since `unsigned long` is 64-bit but the struct is packed — any serialisation that writes these fields to disk or the network will break on a 64-bit build. However, these fields are not persisted (they are repopulated at load from the template maps), so this is only a concern for in-memory cross-process communication.

- `TASK_ACTIVE_LIST_MAX_LEN` was recently bumped from 80 to 120 (comment: "Modified from 50 → 80 2012-04-18", then again). The corresponding client-side buffer size must match exactly; a mismatch causes silent data corruption.

- `TASK_MAX_SIMULTANEOUS_COUT` is 50 but `TASK_ACTIVE_LIST_MAX_LEN` is 120 — sub-tasks of a deeply nested tree consume the remaining 70 slots. `CheckBudget()` enforces the 50-top-task limit; `ActiveTaskList::IsValid()` enforces the 120-entry hard cap.

- Storage pool task selection (9 slots, refresh via `UpdateStorage`) includes a special case: the 9th slot (`index == 8`) is never refreshed when the function is called from the time-limit sweep — only from explicit per-player checks. This prevents Tai Chi (太极) slot tasks from resetting unexpectedly during server tick.
