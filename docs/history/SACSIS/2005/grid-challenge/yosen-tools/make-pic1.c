#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>
#include <math.h>
#include <string.h>
#include <assert.h>
#include "pic.h"

#define NOT_TOO_FEW
#define NOT_TOO_SMALL
#define MAKE_HOLE
#define ALIGN_RECT

#define MYRAND
#define MORTON
//#define LOG

#define FNAMELEN 128

typedef struct {
    int x;
    int y;
} tse_t;

typedef struct {
    int cur;
    int size;
    tse_t *buf;
} task_stack_t;

struct {
    char filename[FNAMELEN];
    char dir[FNAMELEN];
    char ansfilename[FNAMELEN];
    int prob;
    crypt_key_t key;
    int nx;
    int ny;
    int ix;
    int iy;
    int w;
    int h;
    int border;
    int spanning;
} g;

#define MAP(pic, x, y) (pic)->buf[(y)*(pic)->w+(x)]

/******************************************/
/** count */
/*****************/
task_stack_t *ts_new()
{
#define INITIAL_TS_SIZE 1024
    task_stack_t *ts;
    ts = malloc(sizeof(task_stack_t));
    ts->cur = 0;
    ts->size = INITIAL_TS_SIZE;
    ts->buf = malloc(sizeof(tse_t)*ts->size);
    return ts;
}

int ts_push(task_stack_t *ts, int x, int y)
{
    if (ts->cur >= ts->size) {
	/* expand */
	ts->size *= 2;
	ts->buf = realloc(ts->buf, ts->size * sizeof(tse_t));
#ifdef LOG
	fprintf(stderr, "expanded task stack to %d\n", ts->size);
#endif
    }

    ts->buf[ts->cur].x = x;
    ts->buf[ts->cur].y = y;
    ts->cur++;
    return 0;
}

int ts_is_empty(task_stack_t *ts)
{
    return (ts->cur == 0);
}

int ts_pop(task_stack_t *ts, int *px, int *py)
{
    if (ts_is_empty(ts)) {
	/* no data */
	fprintf(stderr, "[ts_pop] ERROR: empty task stack.\n");
	return -1;
    }

    ts->cur--;
    *px = ts->buf[ts->cur].x;
    *py = ts->buf[ts->cur].y;
    return 0;
}

int push_if_unmarked(pic_t *pic, task_stack_t *ts, int x, int y)
{
    if (x < 0 || x >= pic->w || y < 0 || y >= pic->h) {
	return 0;
    }

    if (MAP(pic, x, y) == 1) {
	ts_push(ts, x, y);
	MAP(pic, x, y) = 2;
	return 1;
    }
    return 0;
}

int traverse(pic_t *pic, task_stack_t *ts, int x0, int y0)
{
    int area = 0;
    push_if_unmarked(pic, ts, x0, y0);
    while (1) {
	int x, y;
	if (ts_is_empty(ts)) {
	    break;
	}

	area++;
	ts_pop(ts, &x, &y);
	push_if_unmarked(pic, ts, x+1, y);
	push_if_unmarked(pic, ts, x-1, y);
	push_if_unmarked(pic, ts, x, y+1);
	push_if_unmarked(pic, ts, x, y-1);
    }
    return area;
}

int count_local(pic_t *pic, int *cntp)
{
    int x, y;
    int count = 0;
    int area = 0;
    task_stack_t *ts;
    ts = ts_new();

    for (y = 0; y < pic->h; y++) {
	for (x = 0; x < pic->w; x++) {
	    if (MAP(pic, x, y) == 1) {
		area += traverse(pic, ts, x, y);
		assert(ts_is_empty(ts));
		count++;
	    }
	}
    }    

    *cntp = count;
    return 0;
}

/******************************************/



int isqrt(int n)
{
    if (n <= 1) return n;
    return (int)(sqrt((double)n));
}

#ifdef MYRAND
/****** handmade random generator */
#define RIM 53125
#define RIA 171
#define RIC 11213

int gran;

int my_rand(int den);

int my_srand(int type, int ix, int iy)
{
    int seed = g.prob*100000+type*10000+iy*100+ix+1;
    int i;
    gran = seed % RIM;
    for (i = 0; i < 5; i++) {
	my_rand(1);
    }
    return 0;
}

int my_rand(int den)
{
    int rc;
    rc = gran*den/RIM;
    assert(rc >= 0 && rc < RIM);
    gran = (gran*RIA+RIC)%RIM;
    return rc;
}
#else

int my_srand(int type, int ix, int iy)
{
    int seed = g.prob*100000+type*10000+iy*100+ix+1;
#ifdef LOG
    printf("srand(%d)\n", seed);
#endif
    srand(seed);
    return 0;
}

