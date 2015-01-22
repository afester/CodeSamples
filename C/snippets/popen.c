#include <stdio.h>

int main(){
        FILE* fp;
        fp=popen("./mkpasswd -msha-512 -Sqwertyabc asdf","r");
        char buf[120];
        fscanf(fp,"%s",buf);
        printf("result : %s\n",buf);
printf("------------------------\n");
        fp=popen("./mkpasswd -msha-512 -Sqwertyabc","w");
        fprintf(fp,"asdf");
        //fflush(fp);
        return 0;
}
