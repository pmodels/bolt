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

#define ABT_USE_SCHED_SLEEP 0

#define KMP_ABT_FORK_NUM_WAYS_DEFAULT 4
#define KMP_ABT_FORK_CUTOFF_DEFAULT 2

// ES-local data.
typedef struct kmp_abt_local {
  /* ------------------------------------------------------------------------ */
  // Mostly read only

  ABT_xstream xstream;
  ABT_sched sched;

  // Scheduler
  ABT_pool shared_pool;
  /* ------------------------------------------------------------------------ */
} __attribute__((aligned(CACHE_LINE))) kmp_abt_local_t;

// Global data.
typedef struct kmp_abt_global {
  /* ------------------------------------------------------------------------ */
  // Mostly read only
  int num_xstreams;
  int fork_num_ways;
  int fork_cutoff;

  // ES-local data.
  kmp_abt_local *locals;
} __attribute__((aligned(CACHE_LINE))) kmp_abt_global_t;

extern kmp_abt_global_t __kmp_abt_global;

typedef struct kmp_abt_affinity_place {
  size_t num_ranks;
  int *ranks;
} kmp_abt_affinity_place_t;

typedef struct kmp_abt_affinity_places {
  size_t num_places;
  kmp_abt_affinity_place_t **p_places;
} kmp_abt_affinity_places_t;

extern kmp_abt_affinity_places_t *__kmp_abt_parse_affinity(int num_xstreams,
                                                           const char *str,
                                                           size_t len,
                                                           bool verbose);
extern int __kmp_abt_affinity_place_find
    (const kmp_abt_affinity_place_t *p_place, int rank);
extern void __kmp_abt_affinity_places_free(kmp_abt_affinity_places_t *p_places);

#endif // KMP_USE_ABT
#endif // KMP_ABT_H
