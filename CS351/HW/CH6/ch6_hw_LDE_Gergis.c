/*
	Author: Safe Gergis
	Date: 2024-07-15
	Description: This program measures the time taken to perform various operations using different clock types.
	Credit to: Paul Krzyzanowski for the original code.
*/

#include <stdio.h>	/* for printf */
#include <stdint.h> /* for uint64 definition */
#include <stdlib.h> /* for exit() definition */
#include <time.h>	/* for clock_gettime */
#include <unistd.h> // for sleep
#include <sys/time.h>
#include <sys/syscall.h>
#define ONE_HUNDRED_MILLION 100000000L
#define BILLION 1000000000L

int localpid(void)
{
	static int a[9] = {0};
	return a[0];
}

void print_ns_us_ms(const char *label, uint64_t delta_time)
{
	printf("%s%llu ns = %.3f us = %.6f ms\n", label, delta_time,
		   delta_time / 1000.0, delta_time / 1.0e6);
}

#define REPEATS ONE_HUNDRED_MILLION

uint64_t repeat_get_time_of_day(void)
{
	struct timeval time;
	for (uint64_t i = 0; i < REPEATS; ++i)
	{
		gettimeofday(&time, 0); // Note: won't ACTUALLY call this system call everytime, maybe 1000 / 100_000_000 (1 out of 100,000)
	} //    will use cached memory because resolution of gettimeofday() is so low -- it doesn't change often
	return REPEATS;
}

uint64_t measure_generic(const char *label, clockid_t clockid, void *(*fp)(void *), void *args)
{
	uint64_t diff;
	struct timespec start, end;

	clock_gettime(clockid, &start); /* mark start time */

	uint64_t repeats = (uint64_t)fp(args); // allows for some processes to run multiple times; others only once

	clock_gettime(clockid, &end); /* mark the end time */

	diff = BILLION * (end.tv_sec - start.tv_sec) + end.tv_nsec - start.tv_nsec;
	// printf("diff in measure_generic is: %llu\n", diff);
	if (repeats == REPEATS)
	{
		diff /= REPEATS;
	}
	print_ns_us_ms(label, diff);

	return diff;
}

uint64_t measure_system_call(clockid_t clockid, void *(*fp)(void *args), void *args)
{
	struct timespec start, end;
	uint64_t total_diff = 0;

	for (uint64_t i = 0; i < REPEATS; ++i)
	{
		clock_gettime(clockid, &start);
		fp(args);
		clock_gettime(clockid, &end);

		uint64_t diff = BILLION * (end.tv_sec - start.tv_sec) + end.tv_nsec - start.tv_nsec;
		total_diff += diff;
	}

	uint64_t average_diff = total_diff / REPEATS;
	print_ns_us_ms("System call time = ", average_diff);

	return average_diff;
}
// Function to perform a zero byte read
static void *perform_zero_byte_read(void *args)
{
	int fd = *(int *)args;
	ssize_t result = syscall(SYS_read, fd, NULL, 0);
	return (void *)result;
}

int main(int argc, char **argv)
{
	measure_generic("elapsed time = ", CLOCK_MONOTONIC, (void *)sleep, (void *)1L);

	/* the time spent sleeping will not count (but there is a bit of overhead */
	measure_generic("elapsed process CPU time = ", CLOCK_PROCESS_CPUTIME_ID, (void *)sleep, (void *)1L);

	measure_generic("elapsed process CPU time for gettimeofday() = ", CLOCK_PROCESS_CPUTIME_ID,
					(void *)repeat_get_time_of_day, NULL);

	int fd = 0; // Valid file descriptor
	measure_system_call(CLOCK_MONOTONIC, perform_zero_byte_read, (void *)&fd);

	exit(0);
	return 0;
}