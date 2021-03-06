#include "econio.h"


#if defined(_WIN32) || defined(_WIN64)


#include <windows.h>
#include <stdio.h>
#include <conio.h>
#include <assert.h>

static WORD bgcolor = BLACK;
static WORD fgcolor = LIGHTGRAY;
static bool rawmode = false;

static WORD colormap[] = {
    [BLACK]           = 0,
    [BLUE]            = FOREGROUND_BLUE,
    [GREEN]           = FOREGROUND_GREEN,
    [CYAN]            = FOREGROUND_GREEN | FOREGROUND_BLUE,
    [RED]             = FOREGROUND_RED,
    [MAGENTA]         = FOREGROUND_RED   | FOREGROUND_BLUE,
    [BROWN]           = FOREGROUND_RED   | FOREGROUND_GREEN,
    [LIGHTGRAY]       = FOREGROUND_RED   | FOREGROUND_GREEN | FOREGROUND_BLUE,
    [DARKGRAY]        = FOREGROUND_INTENSITY,
    [LIGHTBLUE]       = FOREGROUND_BLUE  | FOREGROUND_INTENSITY,
    [LIGHTGREEN]      = FOREGROUND_GREEN | FOREGROUND_INTENSITY,
    [LIGHTCYAN]       = FOREGROUND_GREEN | FOREGROUND_BLUE  | FOREGROUND_INTENSITY,
    [LIGHTRED]        = FOREGROUND_RED   | FOREGROUND_INTENSITY,
    [LIGHTMAGENTA]    = FOREGROUND_RED   | FOREGROUND_BLUE  | FOREGROUND_INTENSITY,
    [YELLOW]          = FOREGROUND_RED   | FOREGROUND_GREEN | FOREGROUND_INTENSITY,
    [WHITE]           = FOREGROUND_RED   | FOREGROUND_GREEN | FOREGROUND_BLUE | FOREGROUND_INTENSITY,
};

#define STDOUT GetStdHandle(STD_OUTPUT_HANDLE)

void econio_gotoxy(int x, int y) {
    COORD dwCursorPosition;
    dwCursorPosition.X = (SHORT) x;
    dwCursorPosition.Y = (SHORT) y;
    SetConsoleCursorPosition(STDOUT, dwCursorPosition);
}


void econio_textbackground(int newcolor) {
    if (newcolor == RESET)
        newcolor = BLACK;
    assert(newcolor >= 0 && newcolor < 16);
    bgcolor = (WORD) (colormap[newcolor] << 4);
    SetConsoleTextAttribute(STDOUT, fgcolor | bgcolor);
}


void econio_textcolor(int newcolor) {
    if (newcolor == RESET)
        newcolor = LIGHTGRAY;
    assert(newcolor >= 0 && newcolor < 16);
    fgcolor = (WORD) colormap[newcolor];
    SetConsoleTextAttribute(STDOUT, fgcolor | bgcolor);
}


void econio_clrscr(void) {
    HANDLE hstdout = STDOUT;
    CONSOLE_SCREEN_BUFFER_INFO csbi;
    if (GetConsoleScreenBufferInfo(hstdout, &csbi)) {
        COORD coordScreen = { 0, 0 };
        DWORD cCharsWritten;
        DWORD dwConSize = csbi.dwSize.X * csbi.dwSize.Y;
        FillConsoleOutputCharacter (hstdout, ' ', dwConSize, coordScreen, &cCharsWritten);
        FillConsoleOutputAttribute (hstdout, csbi.wAttributes, dwConSize, coordScreen, &cCharsWritten);
        SetConsoleCursorPosition   (hstdout, coordScreen);
    }
}


void econio_flush() {
    fflush(stdout);
}


void econio_set_title(char const *title) {
    SetConsoleTitle(title);
}


void econio_rawmode() {
    rawmode = true;
}


void econio_normalmode() {
    rawmode = false;
}


bool econio_kbhit() {
    assert(rawmode);
    return _kbhit() != 0;
}


int econio_getch() {
    static struct {
        int code;
        EconioKey key;
    } windowskeycodes[] = {
        {72, UP},
        {80, DOWN},
        {75, LEFT},
        {77, RIGHT},
        {73, PAGEUP},
        {81, PAGEDOWN},
        {71, HOME},
        {79, END},
        {82, INSERT},
        {83, DELETE},
        {141, CTRLUP},
        {145, CTRLDOWN},
        {115, CTRLLEFT},
        {116, CTRLRIGHT},
        {134, CTRLPAGEUP},
        {118, CTRLPAGEDOWN},
        {119, CTRLHOME},
        {117, CTRLEND},
        {146, CTRLINSERT},
        {147, CTRLDELETE},
        {-1, UNKNOWNKEY},
    };

    assert(rawmode);
    int code = _getch();
    if (code == 0x7F)
        return BACKSPACE;
    if (code == 0x0D)
        return ENTER;
    if (code != 0xE0)
        return code;
    code = _getch();

    for (int i = 0; windowskeycodes[i].code != -1; ++i)
        if (code == windowskeycodes[i].code)
            return windowskeycodes[i].key;
    return UNKNOWNKEY;
}


void econio_sleep(double sec) {
    Sleep(sec * 1000);
}


#else // defined _WIN32


