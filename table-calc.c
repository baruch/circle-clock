#include <stdio.h>
#include <stdlib.h>

static inline int time_to_idx(int t, float factor)
{
  float timef = t;
  return timef / factor;
}

int main(int argc, char **argv)
{
	if (argc != 2) {
		fprintf(stderr, "Expected an argument for the number of pixels\n");
		fprintf(stderr, "e.g.: %s 16\n", argv[0]);
		return 1;
	}

	int pixel_count = atoi(argv[1]);
	if (pixel_count <= 0) {
		fprintf(stderr, "Cannot deal with a pixel count that is zero or negative\n");
		return 2;
	}
	fprintf(stderr, "Calculating for pixel count of %d\n", pixel_count);

	double hour_factor = 12.0 / (double)pixel_count;
	double minute_factor = 60.0 / (double)pixel_count;

	printf("#define PIXEL_COUNT %d\n\n", pixel_count);

	// Generate a table of indexes for the hour data
	printf("static const int hour_idx_tbl[] = {\n");
	for (int hour = 0; hour < 12; hour++) {
		printf("  %d,", time_to_idx(hour, hour_factor));
	}
	printf("\n};\n\n");

	// Generate a table of indexes and time to sleep for minute data
	int minutes[60];

	printf("static const struct { int idx, time_to_sleep; } minute_idx_tbl[] = {\n");
	for (int minute = 0; minute < 60; minute++) {
		minutes[minute] = time_to_idx(minute, minute_factor);
	}
	for (int minute = 0; minute < 60; minute++) {
		int next_idx_minute = minute+1;
		// Advance next minute index until we find a different index
		while (next_idx_minute < 60 && minutes[minute] == minutes[next_idx_minute])
			next_idx_minute++;

		int time_to_sleep = (next_idx_minute - minute)*60;
		printf("  { %2d, %3d },\n", minutes[minute], time_to_sleep);
	}
	printf("};\n");

	return 0;
}
