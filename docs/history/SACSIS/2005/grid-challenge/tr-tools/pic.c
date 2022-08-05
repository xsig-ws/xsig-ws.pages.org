#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/time.h>
#include <math.h>
#include <assert.h>
#include "pic.h"

#define DBYTE

#define MY_LITTLE_ENDIAN
//#define LOG

#define MAP(pic, x, y) (pic)->buf[(y)*(pic)->w+(x)]

/**/
int get_key_from_string(char *str, crypt_key_t *key)
{
    int i;
    int slen;
    int len;
    char *p;
    char *ep;

    ep = strchr(str, '-');
    if (ep) {
	slen = ep-str;
    }
    else {
	slen = strlen(str);
    }
    
    assert(slen % 2 == 0);
    len = slen/2;
    
    key->cur = 0;
    key->len = len;
    memset(key->buf, 0, KEY_LIMIT);
    
    p = str;
    for (i = 0; i < len; i++) {
	int c;
	sscanf(p, "%02x", &c);
	key->buf[i] = (unsigned char)c;
	p += 2;
    }
    return 0;
}

int get_default_key(int prob, crypt_key_t *key)
{
    char *str;
    switch (prob) {
    case 1:
	str = 
	    "c9988f6c2fcb4d492baece7fe9e52a3796e5e6fa";
	break;
    case 2:
	str =
	    "267aeade4ada4933fa7bbf106885e963e03d46d9";
	break;
    case 3:
	str = 
	    "07acb0527fc8341f727b6ac77b56815ae25cef46";
	break;
    case 4:
	str = 
	    "b41283d2d4ca5e6e3259b7c01088e57b2e80f77a";
	break;
    case 5:
	str = 
	    "fdea1c3b65a51f307d7f465ad06e5a3ea1085f92";
	break;
    case 6:
	str = 
	    "e02750f448b185705a23996389418df770f24024";
	break;
    default:
	str = 
	    "00000000";
	break;
    }

    get_key_from_string(str, key);
    return 0;
}

/**/

static int myputc(int c, FILE *fp, crypt_key_t *key)
{
    int rc;
    if (key != NULL) {
	c = c ^ (int)key->buf[key->cur];
    }
    rc = putc(c, fp);
    if (rc != EOF && key != NULL) {
	key->cur = (key->cur+1) % key->len;
    }
    return rc;
}

static mygetc(FILE *fp, crypt_key_t *key)
{
    int c;
    c = getc(fp);
    if (c != EOF && key != NULL) {
	c = c ^ (int)key->buf[key->cur];
	key->cur = (key->cur+1) % key->len;
    }
    return c;
}

/**/
int write_int_2b(FILE *fp, int n, crypt_key_t *key)
{
    unsigned int nn = n;
#ifdef MY_LITTLE_ENDIAN
    myputc((nn>>0)&255, fp, key);
    myputc((nn>>8)&255, fp, key);
#else
    myputc((nn>>8)&255, fp, key);
    myputc((nn>>0)&255, fp, key);
#endif
    return 0;
}

int read_int_2b(FILE *fp, int *np, crypt_key_t *key)
{
    unsigned int nn = 0;
#ifdef MY_LITTLE_ENDIAN
    nn = mygetc(fp, key);
    nn += mygetc(fp, key) << 8;
#else
    nn = mygetc(fp);
    nn = (nn<<8) + mygetc(fp, key);
#endif
    *np = nn;
    return 0;
}

int write_pic_header(FILE *fp, int w, int h, crypt_key_t *key)
{
    write_int_2b(fp, w, key);
    write_int_2b(fp, h, key);
    return 0;
}

int read_pic_header(FILE *fp, int *wp, int *hp, crypt_key_t *key)
{
    read_int_2b(fp, wp, key);
    read_int_2b(fp, hp, key);
    return 0;
}

/* If len is too long, produces several shorter runs. */
int write_pic_run(FILE *fp, int color, int len, crypt_key_t *key)
{
    int lim;
    int bytes = 0;
    assert(color == 0 || color == 1);

#ifdef DBYTE
    lim = 64*256;
#else
    lim = 128;
#endif

    while (len > 0) {
	int lenx = len;
	if (lenx > lim) lenx = lim;
	assert(lenx >= 1);

#ifdef DBYTE
	if (lenx <= 64) {
	    myputc((0<<7) | (color<<6) | (lenx-1), fp, key);
	    bytes++;
	}
	else {
	    myputc((1<<7) | (color<<6) | ((lenx-1)>>8), fp, key);
	    myputc((lenx-1)&255, fp, key);
	    bytes+=2;
	}
#else
	myputc((color<<7) | (lenx-1), fp, key);
	bytes++;
#endif

	len -= lenx;
    }

    return bytes;
}

