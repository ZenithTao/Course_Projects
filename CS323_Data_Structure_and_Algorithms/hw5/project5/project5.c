// Liang Tao
// Email: liang.tao@emory.edu    EmoryId: 2019389
//I worked on this assignment alone, using only this semester's course materials.

#include <stdio.h>
#include "header.h"

seg *a;
seg *b;
int counta;
int countb;

// like the given method in primes.c, adapted version
void setBit(int n) {
	int s;
	int i;
	int pos;
	
	s = n/(2*BITS_PER_SEG);     // use the formula below to get the 
	i = (n-s*BITS_PER_SEG*2)/64;  // corresponding index of array and position
	pos = ((n-s*8192)-i*64-1)/2;

	if (s > counta) {
   		int j;
   		for ( j = counta; j < s; j++){ // go to the sth segment    
       		a = a->next;      		
    	}		
    	counta = s;
	} else if (s < counta) {
		  a = head;
   		int j;
   		for ( j = 0; j < s; j++) {           
       		a = a->next;      		
    	}
    	counta = s;	
	}
	
	a->bits[i] = a->bits[i] | (1 << pos) ; // use the given metod
}


// like the given method in primes.c, adapted version
int testBitIs0(int n) {
	int s; 
	int i;
	int pos;
  int r;
	
	s = n / (2*BITS_PER_SEG);  // like in the setBit
	i = (n-s*BITS_PER_SEG*2)/64;
	pos = ((n-s*8192)-i*64-1)/2;

	if (s > countb) {
   		int j;
   		for ( j = countb; j < s; j++) {  // go to the sth segment
       		b = b->next;      		
    	}		
    	countb = s;
	} else if (s < countb) {
    int j;
		b = head;
    for ( j = 0; j < s; j++) {           
       		b = b->next;      		
    }
    	countb = s;	
	}
	
  r = b->bits[i] & (1 << pos) ; // Get bit at pos in the linked list
  if ( r == 0 )
      return 1;        // return 1 to indicate that n is still prime
  else
      return 0;
}


// count the number of primes
int countPrimes(int N) {
  int count = 0;
  int i;
  for ( i = 1; i <= N; i += 2 )
      if (testBitIs0(i))
          count++;
        
  return count+1;  // add 2 because 2 is also a prime
}


// print all the primes
void printPrimes(int N) {
    printf("2 "); // print 2 first
    
    int i; 
    for ( i = 1; i <= N; i += 2 ) {
        if (testBitIs0(i)) printf("%d ", i); 
    }
    
    printf("\n");
}

// main method
void sieveOfE(int N) {
  counta = 0;
  countb = 0;
	a = head;  // two helper pointer
	b = head;  
	
  int i, j, k;
  setBit(1);    // 1 is not prime

	k = 3;
  while (k <= N) {
      for (i = k; i <= N; i += 2) {
          if (testBitIs0(i))
             	break;   // found!!
          }    
                 				 
      for (j = 3*i; j <= N; j = j + i) {  // like the given method
				  if ((j%2) != 0)
	  				  setBit(j); // set th bit
	  	}

      	k = i+2;    // Set up k for next iteration !!!   
   	}
}


void factor(int inp) {
	int k;
	int N;
  int i;
  
	N = inp;
  
	if (N == 1) printf("1\n"); // if N == 1 just print 1
		
	while (N != 1 && N >0){
		if ((N % 2) == 0){
			printf("2\n"); 
			N = N/2;
		} else {
   			for (i = 3; i <= inp; i += 2) {
      			if (testBitIs0(i) && (N % i == 0)) {
         			printf("%d\n", i);
					    N = N/i;
					    break; // break the loop
				    }
			  }
		}		
	}
}

void clearAll( ) {
  int i;
  seg *current;
  current = head;
  
    while(current != NULL) {    // run through the linked list and set all the int arrays to 0
      for (i = 0; i < SIZE_OF_SEG; i++) {  
        current->bits[i] = 0; // set all to 0
      }
    
      current = current->next;  // get the next segment
    }
}