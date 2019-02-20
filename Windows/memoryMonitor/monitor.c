#include <windows.h>
#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>

#include "monitor.h"

static omObserver_t theObserver;
static uint8_t* theMemory = 0;
static int pageSize = 0;

inline static DWORD64 getRegisterValue(PCONTEXT u, int regId) {
    switch(regId) {
        case 0 : return u->Rax;
        case 1 : return u->Rcx;
        case 2 : return u->Rdx;
        case 3 : return u->Rbx;
        case 4 : return u->Rsp;
        case 5 : return u->Rbp;
        case 6 : return u->Rsi;
        case 7 : return u->Rdi;
    }

    printf("Invalid register id: %d\n", regId);
    return 0;
}

LONG __stdcall handler( EXCEPTION_POINTERS* pep ) {
   PCONTEXT u = (PCONTEXT) pep->ContextRecord;
   unsigned char* pc = (unsigned char*) u->Rip;

   // Analyze the opcode which caused the exception
   bool opcodeValid = false;
   int opcodeSize = 0;
   uint8_t* destAddr = 0;
   uint8_t value = 0;
   switch(pc[0]) {
       case 0xC6 :  // MOV r/m8, immediate
            if ( (pc[1] & 0xC0) == 0x00) {           // indirect addressing
                opcodeSize = 4;
                opcodeValid = true;
                destAddr = (uint8_t*) getRegisterValue(u, (pc[2] >> 3) & 0x07);
                destAddr += getRegisterValue(u, pc[2] & 0x07);
                value = pc[3];
            } else if ( (pc[1] & 0xC0) == 0x40) {    // indirect with 8 bit displacement
                opcodeSize = 4;
                opcodeValid = true;
                destAddr = (uint8_t*) getRegisterValue(u, pc[1] & 0x07);
                destAddr += pc[2];
                value = pc[3];
            }
            break;

       case 0x88:   // MOV r/m8, r8
            if ( (pc[1] & 0xC0) == 0x00) {    // indirect
                opcodeSize = 3;
                opcodeValid = true;
                destAddr = (uint8_t*) getRegisterValue(u, (pc[2] >> 3) & 0x07);
                destAddr += getRegisterValue(u, pc[2] & 0x07);
                value = (uint8_t) getRegisterValue(u, (pc[1] & 0x38) >> 3);
            } else if ( (pc[1] & 0xC0) == 0x40) {    // indirect with 8 bit displacement
                opcodeSize = 3;
                opcodeValid = true;
                destAddr = (uint8_t*) getRegisterValue(u, pc[1] & 0x07);
                destAddr += pc[2];
                value = (uint8_t) getRegisterValue(u, (pc[1] & 0x38) >> 3);
            }

       default : 
            break;
   }

   if (!opcodeValid) {
        printf("UNKNOWN OPCODE: %02X %02X %02X %02X %02X %02X\n", pc[0], pc[1], pc[2], pc[3], pc[4], pc[5]);
        exit(1);
   }

   // Unprotect the memory area
   DWORD old;
   VirtualProtect(theMemory, 4096, PAGE_READWRITE, &old);

   // call the observer - observer might decide to write 
   // the new value, any other value or no new value at all
   theObserver(destAddr, value);

   DWORD old2;
   VirtualProtect(theMemory, 4096, old, &old2);

   // skip opcode which caused the exception and continue
   u->Rip = u->Rip + opcodeSize;
   return EXCEPTION_CONTINUE_EXECUTION;
}


uint8_t *omAllocate( omObserver_t observer) {
    theObserver = observer;

    // get the page size
    SYSTEM_INFO system_info;
    GetSystemInfo (&system_info);
    pageSize = system_info.dwPageSize;

    SetUnhandledExceptionFilter(handler);

    theMemory = (uint8_t*) VirtualAlloc(NULL, pageSize,
                            MEM_RESERVE | MEM_COMMIT,
                            PAGE_READONLY);
                         
    return theMemory;
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