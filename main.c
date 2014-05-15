#include <stdio.h>
#include <time.h>
#include <arm_neon.h>

#define SIZE 1600000 // Arrays size

void routine(uint8_t *arr2, uint8_t *arr1, uint8_t *arr0, uint8_t offset);
void neon_routine(uint8_t *arr2, uint8_t *arr1, uint8_t *arr0, uint8_t offset);

int main(void){
  uint32_t i;
  uint8_t arr0[SIZE];
  uint8_t arr1[SIZE];
  uint8_t arr2[SIZE];

  struct timespec start, end;
  uint32_t diff;

  printf("\nStarting the NEON test...\n");

  for(i=0; i<SIZE; i++){
    arr0[i] = i;
    arr1[i] = i;
  }

  printf("Starting the test routine w/o NEON.\n");
  clock_gettime( CLOCK_REALTIME, &start );

  for(i=0; i<300; i++){
    routine(arr2, arr1, arr0, 127);
  }

  clock_gettime( CLOCK_REALTIME, &end );
  diff = (end.tv_sec-start.tv_sec)*1000 + (end.tv_nsec-start.tv_nsec)/1000000;
  printf("Elapsed time: %u ms\n", diff);

  printf("Starting the test routine with NEON.\n");
  clock_gettime( CLOCK_REALTIME, &start );

  for(i=0; i<300; i++){
    neon_routine(arr2, arr1, arr0, 127);
  }

  clock_gettime( CLOCK_REALTIME, &end );
  diff = (end.tv_sec-start.tv_sec)*1000 + (end.tv_nsec-start.tv_nsec)/1000000;
  printf("Elapsed time: %u ms\n\n", diff);

  return 0;
}

void neon_routine(uint8_t *arr2, uint8_t *arr1, uint8_t *arr0, uint8_t offset){

  uint32_t i;
  uint8x16_t arr0_;
  uint8x16_t arr1_;

  uint8x16_t arr3_ = vdupq_n_u8(offset);

  for(i=0; i<SIZE; i+=16){
    arr0_ = vld1q_u8(arr0 + i); // arr0_ = arr0
    arr0_ = vsraq_n_u8(arr3_, arr0_, 1); // arr0_ = arr3 + (arr0_ >> 1)

    arr1_ = vld1q_u8(arr1 + i); // arr1_ = arr1
    arr0_ = vsraq_n_u8(arr0_, arr1_, 1); // arr0_ = arr0_ + (arr1_ >> 1)

    vst1q_u8((arr2+i), arr0_); // arr2 = arr0_
  }
}

void routine(uint8_t *arr2, uint8_t *arr1, uint8_t *arr0, uint8_t offset){

  uint32_t i;
  uint8_t tmp0, tmp1;

  for(i=0; i<SIZE; i++){
    tmp0 = arr0[i] >> 1;
    tmp1 = arr1[i] >> 1;
    arr2[i] = tmp0 + tmp1;
    arr2[i] += offset;
  }
}