/* random number 0...den-1 */
int my_rand(int den)
{
    return rand() % den;
}
#endif

int make_strange_data(pic_t *pic, int ix, int iy)
{
    int sx = pic->w/16;
    int ex = pic->w - sx - 1;
    int sy = pic->h/16;
    int ey = pic->h - sy - 1;
    int x, y;

#if 1
    for (y = sy; y <= ey; y++) {
	if (y%8 < 4) {
	    for (x = sx; x < ex; x ++) {
		if (x%8 < 4) {
		    MAP(pic, x, y) = 1;
		}
	    }
	}
    }
#endif
#if 0
    ex = pic->w/2-1;
    for (y = sy; y <= ey; y++) {
	for (x = sx; x < ex; x += 4) {
	    MAP(pic, x, y) = 1;
	}
    }
#endif
#if 0
    for (y = sy; y <= ey; y++) {
	for (x = sx; x < ex; x += 4) {
	    MAP(pic, x, y) = 1;
	}
    }
#endif
#if 0
    for (x = sx; x <= ex; x++) {
	MAP(pic, x, sy) = 1;
	MAP(pic, x, ey) = 1;
    }
    for (y = sy; y <= ey; y++) {
	MAP(pic, sx, y) = 1;
	MAP(pic, ex, y) = 1;
    }
#endif
    return 0;
}

int make_inner_data(pic_t *pic, int ix, int iy)
{
    int i;
    int maxnr;
    int nr;
    int rxsize;
    int rysize;
    int bd = g.border;

    /* srand from (ix, iy) */
    my_srand(0, ix, iy);

    for (i = 0; i < pic->w*pic->h; i++) {
	pic->buf[i] = 0;
    }

    /* max rect size */
#if 1
    rxsize = isqrt(pic->w)*3;
    rysize = isqrt(pic->h)*3;
#else
#warning smaller rect size
    rxsize = isqrt(pic->w) / 5;
    rysize = isqrt(pic->h) / 5;
#endif

    maxnr = pic->w*pic->h/(rxsize*rysize)/3;
#ifdef NOT_TOO_FEW
    nr = maxnr/2 + my_rand(maxnr/2);
#else
    nr = my_rand(maxnr);
#endif
    /* make random rectangles */
    for (i = 0; i < nr; i++) {
	int x0 = my_rand(pic->w - 3 - rxsize - bd*2) + 1 + bd;
	int y0 = my_rand(pic->h - 3 - rysize - bd*2) + 1 + bd;
#ifdef NOT_TOO_SMALL
	int w = rxsize/2 + my_rand(rxsize/2) + 1;
	int h = rysize/2 + my_rand(rysize/2) + 1;
#else
	int w = my_rand(rxsize) + 1;
	int h = my_rand(rysize) + 1;
#endif

#ifdef ALIGN_RECT
#warning ALIGN 4
	/* align */
	if (w >= 4 && h >= 4) {
	    if (my_rand(100) < 70) {
		x0 = ((x0+3)/4)*4;
		y0 = ((y0+3)/4)*4;
		w = (w/4)*4;
		h = (h/4)*4;
	    }
	}
#endif

	assert(x0 > bd && y0 > bd);
	assert (x0+w < pic->w-bd-1);
	assert (y0+h < pic->h-bd-1);

	{
	    int x;
	    int y;
	    for (y = y0; y < y0+h; y++) {
		for (x = x0; x < x0+w; x++) {
		    MAP(pic, x, y) = 1;
		}
	    }
#ifdef MAKE_HOLE
#warning MAKE_HOLE ON
	    if (my_rand(100) < 20) {
		y = y0+my_rand(h);
		x = x0+my_rand(w);
		MAP(pic, x, y) = 0;
	    }
#else
#warning MAKE_HOLE OFF
#endif
	}
    }

    return 0;
}

