# SDK / small-TU matching — field notes

Branch: `sync-doldecomp`. Scope: matching whole SDK (`lib/rvl`, `lib/MSL`, `lib/egg`,
`lib/gamespy`, `lib/runtime`, `lib/nw4r`) functions and completing small units.
All changes source-only; full `ninja` build stays green.

## Result of this session

- SDK matched **86.57% → 87.19%**; matched functions **1855 → 1908** (+53).
- Whole units completed: `rvlAxFx`, `MSL/src/float`, `rvlAx`,
  `gamespy/ghttp/ghttpCommon`, `runtime/__start`.
- The entire `rvlAx*` family is now effectively fully matched
  (`rvlAxAlloc`, `rvlAxVpb`, `rvlAxCl`, `rvlAxAux`, `rvlAxSpb`, `rvlAxComp`,
  `rvlAxFx`). 5 `Enemy::AI` methods fixed too.

### Commits (oldest → newest)
```
50b8196  rvlAxFx, MSL/src/float, rvlAx (100%); eggQuat::normalise + WIP
7d5ef38  11 AX accessors
7269cfa  rvlAxAlloc stack ops
251bb51  AXRmtGetSamplesLeft
8d53968  AXSetMasterVolume + AXSetAux{A,B,C}ReturnVolume
48221d0  __AXGetAuxA{Input,Output,InputDpl2,OutputDpl2R,Ls,Rs}
3f78f0c  __AXGetAux{B,C}{Input,Output,...}
f0b09ae  AX{A,B,C} Aux callback getters/registers
acd9479  ghttpCommon complete (SDA21 symbol fix)
f0e351a  runtime/__start complete (InitMetroTRK symbol fix)
6b4520d  KartObjectProxy::isCpu const; AI::isSameTeam(AI*)
986e4c4  AI::getStick; remove spurious non-const KartObjectProxy::getInput
```

## Root-cause patterns that pay off (both exhausted as of now)

1. **Hardcoded address where retail uses a named symbol, inside inline asm.**
   - `ghiDoReceive`: `lwz r0, -30244(r13)` → `lwz r0, ghiThrottleTimeDelay(r13)`.
   - `__start`: `lis r6, 0x8002; addi r6, r6, -15540` →
     `lis r6, InitMetroTRK@ha; addi r6, r6, InitMetroTRK@l` (and declare
     `extern void InitMetroTRK(void);`).
   - Diagnose with `objdiff` **reloc targets**, not raw instruction bytes
     (the mismatching insns render as `bl 0x0` / `lwz r0, 0(0)`).

2. **A spurious non-const overload shadowing retail's `const` one.**
   - `KartObjectProxy` declared both `isCpu()` / `isCpu() const`, and
     `getInput()` (`System::KPad*`) / `getInput() const` (`KartInput&`).
     Retail ships **only the `const` forms**. Deleting the non-const decls (and
     the `getInput()` definition) flipped call sites from `...Fv` to the
     matching `...CFv` and completed 5 `Enemy::AI` functions at once.
   - Related: wrong signature/const-ness changes the mangled name →
     function shows as `None` (retail-only). `AI::isSameTeam` takes `AI*` (not
     `const AI&`); `AI::getStick()` is non-const.

## Tools

- `build/mwcc-debug/tu.py <tu> [sym-substr ...]` — compiles a TU with its exact
  ninja command and objdiff's each function vs the retail object. Patched to
  also find targets under `build/RMCP01/<Module>/obj/...` (e.g. `StaticR`).
- `build/mwcc-debug/sigs.py [tu-filter]` — per unit, diffs FUNC symbol sets
  (retail vs ours); flags retail-only (missing/mis-signed) and ours-only
  (extra/mis-signed). No remaining candidates of class (2).
- `build/mwcc-debug/dis_count.py <obj> <sym>` — dump one function's insns.
- `build/mwcc-debug/compare_two.py` — index-by-index opcode+operand diff.
- Inline diff snippet (retail .o vs ours .o):
  `python -c "import sys;sys.path.insert(0,r'build/mwcc-debug');import dis_count as D; ..."`
  (load both, `find_symbol`, `disasm`, compare `mnemonic+' '+op_str`).

