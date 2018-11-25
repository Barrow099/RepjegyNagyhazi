//
// Created by barrow099 on 10/9/18.
//

#include <stdio.h>
#include "menu.h"
#include "c-econio/econio.h"
#include "string-utils.h"
#include <string.h>
#include <stdlib.h>
#include <math.h>

void make_box(int width, int height, EconioColor color);
void make_box_offset(int startx, int starty, int width, int height, EconioColor color, int shadow);
void make_window(int width, int height, int term_width, int term_height);
void print_line_center(string text, int max_len, int x, int y, EconioColor bg, EconioColor fg);\
void print_sep(int length, int x, int y);
int options_wait(string *options, int num, int x, int y, int len);
void input_wait(int x, int y, int w, string *options, int optscount, string *return_values);
void message_wait();

int options_list_wait(OptionNode *pNode, int x, int y, int len) ;

string intput_wait(int i, int i1, size_t i2);

int menu_options(string question, string *options, int num) {
    econio_clrscr();
    econio_rawmode();

    int height = 2 + 1 + num + 1;

    make_box(80, 25, BLUE);
    make_window(40, height, 80, 24);

    //Window start pos
    int startx = (80 / 2) - (40 / 2);
    int starty = (25 / 2) - (height / 2);

    print_line_center(question, 38, startx + 1, starty + 1, LIGHTGRAY, BLACK);
    print_sep(40, startx, starty + 2);

    int c = options_wait(options, num, startx + 1, starty + 3, 32);

    //Restore terminal


    econio_normalmode();
    econio_textcolor(WHITE);
    econio_textbackground(BLACK);
    econio_clrscr();
    econio_gotoxy(0, 0);
    return c;
}
string menu_input(string question) {
    econio_clrscr();
    econio_rawmode();

    int height = 9;
    int width = (int) (4 + 2 + strlen(question));
    width = width > 20 ? width : 20;

    make_box(80, 25, BLUE);
    make_window(width, height, 80, 24);

    //Window start pos
    int startx = (80 / 2) - (width / 2);
    int starty = (25 / 2) - (height / 2);

    print_line_center(question, width - 2, startx + 1, starty + 2, LIGHTGRAY, BLACK);
    print_line_center("<OK>", width - 2, startx + 1, starty + 6, RED, WHITE);

    string s = intput_wait(startx + 3, starty + 4, width - 5);

    //Restore terminal

    econio_normalmode();
    econio_textcolor(WHITE);
    econio_textbackground(BLACK);
    econio_clrscr();
    econio_gotoxy(0, 0);
    return s;
}

string intput_wait(int x, int y, size_t len) {
    char* str = malloc(sizeof(char) * 128);
    econio_gotoxy(x,y);
    econio_textbackground(LIGHTBLUE);
    econio_textcolor(WHITE);
    for(int i = 0; i < len; i++) {
        printf("_");
    }
    econio_gotoxy(x,y);
    econio_normalmode();
    scanf("%[^\n]",str);
    econio_rawmode();
    return str;
}

void menu_message(string message) {
    econio_clrscr();
    econio_rawmode();

    int height = 7;
    int width = (int) (4 + 2 + strlen(message));

    make_box(80, 25, BLUE);
    make_window(width, height, 80, 24);

    //Window start pos
    int startx = (80 / 2) - (width / 2);
    int starty = (25 / 2) - (height / 2);

    print_line_center(message, width - 2, startx + 1, starty + 2, LIGHTGRAY, BLACK);
    print_line_center("<OK>", width - 2, startx + 1, starty + 4, RED, BLACK);

    message_wait();

    //Restore terminal


    econio_normalmode();
    econio_textcolor(WHITE);
    econio_textbackground(BLACK);
    econio_clrscr();
    econio_gotoxy(0, 0);
}
int menu_options_list(string question, OptionNode *listHead) {

    int max = 0;
    OptionNode* p = listHead;
    while(p) {
        if(strlen(p->value) + 7 > max)
            max = strlen(p->value) + 7;
        p = p->nextNode;
    }
    max = strlen(question) > max ? strlen(question) : max;
    int width = max + 10;
    int height = 2 + 1 + 6 + 3 + 1;
    int startx = (80 / 2) - (width / 2);
    int starty = (25 / 2) - (height / 2);
    econio_clrscr();
    econio_rawmode();
    make_box(80, 25, BLUE);
    make_window(width, height, 80, 24);
    print_line_center(question, width - 7, startx + 1, starty + 1, LIGHTGRAY, BLACK);
    print_sep(width, startx, starty + 2);
    econio_flush();
    return options_list_wait(listHead, startx + 1, starty + 3, width - 10);
}
void * menu_input_list(string question, bool (*pfV)(string *, string), void *(*pvC)(string *), string *options, int num) {
    int height = 2 + 1 + num + 3 + 1;
    //Window start pos
    int startx = (80 / 2) - (60 / 2);
    int starty = (25 / 2) - (height / 2);


    string *res = allocate_string_array(num, 64);

    bool bad = true;
    while (bad) {
        econio_clrscr();
        econio_rawmode();
        make_box(80, 25, BLUE);
        make_window(60, height, 80, 24);
        print_line_center(question, 58, startx + 1, starty + 1, LIGHTGRAY, BLACK);
        print_sep(60, startx, starty + 2);
        input_wait(startx + 1, starty + 3, 52, options, num, res);
        char msg[64];
        bad = !pfV(res, msg);
        if (bad)
            menu_message(msg);
    }
    void *data = pvC(res);
    free_string_array(res, num);
    econio_normalmode();
    econio_textcolor(WHITE);
    econio_textbackground(BLACK);
    econio_clrscr();
    econio_gotoxy(0, 0);
    return data;
}



