#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>
#include <math.h>
#include <assert.h>
#include "pic.h"

#define DATADIR_FMT "./data%02d"

#define MORTON
//#define LOG
#define MYRAND

struct {
    char dir[64];
    int prob;
    crypt_key_t key;
    int nx;
    int ny;
    int w;
    int h;
    int border;
    int spanning;
} g;

#define MAP(pic, x, y) (pic)->buf[(y)*(pic)->w+(x)]

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
    rxsize = isqrt(pic->w);
    rysize = isqrt(pic->h);
#else
#warning smaller rect size
    rxsize = isqrt(pic->w) / 5;
    rysize = isqrt(pic->h) / 5;
#endif

    maxnr = pic->w*pic->h/(rxsize*rysize)/5;
    nr = my_rand(maxnr);
    /* make random rectangles */
    for (i = 0; i < nr; i++) {
	int x0 = my_rand(pic->w - 3 - rxsize - bd*2) + 1 + bd;
	int y0 = my_rand(pic->h - 3 - rysize - bd*2) + 1 + bd;
	int w = my_rand(rxsize) + 1;
	int h = my_rand(rysize) + 1;
	assert (x0+w < pic->w-1);
	assert (y0+h < pic->h-1);

	{
	    int x;
	    int y;
	    for (y = y0; y < y0+h; y++) {
		for (x = x0; x < x0+w; x++) {
		    MAP(pic, x, y) = 1;
		}
	    }
	}
    }

    return 0;
}

/* picflag=0: this pic is upper of the border. */
/* picflag=1: this pic is lower of the border. */
/* ix, iy: coord of this pic. */
int make_span_y(pic_t *pic, int ix, int iy, int picflag)
{
    int rxsize;
    int i;
    int nr;
    int maxnr;
    int bd = g.border;
    int px;

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

    return 0;
}

/* picflag=0: this pic is left of the border. */
/* picflag=1: this pic is right of the border. */
/* ix, iy: coord of this pic. */
int make_span_x(pic_t *pic, int ix, int iy, int picflag)
{
    int rysize;
    int i;
    int nr;
    int maxnr;
    int bd = g.border;
    int py;

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

    return 0;
}


int get_filename(char *fn, int ix, int iy)
{
    sprintf(fn, "%s/%02d%03d%03d.mdt", g.dir, g.prob, ix, iy);
    return 0;
}


int make_one(pic_t *pic, int ix, int iy)
{
    char filename[64];
    
#ifdef LOG
    printf("make data on memory\n");
#endif
    make_inner_data(pic, ix, iy);

    if (g.spanning > 0) {
	if (ix > 0) {
	    /* make buildings at left border */
	    make_span_x(pic, ix, iy, 1);
	}
	
	if (ix < g.nx-1) {
	    /* make buildings at right border */
	    make_span_x(pic, ix, iy, 0);
	}
	
	if (iy > 0) {
	    /* make buildings at upper border */
	    make_span_y(pic, ix, iy, 1);
	}
	
	if (iy < g.ny-1) {
	    /* make buildings at lower border */
	    make_span_y(pic, ix, iy, 0);
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
#if 0 && defined MORTON
	pic_t *picm;
	picm = pic_new(pic->w, pic->h);
	pic_to_morton(picm, pic);
#endif

	get_filename(filename, ix, iy);
#ifdef LOG
	printf("try to write to [%s]\n", filename);
#endif
#ifdef MORTON
#ifdef LOG
	printf("In MORTON ORDER!\n");
#endif
	//pic_writefile(picm, filename, &g.key);
	//pic_delete(picm);
	pic_morton_writefile(pic, filename, &g.key);
#else
#ifdef LOG
	printf("In ROW MAJOR ORDER!\n");
#endif
	pic_writefile(pic, filename, &g.key);
#endif
    }
    return 0;
}

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


int main(int argc, char *argv[])
{
    if (argc < 8) {
	printf("Specify data-dir, problen no, #pic-X, #pic-Y, pic-width, pic-height, span-flag\n");
	exit(1);
    }

    strcpy(g.dir, argv[1]);
    g.prob = atoi(argv[2]);
    g.nx = atoi(argv[3]);
    g.ny = atoi(argv[4]);
    g.w = atoi(argv[5]);
    g.h = atoi(argv[6]);
    g.spanning = atoi(argv[7]);

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

    make_main();
}

