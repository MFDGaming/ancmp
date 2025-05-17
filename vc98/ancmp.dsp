# Microsoft Developer Studio Project File - Name="ancmp" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Static Library" 0x0104

CFG=ancmp - Win32 Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "ancmp.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "ancmp.mak" CFG="ancmp - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "ancmp - Win32 Release" (based on "Win32 (x86) Static Library")
!MESSAGE "ancmp - Win32 Debug" (based on "Win32 (x86) Static Library")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
CPP=cl.exe
RSC=rc.exe

!IF  "$(CFG)" == "ancmp - Win32 Release"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "Release"
# PROP BASE Intermediate_Dir "Release"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "Release"
# PROP Intermediate_Dir "Release"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_MBCS" /D "_LIB" /YX /FD /c
# ADD CPP /nologo /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_MBCS" /D "_LIB" /YX /FD /c
# ADD BASE RSC /l 0x409 /d "NDEBUG"
# ADD RSC /l 0x409 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LIB32=link.exe -lib
# ADD BASE LIB32 /nologo
# ADD LIB32 /nologo

!ELSEIF  "$(CFG)" == "ancmp - Win32 Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "Debug"
# PROP BASE Intermediate_Dir "Debug"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "Debug"
# PROP Intermediate_Dir "Debug"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_MBCS" /D "_LIB" /YX /FD /GZ /c
# ADD CPP /nologo /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_MBCS" /D "_LIB" /D "ANCMP_NO_STDINT" /D LINKER_DEBUG=0 /YX /FD /GZ /c
# ADD BASE RSC /l 0x409 /d "_DEBUG"
# ADD RSC /l 0x409 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LIB32=link.exe -lib
# ADD BASE LIB32 /nologo
# ADD LIB32 /nologo

!ENDIF 

# Begin Target

# Name "ancmp - Win32 Release"
# Name "ancmp - Win32 Debug"
# Begin Group "ancmp"

# PROP Default_Filter ""
# Begin Group "wchar"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\ancmp\wchar\android_wchar.h
# End Source File
# Begin Source File

SOURCE=..\ancmp\wchar\android_wchar_internal.h
# End Source File
# Begin Source File

SOURCE=..\ancmp\wchar\android_wcpcpy.c
# End Source File
# Begin Source File

SOURCE=..\ancmp\wchar\android_wcpcpy.h
# End Source File
# Begin Source File

SOURCE=..\ancmp\wchar\android_wcpncpy.c
# End Source File
# Begin Source File

SOURCE=..\ancmp\wchar\android_wcpncpy.h
# End Source File
# Begin Source File

SOURCE=..\ancmp\wchar\android_wcscasecmp.c
# End Source File
# Begin Source File

SOURCE=..\ancmp\wchar\android_wcscasecmp.h
# End Source File
# Begin Source File

SOURCE=..\ancmp\wchar\android_wcscat.c
# End Source File
# Begin Source File

SOURCE=..\ancmp\wchar\android_wcscat.h
# End Source File
# Begin Source File

SOURCE=..\ancmp\wchar\android_wcschr.c
# End Source File
# Begin Source File

SOURCE=..\ancmp\wchar\android_wcschr.h
# End Source File
# Begin Source File

SOURCE=..\ancmp\wchar\android_wcscmp.c
# End Source File
# Begin Source File

SOURCE=..\ancmp\wchar\android_wcscmp.h
# End Source File
# Begin Source File

SOURCE=..\ancmp\wchar\android_wcscoll.c
# End Source File
# Begin Source File

SOURCE=..\ancmp\wchar\android_wcscoll.h
# End Source File
# Begin Source File

SOURCE=..\ancmp\wchar\android_wcscpy.c
# End Source File
# Begin Source File

SOURCE=..\ancmp\wchar\android_wcscpy.h
# End Source File
# Begin Source File

SOURCE=..\ancmp\wchar\android_wcscspn.c
# End Source File
# Begin Source File

SOURCE=..\ancmp\wchar\android_wcscspn.h
# End Source File
# Begin Source File

