#include "android_io.h"
#include <stdlib.h>
#include <stdarg.h>
#include <errno.h>
#include <string.h>

/* stdin, stdout, stderr */
android_file_t android_sf[3];

static FILE *get_fp(custom_file_t *stream) {
    if (stream == NULL) {
        return NULL;
    }
    if (stream == (custom_file_t *)(&android_sf[0])) {
        return stdin;
    }
    if (stream == (custom_file_t *)(&android_sf[1])) {
        return stdout;
    }
    if (stream == (custom_file_t *)(&android_sf[2])) {
        return stderr;
    }
    return stream->file;
}

custom_file_t *android_fopen(const char *filename, const char *mode) {
    char *real_mode = (char *)mode;
    FILE *file;

    if (strcmp(mode, "r") == 0) {
        real_mode = "rb";
    } else if (strcmp(mode, "r+") == 0) {
        real_mode = "r+b";
    } else if (strcmp(mode, "w") == 0) {
        real_mode = "wb";
    } else if (strcmp(mode, "w+") == 0) {
        real_mode = "w+b";
    } else if (strcmp(mode, "a") == 0) {
        real_mode = "ab";
    } else if (strcmp(mode, "a+") == 0) {
        real_mode = "a+b";
    }
    file = fopen(filename, real_mode);
    if (file) {
        custom_file_t *cfile = (custom_file_t *)malloc(sizeof(custom_file_t));
        if (cfile == NULL) {
            return NULL;
        }
        cfile->file = file;
        cfile->afile._file = (short)fileno(file);
        cfile->afile._flags = 0;
        printf("\x1b[32mSuccessfully opened %s\x1b[0m\n", filename);
        return cfile;
    }
    printf("\x1b[31mFailed to open %s due to %d\x1b[0m\n", filename, errno);
    return NULL;
}

custom_file_t *android_fdopen(int fd, const char *mode) {
    char *real_mode = (char *)mode;
    FILE *file;

    if (strcmp(mode, "r") == 0) {
        real_mode = "rb";
    } else if (strcmp(mode, "r+") == 0) {
        real_mode = "r+b";
    } else if (strcmp(mode, "w") == 0) {
        real_mode = "wb";
    } else if (strcmp(mode, "w+") == 0) {
        real_mode = "w+b";
    } else if (strcmp(mode, "a") == 0) {
        real_mode = "ab";
    } else if (strcmp(mode, "a+") == 0) {
        real_mode = "a+b";
    }
    file = fdopen(fd, real_mode);
    if (file) {
        custom_file_t *cfile = (custom_file_t *)malloc(sizeof(custom_file_t));
        if (cfile == NULL) {
            fclose(file);
            return NULL;
        }
        cfile->file = file;
        cfile->afile._file = (short)fileno(file);
        cfile->afile._flags = 0;
        printf("\x1b[32mSuccessfully opened %d\x1b[0m\n", fd);
        return cfile;
    }
    printf("\x1b[31mFailed to open %d due to %d\x1b[0m\n", fd, errno);
    return NULL;
}

int android_fclose(custom_file_t *stream) {
    if (stream != NULL) {
        FILE *fp = get_fp(stream);
        if (fp != stdin && fp != stdout && fp != stderr) {
            int ret = fclose(fp);
            free(stream);
            return ret;
        }
        
    }
    printf("\x1b[31mFailed to close file due to %d\x1b[0m\n", errno);
    return -1;
}

int android_putc(int c, custom_file_t *stream) {
    return putc(c, get_fp(stream));
}

int android_fputs(const char *s, custom_file_t *stream) {
    return fputs(s, get_fp(stream));
}

int android_ungetc(int c, custom_file_t *stream) {
    return ungetc(c, get_fp(stream));
}

size_t android_fread(void *ptr, size_t size, size_t nmemb, custom_file_t *stream) {
    return fread(ptr, size, nmemb, get_fp(stream));
}

size_t android_fwrite(const void *ptr, size_t size, size_t nmemb, custom_file_t *stream) {
    return fwrite(ptr, size, nmemb, get_fp(stream));
}

long android_ftell(custom_file_t *stream) {
    return ftell(get_fp(stream));
}

int android_fgetpos(custom_file_t *stream, android_fpos_t *pos) {
    fpos_t tmp;
    int ret = fgetpos(get_fp(stream), &tmp);
    memcpy(pos, &tmp, (sizeof(fpos_t) > sizeof(android_fpos_t)) ? sizeof(android_fpos_t) : sizeof(fpos_t));
    return ret;
}

int android_fsetpos(custom_file_t *stream, const android_fpos_t *pos) {
    fpos_t tmp;
    memcpy(&tmp, pos, (sizeof(fpos_t) > sizeof(android_fpos_t)) ? sizeof(android_fpos_t) : sizeof(fpos_t));
    return fsetpos(get_fp(stream), &tmp);
}

int android_fseek(custom_file_t *stream, long offset, int whence) {
    return fseek(get_fp(stream), offset, whence);
}

int android_fflush(custom_file_t *stream) {
    return fflush(get_fp(stream));
}

int android_setvbuf(custom_file_t *stream, char *buf, int mode, size_t size) {
    return setvbuf(get_fp(stream), buf, mode, size);
}

int android_getc(custom_file_t *stream) {
    return getc(get_fp(stream));
}

int android_fprintf(custom_file_t *stream, const char *format, ...) {
    va_list args;
    int ret;

    va_start(args, format);
    ret = vfprintf(get_fp(stream), format, args);
    va_end(args);
    return ret;
}

int android_fscanf(custom_file_t *stream, const char *format, ...) {
    va_list args;
    int ret;

    va_start(args, format);
    ret = vfscanf(get_fp(stream), format, args);
    va_end(args);
    return ret;
}

char *android_fgets(char *s, int n, custom_file_t *stream) {
    return fgets(s, n, get_fp(stream));
}

int android_fputc(int c, custom_file_t *stream) {
    return fputc(c, get_fp(stream));
}

wint_t android_putwc(wchar_t wc, custom_file_t *stream) {
    return putwc(wc, get_fp(stream));
}

wint_t android_ungetwc(wint_t wc, custom_file_t *stream) {
	return ungetwc(wc, get_fp(stream));
}

wint_t android_getwc(custom_file_t *stream) {
    return getwc(get_fp(stream));
}

int android_ferror(custom_file_t *stream) {
    return ferror(get_fp(stream));
}

int android_feof(custom_file_t *stream) {
    return feof(get_fp(stream));
}
