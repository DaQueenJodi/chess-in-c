#ifndef C_UTIL_HH
#define C_UTIL_HH

#include <stdio.h>
#include <stdlib.h>

void DIE(char *msg);
#define DIEF(...) do { fprintf(stderr, __VA_ARGS__); exit(1); } while (0)

typedef struct {
	int x;
	int y;
} Point;

Point point_new(int x, int y);
Point point_add(Point a, Point b);

typedef struct {
	Point *buff;
	size_t len;
	size_t cap;
} PointVec;

PointVec *pointvec_new(void);
void pointvec_append(PointVec *pv, Point p);
#endif
