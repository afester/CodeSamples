#include <stdio.h>
#include <unistd.h>



typedef struct _sysconf {
   const char* name;
   int id;
}SYSCONF;

static SYSCONF sysconfNames[] = {
{"ARG_MAX", _SC_ARG_MAX},
{"CHILD_MAX", _SC_CHILD_MAX},
{"HOST_NAME_MAX", _SC_HOST_NAME_MAX},
{"LOGIN_NAME_MAX", _SC_LOGIN_NAME_MAX},
{"clock ticks", _SC_CLK_TCK},
{"OPEN_MAX", _SC_OPEN_MAX},
{"PAGESIZE", _SC_PAGESIZE},
{"RE_DUP_MAX", _SC_RE_DUP_MAX},
{"STREAM_MAX", _SC_STREAM_MAX},
{"TTY_NAME_MAX", _SC_TTY_NAME_MAX},
{"TZNAME_MAX", _SC_TZNAME_MAX},
{"_POSIX_VERSION", _SC_VERSION},
{"BC_BASE_MAX", _SC_BC_BASE_MAX},
{"BC_DIM_MAX", _SC_BC_DIM_MAX},
{"BC_SCALE_MAX", _SC_BC_SCALE_MAX},
{"BC_STRING_MAX", _SC_BC_STRING_MAX},
{"COLL_WEIGHTS_MAX", _SC_COLL_WEIGHTS_MAX},
{"EXPR_NEST_MAX", _SC_EXPR_NEST_MAX},
{"LINE_MAX", _SC_LINE_MAX},
{"RE_DUP_MAX", _SC_RE_DUP_MAX},
{"POSIX2_VERSION", _SC_2_VERSION},
{"POSIX2_C_DEV", _SC_2_C_DEV},
{"POSIX2_FORT_DEV", _SC_2_FORT_DEV},
{"POSIX2_FORT_RUN", _SC_2_FORT_RUN},
{"_POSIX2_LOCALEDEF", _SC_2_LOCALEDEF},
{"POSIX2_SW_DEV", _SC_2_SW_DEV},
{"PHYS_PAGES", _SC_PHYS_PAGES},
{"AVPHYS_PAGES", _SC_AVPHYS_PAGES},
{"NPROCESSORS_CONF", _SC_NPROCESSORS_CONF},
{"NPROCESSORS_ONLN", _SC_NPROCESSORS_ONLN},
};

static const unsigned int sysconfNamesLength = sizeof(sysconfNames) / sizeof(SYSCONF);

void dumpSysconf() {
   int i = 0;
   for (i = 0;  i < sysconfNamesLength;  i++) {
      printf("%s: %ld\n", sysconfNames[i].name, sysconf(sysconfNames[i].id));
   }
}


int main() {
   dumpSysconf();
   return 0;
}
