//
// Created by barrow099 on 10/24/18.
//
#include "string-utils.h"
#include <string.h>
#include <stdlib.h>
#include <ctype.h>

int stra_max(char **strs, int num) {
    int max = 0;
    for (int x = 0; x < num; x++) {
        if (strlen(strs[x]) > max)
            max = (int) strlen(strs[x]);
    }
    return max;
}

string *allocate_string_array(int itemnum, int item_length) {
    string *p = malloc(sizeof(string *) * itemnum);

    for (int i = 0; i < itemnum; i++) {
        p[i] = calloc((size_t) item_length, sizeof(char));
    }
    return p;
}

void free_string_array(string *array, int count) {

    for (int i = 0; i < count; i++) {
        free(array[i]);
    }
    free(array);
}

void strrmv(string str, int index) {
    string new = malloc(strlen(str) * sizeof(char));
    strncpy(new, str, (size_t) index);
    string after = str + index + 1;
    strcat(new, after);
    memset(str, 0, strlen(str));
    memcpy(str, new, strlen(new));
    free(new);
}

void strtrm(string str) {
    string s = malloc(strlen(str) * sizeof(char));
    int pos = 0;
    for (int i = 0; i < strlen(str); i++) {
        if (!isspace(str[i]))
            s[pos++] = str[i];
    }
    memset(str, 0, strlen(str));
    memcpy(str, s, strlen(s));
    free(s);
}

int strcnt(string str, char charToFind) {
    int cnt = 0;
    for (int i = 0; str[i] != 0; i++) {
        if (str[i] == charToFind)
            cnt++;
    }
    return cnt;
}

string strrmc(string str, char charToRemove) {
    int olen = strlen(str);
    int oc = strcnt(str, charToRemove);
    string nstr = malloc(sizeof(char) * (olen - oc + 1));
    int nsp = 0;
    for (int i = 0; i < olen + 1; i++) {
        if (str[i] != charToRemove)
            nstr[nsp++] = str[i];
    }
    return nstr;
}

int streqq(string const str_o, string const ref_o) {
    char str[192];
    char ref[192];
    strcpy(str,str_o);
    strcpy(ref,ref_o);
    strlow(str);
    strlow(ref);
    int eq = 0;
    int all = strlen(ref);
    for(int i = 0, j = 0; str[i] != 0 && ref[j] != 0;i++, j++) {
        int i1 = str[i];
        if(i1 < 0)
            i++;
        else {
            if(str[i] == ref[j])
                eq++;
        }


    }
    double bp = (double) eq / (double) all;
    return (int) (bp*100);

}

void strlow(string str) {
    for(int i = 0; str[i] != 0;i++) {
        if(str[i] >= 'A' && str[i] <= 'Z')
            str[i] = tolower(str[i]);
    }
}
