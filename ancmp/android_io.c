#include "android_io.h"
#include <stdlib.h>
#include <stdarg.h>

// stdin, stdout, stderr
android_file_t android_sf[3];

static FILE *get_fp(FILE *stream) {
    if (stream == (FILE *)(&android_sf[0])) {
        return stdin;
    }
    if (stream == (FILE *)(&android_sf[1])) {
        return stdout;
    }
    if (stream == (FILE *)(&android_sf[2])) {
        return stderr;
    }
    return stream;
}

int android_fclose(FILE *stream) {
    return fclose(get_fp(stream));
}

int android_putc(int c, FILE *stream) {
    return putc(c, get_fp(stream));
}

int android_fputs(const char *s, FILE *stream) {
    return fputs(s, get_fp(stream));
}

int android_ungetc(int c, FILE *stream) {
    return putc(c, get_fp(stream));
}

size_t android_fread(void *ptr, size_t size, size_t nmemb, FILE *stream) {
    return fread(ptr, size, nmemb, get_fp(stream));
}

size_t android_fwrite(const void *ptr, size_t size, size_t nmemb, FILE *stream) {
    return fwrite(ptr, size, nmemb, get_fp(stream));
}

long android_ftell(FILE *stream) {
    return ftell(get_fp(stream));
}

int android_fgetpos(FILE *stream, android_fpos_t *pos) {
    fpos_t tmp;
    int ret = fgetpos(get_fp(stream), &tmp);
    *pos = *((android_fpos_t *)(&tmp));
    return ret;
}

int android_fsetpos(FILE *stream, const android_fpos_t *pos) {
    long long tmp = *pos;
    return fsetpos(get_fp(stream), (fpos_t *)&tmp);
}

int android_fseek(FILE *stream, long offset, int whence) {
    return fseek(get_fp(stream), offset, whence);
}

int android_fflush(FILE *stream) {
    return fflush(get_fp(stream));
}

int android_setvbuf(FILE *stream, char *buf, int mode, size_t size) {
    return setvbuf(get_fp(stream), buf, mode, size);
}

int android_getc(FILE *stream) {
    return getc(get_fp(stream));
}

int android_fprintf(FILE *stream, const char *restrict format, ...) {
    va_list args;
    va_start(args, format);
    return vfprintf(get_fp(stream), format, args);
}

int android_fscanf(FILE *stream, const char *restrict format, ...) {
    va_list args;
    va_start(args, format);
    return vfscanf(get_fp(stream), format, args);
}

char *android_fgets(char *s, int n, FILE *stream) {
    return fgets(s, n, get_fp(stream));
}

int android_fputc(int c, FILE *stream) {
    return fputc(c, get_fp(stream));
}

wint_t android_putwc(wchar_t wc, FILE *stream) {
    return putwc(wc, get_fp(stream));
}

wint_t android_ungetwc(wint_t wc, FILE *stream) {
	return ungetc(wc, get_fp(stream));
}

wint_t android_getwc(FILE *stream) {
    return getwc(get_fp(stream));
}

int android_ferror(FILE *stream) {
    return ferror(get_fp(stream));
}

int android_feof(FILE *stream) {
    return feof(get_fp(stream));
}