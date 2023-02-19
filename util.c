#include "util.h"
#include <stdio.h>
#include <stdlib.h>

Point point_new(int x, int y) {
	Point p;
	p.x = x;
	p.y = y;
	return p;
}

Point point_add(Point a, Point b) {
	Point p;
	p.x = a.x + b.x;
	p.y = a.y + b.y;
	return p;
}

PointVec *pointvec_new(void) {
	PointVec *pv = malloc(sizeof(PointVec));
	if (pv == NULL) DIE("failed to allocate pointvec");
	pv->len = 0;
	pv->cap = 5;
	pv->buff = calloc(pv->cap, sizeof(PointVec));
	return pv;
}
void pointvec_append(PointVec *pv, Point p) {
	if (pv->len == pv->cap) {
		pv->cap *= 2;
		pv->buff = realloc(pv->buff, sizeof(PointVec) * pv->cap);
		if (pv->buff == NULL) DIE("failed to reallocate pointvec");
	}
	pv->buff[pv->len++] = p;
}

void DIE(char *msg) {
	perror(msg);
	exit(1);
}
