// Liang Tao
// Email: liang.tao@emory.edu    EmoryId: 2019389
//I worked on this assignment alone, using only this semester's course materials.

#define EXTERN
#include <stdio.h>
#include "header.h"
extern void clearAll();
extern void setBit(int N);
extern int testBitIs0(int N);

void sieveOfE(int N) {
   clearAll();       // initiate
   int i, j, k;
   
   // initialize prime[]
   setBit(0); // 0 and 1 are not prime numbers
   setBit(1);

   k = 2; // start with 2 to find all primes
   
   // as the website introduce
   while (k <= N) {
      for (i = k; i <= N; i++) {
         if (testBitIs0(i)) break;     // see the testBitIs0 function
      }
      
      for (j = 2 * i; j <= N; j = j + i){  
         setBit(j);
      }
      k = i + 1;
   }
}

int countPrimes(int N) {
   int count = 0;    // count variable
   int i;
   for (i = 0; i <= N; i++) {
      if (testBitIs0(i)) count++; // if testBitIs0 = 1 then it's a prime
   }
   return count;
}

void printPrimes(int N) {
   int i;
   for(i = 0; i <= N; i++) {
      if (testBitIs0(i)) printf("%d ",i); // if i is a prime then print it out
   }
}

void clearAll() {       // clear all the records, used as the 
   int i;
   for (i = 0; i < MAX; i++)
      prime[i] = 0;
}

void setBit(int N) {    // set the bit
      int i = N/32;
      int pos = N%32;

      unsigned int flag = 1;   // flag = 0000.....00001

      flag = flag << pos;      // flag = 0000...010...000   (shifted k positions)

      prime[i] = prime[i] | flag;      // Set the bit at the k-th position in A[i]
}

int testBitIs0(int N) {    // test the bit
      int i = N/32;
      int pos = N%32;
      unsigned int flag = 1;  // flag = 0000.....00001

      flag = flag << pos;     // flag = 0000...010...000   (shifted k positions)

      if ( prime[i] & flag )  // Test the bit at the k-th position in prime[i]
         return 0;
      else
         return 1;
}