int read_pic_run(FILE *fp, int *colorp, int *lenp, crypt_key_t *key)
{
    int color;
    int len;
    int bytes = 0;
    
    int dat;
    
    dat = mygetc(fp, key);
    bytes++;
    
    if (dat == EOF) {
	fprintf(stderr, "[read_pic_run] EOF?\n");
	*lenp = -1;
	return 0;
    }
#ifdef DBYTE
    color = (dat >> 6) & 1;
    if ((dat >> 7) != 0) {
	/* two byte data */
	int dat2;
	dat2 = mygetc(fp, key);
	if (dat2 == EOF) {
	    fprintf(stderr, "[read_pic_run] EOF?\n");
	    *lenp = -1;
	    return bytes;
	}
	len = ((dat & 63) << 8 | dat2) + 1;
    }
    else {
	/* one byte data */
	len = (dat & 63) + 1;
    }
#else
    color = dat >> 7;
    len = (dat & 127) + 1;
#endif

    *colorp = color;
    *lenp = len;
    
    return bytes;
}

/* pic_t structure *******************************************/
pic_t *pic_new(int w, int h)
{
    pic_t *pic;
    pic = malloc(sizeof(pic_t));
    if (!pic) {
	fprintf(stderr, "malloc failed\n");
	exit(1);
    }
    pic->w = w;
    pic->h = h;
    if (w > 0 && h > 0) {
	pic->buf = malloc(w*h);
    }
    else {
	pic->buf = NULL;
    }
    return pic;
}

int pic_delete(pic_t *pic)
{
    if (pic->buf) {
	free(pic->buf);
    }
    free(pic);
    return 0;
}

static int pic_resize(pic_t *pic, int w, int h)
{
    int lim;
    pic->w = w;
    pic->h = h;
    lim = w*h;
    if (pic->buf) {
	pic->buf = realloc(pic->buf, lim);
    }
    else {
	pic->buf = malloc(lim);
    }
    
    if (!pic->buf) {
	fprintf(stderr, "malloc failed\n");
	exit(1);
    }
    return 0;
}

/*****************/

int pic_write(pic_t *pic, FILE *ofp, crypt_key_t *key)
{
    int cur;
    int lim = pic->w*pic->h;
    int bytes = 0;

    /* init key cursor */
    key->cur = 0;

    /* header */
    write_pic_header(ofp, pic->w, pic->h, key);

    cur = 0;
    while (1) {
	int color;
	int len = 0;
	int rc;

	if (cur >= lim) {
	    break;
	}

	color = pic->buf[cur];
	assert(color == 0 || color == 1);

	while (1) {
	    if (cur >= lim || pic->buf[cur] != color) {
		break;
	    }

	    cur++;
	    len++;
	}

	assert(len >= 1);
	rc = write_pic_run(ofp, color, len, key);
	if (rc <= 0) {
	    printf("[pic_write] failed to write?\n");
	    return -1;
	}
	bytes += rc;
    }

#ifdef LOG
    fprintf(stderr,
	    "[pic_write] %d bytes written(except hdr)\n", bytes);
#endif
    return 0;
}

int pic_writefile(pic_t *pic, char *filename, crypt_key_t *key)
{
    FILE *ofp;
    int rc;
    ofp = fopen(filename, "wb");
    if (!ofp) {
	printf("Failed to write open %s!!!\n", filename);
	exit(1);
    }
    rc = pic_write(pic, ofp, key);
    
    fclose(ofp);
    return rc;
}

int pic_read(pic_t *pic, FILE *ifp, crypt_key_t *key)
{
    int w = 0;
    int h = 0;
    int cur;
    int lim;

    /* init key cursor */
    key->cur = 0;

    /* header */
    read_pic_header(ifp, &w, &h, key);

#ifdef LOG
    fprintf(stderr, "pic size = %d x %d\n", w, h);
#endif
    pic->w = w;
    pic->h = h;
    lim = w*h;

    pic_resize(pic, w, h);

    cur = 0;
    while (1) {
	int color;
	int len;
	int i;

	if (cur >= lim) {
	    break;
	}

	read_pic_run(ifp, &color, &len, key);

	for (i = 0; i < len; i++) {
	    pic->buf[cur] = color;
	    cur++;
	}
    }

    return 0;
}

int pic_readfile(pic_t *pic, char *filename, crypt_key_t *key)
{
    FILE *ifp;
    int rc;
    ifp = fopen(filename, "rb");
    if (!ifp) {
	fprintf(stderr, "Failed to r-open [%s]!!!\n", filename);
	exit(1);
    }

    rc = pic_read(pic, ifp, key);
    fclose(ifp);
    return rc;
}