/* picflag=0: this pic is upper of the border. */
/* picflag=1: this pic is lower of the border. */
/* ix, iy: coord of this pic. */
int make_span_y(pic_t *pic, int ix, int iy, int picflag, int *cntp)
{
    int rxsize;
    int i;
    int nr;
    int maxnr;
    int bd = g.border;
    int px;
    int cnt = 0;

    if (picflag == 0) {
	/* srand from (ix, iy) */
	my_srand(1, ix, iy);
    }
    else {
	/* srand from (ix, iy-1) */
	my_srand(1, ix, iy-1);
    }

    /* max rect size */
    rxsize = isqrt(g.w);

    px = bd;
    while (px < g.w - bd) {
	int sx, ex, sy, ey;
	/* make space */
	px += my_rand(rxsize*3) + 1;

	/* make a building */
	sx = px;
	px += my_rand(rxsize) + 1;
	ex = px;
	if (ex >= g.w - bd) {
	    break;
	}

	if (my_rand(32) == 0) {
	    sy = g.h;
	}
	else {
	    sy = g.h - bd + (my_rand(2*bd));
	}
	ey = g.h - bd + (my_rand(2*bd));
	if (ey < sy) {
	    int tmp;
	    tmp = sy;
	    sy = ey;
	    ey = tmp;
	}
	if (sy == ey) {
	    continue;
	}

	if (sy < g.h && ey > g.h) {
	    cnt++;
	}

	if (sy == g.h || ey == g.h) {
#ifdef LOG
	    printf("\nAT FILE BORDER (y),  %d--%d\n", sy, ey);
#endif
	}
#if 0
	else if (sy < g.h && ey > g.h) {
	    printf("\nSPAN (y)\n");
	}
	else {
	    printf("\nNOT SPAN (y)\n");
	}
#endif
	{
	    int x, y;
	    if (picflag == 0) {
		for (y = sy; y < ey; y++) {
		    if (y < g.h) {
			for (x = sx; x < ex; x++) {
			    MAP(pic, x, y) = 1;
			}
		    }
		}
	    }
	    else {
		for (y = sy-g.h; y < ey-g.h; y++) {
		    if (y >= 0) {
			for (x = sx; x < ex; x++) {
			    MAP(pic, x, y) = 1;
			}
		    }
		}
	    }
	}

    }

    *cntp = cnt;
    return 0;
}

/* picflag=0: this pic is left of the border. */
/* picflag=1: this pic is right of the border. */
/* ix, iy: coord of this pic. */
int make_span_x(pic_t *pic, int ix, int iy, int picflag, int *cntp)
{
    int rysize;
    int i;
    int nr;
    int maxnr;
    int bd = g.border;
    int py;
    int cnt = 0;

    if (picflag == 0) {
	/* srand from (ix, iy) */
	my_srand(2, ix, iy);
    }
    else {
	/* srand from (ix-1, iy) */
	my_srand(2, ix-1, iy);
    }

    /* max rect size */
    rysize = isqrt(g.h);

    py = bd;
    while (py < g.h - bd) {
	int sx, ex, sy, ey;
	/* make space */
	py += my_rand(rysize*3) + 1;

	/* make a building */
	sy = py;
	py += my_rand(rysize) + 1;
	ey = py;
	if (ey >= g.h - bd) {
	    break;
	}

	if (my_rand(32) == 0) {
	    sx = g.w;
	}
	else {
	    sx = g.w - bd + (my_rand(2*bd));
	}
	ex = g.w - bd + (my_rand(2*bd));
	if (ex < sx) {
	    int tmp;
	    tmp = sx;
	    sx = ex;
	    ex = tmp;
	}
	if (sx == ex) {
	    continue;
	}

	if (sx < g.w && ex > g.w) {
	    cnt++;
	}

	if (sx == g.w || ex == g.w) {
#ifdef LOG
	    printf("\nAT FILE BORDER (x)\n");
#endif
	}
#if 0
	else if (sx < g.w && ex > g.w) {
	    printf("\nSPAN (X)\n");
	}
	else {
	    printf("\nNOT SPAN (y)\n");
	}
#endif
	{
	    int x, y;
	    if (picflag == 0) {
		for (y = sy; y < ey; y++) {
		    for (x = sx; x < ex; x++) {
			if (x < g.w) {
			    MAP(pic, x, y) = 1;
			}
		    }
		}
	    }
	    else {
		for (y = sy; y < ey; y++) {
		    for (x = sx-g.w; x < ex-g.w; x++) {
			if (x >= 0) {
			    MAP(pic, x, y) = 1;
			}
		    }
		}
	    }
	}

    }

    *cntp = cnt;
    return 0;
}

int get_ansfilename(char *fn)
{
    sprintf(fn, "%s/%02d%03d%03d.ans", g.dir, g.prob, g.ix, g.iy);
    return 0;
}

