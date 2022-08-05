typedef struct {
    int w;
    int h;
    char *buf;
} pic_t;

#define KEY_LIMIT 128

typedef struct {
    int len;
    int cur; /* internally used */
    unsigned char buf[KEY_LIMIT];
} crypt_key_t;

pic_t *pic_new(int w, int h);

int pic_write(pic_t *pic, FILE *ofp, crypt_key_t *key);
int pic_writefile(pic_t *pic, char *filename, crypt_key_t *key);
int pic_read(pic_t *pic, FILE *ifp, crypt_key_t *key);
int pic_readfile(pic_t *pic, char *filename, crypt_key_t *key);


int pic_to_morton(pic_t *picm, pic_t *pic);
int pic_from_morton(pic_t *pic, pic_t *picm);

