#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <arm_neon.h>

#define SIZE 64000000 // Arrays size

void routine(uint8_t *arr2, uint8_t *arr1, uint8_t *arr0);
void neon_routine(uint8_t *arr2, uint8_t *arr1, uint8_t *arr0);

int main(void){
  uint32_t i;
  uint8_t *arr0;
  uint8_t *arr1;
  uint8_t *arr2;

  arr0 = (uint8_t *)malloc(SIZE);
  arr1 = (uint8_t *)malloc(SIZE);
  arr2 = (uint8_t *)malloc(SIZE);

  struct timespec start, end;
  uint32_t diff;

  printf("\nStarting the NEON test...\n");

  for(i=0; i<SIZE; i++){ // Populates the input arrays
    arr0[i] = i;
    arr1[i] = i;
  }

  printf("Starting the test routine w/o NEON.\n");
  clock_gettime( CLOCK_REALTIME, &start );

  routine(arr2, arr1, arr0);

  clock_gettime( CLOCK_REALTIME, &end );
  diff = (end.tv_sec-start.tv_sec)*1000 + (end.tv_nsec-start.tv_nsec)/1000000;
  printf("Elapsed time: %u ms\n", diff);

  printf("Starting the test routine with NEON.\n");
  clock_gettime( CLOCK_REALTIME, &start );

  neon_routine(arr2, arr1, arr0);

  clock_gettime( CLOCK_REALTIME, &end );
  diff = (end.tv_sec-start.tv_sec)*1000 + (end.tv_nsec-start.tv_nsec)/1000000;
  printf("Elapsed time: %u ms\n\n", diff);

  return 0;
}

void neon_routine(uint8_t *arr2, uint8_t *arr1, uint8_t *arr0){

  uint32_t i;
  uint8x16_t arr0_;
  uint8x16_t arr1_;

  for(i=0; i<SIZE; i+=16){
    arr0_ = vld1q_u8(arr0 + i); // arr0_ = arr0
    arr0_ = vshrq_n_u8(arr0_, 1); // arr0_ = arr0_ >> 1

    arr1_ = vld1q_u8(arr1 + i); // arr1_ = arr1
    arr0_ = vsraq_n_u8(arr0_, arr1_, 1); // arr0_ = arr0_ + (arr1_ >> 1)

    vst1q_u8((arr2+i), arr0_); // arr2 = arr0_
  }
}

void routine(uint8_t *arr2, uint8_t *arr1, uint8_t *arr0){

  uint32_t i;

  for(i=0; i<SIZE; i++){
    arr2[i] = (arr0[i] >> 1) + (arr1[i] >> 1);
  }
}