int make_one(pic_t *pic, int ix, int iy)
{
    int span_x_count = 0;
    int span_y_count = 0;
    int local_count = 0;
    
#ifdef LOG
    printf("make data on memory\n");
#endif
    make_inner_data(pic, ix, iy);

    if (g.spanning > 0) {
	int dummy = 0;
	if (ix > 0) {
	    /* make buildings at left border */
	    make_span_x(pic, ix, iy, 1, &span_x_count);
	}
	
	if (ix < g.nx-1) {
	    /* make buildings at right border */
	    make_span_x(pic, ix, iy, 0, &dummy);
	}
	
	if (iy > 0) {
	    /* make buildings at upper border */
	    make_span_y(pic, ix, iy, 1, &span_y_count);
	}
	
	if (iy < g.ny-1) {
	    /* make buildings at lower border */
	    make_span_y(pic, ix, iy, 0, &dummy);
	}
    }
    else {
	/* check that no building in border */
	int x;
	int y;
	for (y = 0; y < pic->h; y++) {
	    assert(MAP(pic, 0, y) == 0);
	    assert(MAP(pic, pic->w-1, y) == 0);
	}
	for (x = 0; x < pic->w; x++) {
	    assert(MAP(pic, x, 0) == 0);
	    assert(MAP(pic, x, pic->h-1) == 0);
	}
    }

    {
#ifdef LOG
	printf("try to write to [%s]\n", g.filename);
#endif
#ifdef MORTON

#  ifdef LOG
	printf("In MORTON ORDER!\n");
#  endif
	pic_morton_writefile(pic, g.filename, &g.key);
#else
#  ifdef LOG
	printf("In ROW MAJOR ORDER!\n");
#  endif
	pic_writefile(pic, filename, &g.key);
#endif
    }

    count_local(pic, &local_count);

    {
	/* write partial answer file */
	FILE *fp;

	fp = fopen(g.ansfilename, "w");
	if (!fp) {
	    fprintf(stderr, "Failed to w-open [%s]\n", g.ansfilename);
	    exit(1);
	}

	fprintf(fp,
		"%02d%03d%03d %d %d %d\n", g.prob, ix, iy,
		local_count, span_x_count, span_y_count);
	fclose(fp);
    }
    return 0;
}

#if 1
int make_one_main(int ix, int iy)
{
    pic_t *pic;

    pic = pic_new(g.w, g.h);
    make_one(pic, ix, iy);
    return 0;
}
#else
int make_main()
{
    int ix;
    int iy;
    pic_t *pic;

    pic = pic_new(g.w, g.h);

    for (iy = 0; iy < g.ny; iy++) {
	for (ix = 0; ix < g.nx; ix++) {
	    make_one(pic, ix, iy);
	    printf(".");
	    fflush(stdout);
	}
    }
    printf("\n");
    return 0;
}
#endif

/* Parse fname that looks like ".../.../.../PPXXXYYY.mdt" (ext may be "rdt") */
int parse_filename(char *fname)
{
    char *p;
    int rc;
    p = strrchr(fname, '/');
    if (p) {
	assert(p-fname < 63);
	strncpy(g.dir, fname, p-fname);
	g.dir[p-fname] = 0;
	p++; /* skip '/' */
    }
    else {
	p = fname;
	strcpy(g.dir, "");
    }

    rc = sscanf(p, "%02d%03d%03d", &g.prob, &g.ix, &g.iy);

    if (rc != 3 || g.ix >= g.nx || g.iy >= g.ny) {
	fprintf(stderr, "filename [%s] seems invalid.\n", fname);
	exit(1);
    }

    get_ansfilename(g.ansfilename);
    return 0;
}

int main(int argc, char *argv[])
{
    if (argc < 7) {
	printf("Specify output-pic-pathname, #pic-X, #pic-Y, pic-width, pic-height, span-flag\n");
	exit(1);
    }

#if 1
    strcpy(g.filename, argv[1]);
    g.nx = atoi(argv[2]);
    g.ny = atoi(argv[3]);
    g.w = atoi(argv[4]);
    g.h = atoi(argv[5]);
    g.spanning = atoi(argv[6]);

    parse_filename(g.filename);
#else
    strcpy(g.dir, argv[1]);
    g.prob = atoi(argv[2]);
    g.nx = atoi(argv[3]);
    g.ny = atoi(argv[4]);
    g.ix = atoi(argv[5]);
    g.iy = atoi(argv[6]);
    g.w = atoi(argv[7]);
    g.h = atoi(argv[8]);
    g.spanning = atoi(argv[9]);
#endif

    /* other global data */
    get_default_key(g.prob, &g.key);
    if (g.w != g.h) {
	fprintf(stderr, "Width and height must be same\n");
	exit(1);
    }
    if ((g.w & (g.w-1)) != 0) {
	fprintf(stderr, "Picture size must be 2^i\n");
	exit(1);
    }
    if (g.w < 64) {
	fprintf(stderr, "Picture size is too small\n");
	exit(1);
    }
    if (g.w > 16384) {
	fprintf(stderr, "Picture size is too large\n");
	exit(1);
    }

    if (g.spanning) {
	g.border = g.w/16;
    }
    else {
	g.border = 0;
    }
#if 1
    make_one_main(g.ix, g.iy);
#else
    make_main();
#endif
    exit(0);
}
