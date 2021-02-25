// RUN: %libomp-compile-and-run
#include <stdio.h>
#include "omp_testsuite.h"
#include "omp_my_sleep.h"

// https://github.com/pmodels/bolt/issues/96
int foo() {
   int err = 0;
   int x = 1;

   #pragma omp task depend(out:x) shared(x)
   {
      my_sleep(5); // 5 seconds
      x = 2;
   }

   #pragma omp task depend(in:x) shared(x, err)
   {
      if (x != 2) {
         err += 1;
      }
   }

   // While the first task is sleeping, the following should be executed.
   // Note that, in theory, the following execution can be delayed, so
   // false-positive may happen.
   if (x == 2)
      err += 10;

   #pragma omp taskwait

   if (x != 2)
      err += 100;
   return err;
}

int main() {
   int i;
   if (omp_get_max_threads() < 2)
      omp_set_num_threads(2);
   for (i = 0; i < 5; i++) {
      int rc = 0;
      #pragma omp parallel
      {
         #pragma omp single nowait
         rc = foo();
      }
      if (rc) {
         printf("FAIL: rc = %d\n", rc);
         return rc;
      }
   }
   return 0;
}
