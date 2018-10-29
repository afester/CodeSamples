#include <windows.h>
#include <stdio.h>
#include <stdint.h>

#define PORTA (*(SFR+0))
#define PORTB (*(SFR+1))
#define PORTC (*(SFR+2))
#define PORTD (*(SFR+3))

LONG __stdcall handler( EXCEPTION_POINTERS* pep ) {
   printf("HANDLER");
   PCONTEXT u = (PCONTEXT) pep->ContextRecord;
   unsigned char* pc = (unsigned char*) u->Rip;

   // Here, we need to analyze the opcode....
   //
   printf("HANDLER2 %016X\n", pc);
   printf("%02X %02X %02X %02X %02X %02X\n", pc[0], pc[1], pc[2], pc[3], pc[4], pc[5]);
    // 00000001400055CD: C6 40 03 53        mov         byte ptr [rax+3],53h
    printf("Dest: %016X, Value: %02X\n", u->Rax + pc[2], pc[3]);
    // 00000001400055CD: C6 40 03 53        mov         byte ptr [rax+3],53h
    printf("Dest: %016X, Value: %02X\n", u->Rax + pc[2], pc[3]);

    return EXCEPTION_EXECUTE_HANDLER; 
}


int main (int argc, char ** argv) {
    SYSTEM_INFO system_info;
    GetSystemInfo (&system_info);
    int pagesize = system_info.dwPageSize;
    printf("Page size: %d\n", pagesize);

    SetUnhandledExceptionFilter(handler);

    uint8_t*  SFR = (uint8_t*) VirtualAlloc(NULL, pagesize,
                         MEM_RESERVE | MEM_COMMIT,
                         PAGE_READONLY | PAGE_GUARD);
    printf("Memory page: %016X - %016X\n", SFR, SFR + pagesize - 1);

//    PORTD = 0x53;
//     PORTD |= 0x20;
    uint8_t val = PORTD;    // GUARD handler also called on memory read(?)

    return 0;
}


#if 0
// Linux

#define _GNU_SOURCE

#include <unistd.h>
#include <stdio.h>
#include <stdint.h>
#include <sys/mman.h>

#include <signal.h>
#include <sys/ucontext.h>

#define PORTA (*(SFR+0))
#define PORTB (*(SFR+1))
#define PORTC (*(SFR+2))
#define PORTD (*(SFR+3))

static void handler(int sig, siginfo_t* si, void* unused) {
   ucontext_t* u = (ucontext_t*) unused;
   unsigned char* pc = (unsigned char*) u->uc_mcontext.gregs[REG_RIP];

   // Here, we need to analyze the opcode....
   //
   printf("%02X %02X %02X %02X %02X %02X\n", pc[0], pc[1], pc[2], pc[3], pc[4], pc[5]);
   // printf("Dest: %016X, Value: %02X\n", u->uc_mcontext.gregs[REG_RDX], u->uc_mcontext.gregs[REG_RAX] & 0xFF);
   printf("Dest: %016X, Value: %02X\n", u->uc_mcontext.gregs[REG_RAX], pc[2]);
   exit(0);
}

int main() {
   int pagesize = sysconf(_SC_PAGE_SIZE);
   printf("Page size: %d\n", pagesize);

   struct sigaction sa;
   sa.sa_flags = SA_SIGINFO;
   sigemptyset(&sa.sa_mask);
   sa.sa_sigaction = handler;
   if (sigaction(SIGSEGV, &sa, NULL) == -1) 
       perror("sigaction");

   // uint8_t* SFR = calloc(pagesize, 1);
   uint8_t* SFR = memalign(pagesize, pagesize); // calloc(pagesize, 1);
   printf("Memory page: %016X - %016X\n", SFR, SFR + pagesize - 1);
   int x = mprotect(SFR, 256, PROT_READ);
   printf("RESULT: %d\n", x);
   perror("XX");

//    PORTD |= 0x20;
   PORTD = 0x42;
}
#endif