SOURCE=..\ancmp\wchar\android_wcsdup.c
# End Source File
# Begin Source File

SOURCE=..\ancmp\wchar\android_wcsdup.h
# End Source File
# Begin Source File

SOURCE=..\ancmp\wchar\android_wcslcat.c
# End Source File
# Begin Source File

SOURCE=..\ancmp\wchar\android_wcslcat.h
# End Source File
# Begin Source File

SOURCE=..\ancmp\wchar\android_wcslcpy.c
# End Source File
# Begin Source File

SOURCE=..\ancmp\wchar\android_wcslcpy.h
# End Source File
# Begin Source File

SOURCE=..\ancmp\wchar\android_wcslen.c
# End Source File
# Begin Source File

SOURCE=..\ancmp\wchar\android_wcslen.h
# End Source File
# Begin Source File

SOURCE=..\ancmp\wchar\android_wcsncasecmp.c
# End Source File
# Begin Source File

SOURCE=..\ancmp\wchar\android_wcsncasecmp.h
# End Source File
# Begin Source File

SOURCE=..\ancmp\wchar\android_wcsncat.c
# End Source File
# Begin Source File

SOURCE=..\ancmp\wchar\android_wcsncat.h
# End Source File
# Begin Source File

SOURCE=..\ancmp\wchar\android_wcsncmp.c
# End Source File
# Begin Source File

SOURCE=..\ancmp\wchar\android_wcsncmp.h
# End Source File
# Begin Source File

SOURCE=..\ancmp\wchar\android_wcsncpy.c
# End Source File
# Begin Source File

SOURCE=..\ancmp\wchar\android_wcsncpy.h
# End Source File
# Begin Source File

SOURCE=..\ancmp\wchar\android_wcsnlen.c
# End Source File
# Begin Source File

SOURCE=..\ancmp\wchar\android_wcsnlen.h
# End Source File
# Begin Source File

SOURCE=..\ancmp\wchar\android_wcspbrk.c
# End Source File
# Begin Source File

SOURCE=..\ancmp\wchar\android_wcspbrk.h
# End Source File
# Begin Source File

SOURCE=..\ancmp\wchar\android_wcsrchr.c
# End Source File
# Begin Source File

SOURCE=..\ancmp\wchar\android_wcsrchr.h
# End Source File
# Begin Source File

SOURCE=..\ancmp\wchar\android_wcsspn.c
# End Source File
# Begin Source File

SOURCE=..\ancmp\wchar\android_wcsspn.h
# End Source File
# Begin Source File

SOURCE=..\ancmp\wchar\android_wcsstr.c
# End Source File
# Begin Source File

SOURCE=..\ancmp\wchar\android_wcsstr.h
# End Source File
# Begin Source File

SOURCE=..\ancmp\wchar\android_wcstok.c
# End Source File
# Begin Source File

SOURCE=..\ancmp\wchar\android_wcstok.h
# End Source File
# Begin Source File

SOURCE=..\ancmp\wchar\android_wcswidth.c
# End Source File
# Begin Source File

SOURCE=..\ancmp\wchar\android_wcswidth.h
# End Source File
# Begin Source File

SOURCE=..\ancmp\wchar\android_wcsxfrm.c
# End Source File
# Begin Source File

SOURCE=..\ancmp\wchar\android_wcsxfrm.h
# End Source File
# Begin Source File

SOURCE=..\ancmp\wchar\android_wctype.c
# End Source File
# Begin Source File

SOURCE=..\ancmp\wchar\android_wctype.h
# End Source File
# Begin Source File

SOURCE=..\ancmp\wchar\android_wmemchr.c
# End Source File
# Begin Source File

SOURCE=..\ancmp\wchar\android_wmemchr.h
# End Source File
# Begin Source File

SOURCE=..\ancmp\wchar\android_wmemcmp.c
# End Source File
# Begin Source File

SOURCE=..\ancmp\wchar\android_wmemcmp.h
# End Source File
# Begin Source File