#include <assert.h>
#include <stdio.h>
#include <termios.h>
#include <unistd.h>
#include <stdbool.h>
#include <sys/time.h>
#include <sys/types.h>
#include <ctype.h>
#include <string.h>
#include <time.h>


void econio_textcolor(int color) {
    static int colormap[] = { 30, 34, 32, 36, 31, 35, 33, 37, 90, 94, 92, 96, 91, 95, 93, 97, 39 };

    assert(color >= 0 && color <= 16);
    printf("\033[%dm", colormap[color]);
}


void econio_textbackground(int color) {
    static int colormap[] = { 40, 44, 42, 46, 41, 45, 43, 47, 100, 104, 102, 106, 101, 105, 103, 107, 49 };

    assert(color >= 0 && color <= 16);
    printf("\033[%dm", colormap[color]);
}


void econio_gotoxy(int x, int y) {
    printf("\033[%d;%dH", y+1, x+1);
}


void econio_clrscr() {
    printf("\033[2J");
    econio_gotoxy(0, 0);
}


void econio_flush() {
    fflush(stdout);
}


void econio_set_title(char const *title) {
    printf("\033]2;%s\007", title);
}


void econio_rawmode() {
    int fd = fileno(stdin);
    struct termios attr;
    tcgetattr(fd, &attr);
    attr.c_lflag &= ~ICANON & ~ECHO;
    tcsetattr(fd, TCSADRAIN, &attr);
    setvbuf(stdin, NULL, _IONBF, BUFSIZ);
}


void econio_normalmode() {
    int fd = fileno(stdin);
    struct termios attr;
    tcgetattr(fd, &attr);
    attr.c_lflag |= ICANON | ECHO;
    tcsetattr(fd, TCSADRAIN, &attr);
    setvbuf(stdin, NULL, _IOLBF, BUFSIZ);
}


static bool inrawmode() {
    int fd = fileno(stdin);
    struct termios attr;
    tcgetattr(fd, &attr);
    return (attr.c_lflag & (ICANON | ECHO)) == 0;
}


bool econio_kbhit() {
    assert(inrawmode());
    econio_flush();

    fd_set rfds;
    FD_ZERO(&rfds);
    FD_SET(fileno(stdin), &rfds);
    struct timeval tv;
    tv.tv_sec = 0;
    tv.tv_usec = 0;
    int retval = select(1, &rfds, NULL, NULL, &tv);
    if (retval == -1) {
        perror("select()");
        return false;
    }
    return retval != 0;
}


static int rawgetch() {
    return fgetc(stdin);
}


int econio_getch() {
    static struct {
        char const *escape;
        EconioKey key;
    } unixkeycodes[] = {
        {"\033OP", F1},
        {"\033OQ", F2},
        {"\033OR", F3},
        {"\033OS", F4},
        {"\033[15~", F5},
        {"\033[17~", F6},
        {"\033[18~", F7},
        {"\033[19~", F8},
        {"\033[20~", F9},
        {"\033[21~", F10},
        {"\033[23~", F11},
        {"\033[24~", F12},
        {"\033[A", UP},
        {"\033[B", DOWN},
        {"\033[D", LEFT},
        {"\033[C", RIGHT},
        {"\033[5~", PAGEUP},
        {"\033[6~", PAGEDOWN},
        {"\033[H", HOME},
        {"\033[F", END},
        {"\033[2~", INSERT},
        {"\033[3~", DELETE},
        {"\033[1;5A", CTRLUP},
        {"\033[1;5B", CTRLDOWN},
        {"\033[1;5D", CTRLLEFT},
        {"\033[1;5C", CTRLRIGHT},
        {"\033[5;5~", CTRLPAGEUP},
        {"\033[6;5~", CTRLPAGEDOWN},
        {"\033[1;5H", CTRLHOME},
        {"\033[1;5F", CTRLEND},
        {"\033[3;5~", CTRLDELETE},
        {NULL, 0},
    };

    assert(inrawmode());
    econio_flush();

    char s[10];
    int i = 0;
    s[i++] = rawgetch();
    if (s[i-1] == 0x7F)
        return BACKSPACE;
    if (s[i-1] != 0x1B || !econio_kbhit())     // only an escape sequence if other chars can be read
        return s[i-1];

    // read following chars and concatenate to see the escape sequence
    s[i++] = rawgetch();
    if (s[i-1] == 'O') {    // VT100 f1-f4: OP-OS
        s[i++] = rawgetch();
    } else if (s[i-1] == '[') { // other: always delimited by uppercase char or tilde
        s[i++] = rawgetch();
        while (!(isupper(s[i-1]) || s[i-1] == '~') && (i < sizeof(s)/sizeof(s[0])-1))
            s[i++] = rawgetch();
    } else {    // unknown sequence, return verbatim
        ungetc(s[i-1], stdin);
        return s[0];
    }
    s[i] = '\0';
    for (i = 0; unixkeycodes[i].escape != NULL; ++i)
        if (strcmp(unixkeycodes[i].escape, s) == 0)
            return unixkeycodes[i].key;
    return UNKNOWNKEY;
}


void econio_sleep(double sec) {
    struct timespec req, rem;
    req.tv_sec = (time_t) sec;
    req.tv_nsec = (long) ((sec - req.tv_sec) * 1e9);
    nanosleep(&req, &rem);
}


#endif // defined _WIN32
