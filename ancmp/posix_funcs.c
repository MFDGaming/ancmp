#include "posix_funcs.h"
#include <errno.h>

#ifdef _WIN32
#include <windows.h>
#include <io.h>

/**
 * Translates a Windows error code to a POSIX errno value.
 *
 * @param dwError
 *     The Windows error code to translate.
 *
 * @return 
 *     The corresponding POSIX errno value.
 */
static int translate_windows_error_to_errno(DWORD dwError) {
    switch (dwError) {
        case ERROR_FILE_NOT_FOUND:
        case ERROR_PATH_NOT_FOUND:
            return ENOENT;
        case ERROR_TOO_MANY_OPEN_FILES:
            return EMFILE;
        case ERROR_ACCESS_DENIED:
            return EACCES;
        case ERROR_INVALID_HANDLE:
            return EBADF;
        case ERROR_NOT_ENOUGH_MEMORY:
        case ERROR_OUTOFMEMORY:
            return ENOMEM;
        case ERROR_NOT_READY:
        case ERROR_CRC:
            return EIO;
        case ERROR_SHARING_VIOLATION:
        case ERROR_LOCK_VIOLATION:
            return EBUSY;
        case ERROR_HANDLE_EOF:
            return 0;
        case ERROR_BROKEN_PIPE:
            return EPIPE;
        default:
            return EINVAL;
    }
}

long pread(int fd, void *buf, size_t count, off_t offset)
{
    long unsigned int read_bytes = 0;

    OVERLAPPED overlapped;
    memset(&overlapped, 0, sizeof(OVERLAPPED));

    overlapped.OffsetHigh = (sizeof(off_t) <= sizeof(DWORD)) ?
                    (DWORD)0 : (DWORD)((offset >> 32) & 0xFFFFFFFFL);
    overlapped.Offset = (sizeof(off_t) <= sizeof(DWORD)) ?
                    (DWORD)offset : (DWORD)(offset & 0xFFFFFFFFL);

    HANDLE file = (HANDLE)_get_osfhandle(fd);
    if (file == INVALID_HANDLE_VALUE) {
        errno = EBADF;
        return -1;
    }
    SetLastError(0);
    BOOL RF = ReadFile(file, buf, count, &read_bytes, &overlapped);

    /* For some reason it errors when it hits end of file so we don't want to check that */
    if ((RF == 0) && GetLastError() != ERROR_HANDLE_EOF) {
        DWORD dwError = GetLastError();
        errno = translate_windows_error_to_errno(dwError);
        return -1;
    }

    return read_bytes;
}
#endif

size_t bsd_strlcpy(char *dst, const char *src, size_t siz)
{
	char *d = dst;
	const char *s = src;
	size_t n = siz;

	/* Copy as many bytes as will fit */
	if (n != 0) {
		while (--n != 0) {
			if ((*d++ = *s++) == '\0')
				break;
		}
	}

	/* Not enough room in dst, add NUL and traverse rest of src */
	if (n == 0) {
		if (siz != 0)
			*d = '\0';		/* NUL-terminate dst */
		while (*s++)
			;
	}

	return(s - src - 1);	/* count does not include NUL */
}

char *bsd_strsep(char **stringp, const char *delim)
{
	char *s;
	const char *spanp;
	int c, sc;
	char *tok;

	if ((s = *stringp) == NULL)
		return (NULL);
	for (tok = s;;) {
		c = *s++;
		spanp = delim;
		do {
			if ((sc = *spanp++) == c) {
				if (c == 0)
					s = NULL;
				else
					s[-1] = 0;
				*stringp = s;
				return (tok);
			}
		} while (sc != 0);
	}
	/* NOTREACHED */
}