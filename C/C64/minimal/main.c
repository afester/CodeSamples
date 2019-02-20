/**
 * This is a CL65 sample program which makes use of the runtime library provided with the compiler.
 */

#include <stdint.h>
#include <conio.h>


void updateStatus(uint8_t c) {
    uint8_t xpos = wherex();
    uint8_t ypos = wherey();

    gotoxy(0, 24);
    textcolor(1);
    revers(1);
    cprintf("Ln: %-2d Col: %-2d  %3d   INS               ", ypos, xpos, c);

    textcolor(7);
    revers(0);
    gotoxy(xpos, ypos);
}

static const char* menu[] = {"File", "Edit", "Search", "Help"};
static const uint8_t menuSize = sizeof(menu) / sizeof(char*);

static const char* fileMenu[] = {"New        ", 
                                 "Open ...   ", 
                                 "Save       ", 
                                 "Save as ..."};
static const uint8_t fileMenuSize = sizeof(fileMenu) / sizeof(char*);

void renderMenu(uint8_t current) {
    uint8_t idx = 0;

    textcolor(1);
    revers(1);
    gotoxy(0, 0);
    for (idx = 0;  idx < menuSize;  idx++) {
        if (idx == current) {
            textcolor(3);
            cputs(menu[idx]);
            
            /*****************/
            cputsxy(0, 1, "\xf0\xc0\xc0\xc0\xc0\xee");
            gotoxy(0, 2);
            cprintf("\xdd%s\xdd", fileMenu[0]);
            gotoxy(0, 3);
            cprintf("\xdd%s\xdd", fileMenu[1]);
            gotoxy(0, 4);
            cprintf("\xdd%s\xdd", fileMenu[2]);
            gotoxy(0, 5);
            cprintf("\xdd%s\xdd", fileMenu[3]);
            cputsxy(0, 6, "\xed\xc0\xc0\xc0\xc0\xfd");
            /*****************/
            
        } else {
            cputs(menu[idx]);
        }
        textcolor(1);
        cputs("  ");
    }
    for (idx = wherex();  idx < 40;  idx++) {
        cputc(' ');
    }
}

void processMenu() {
    uint8_t c = 0;
    uint8_t current = 0;

    cursor(0);
    while(1) {
        renderMenu(current);

        c = cgetc();
        if (c == 157) {
            // Left
            if (current == 0) {
                current = menuSize - 1;
            } else {
                current--;
            }
        } else if (c == 29) {
            // Right
            current++;
            if (current >= menuSize) {
                current = 0;
            }
        } else if (c == 3) {
            break;
        }
    }

    renderMenu(100);
    cursor(1);
    revers(0);
}


int main() {
    clrscr();
    bordercolor(0);
    bgcolor(6);

    renderMenu(100);

    textcolor(7);
    gotoxy(0, 1);
    updateStatus(' ');
    cursor(1);
    while(1) {
        char c;
/*
        uint8_t row;
        uint8_t col;
        for (row = 0;  row < 16;  row++) {
            gotoxy(0, row + 3);
            cprintf("%3d ", row * 16);
            for (col = 0;  col < 16;  col++) {
                cputc(row * 16 + col);
                cputc(' ');
            }
        }
*/
        c = cgetc();

        if (c == 20) {
            // backspace
        } else if (c == 13) {
            // Return
        } else if (c == 157) {
            // Left
            uint8_t xpos = wherex() - 1;
            gotox(xpos);
        } else if (c == 29) {
            // Right
            uint8_t xpos = wherex() + 1;
            gotox(xpos);
        } else if (c == 17) {
            // Down
            uint8_t ypos = wherey() + 1;
            gotoy(ypos);
        } else if (c == 145) {
            // Up
            uint8_t ypos = wherey() - 1;
            gotoy(ypos);
        } else if (c == 19) {
            // Pos1
        } else if (c == 3) {
            // ESC -> Menu
            processMenu();
        } else {
            cputc(c);
        }        

        
        updateStatus(c);
    }
}
