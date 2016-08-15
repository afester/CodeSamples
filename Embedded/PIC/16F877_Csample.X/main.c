// http://stackoverflow.com/questions/36491959/pic16f877a-uart-embedded-c-code

#include <pic.h>

void pic_init(void) {
    TRISC7=1;
    TRISC6=0;
}

void uart_init(void) {
    TXSTA=0x20;
    RCSTA=0x90;
    SPBRG=15;
}

void tx(unsigned char byte) {
    int i;
    TXREG = byte;
    while(!TXIF);
    for(i=0;i<400;i++);
}

void string_uart(char *q) {
    while(*q) {
        //*(*q++);
        tx(*q);
        q++;
    }
}

unsigned char rx() {
    while(!RCIF);
    return RCREG;
}

void main() {
    pic_init();
    uart_init();
    tx('N');
    rx();
    string_uart("test program");
}
