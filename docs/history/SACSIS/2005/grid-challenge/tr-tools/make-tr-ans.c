#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>
#include <math.h>
#include <assert.h>
#include "pic.h"

#define MORTON
/*#define LOG*/
#define MYRAND

#define MAP(pic, x, y) (pic)->buf[(y)*(pic)->w+(x)]

typedef struct {
    int count;
    int area;
} res_t;

typedef struct {
    int used;
    int w;
    int h;
    int local_count;
    int span_x_count;
    int span_y_count;
    char *l_border;
    char *r_border;
    char *u_border;
    char *d_border;
} picinfo_t;

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
    char *dir;
    int prob;
    crypt_key_t key;
    int nx;
    int ny;
    int w;
    int h;

    picinfo_t *picinfos;

} g;


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

int count_local(pic_t *pic, picinfo_t *pi)
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

    pi->w = pic->w;
    pi->h = pic->h;
    pi->local_count = count;
    return 0;
}


/*******************************/


int count_span(char *my_border, char *neighbor, int len)
{
    int count;
    int i;

    int spanflag = 0;
    int prevc;
    int nowc;

    count = 0;
    prevc = 0;
    for (i = 0; i < len; i++) {
	nowc = my_border[i];
	if (nowc > 0) {
	    if (neighbor[i] > 0) {
		spanflag = 1;
	    }
	}
	else {
	    if (prevc > 0 && nowc == 0) {
		/* a line finished */
		if (spanflag) {
		    //printf("SPAN\n");
		    count++;
		}
		else {
		    //printf("NO SPAN\n");
		}
	    }
	    spanflag = 0;
	}

	prevc = nowc;
    }

    if (prevc > 0) {
	/* a line finished */
	if (spanflag) {
#ifdef LOG
	    printf("SPAN (at end)\n");
#endif
	    count++;
	}
	else {
#ifdef LOG
	    printf("NO SPAN (at end)\n");
#endif
	}
    }

    return count;
}


int get_filename(char *fn, int ix, int iy)
{
    sprintf(fn, "%s/%02d%03d%03d.mdt", g.dir, g.prob, ix, iy);
    return 0;
}

int get_fileid(char *fn, int ix, int iy)
{
    /* no path, no ext */
    sprintf(fn, "%02d%03d%03d", g.prob, ix, iy);
    return 0;
}

int get_ansfilename(char *fn)
{
    sprintf(fn, "%s/%02d.ans", g.dir, g.prob);
    return 0;
}

int count_local_forall()
{
    int ix;
    int iy;
    pic_t *pic;
#ifdef MORTON
    pic_t *picm;
#endif

    g.picinfos = calloc(g.nx*g.ny, sizeof(picinfo_t));

    pic = pic_new(0, 0);
#ifdef MORTON
    picm = pic_new(0, 0);
#endif

    for (iy = 0; iy < g.ny; iy++) {
	for (ix = 0; ix < g.nx; ix++) {
	    char filename[64];
	    res_t ress;
	    res_t yress;
	    res_t xress;
	    picinfo_t *pi = &g.picinfos[iy*g.nx+ix];

	    {
		struct timeval st, et;

		get_filename(filename, ix, iy);
#ifdef LOG
		printf("read [%s]\n", filename);
		gettimeofday(&st, NULL);
#endif
#ifdef MORTON
#ifdef LOG
		printf("In MORTON ORDER!\n");
#endif
		//pic_readfile(picm, filename, &g.key);
		//pic_from_morton(pic, picm);
		pic_morton_readfile(pic, filename, &g.key);
#else
#ifdef LOG
		printf("In ROW MAJOR ORDER!\n");
#endif
		pic_readfile(pic, filename, &g.key);
#endif
#ifdef LOG
		gettimeofday(&et, NULL);
		printf("reading took %d us\n",
		       (et.tv_sec-st.tv_sec)*1000000+(et.tv_usec-st.tv_usec));
		printf("count [%s] locally...\n", filename);
#endif
	    }

	    pi->used = 1;
	    count_local(pic, pi);

	    printf(".");
	    fflush(stdout);

#ifdef LOG
	    printf("local count at [%d,%d] -> %d\n", ix, iy, pi->local_count);
#endif	    
	    {
		int x;
		int y;
		/* copy border for later */
		pi->r_border = malloc(pic->h);
		pi->l_border = malloc(pic->h);
		for (y = 0; y < pic->h; y++) {
		    pi->l_border[y] = MAP(pic, 0, y);
		    pi->r_border[y] = MAP(pic, pic->w-1, y);
		}
		pi->u_border = malloc(pic->w);
		pi->d_border = malloc(pic->w);
		for (x = 0; x < pic->w; x++) {
		    pi->u_border[x] = MAP(pic, x, 0);
		    pi->d_border[x] = MAP(pic, x, pic->h-1);
		}
	    }

	}
    }

    return 0;
}

