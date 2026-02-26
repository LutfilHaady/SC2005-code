# Lab 2 Implementation Notes

## What was implemented

1. **User-level threading** (`user/uthread.c`, `user/uthread_switch.S`)
   - Added `struct thread_context` (ra, sp, s0–s11) to save/restore on switch.
   - `thread_switch` saves current registers to the first argument and restores from the second (same layout as kernel `swtch`).
   - `thread_create`: sets `context.ra = func`, `context.sp` to the new thread’s stack (with a fake return address so that if `func` returns, control goes to `thread_exit`).
   - `thread_schedule`: calls `thread_switch(&t->context, &next_thread->context)` when switching threads.

2. **Scheduler: even-PID priority** (`kernel/proc.c`)
   - First pass: pick a RUNNABLE process with **even** PID (`pid % 2 == 0`).
   - Second pass: if none, pick any RUNNABLE process (round-robin among the rest).

3. **Process table user program** (`user/procinfo.c`) and **getprocinfo** syscall
   - New syscall `getprocinfo(struct procinfo *buf, int n)` copies (pid, state) for all non-UNUSED processes into the user buffer.
   - `procinfo` user program calls it and prints PID and state (UNUSED, USED, SLEEPING, RUNNABLE, RUNNING, ZOMBIE).

## Build and test (on lab VM or Linux with RISC-V tools)

```bash
git checkout lab2
make clean
make qemu
# In xv6 shell:
uthread_test    # Should print thread_a/b/c started and 0..99 for each
procinfo        # Prints all process PIDs and states
```

## Quiz: Why does `swtch` save/restore only callee-save registers?

**Callee-saved** (s0–s11, ra, sp) must be preserved by the callee across a call. When we switch to another process and later switch back, that process may have used the same registers, so we must save and restore them to preserve the “thread of execution” state.

**Caller-saved** (e.g. a0–a7, t0–t6) are not required to be preserved by the callee; the caller saves them if it needs them. So when we call `swtch`, the *caller* (e.g. `scheduler` or `sched`) is responsible for any caller-saved values it cares about. The code that returns from `swtch` does not rely on caller-saved registers being unchanged, so `swtch` does not need to save/restore them.