SOURCE=..\ancmp\wchar\android_wmemcpy.c
# End Source File
# Begin Source File

SOURCE=..\ancmp\wchar\android_wmemcpy.h
# End Source File
# Begin Source File

SOURCE=..\ancmp\wchar\android_wmemmove.c
# End Source File
# Begin Source File

SOURCE=..\ancmp\wchar\android_wmemmove.h
# End Source File
# Begin Source File

SOURCE=..\ancmp\wchar\android_wmemset.c
# End Source File
# Begin Source File

SOURCE=..\ancmp\wchar\android_wmemset.h
# End Source File
# End Group
# Begin Group "string"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\ancmp\string\android_bcopy.c
# End Source File
# Begin Source File

SOURCE=..\ancmp\string\android_bcopy.h
# End Source File
# Begin Source File

SOURCE=..\ancmp\string\android_index.c
# End Source File
# Begin Source File

SOURCE=..\ancmp\string\android_index.h
# End Source File
# Begin Source File

SOURCE=..\ancmp\string\android_memccpy.c
# End Source File
# Begin Source File

SOURCE=..\ancmp\string\android_memccpy.h
# End Source File
# Begin Source File

SOURCE=..\ancmp\string\android_memchr.c
# End Source File
# Begin Source File

SOURCE=..\ancmp\string\android_memchr.h
# End Source File
# Begin Source File

SOURCE=..\ancmp\string\android_memcmp.c
# End Source File
# Begin Source File

SOURCE=..\ancmp\string\android_memcmp.h
# End Source File
# Begin Source File

SOURCE=..\ancmp\string\android_memcpy.c
# End Source File
# Begin Source File

SOURCE=..\ancmp\string\android_memcpy.h
# End Source File
# Begin Source File

SOURCE=..\ancmp\string\android_memmem.c
# End Source File
# Begin Source File

SOURCE=..\ancmp\string\android_memmem.h
# End Source File
# Begin Source File

SOURCE=..\ancmp\string\android_memmove.c
# End Source File
# Begin Source File

SOURCE=..\ancmp\string\android_memmove.h
# End Source File
# Begin Source File

SOURCE=..\ancmp\string\android_memrchr.c
# End Source File
# Begin Source File

SOURCE=..\ancmp\string\android_memrchr.h
# End Source File
# Begin Source File

SOURCE=..\ancmp\string\android_memset.c
# End Source File
# Begin Source File

SOURCE=..\ancmp\string\android_memset.h
# End Source File
# Begin Source File

SOURCE=..\ancmp\string\android_memswap.c
# End Source File
# Begin Source File

SOURCE=..\ancmp\string\android_memswap.h
# End Source File
# Begin Source File

SOURCE=..\ancmp\string\android_strcasecmp.c
# End Source File
# Begin Source File

SOURCE=..\ancmp\string\android_strcasecmp.h
# End Source File
# Begin Source File

SOURCE=..\ancmp\string\android_strcasestr.c
# End Source File
# Begin Source File

SOURCE=..\ancmp\string\android_strcasestr.h
# End Source File
# Begin Source File

SOURCE=..\ancmp\string\android_strcat.c
# End Source File
# Begin Source File

SOURCE=..\ancmp\string\android_strcat.h
# End Source File
# Begin Source File

SOURCE=..\ancmp\string\android_strchr.c
# End Source File
# Begin Source File

SOURCE=..\ancmp\string\android_strchr.h
# End Source File
# Begin Source File

SOURCE=..\ancmp\string\android_strcmp.c
# End Source File
# Begin Source File

SOURCE=..\ancmp\string\android_strcmp.h
# End Source File
# Begin Source File

SOURCE=..\ancmp\string\android_strcoll.c
# End Source File
# Begin Source File

SOURCE=..\ancmp\string\android_strcoll.h
# End Source File
# Begin Source File

SOURCE=..\ancmp\string\android_strcpy.c
# End Source File
# Begin Source File

SOURCE=..\ancmp\string\android_strcpy.h
# End Source File
# Begin Source File