int count_span_forall()
{
    int ix;
    int iy;
    for (iy = 0; iy < g.ny; iy++) {
	for (ix = 0; ix < g.nx; ix++) {
	    picinfo_t *pi = &g.picinfos[iy*g.nx+ix];
	    pi->span_x_count = 0;
	    pi->span_y_count = 0;

	    if (pi->used) {

		if (ix > 0) {
		    picinfo_t *pil;
		    pil = &g.picinfos[iy*g.nx+(ix-1)];
		    if (pil->used) {
			pi->span_x_count =
			    count_span(pi->l_border, pil->r_border, pi->h);
		    }
		}

		if (iy > 0) {
		    picinfo_t *piu;
		    piu = &g.picinfos[(iy-1)*g.nx+ix];
		    if (piu->used) {
			pi->span_y_count =
			    count_span(pi->u_border, piu->d_border, pi->w);
		    }
		}
	    }
	}
    }
    return 0;
}

int make_ansfile()
{
    FILE *ofp;
    char ansname[64];
    int ix;
    int iy;
    int total_count = 0;

    /* *** First line */
    get_ansfilename(ansname);
    ofp = fopen(ansname, "w");
#if 1
    fprintf(ofp, "%d %d %d %d\n",
	    g.prob, g.nx, g.ny, 0);
#else
    fprintf(ofp, "%d %d %d %d\n",
	    g.prob, g.ny, g.nx, 0);
#endif

    for (iy = 0; iy < g.ny; iy++) {
	for (ix = 0; ix < g.nx; ix++) {
	    char filename[64];
	    picinfo_t *pi = &g.picinfos[iy*g.nx+ix];

	    get_fileid(filename, ix, iy);
	    fprintf(ofp, "%s %d %d %d\n",
		    filename, pi->local_count,
		    pi->span_x_count, pi->span_y_count);
	    fflush(ofp);

	    total_count += 
		pi->local_count - pi->span_x_count - pi->span_y_count;

	}
    }
    printf("\n");
    fclose(ofp);
    return total_count;
}

int check_data(int total_count)
{
    int ans = -1;
#ifdef MYRAND
    switch (g.prob) {
    case 1:
	ans = 805;
	break;
    case 2:
	ans = 37654;
	break;
    case 3:
	ans = 77058;
	break;
    case 4:
	ans = 1055;
	break;
    case 5:
	ans = 42931;
	break;
    case 6:
	ans = 83362;
	break;
    }
#else
    switch (g.prob) {
    case 1:
	ans = 869;
	break;
    case 2:
	ans = 40240;
	break;
    case 3:
	ans = 72710;
	break;
    case 4:
	ans = 1023;
	break;
    case 5:
	ans = 39862;
	break;
    case 6:
	ans = 90019;
	break;
    }
#endif

    if (ans < 0) {
	/* no check */
	return 0;
    }

    if (ans != total_count) {
	fprintf(stderr, "WARNING: Created data (%d) may differ from committee's data.\n", total_count);
	fprintf(stderr, "WARNING: Consult committee.\n");
    }

    return 0;
}

int main(int argc, char *argv[])
{
    if (argc < 5) {
	printf("Specify in-dir, problem no, #pic-X, #pic-Y\n");
	exit(1);
    }

    g.dir = argv[1];
    g.prob = atoi(argv[2]);
    g.nx = atoi(argv[3]);
    g.ny = atoi(argv[4]);

    get_default_key(g.prob, &g.key);

    count_local_forall();
    count_span_forall();

    {
	int total_count;
	total_count = make_ansfile();
	check_data(total_count);
    }
    return 0;
}


