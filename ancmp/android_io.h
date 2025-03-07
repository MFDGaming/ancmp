#pragma once

#include <sys/types.h>
#include <stdio.h>

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

extern android_file_t android_sf[];

int android_fclose(FILE *stream);

int android_putc(int c, FILE *stream);

int android_fputs(const char *s, FILE *stream);

int android_ungetc(int c, FILE *stream);

size_t android_fread(void *ptr, size_t size, size_t nmemb, FILE *stream);

size_t android_fwrite(const void *ptr, size_t size, size_t nmemb, FILE *stream);

long android_ftell(FILE *stream);

int android_fgetpos(FILE *stream, android_fpos_t *pos);

int android_fsetpos(FILE *stream, const android_fpos_t *pos);

int android_fseek(FILE *stream, long offset, int whence);

int android_fflush(FILE *stream);

int android_setvbuf(FILE *stream, char *buf, int mode, size_t size);

int android_getc(FILE *stream);