/******* handle morton format directly */

typedef struct {
    int color;
    int len;
} run_t;

struct {
    int offs[16];
} pg;

static int make_morton_offs(int w)
{
    int i = 0;
    int *offs = pg.offs;
    offs[i++] = 0 + 0 * w;
    offs[i++] = 1 + 0 * w;
    offs[i++] = 0 + 1 * w;
    offs[i++] = 1 + 1 * w;
    offs[i++] = 2 + 0 * w;
    offs[i++] = 3 + 0 * w;
    offs[i++] = 2 + 1 * w;
    offs[i++] = 3 + 1 * w;
    offs[i++] = 0 + 2 * w;
    offs[i++] = 1 + 2 * w;
    offs[i++] = 0 + 3 * w;
    offs[i++] = 1 + 3 * w;
    offs[i++] = 2 + 2 * w;
    offs[i++] = 3 + 2 * w;
    offs[i++] = 2 + 3 * w;
    offs[i++] = 3 + 3 * w;
    assert(i == 16);
    return 0;
}

static int morton_write_one(FILE *ofp, run_t *crun, int color,
			    crypt_key_t *key)
{
    if (crun->color != color) {
	if (crun->color != -1) {
	    /* flush pervious run */
	    write_pic_run(ofp, crun->color, crun->len, key);
	}
	crun->color = color;
	crun->len = 0;
    }
    crun->len++;
    return 0;
}

static morton_write_rec(pic_t *pic, FILE *ofp, run_t *crun,
			int sx, int sy, int d, crypt_key_t *key)
{
#if 1
    if (d == 4) {
	int i;
	char *p = &MAP(pic, sx, sy);
	for (i = 0; i < 16; i++) {
	    int c = *(p+pg.offs[i]);
	    if (crun->color != c) {
		if (crun->color != -1) {
		    /* flush pervious run */
		    write_pic_run(ofp, crun->color, crun->len, key);
		}
		crun->color = c;
		crun->len = 0;
	    }
	    crun->len++;
	}
	return 0;
    }
#else
    if (d == 2) {
	morton_write_one(ofp, crun, MAP(pic, sx   , sy   ), key);
	morton_write_one(ofp, crun, MAP(pic, sx+1 , sy   ), key);
	morton_write_one(ofp, crun, MAP(pic, sx   , sy+1 ), key);
	morton_write_one(ofp, crun, MAP(pic, sx+1 , sy+1 ), key);
	return 0;
    }
#endif

    {
	int r = d/2;
	assert(d >= 8);
	morton_write_rec(pic, ofp, crun, sx+0, sy+0, r, key);
	morton_write_rec(pic, ofp, crun, sx+r, sy+0, r, key);
	morton_write_rec(pic, ofp, crun, sx+0, sy+r, r, key);
	morton_write_rec(pic, ofp, crun, sx+r, sy+r, r, key);
    }
    return 0;
}

int pic_morton_write(pic_t *pic, FILE *ofp, crypt_key_t *key)
{
    run_t crun;
    /* init key cursor */
    key->cur = 0;

    assert(pic->w >= 4);
    make_morton_offs(pic->w);
    /* header */
    write_pic_header(ofp, pic->w, pic->h, key);
    /* data */
    crun.color = -1;
    crun.len = 0;
    morton_write_rec(pic, ofp, &crun, 0, 0, pic->w, key);
    /* last run */
    if (crun.color >= 0) {
	write_pic_run(ofp, crun.color, crun.len, key);
    }

    return 0;
}

int pic_morton_writefile(pic_t *pic, char *filename, crypt_key_t *key)
{
    FILE *ofp;
    int rc;
    ofp = fopen(filename, "wb");
    if (!ofp) {
	printf("Failed to write open %s!!!\n", filename);
	exit(1);
    }
    rc = pic_morton_write(pic, ofp, key);
    
    fclose(ofp);
    return rc;
}

/* */

static int morton_read_one(FILE *ifp, run_t *crun, crypt_key_t *key)
{
    if (crun->len == 0) {
	read_pic_run(ifp, &crun->color, &crun->len, key);
    }
    assert(crun->len > 0);
    crun->len--;
    return crun->color;
}

