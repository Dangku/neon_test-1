# neon_test
This program tests the ARM NEON™ instruction set, with a simple and comparative
routine.

The routine is to divide by two all the elements of two arrays, then add all
the elements of the same index of these two arrays, and then the result is
incremented in 127 and stored in a third array.

Each test run 300 times the test routine and measure the elapsed time.

## To build and test

cd neon_test
./build
./neon_test.out