//Private helpers
void make_box(int width, int height, EconioColor color) {
    make_box_offset(0, 0, width, height, color, 0);
}

void make_box_offset(int startx, int starty, int width, int height, EconioColor color, int shadow) {
    econio_textbackground(color);
    for (int y = starty; y < height + starty; y++) {
        for (int x = startx; x < width + startx; x++) {
            econio_gotoxy(x, y);
            putchar(' ');
        }
    }
    if (shadow != 0) {
        if (shadow == 1) {
            econio_textbackground(DARKGRAY);
        } else {
            econio_textbackground(BLACK);
        }
        int vert = starty + height;
        for (int x = startx; x < startx + width; x++) {
            econio_gotoxy(x, vert);
            putchar(' ');
        }


        int hor = startx + width;
        for (int y = starty; y < starty + height + 1; y++) {
            econio_gotoxy(hor, y);
            putchar(' ');
            putchar(' ');
        }
    }
    econio_textbackground(BLACK);
}

void make_window(int width, int height, int term_width, int term_height) {
    int startx = (term_width / 2) - (width / 2);
    int starty = (term_height / 2) - (height / 2);
    make_box_offset(startx, starty, width, height, LIGHTGRAY, 2);

    econio_textbackground(LIGHTGRAY);
    econio_textcolor(BLACK);

    //Print border
    econio_gotoxy(startx, starty);
    printf("╔");
    econio_gotoxy(startx, starty + height - 1);
    printf("╚");
    econio_gotoxy(startx + width - 1, starty);
    printf("╗");
    econio_gotoxy(startx + width - 1, starty + height - 1);
    printf("╝");

    econio_gotoxy(startx + 1, starty);
    for (int x = 0; x < width - 2; x++) {
        printf("═");
    }
    econio_gotoxy(startx + 1, starty + height - 1);
    for (int x = 0; x < width - 2; x++) {
        printf("═");
    }
    for (int y = starty + 1; y < (starty + height - 1); y++) {
        econio_gotoxy(startx, y);
        printf("║");
        econio_gotoxy(startx + width - 1, y);
        printf("║");
    }
    //Border done
}

void print_line_center(string text, int max_len, int x, int y, EconioColor bg, EconioColor fg) {
    int len = (int) strlen(text);
    if (len > max_len)
        fprintf(stderr, "LINE LONGER THAN MAX LENGTH");

    int spaces = (max_len / 2) - (len / 2) + x;
    econio_textcolor(fg);
    econio_textbackground(bg);
    econio_gotoxy(spaces, y);
    printf("%s", text);
}

void print_sep(int length, int x, int y) {
    econio_gotoxy(x, y);
    printf("╟");
    for (int i = 0; i < length - 2; i++) {
        printf("─");
    }
    printf("╢");
}


int options_list_wait(OptionNode *pNode, int x, int y, int len) {
    int startx = x + 1;
    int choice = -1;
    int current = 0;
    int current_index = 0;
    int viewStart = 0;

    int item_count = 1;
    OptionNode* head = pNode;
    while(head->nextNode) {
        item_count++;
        head = head->nextNode;
    }

    while(choice == -1) {
        OptionNode* node = pNode;
        int index = 0;
        //Skip to view start
        while(index < viewStart && index < item_count) {
            node = node->nextNode;
            index++;
        }
        while (index - viewStart < 7 && index < item_count) {
            string text = node->value;
            char sel[4];
            if(index == current_index) {
                strcpy(sel, "[*]");
                econio_textbackground(WHITE);
                econio_textcolor(BLUE);
            }else {
                sprintf(sel, "[%d]", index);
                econio_textcolor(WHITE);
                econio_textbackground(BLUE);

            }
            econio_gotoxy(startx,y + (index - viewStart));
            char msg[192];
            sprintf(msg,"%s %s",sel,text);
            printf("%s",msg);
            for (int i = 0; i < len - (int)strlen(msg) && len - (int)strlen(msg) > 0; i++) {
                printf(" ");
            }
            index++;
            econio_flush();
            node = node->nextNode;
        }

        //Draw scrollbar
        econio_textcolor(DARKGRAY);
        econio_textbackground(DARKGRAY);
        for(int i = 0; i < 7;i++) {
            econio_gotoxy(startx + len - 1, y + i);
            printf(" ");
        }
        econio_textbackground(BLACK);
        econio_textcolor(BLACK);
        int progress = floor((double )current_index / ((double )item_count) * 7.0);
        int py = y + progress;
        econio_gotoxy(startx + len - 1, py);
        printf(" ");
        econio_flush();


        int ch = econio_getch();
        switch (ch) {
            case ENTER: choice = current_index;
                break;
            case UP:
                current_index--;
                if(current_index < 0)
                    current_index = 0;
                if(current_index < viewStart)
                    viewStart--;
                break;
            case DOWN:
                current_index++;
                if(current_index > item_count -1)
                    current_index = item_count - 1;
                if(current_index > viewStart + 6)
                    viewStart++;
                break;
            case ESCAPE:
                return -1;
            default: break;
        }
    }
}