static int morton_read_rec(pic_t *pic, FILE *ifp, run_t *crun,
		       int sx, int sy, int d, crypt_key_t *key)
{
#if 1
    if (d == 4) {
	int i;
	char *p = &MAP(pic, sx, sy);
	for (i = 0; i < 16; i++) {
	    int c;
	    if (crun->len == 0) {
		read_pic_run(ifp, &crun->color, &crun->len, key);
	    }
	    assert(crun->len > 0);
	    crun->len--;
	    c = crun->color;
	    *(p+pg.offs[i]) = c;
	}
	return 0;
    }
#else
    if (d == 2) {
	MAP(pic, sx   , sy   ) = morton_read_one(ifp, crun, key);
	MAP(pic, sx+1 , sy   ) = morton_read_one(ifp, crun, key);
	MAP(pic, sx   , sy+1 ) = morton_read_one(ifp, crun, key);
	MAP(pic, sx+1 , sy+1 ) = morton_read_one(ifp, crun, key);
	return 0;
    }
#endif

    {
	int r = d/2;
	assert(d >= 8);
	morton_read_rec(pic, ifp, crun, sx+0, sy+0, r, key);
	morton_read_rec(pic, ifp, crun, sx+r, sy+0, r, key);
	morton_read_rec(pic, ifp, crun, sx+0, sy+r, r, key);
	morton_read_rec(pic, ifp, crun, sx+r, sy+r, r, key);
    }
    return 0;
}

int pic_morton_read(pic_t *pic, FILE *ifp, crypt_key_t *key)
{
    int w = 0;
    int h = 0;
    run_t crun;

    /* init key cursor */
    key->cur = 0;

    /* header */
    read_pic_header(ifp, &w, &h, key);

#ifdef LOG
    fprintf(stderr, "pic size = %d x %d\n", w, h);
#endif
    pic->w = w;
    pic->h = h;
    assert(w >= 4);
    make_morton_offs(w);

    pic_resize(pic, w, h);

    /* data */
    crun.color = -1;
    crun.len = 0;
    morton_read_rec(pic, ifp, &crun,
		    0, 0, pic->w, key);
    assert(crun.len == 0);

    return 0;
}

int pic_morton_readfile(pic_t *pic, char *filename, crypt_key_t *key)
{
    FILE *ifp;
    int rc;
    ifp = fopen(filename, "rb");
    if (!ifp) {
	fprintf(stderr, "Failed to r-open [%s]!!!\n", filename);
	exit(1);
    }

    rc = pic_morton_read(pic, ifp, key);
    fclose(ifp);
    return rc;
}

/******* morton format conversion */
static to_morton_rec(pic_t *picm, pic_t *pic, int idx,
		     int sx, int sy, int d)
{
    if (d == 2) {
	picm->buf[idx+0] = MAP(pic, sx   , sy   );
	picm->buf[idx+1] = MAP(pic, sx+1 , sy   );
	picm->buf[idx+2] = MAP(pic, sx   , sy+1 );
	picm->buf[idx+3] = MAP(pic, sx+1 , sy+1 );

	return 0;
    }

    {
	int r = d/2;
	assert(d >= 4);
	to_morton_rec(picm, pic, idx+0*r*r, sx+0, sy+0, r);
	to_morton_rec(picm, pic, idx+1*r*r, sx+r, sy+0, r);
	to_morton_rec(picm, pic, idx+2*r*r, sx+0, sy+r, r);
	to_morton_rec(picm, pic, idx+3*r*r, sx+r, sy+r, r);
    }
    return 0;
}

int pic_to_morton(pic_t *picm, pic_t *pic)
{
    assert(pic->w == pic->h);
    assert((pic->w & (pic->w - 1)) == 0); /* 2^n */

    pic_resize(picm, pic->w, pic->h);

    to_morton_rec(picm, pic, 0 /* s-idx*/,
		  0, 0, pic->w);
    return 0;
}

/* */
static from_morton_rec(pic_t *pic, pic_t *picm, int idx,
		     int sx, int sy, int d)
{
    if (d == 2) {
	MAP(pic, sx   , sy   ) = picm->buf[idx+0];
	MAP(pic, sx+1 , sy   ) = picm->buf[idx+1];
	MAP(pic, sx   , sy+1 ) = picm->buf[idx+2];
	MAP(pic, sx+1 , sy+1 ) = picm->buf[idx+3];

	return 0;
    }

    {
	int r = d/2;
	assert(d >= 4);
	from_morton_rec(pic, picm, idx+0*r*r, sx+0, sy+0, r);
	from_morton_rec(pic, picm, idx+1*r*r, sx+r, sy+0, r);
	from_morton_rec(pic, picm, idx+2*r*r, sx+0, sy+r, r);
	from_morton_rec(pic, picm, idx+3*r*r, sx+r, sy+r, r);
    }
    return 0;
}

int pic_from_morton(pic_t *pic, pic_t *picm)
{
    assert(picm->w == picm->h);
    assert((picm->w & (picm->w - 1)) == 0); /* 2^n */

    pic_resize(pic, picm->w, picm->h);

    from_morton_rec(pic, picm, 0,
		    0, 0, pic->w);

    return 0;
}
