#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#define __USE_GNU
#include <dlfcn.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>


typedef int (*orig___lxstat_type)(int __ver, __const char *__filename,
                                  struct stat *__stat_buf);

static orig___lxstat_type orig___lxstat;

static void myInit() __attribute__((constructor));

void myInit() {
    orig___lxstat = (orig___lxstat_type)dlsym(RTLD_NEXT,"__lxstat");
}

static const char prefix[] = "/scratch/afester/git/afester_NewFeatureView";

int __lxstat (int __ver, __const char *__filename,
              struct stat *__stat_buf) {
   const char* cwd = getcwd(NULL, 0);
   printf("CWD: %s - PATH: %s\n", cwd, __filename);
   if (strncmp(cwd, prefix, strlen(prefix)) == 0) {

      if (strncmp(__filename, "..", 2) == 0) {
      printf("REWRITE ..\n");
      }
      else if (strncmp(__filename, "../..", 5) == 0) {
      printf("REWRITE ../..\n");
      }

   }
   free((void*) cwd);
   return orig___lxstat(__ver, __filename, __stat_buf);
}

