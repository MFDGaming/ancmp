#ifndef ANCMP_ANDROID_IO_H
#define ANCMP_ANDROID_IO_H

#include <sys/types.h>
#include <stdio.h>
#include <wchar.h>
#include <wctype.h>

typedef off_t android_fpos_t;

typedef struct {
	unsigned char *_base;
	int	_size;
} android_sbuf_t;

typedef	struct {
	unsigned char *_p;	/* current position in (some) buffer */
	int	_r;		/* read space left for getc() */
	int	_w;		/* write space left for putc() */
	short	_flags;		/* flags, below; this FILE is free if 0 */
	short	_file;		/* fileno, if Unix descriptor, else -1 */
	android_sbuf_t _bf;	/* the buffer (at least 1 byte, if !NULL) */
	int	_lbfsize;	/* 0 or -_bf._size, for inline putc */

	/* operations */
	void	*_cookie;	/* cookie passed to io functions */
	int	(*_close)(void *);
	int	(*_read)(void *, char *, int);
	android_fpos_t	(*_seek)(void *, android_fpos_t, int);
	int	(*_write)(void *, const char *, int);

	/* extension data, to avoid further ABI breakage */
	android_sbuf_t _ext;
	/* data for long sequences of ungetc() */
	unsigned char *_up;	/* saved _p when _p is doing ungetc data */
	int	_ur;		/* saved _r when _r is counting ungetc data */

	/* tricks to meet minimum requirements even when malloc() fails */
	unsigned char _ubuf[3];	/* guarantee an ungetc() buffer */
	unsigned char _nbuf[1];	/* guarantee a getc() buffer */

	/* separate buffer for fgetln() when line crosses buffer boundary */
	android_sbuf_t _lb;	/* buffer for fgetln() */

	/* Unix stdio files get aligned to block boundaries on fseek() */
	int	_blksize;	/* stat.st_blksize (may be != _bf._size) */
	android_fpos_t	_offset;	/* current lseek offset */
} android_file_t;

typedef struct {
    android_file_t afile;
    FILE *file;
} custom_file_t;

extern android_file_t android_sf[];

custom_file_t *android_fopen(const char *filename, const char *mode);

custom_file_t *android_fdopen(int fd, const char *mode);

int android_fclose(custom_file_t *stream);

int android_putc(int c, custom_file_t *stream);

int android_fputs(const char *s, custom_file_t *stream);

int android_ungetc(int c, custom_file_t *stream);

size_t android_fread(void *ptr, size_t size, size_t nmemb, custom_file_t *stream);

size_t android_fwrite(const void *ptr, size_t size, size_t nmemb, custom_file_t *stream);

long android_ftell(custom_file_t *stream);

int android_fgetpos(custom_file_t *stream, android_fpos_t *pos);

int android_fsetpos(custom_file_t *stream, const android_fpos_t *pos);

int android_fseek(custom_file_t *stream, long offset, int whence);

int android_fflush(custom_file_t *stream);

int android_setvbuf(custom_file_t *stream, char *buf, int mode, size_t size);

int android_getc(custom_file_t *stream);

int android_fprintf(custom_file_t *stream, const char *restrict format, ...);

int android_fscanf(custom_file_t *stream, const char *restrict format, ...);

char *android_fgets(char *s, int n, custom_file_t *stream);

int android_fputc(int c, custom_file_t *stream);

wint_t android_putwc(wchar_t wc, custom_file_t *stream);

wint_t android_ungetwc(wint_t wc, custom_file_t *stream);

wint_t android_getwc(custom_file_t *stream);

int android_ferror(custom_file_t *stream);

int android_feof(custom_file_t *stream);

#endif