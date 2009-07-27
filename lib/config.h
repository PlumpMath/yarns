#ifndef __YARNS_CONFIG__
#define __YARNS_CONFIG__

#define YARNS_STACK_PAGES 8
#define YARNS_ENABLE_SMP
// in �s
#define YARNS_DEAD_SLEEP_TIME 50

// schedulers
#define YARNS_SCHED_MULTILEVEL 1
#define YARNS_SCHED_ROUND_ROBIN 2
#define YARNS_SCHED_RANDOM 3
#define YARNS_SCHED_RB 4
#define YARNS_SCHED_STAIRCASE 5
#define YARNS_SCHEDULER YARNS_SCHED_STAIRCASE
// in �s
#define YARNS_TIMESLICE 10

// synergy methods
#define YARNS_SYNERGY_COOPERATIVE 1
#define YARNS_SYNERGY_MARKED 2
#define YARNS_SYNERGY_PREEMPTIVE 3
#define YARNS_SYNERGY YARNS_SYNERGY_COOPERATIVE

// memory allocation and control
//#define YARNS_MEMORY_ACCESS_CONTROL
#define YARNS_USE_SYSTEM_ALLOCATOR

// target
#define YARNS_TARGET_MACH 1
#define YARNS_TARGET_LINUX 2
#define YARNS_SELECTED_TARGET YARNS_TARGET_MACH

// maxima
#define YARNS_MAX_PROCESSES 65536

// overrides
//#define YARNS_OVERRIDE_CORE_COUNT 1

#endif
