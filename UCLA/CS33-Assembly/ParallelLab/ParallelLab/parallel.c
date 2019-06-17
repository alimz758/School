//OpenMP version.  Edit and submit only this file.
/* Enter your details below
 * Name : Ali Mirabzadeh
 * UCLA ID : 305179067
 * Email : thealimz758@g.ucla.edu
 */

#include <stdlib.h>
#include <stdio.h>
#include <omp.h>
#include <math.h>

#include "utils.h"

void work_it_par(long *old, long *new) {
  int i, j, k;
  int ii, jj, kk;
  int u, v, w;
  long compute_it;
  long aggregate=1.0;
  int iDIM;
  int jDIM;
  float func_const = we_need_the_func();
  float func_const2 = gimmie_the_func();
  

#pragma omp parallel for private(i,k,j, compute_it) reduction(+:aggregate)
  for (i=1; i<DIM-1; i++) {
    //iDIM = i*DIM*DIM;
    #pragma omp nowait
    for (j=1; j<DIM-1; j++) {
      //jDIM = j*DIM;
     // #pragma omp reduction(+:aggregate) 
      for (k=1; k<DIM-1; k++) {
        compute_it = old[i*DIM*DIM+j*DIM+k] * we_need_the_func();
        aggregate+= compute_it / gimmie_the_func();
      }
    }
  }
  printf("AGGR:%ld\n",aggregate);

#pragma omp parallel private(i,ii,j,jj,k,kk,u,v,w,old,new) 
  for (i=1; i<DIM-1; i += 2) {
#pragma omp for nowait
    for (j=1; j<DIM-1; j+= 2) {
      
      for (k=1; k<DIM-1; k += 2) {

        for (ii=i; ii < i+2 ; ii++ )
        {
          for (jj=j; jj < j+2 ; jj++ )
          {
            //jDIM = jj*DIM;
            for (kk=k; kk < k+2; kk++ )
            {
              new[ii*DIM*DIM+jj*DIM+k]=0;
              //most inner of tiling 
              for (u=-1; u<=1; u++) {
                for (v=-1; v<=1; v++) {
                  for (w=-1; w<=1; w++) {
                    new[ii*DIM*DIM+jj*DIM+kk]+=old[(ii+u)*DIM*DIM+(jj+v)*DIM+(kk+w)];
                  }
                }
              }
              new[ii*DIM*DIM+jj*DIM+k]/=27;
            }
          }
        }

      }
    }
  }
 
  for (i=1; i<DIM-1; i++) {
    
    //iDIM = i*DIM*DIM;
    for (j=1; j<DIM-1; j++) {
      //jDIM = j*DIM;
      for (k=1; k<DIM-1; k++) {
        u=(new[i*DIM*DIM+j*DIM+k]/100);
        if (u<=0) u=0;
        if (u>=9) u=9;
          histogrammy[u]++;
      }
    }
}

}