SOURCE=..\ancmp\string\android_strcspn.c
# End Source File
# Begin Source File

SOURCE=..\ancmp\string\android_strcspn.h
# End Source File
# Begin Source File

SOURCE=..\ancmp\string\android_strdup.c
# End Source File
# Begin Source File

SOURCE=..\ancmp\string\android_strdup.h
# End Source File
# Begin Source File

SOURCE=..\ancmp\string\android_string.h
# End Source File
# Begin Source File

SOURCE=..\ancmp\string\android_strlcat.c
# End Source File
# Begin Source File

SOURCE=..\ancmp\string\android_strlcat.h
# End Source File
# Begin Source File

SOURCE=..\ancmp\string\android_strlcpy.c
# End Source File
# Begin Source File

SOURCE=..\ancmp\string\android_strlcpy.h
# End Source File
# Begin Source File

SOURCE=..\ancmp\string\android_strlen.c
# End Source File
# Begin Source File

SOURCE=..\ancmp\string\android_strlen.h
# End Source File
# Begin Source File

SOURCE=..\ancmp\string\android_strncat.c
# End Source File
# Begin Source File

SOURCE=..\ancmp\string\android_strncat.h
# End Source File
# Begin Source File

SOURCE=..\ancmp\string\android_strncmp.c
# End Source File
# Begin Source File

SOURCE=..\ancmp\string\android_strncmp.h
# End Source File
# Begin Source File

SOURCE=..\ancmp\string\android_strncpy.c
# End Source File
# Begin Source File

SOURCE=..\ancmp\string\android_strncpy.h
# End Source File
# Begin Source File

SOURCE=..\ancmp\string\android_strndup.c
# End Source File
# Begin Source File

SOURCE=..\ancmp\string\android_strndup.h
# End Source File
# Begin Source File

SOURCE=..\ancmp\string\android_strnlen.c
# End Source File
# Begin Source File

SOURCE=..\ancmp\string\android_strnlen.h
# End Source File
# Begin Source File

SOURCE=..\ancmp\string\android_strpbrk.c
# End Source File
# Begin Source File

SOURCE=..\ancmp\string\android_strpbrk.h
# End Source File
# Begin Source File

SOURCE=..\ancmp\string\android_strrchr.c
# End Source File
# Begin Source File

SOURCE=..\ancmp\string\android_strrchr.h
# End Source File
# Begin Source File

SOURCE=..\ancmp\string\android_strsep.c
# End Source File
# Begin Source File

SOURCE=..\ancmp\string\android_strsep.h
# End Source File
# Begin Source File

SOURCE=..\ancmp\string\android_strspn.c
# End Source File
# Begin Source File

SOURCE=..\ancmp\string\android_strspn.h
# End Source File
# Begin Source File

SOURCE=..\ancmp\string\android_strstr.c
# End Source File
# Begin Source File

SOURCE=..\ancmp\string\android_strstr.h
# End Source File
# Begin Source File

SOURCE=..\ancmp\string\android_strtok.c
# End Source File
# Begin Source File

SOURCE=..\ancmp\string\android_strtok.h
# End Source File
# Begin Source File

SOURCE=..\ancmp\string\android_strtotimeval.c
# End Source File
# Begin Source File

SOURCE=..\ancmp\string\android_strtotimeval.h
# End Source File
# Begin Source File

SOURCE=..\ancmp\string\android_strxfrm.c
# End Source File
# Begin Source File

SOURCE=..\ancmp\string\android_strxfrm.h
# End Source File
# End Group
# Begin Group "tzcode"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\ancmp\tzcode\android_strftime.c
# End Source File
# Begin Source File

SOURCE=..\ancmp\tzcode\android_strftime.h
# End Source File
# Begin Source File

SOURCE=..\ancmp\tzcode\android_tz.c
# End Source File
# Begin Source File

SOURCE=..\ancmp\tzcode\android_tz.h
# End Source File
# End Group
# Begin Group "inet"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\ancmp\inet\android_inet.h
# End Source File
# Begin Source File