Mine near-complete units from `build/RMCP01/report.json` (regenerated each
build): sort `units` by `measures.fuzzy_match_percent` where
`total_functions - matched_functions` is small.

## Remaining tail (hard; needs focused RE, not quick wins)

- **Unimplemented large functions:** `WheelPhysicsHolder::applySuspensions`
  (~276 insns, `kart/KartWheelPhysics`), `NandFileStream::Open`
  (`nw4r/ut/ut_nandFileStream`), `RaceConfig::Scenario::initControllers`
  (168 insns, currently `#ifdef NON_MATCHING`),
  `RaceConfig::Scenario::copyPrevPositions`, `DMAErrorHandler` /
  `__OSCacheInit` (need retail rodata string blob `@878_8028F0E8`).
- **Allocator / scheduler ties** (identical logic, compiler picks the other
  register/order — no source perturbation found so far):
  `AI::handleBump` (r30/r31, 94.4%), `AIControlDemo::initAfterManager`
  (r5/r0, 80%), `__OSUnmaskInterrupts` (61.9%; scheduling).
- **Vtable / struct RE:** `eggHeap::dumpAll` (59%), `eggExpHeap::create`
  (56%), `eggQuat` remainder (`setAxisRotation` 92%, `inverse` 50%).

## Session 2 update — `system/NandUtil` (4/15 → 12/15)

Every NAND wrapper shares one template: a 3-iteration loop with a `switch` on the
return code, a `break`/retry flag, and `OSSleepTicks(OSMillisecondsToTicks(100ll))`.
Cloned it for the 8 missing ones:

- `NandUtil_delete`, `create` (`NANDCreate(a1,a2,0)`; cases 0/-6→0, -4→6, -1→3),
  `createDir` (`NANDCreateDir(a1,a2,0)`; same cases),
  `getStatus` (`NANDGetStatus`, 2nd arg `NANDStatus*`), `setStatus`
  (`NANDSetStatus`), `getType` (`NANDGetType` + out enum; **case order matters** —
  the `-12` body must precede `-3/-2`), `check` (`NANDCheck(u32,u32,u32*)`; only
  0 / retry / default), `getLength` (`NANDGetLength`).
- All are `extern "C"` → every new function needs a declaration inside the
  `extern "C" { }` block in `NandUtil.hpp`, else it mangles (`NandUtil_x__F...`)
  and shows as `None`.

Remaining:
- `NandUtil_read` / `NandUtil_write`: **two** retry loops (`NANDSeek` then
  `NANDRead`/`NANDWrite`), logic reproduced correctly but the compiled saved-GPR
  range differs (r21 vs r22 base) → every register renumbered (52.5%). An
  allocator tie; not yet cracked.
- `fn_1_1B984` corresponds to the already-100% `@524` jump table.

## Session 3 — exhausted the "cheap" veins

- Ran a systematic signature/const scan (retail-only vs ours-only FUNC symbols
  sharing a base name, across every unit). Result: **no more fixable pairs**;
  the only hits are false positives plus:
  - `RKSystem`: ours emits `getSysHeap__Q23EGG131TSystem<...>Fv`, retail wants
    `getSysHeap__Q23EGG10BaseSystemFv` (template-vs-base qualification). File is
    explicitly labelled NonMatching (see its header comment) — skip.
  - `field/RKGndCol::searchMultiBlockRecursive*`: source comments say
    "regswap+inswap" (allocator tie); retail mangling is on `KCollisionOctree`.
  - `runtime/eabi`: `__div2u/__div2i/__mod2u/__mod2i/__shl2i/__shr2u/
    __cvt_ull_flt/__cvt_dbl_ull/__save_gpr/__restore_gpr` are MWCC runtime
    helpers — matching them means reproducing the compiler runtime, not RE.
- Conclusion: the remaining unmatched functions fall into exactly three buckets
  — (a) allocator/scheduler ties, (b) large unimplemented RE, (c) known
  NonMatching TUs. No further mechanical wins.