int options_wait(string *options, int num, int x, int y, int len) {
    int startx = x + 1;
    int choice = -1;
    int current = 0;
    int max = len;
    while (choice == -1) {
        for (int c = 0; c < num; c++) {
            string text = options[c];
            char sel[4];
            if (c == current) {
                strcpy(sel, "[*]");
                econio_textbackground(WHITE);
                econio_textcolor(BLUE);
            } else {
                sprintf(sel, "[%d]", c + 1);
                econio_textbackground(BLUE);
                econio_textcolor(WHITE);
            }
            econio_gotoxy(startx, y + c);
            printf("%s %s", sel, text);
            int rem = max - (int) strlen(text);
            for (int i = 0; i < rem; i++) {
                printf(" ");
            }

        }
        econio_gotoxy(startx + 1, y + current);
        int ch = econio_getch();
        switch (ch) {
            case DOWN:
                current = current + 1 > num - 1 ? num - 1 : current + 1;
                break;
            case UP:
                current = current - 1 < 0 ? 0 : current - 1;
                break;
            case ENTER:
                choice = current;
            default:
                break;
        }
    }
    return choice;

}

void input_wait(int x, int y, int w, string *options, int optscount, string *return_values) {
    bool isokd = false;
    int selected = 0;
    string *opts = options;
    int opt_c = optscount;
    int max = stra_max(opts, opt_c);


    string *szovegek = (string *) calloc((size_t) opt_c, sizeof(string));

    for (int i = 0; i < opt_c; i++) {
        szovegek[i] = calloc(64, sizeof(char));
    }
    for (int i = 0; i < opt_c; i++) {
        strcpy(szovegek[i], return_values[i]);
    }
    bool autotype = true;
    while (!isokd) {
        //Draw menu
        econio_textbackground(LIGHTGRAY);
        econio_textcolor(BLACK);
        for (int i = 0; i < opt_c; i++) {
            string t = opts[i];
            econio_gotoxy(x, y + i);
            printf("%s", t);
        }


        for (int i = 0; i < opt_c; i++) {
            if (i == selected) {
                econio_textbackground(WHITE);
                econio_textcolor(BLUE);
            } else {
                econio_textcolor(BLACK);
                econio_textbackground(BLUE);
            }
            econio_gotoxy(x + max + 1, y + i);
            string text = szovegek[i];
            int len = (int) strlen(text);

            printf("%s", text);

            for (int j = 0; j < w - max - 2 - len; j++) {
                printf("_");
            }
        }

        print_line_center("<OK>", 58, x, y + opt_c + 1, selected == opt_c ? RED : LIGHTGRAY, BLACK);

        econio_gotoxy(x + max + 1, y + selected);

        bool wait = true;

        while (wait) {
            int ch;
            if(autotype)
                ch = ENTER;
            else
                ch = econio_getch();
            switch (ch) {
                case UP:
                    selected--;
                    if (selected < 0) selected = 0;
                    wait = false;
                    break;
                case TAB:
                case DOWN:
                    selected++;
                    if (selected > opt_c) selected = opt_c;
                    wait = false;
                    break;
                case ENTER:

                    if (selected == opt_c)
                        isokd = true;
                    else {
                        econio_textcolor(RED);
                        econio_textbackground(WHITE);
                        econio_gotoxy(x + max + 1, y + selected);
                        string text = szovegek[selected];
                        int len = (int) strlen(text);

                        printf("%s", text);

                        for (int j = 0; j < w - max - 2 - len; j++) {
                            printf("_");
                        }
                        econio_gotoxy(x + max + 1, y + selected);
                        econio_flush();

                        econio_normalmode();
                        scanf("%[^\n]", szovegek[selected]);
                        econio_rawmode();
                        selected++;
                        if(selected < opt_c)
                            autotype = true;
                        else
                            autotype = false;
                        if (selected > opt_c) selected = opt_c;
                        wait = false;
                    }
                    wait = false;
                    break;
                default:
                    break;
            }
        }
    }
    for (int i = 0; i < opt_c; i++) {
        strtrm(szovegek[i]);
        strcpy(return_values[i], szovegek[i]);
        free(szovegek[i]);
    }
    free(szovegek);

}

void message_wait() {
    bool ok = false;
    while (!ok) {
        int ch = econio_getch();
        if (ch == ENTER)
            ok = true;
    }
}