SOURCE=..\ancmp\inet\android_inet_ntop.c
# End Source File
# Begin Source File

SOURCE=..\ancmp\inet\android_inet_ntop.h
# End Source File
# End Group
# Begin Source File

SOURCE=..\ancmp\abi_fix.h
# End Source File
# Begin Source File

SOURCE=..\ancmp\ancmp_stdint.h
# End Source File
# Begin Source File

SOURCE=..\ancmp\android_aeabi.c
# End Source File
# Begin Source File

SOURCE=..\ancmp\android_aeabi.h
# End Source File
# Begin Source File

SOURCE=..\ancmp\android_arc4random.c
# End Source File
# Begin Source File

SOURCE=..\ancmp\android_arc4random.h
# End Source File
# Begin Source File

SOURCE=..\ancmp\android_atomic.c
# End Source File
# Begin Source File

SOURCE=..\ancmp\android_atomic.h
# End Source File
# Begin Source File

SOURCE=..\ancmp\android_auxvec.h
# End Source File
# Begin Source File

SOURCE=..\ancmp\android_ctype.c
# End Source File
# Begin Source File

SOURCE=..\ancmp\android_ctype.h
# End Source File
# Begin Source File

SOURCE=..\ancmp\android_cxa.c
# End Source File
# Begin Source File

SOURCE=..\ancmp\android_cxa.h
# End Source File
# Begin Source File

SOURCE=..\ancmp\android_cxa_guard.c
# End Source File
# Begin Source File

SOURCE=..\ancmp\android_cxa_guard.h
# End Source File
# Begin Source File

SOURCE=..\ancmp\android_dirent.c
# End Source File
# Begin Source File

SOURCE=..\ancmp\android_dirent.h
# End Source File
# Begin Source File

SOURCE=..\ancmp\android_dlfcn.c
# End Source File
# Begin Source File

SOURCE=..\ancmp\android_dlfcn.h
# End Source File
# Begin Source File

SOURCE=..\ancmp\android_elf.h
# End Source File
# Begin Source File

SOURCE=..\ancmp\android_elf_exec.h
# End Source File
# Begin Source File

SOURCE=..\ancmp\android_errno.c
# End Source File
# Begin Source File

SOURCE=..\ancmp\android_errno.h
# End Source File
# Begin Source File

SOURCE=..\ancmp\android_fcntl.c
# End Source File
# Begin Source File

SOURCE=..\ancmp\android_fcntl.h
# End Source File
# Begin Source File

SOURCE=..\ancmp\android_fnmatch.c
# End Source File
# Begin Source File

SOURCE=..\ancmp\android_fnmatch.h
# End Source File
# Begin Source File

SOURCE=..\ancmp\android_futex.c
# End Source File
# Begin Source File

SOURCE=..\ancmp\android_futex.h
# End Source File
# Begin Source File

SOURCE=..\ancmp\android_io.c
# End Source File
# Begin Source File

SOURCE=..\ancmp\android_io.h
# End Source File
# Begin Source File

SOURCE=..\ancmp\android_ioctl.c
# End Source File
# Begin Source File

SOURCE=..\ancmp\android_ioctl.h
# End Source File
# Begin Source File

SOURCE=..\ancmp\android_linker_defs.h
# End Source File
# Begin Source File

SOURCE=..\ancmp\android_logd.c
# End Source File
# Begin Source File

SOURCE=..\ancmp\android_logd.h
# End Source File
# Begin Source File

SOURCE=..\ancmp\android_math.c
# End Source File
# Begin Source File

SOURCE=..\ancmp\android_math.h
# End Source File
# Begin Source File

SOURCE=..\ancmp\android_memmap.c
# End Source File
# Begin Source File

SOURCE=..\ancmp\android_memmap.h
# End Source File
# Begin Source File

SOURCE=..\ancmp\android_mmap.c
# End Source File
# Begin Source File

SOURCE=..\ancmp\android_mmap.h
# End Source File
# Begin Source File

SOURCE=..\ancmp\android_operator_new.c
# End Source File
# Begin Source File

