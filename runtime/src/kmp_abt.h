/*
 * kmp_abt.h -- header file.
 */

//===----------------------------------------------------------------------===//
//
//                     The LLVM Compiler Infrastructure
//
// This file is dual licensed under the MIT and the University of Illinois Open
// Source Licenses. See LICENSE.txt for details.
//
//===----------------------------------------------------------------------===//

#ifndef KMP_ABT_H
#define KMP_ABT_H

#if KMP_USE_ABT

#include <abt.h>

#define ABT_USE_PRIVATE_POOLS 1
#define ABT_USE_SCHED_SLEEP 0

// ES-local data.
typedef struct kmp_abt_local {
  /* ------------------------------------------------------------------------ */
  // Mostly read only

  ABT_xstream xstream;
  ABT_sched sched;

  // Scheduler
  ABT_pool priv_pool;
  ABT_pool shared_pool;
  /* ------------------------------------------------------------------------ */
} __attribute__((aligned(CACHE_LINE))) kmp_abt_local_t;

// Global data.
typedef struct kmp_abt_global {
  /* ------------------------------------------------------------------------ */
  // Mostly read only
  int num_xstreams;

  // ES-local data.
  kmp_abt_local *locals;
} __attribute__((aligned(CACHE_LINE))) kmp_abt_global_t;

extern kmp_abt_global_t __kmp_abt_global;

#endif // KMP_USE_ABT
#endif // KMP_ABT_H
