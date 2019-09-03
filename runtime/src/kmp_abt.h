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

#endif // KMP_USE_ABT
#endif // KMP_ABT_H