SOURCE=..\ancmp\android_operator_new.h
# End Source File
# Begin Source File

SOURCE=..\ancmp\android_posix_types.c
# End Source File
# Begin Source File

SOURCE=..\ancmp\android_posix_types.h
# End Source File
# Begin Source File

SOURCE=..\ancmp\android_pthread.c
# End Source File
# Begin Source File

SOURCE=..\ancmp\android_pthread.h
# End Source File
# Begin Source File

SOURCE=..\ancmp\android_pthread_attr.c
# End Source File
# Begin Source File

SOURCE=..\ancmp\android_pthread_attr.h
# End Source File
# Begin Source File

SOURCE=..\ancmp\android_pthread_cond.c
# End Source File
# Begin Source File

SOURCE=..\ancmp\android_pthread_cond.h
# End Source File
# Begin Source File

SOURCE=..\ancmp\android_pthread_key.c
# End Source File
# Begin Source File

SOURCE=..\ancmp\android_pthread_key.h
# End Source File
# Begin Source File

SOURCE=..\ancmp\android_pthread_mutex.c
# End Source File
# Begin Source File

SOURCE=..\ancmp\android_pthread_mutex.h
# End Source File
# Begin Source File

SOURCE=..\ancmp\android_pthread_threads.c
# End Source File
# Begin Source File

SOURCE=..\ancmp\android_pthread_threads.h
# End Source File
# Begin Source File

SOURCE=..\ancmp\android_sched.h
# End Source File
# Begin Source File

SOURCE=..\ancmp\android_socket.c
# End Source File
# Begin Source File

SOURCE=..\ancmp\android_socket.h
# End Source File
# Begin Source File

SOURCE=..\ancmp\android_sprint.c
# End Source File
# Begin Source File

SOURCE=..\ancmp\android_sprint.h
# End Source File
# Begin Source File

SOURCE=..\ancmp\android_stat.c
# End Source File
# Begin Source File

SOURCE=..\ancmp\android_stat.h
# End Source File
# Begin Source File

SOURCE=..\ancmp\android_strto.c
# End Source File
# Begin Source File

SOURCE=..\ancmp\android_strto.h
# End Source File
# Begin Source File

SOURCE=..\ancmp\android_sysconf.c
# End Source File
# Begin Source File

SOURCE=..\ancmp\android_sysconf.h
# End Source File
# Begin Source File

SOURCE=..\ancmp\android_thread_id.c
# End Source File
# Begin Source File

SOURCE=..\ancmp\android_thread_id.h
# End Source File
# Begin Source File

SOURCE=..\ancmp\android_time.c
# End Source File
# Begin Source File

SOURCE=..\ancmp\android_time.h
# End Source File
# Begin Source File

SOURCE=..\ancmp\android_tls.h
# End Source File
# Begin Source File

SOURCE=..\ancmp\debugger.c
# End Source File
# Begin Source File

SOURCE=..\ancmp\hooks.c
# End Source File
# Begin Source File

SOURCE=..\ancmp\hooks.h
# End Source File
# Begin Source File

SOURCE=..\ancmp\linker.c
# End Source File
# Begin Source File

SOURCE=..\ancmp\linker.h
# End Source File
# Begin Source File

SOURCE=..\ancmp\linker_debug.c
# End Source File
# Begin Source File

SOURCE=..\ancmp\linker_debug.h
# End Source File
# Begin Source File

SOURCE=..\ancmp\linker_environ.c
# End Source File
# Begin Source File

SOURCE=..\ancmp\linker_environ.h
# End Source File
# Begin Source File

SOURCE=..\ancmp\linker_format.c
# End Source File
# Begin Source File

SOURCE=..\ancmp\linker_format.h
# End Source File
# Begin Source File

SOURCE=..\ancmp\posix_funcs.c
# End Source File
# Begin Source File

SOURCE=..\ancmp\posix_funcs.h
# End Source File
# Begin Source File

SOURCE=..\ancmp\rt.c
# End Source File
# End Group
# End Target
# End Project