## Session 4 — biggest gap: `system/KPadController` (~16 KB, 112 fns)

Largest absolute gap in the tree. Ours had ~21 fns; **~20 more now match** (trivial
virtuals + getters). It's the highest-yield target because most missing fns are
tiny overrides. Patterns:

- Base virtuals that are no-ops: `setDpdEnabled`, `startMotor`, `stopMotor`,
  `stopMotorHard`, `setDriftIsAuto`, `setInputState` → `{}` (`blr`).
- Constant returns: `getBatteryLevelAsync()->2`, `getChan()->-1`,
  `isRumbleEnabled()->false`, `isDpdEnabled()->false`,
  `KPadGhostController::hasGhostBuffer()-> mGhostBuffer != nullptr`.
- Member getters: `KPad{GC,Wii}Controller::getChan`, `getRecordedBuffer(Size)`.
- Tail-calls: `KPadGCController::{start,stop,stopMotorHard}Motor()` →
  `PADControlMotor(mChan, {1,0,2})`; `KPadWiiController::...` → `WPADControlMotor`;
  `KPad::startMotor/stopMotor/stopMotorHard` → `mController-><same>()` (virtual).
- `KPadWiiController::stopMotorHard()` → `stopMotor()` (virtual dispatch).
- `KPadWiiController::isRumbleEnabled()` → `mType == 2`.

Gotchas:
- `WPADControlMotor(s32,u32)` was missing from `lib/rvl/wpad/wpad.h` (which had only
  structs) — added.
- `KPadGhostController::hasGhostBuffer` is **non-const** in retail (header had
  `const` → mangled `...CFv`).
- `KPadGCController::getControlSource()` returns **3**, `KPadGhostController`'s
  returns **4** — our `eControlSource` enum (GAMECUBE=4, GHOST=5) is off by one for
  these; don't guess, verify against the constant.

Still None (~75): the `KPad*Stream` classes (`getSequenceSize` → 2, need their own
header), the constructors/`calc`/`calcInner`, `raceToUIInput*`, `updateBattery*`.

### Base-ctor attempt (`KPadController::KPadController()`)

Reconstructed as an init-list; reached **66.7%** (63 vs 59 insns) with
`: mIsConnected(false), mBattery(7), mOutOfBattery(false)`. Full store sequence
now matches byte-for-byte (all offsets/values identical). The **only** residual
is the input-state flag bytes:

- `mRaceInputState` +0x14 (`0x18`): both emit `rlwinm …,0x19,0x17` — matches.
- `mUIInputState` +0x30 (`0x4c`) and `mPrevUIInputState` +0x30 (`0x8c`):
  **retail ctor emits one `rlwinm …,0x1a,0x17`; ours emits two**
  (`…,0x19,0x17` early + `…,0x1a,0x18` late) because our
  `KPadUIInputState::reset()` clears `mFlags_0` and `mFlags_1` far apart, so the
  inliner can't coalesce them.
- Moving the two flag clears adjacent in `reset()` **breaks the standalone**
  `reset()` (100%→73.9%) — the standalone *needs* them apart.
- Conclusion: retail's **member constructor is not `{ reset(); }`** (the ctor
  clears a single flag → `0x1a,0x17`, `reset()` clears two). Fix = give
  `KPadUIInputState()` its own body (init fields + clear one flag) distinct from
  `reset()`. That's the last hurdle; then 7 subclass ctors + 8 `__dt__` cascade.
  Members at 0x04/0x1c/0x5c; scalars `mIsConnected=false`(0x50),
  `mBattery=7`(0x54), `mOutOfBattery=false`(0x58).

## Stream classes (RE'd, not yet implemented)

Retail declares a hierarchy we don't have at all (only a forward-decl in
`KPadController.hpp:14`): base `KPadGhostButtonsStream` + subclasses
`KPadFaceButtonsStream`, `KPadTrickButtonsStream`, `KPadDirectionButtonsStream`.
They live in the `KPadController` TU. Derived from the asm:

- **Layout** (from `readFrame`/`writeFrame`/`readGhostBuffer`):
  `0x0` vptr, `0x4` `u8* mBuffer`, `0x8` `s32 mFrame`, `0xc` `u32 mSize`,
  `0x14` `s32 mState` (0 = write, 1 = read).
- **Vtable slots used:** `+0x0c` `readFrame()`, `+0x10` `getSequenceSize()`
  (pure in base; all three subclasses `return 2;`), `+0x14` `writeFrame(u8)`,
  `+0x24` a predicate called inside `readFrame`.
- **`readFrame()`** (base impl): returns 0 unless `mState==1`; reads `mFrame`;
  if `<0` → `mFrame=0`; else `buf = mBuffer + mFrame*getSequenceSize()`; calls the
  `+0x24` predicate on that pointer; on true → `mFrame++`, wraps when
  `mFrame*getSequenceSize() >= mSize`. `writeFrame(u8)` is the symmetric
  state==0 path.
- **`KPadGhostController`** (`this+0x94` = `KPadGhostButtonsStream* mButtonsStreams[3]`,
  `this+0x90` = `u16* mGhostBuffer`, `this+0xa6` = `_a6`):
  - `readGhostBuffer(u16*, bool)`: sets `mDriftIsAuto=isAuto` (0x51),
    `mGhostBuffer=data` (0x90), then for each of 3 streams:
    `stream->mBuffer = data + 8 + Σ(buttons*getSequenceSize())` (walks the buffer).
  - `getStreamSize()`: 0 if `!this->_a6`; else `Σ stream->mFrame *
    stream->getSequenceSize()`.

Landing this needs the 4 classes, their vtables (must match retail's data
symbols), and ~9 methods together — a self-contained chunk, not incremental.
No speculative classes were added (they'd emit mismatched vtables).

## Session 5 — leaf-function sweep (highest-yield mode)

The single most productive technique: sweep **leaf functions** (no float math, no
multiple live constants) across units and match them with `extern "C"` + `void*`.

Recipe:
- Constants/empties: `int fn_1_XXXX() { return N; }`, `void ...() {}`.
- Field/index getters: `*(u32*)((u8*)self + OFF)`; indexed
  `*(u32*)((u8*)self + i*4 + OFF)`.
- Pointer chains: `a=*(u8**)(self+0); b=*(u8**)(a+OFF); return *(u8**)(b+OFF);`.
- `lbl_1_data_*` / `lbl_1_rodata_*` constants → `extern "C"` externs.
- Deleting destructors: `if (self && flags>0) ::operator delete(self); return self;`.
- Unmangled `*_*` symbols (e.g. `PlayerSub10_*`) are C-style → define as
  `extern "C"` with `void* self`; mangled `__Q...` symbols need the class.
- Empty/unimplemented TUs (`ROOM.cpp`, `USER.cpp`, `KartSus.cpp`, …) → write
  `namespace Net {}` plus an `extern "C" { … }` block.

Results (all source-only, each verified with `tu.py` before commit):
- `KartObjectProxy` ~90→111/203, `KartMove` 0→25, `KartCollide` +15,
  `RaceManager` +15, `ui/Page` +5, `KartSus` +3, `CourseMap` +1,
  `net/packets/{ROOM,SELECT,USER,ITEM,EVENT}` +17, `net/NetManager` +2.

Hard limits (avoid — low hit rate):
- float math / multiple live constants → MWCC allocator/scheduler tie
  (e.g. `PlayerSub10_updateHopPhysics`, `PlayerBoost_reset`) — identical logic,
  different registers/schedule.
- enum-range optimization (MWCC drops `>=0` checks).
- functions calling still-unimplemented symbols → undefined refs break the link.
- dead-stripped virtuals (no instantiated vtable).

Final metrics after this session: **2057 / 26639** functions matched (was 1908),
SDK 87.19%, build green.

## Resume checklist

1. `git log --oneline -12` — confirm HEAD at `986e4c4` and tree clean.
2. `ninja` — confirm green; report.json refreshes for mining.
3. Pick a target above; use `tu.py` to score and the inline diff to see rows.
4. Only commit 100% functions; keep commits source-only and focused.
