/** @file
 * Implementacja modułu udostępniającego operacje na numerach telefonów przez interfejs tekstowy
 *
 * @author Magdalena Augustyńska <ma370723@mimuw.edu.pl>
 * @date 01.06.2018
 */

#include <stdlib.h>
#include <stdio.h>
#include <ctype.h>

#include "phone_forward.h"
#include "base.h"
#include "utils.h"

void freeMemory(struct WorkingStruct *ws) {
    char c = (char) getchar();
    while (c != EOF)c = (char) getchar();
    delNode(ws->bases);
    delNum(ws->argument1);
    ws->argument1 = NULL;
    delNum(ws->argument2);
    ws->argument2 = NULL;
    free(ws);
}

void end(struct WorkingStruct *ws) {
    freeMemory(ws);
    exit(1);
}

void syntaxError(int signNumber, struct WorkingStruct *ws) {
    fprintf(stderr, "ERROR %d\n", signNumber);
    end(ws);
}

void executionError(int signNumber, char *op, struct WorkingStruct *ws) {
    fprintf(stderr, "ERROR %s %d\n", op, signNumber);
    end(ws);
}

void eofError(struct WorkingStruct *ws) {
    fprintf(stderr, "ERROR EOF\n");
    end(ws);
}

void printPhfwdReverse(int signNumber, struct WorkingStruct *ws) {
    if (ws->pf == NULL)executionError(signNumber, "?", ws);
    struct PhoneNumbers const *pnum = phfwdReverse(ws->pf, ws->argument1->array);
    delNum(ws->argument1);
    ws->argument1 = NULL;
    if (pnum == NULL)executionError(signNumber, "?", ws);
    if (pnum->numberOfPhoneNumbers == 0) {
        executionError(signNumber, "?", ws);
    }
    size_t idx = 0;
    char const *num;
    while ((num = phnumGet(pnum, idx++)) != NULL)
        printf("%s\n", num);
    phnumDelete(pnum);
}

void printPhfwdGet(int signNumber, struct WorkingStruct *ws) {
    if (ws->pf == NULL)executionError(signNumber, "?", ws);
    struct PhoneNumbers const *pnum = phfwdGet(ws->pf, ws->argument1->array);
    delNum(ws->argument1);
    ws->argument1 = NULL;
    if (pnum == NULL)executionError(signNumber, "?", ws);
    if (pnum->numberOfPhoneNumbers == 0) {
        executionError(signNumber, "?", ws);
    }
    printf("%s\n", phnumGet(pnum, 0));
    phnumDelete(pnum);
}

void add(int signNumber, struct WorkingStruct *ws) {
    if (ws->pf == NULL) {
        executionError(signNumber, ">", ws);
    }
    bool ok = phfwdAdd(ws->pf, ws->argument1->array, ws->argument2->array);
    if (ok == false)executionError(signNumber, ">", ws);
    delNum(ws->argument1);
    ws->argument1 = NULL;
    delNum(ws->argument2);
    ws->argument2 = NULL;
}

bool correctId(char *c) {
    return strcmp(c, "NEW") != 0 && strcmp(c, "DEL") != 0;
}

int parse(struct WorkingStruct *ws) {
    int i = 1;
    int state = 1;
    int last = 1;
    int op = 1;
    int inputStart = 1;
    char c = (char) getchar();
    while (c != EOF) {
        switch (state) {
            case 1:
                if (isspace(c));
                else if (isDigit(c)) {
                    inputStart = i;
                    state = 2;
                    ws->argument1 = newNum();
                    addToNum(ws->argument1, c);
                } else if (c == 'N') {
                    op = i;
                    inputStart = i;
                    state = 14;
                } else if (c == '?') {
                    op = i;
                    inputStart = i;
                    state = 12;
                } else if (c == 'D') {
                    op = i;
                    inputStart = i;
                    state = 7;
                } else if (c == '$') {
                    last = 1;
                    state = 18;
                    inputStart = i;
                } else if (c == '@'){
                    op = i;
                    inputStart = i;
                    state = 23;
                }
                else syntaxError(i, ws);
                i++;
                break;
            case 2:
                if (isspace(c))state = 3;
                else if (isDigit(c))addToNum(ws->argument1, c);
                else if (c == '?') {
                    op = i;
                    state = 6;
                } else if (c == '>') {
                    op = i;
                    state = 4;
                } else if (c == '$') {
                    last = 2;
                    state = 18;
                } else syntaxError(i, ws);
                i++;
                break;
            case 3:
                if (c == '>') {
                    op = i;
                    state = 4;
                } else if (isspace(c));
                else if (c == '?') {
                    op = i;
                    state = 6;
                } else if (c == '$') {
                    last = 3;
                    state = 18;
                } else syntaxError(i, ws);
                i++;
                break;
            case 4:
                if (isspace(c));
                else if (isDigit(c)) {
                    inputStart = i;
                    ws->argument2 = newNum();
                    addToNum(ws->argument2, c);
                    state = 5;
                } else if (c == '$') {
                    last = 4;
                    state = 18;
                } else syntaxError(i, ws);
                i++;
                break;
            case 5:
                if (isDigit(c)) {
                    i++;
                    addToNum(ws->argument2, c);
                } else if (c == '$') {
                    last = 5;
                    state = 18;
                    i++;
                } else {
                    add(op, ws);
                    ungetc(c, stdin);
                    state = 1;
                }
                break;
            case 6:
                state = 1;
                ungetc(c, stdin);
                printPhfwdGet(op, ws);
                break;
            case 7:
                if (c == 'E')state = 8;
                else if (c == '$') {
                    last = 7;
                    state = 18;
                } else syntaxError(inputStart, ws);
                i++;
                break;
            case 8:
                if (c == 'L')state = 21;
                else if (c == '$') {
                    last = 8;
                    state = 18;
                } else syntaxError(inputStart, ws);
                i++;
                break;
            case 9:
                if (isspace(c));
                else if (isDigit(c)) {
                    inputStart = i;
                    ws->argument1 = newNum();
                    addToNum(ws->argument1, c);
                    state = 10;
                } else if (isalpha(c)) {
                    inputStart = i;
                    ws->argument1 = newNum();
                    addToNum(ws->argument1, c);
                    state = 11;
                } else if (c == '$') {
                    last = 9;
                    state = 18;
                } else syntaxError(i, ws);
                i++;
                break;
            case 10:
                if (isDigit(c)) {
                    addToNum(ws->argument1, c);
                    i++;
                } else if (c == '$') {
                    last = 10;
                    state = 18;
                    i++;
                } else {
                    state = 1;
                    ungetc(c, stdin);
                    if (ws->pf == NULL)executionError(op, "DEL", ws);
                    phfwdRemove(ws->pf, ws->argument1->array);
                    delNum(ws->argument1);
                    ws->argument1 = NULL;
                }
                break;
            case 11:
                if (isalpha(c)||isDigit(c)) {
                    addToNum(ws->argument1, c);
                    i++;
                } else if (c == '$') {
                    last = 11;
                    state = 18;
                    i++;
                } else {
                    bool correct = true;
                    if (!correctId(ws->argument1->array)) {
                        syntaxError(inputStart, ws);
                    }
                    ws->bases = delBase(ws->bases, ws->argument1->array, &(ws->pf), &correct);
                    if (correct == false)executionError(op, "DEL", ws);
                    delNum(ws->argument1);
                    ws->argument1 = NULL;
                    state = 1;
                    ungetc(c, stdin);
                }
                break;
            case 12:
                if (isspace(c));
                else if (isDigit(c)) {
                    inputStart = i;
                    ws->argument1 = newNum();
                    addToNum(ws->argument1, c);
                    state = 13;
                } else if (c == '$') {
                    last = 12;
                    state = 18;
                } else syntaxError(i, ws);
                i++;
                break;
            case 13:
                if (isDigit(c)) {
                    addToNum(ws->argument1, c);
                    i++;
                } else if (c == '$') {
                    last = 13;
                    state = 18;
                    i++;
                } else {
                    printPhfwdReverse(op, ws);
                    state = 1;
                    ungetc(c, stdin);
                }
                break;
            case 14:
                if (c == 'E') {
                    state = 15;
                } else if (c == '$') {
                    last = 14;
                    state = 18;
                } else syntaxError(inputStart, ws);
                i++;
                break;
            case 15:
                if (c == 'W') {
                    state = 22;
                } else if (c == '$') {
                    last = 15;
                    state = 18;
                } else syntaxError(inputStart, ws);
                i++;
                break;
            case 16:
                if (isspace(c));
                else if (isalpha(c)) {
                    inputStart = i;
                    ws->argument1 = newNum();
                    addToNum(ws->argument1, c);
                    state = 17;
                } else if (c == '$') {
                    last = 16;
                    state = 18;
                } else syntaxError(i, ws);
                i++;
                break;
            case 17:
                if (isDigit(c) || isalpha(c)) {
                    addToNum(ws->argument1, c);
                    i++;
                } else if (c == '$') {
                    last = 17;
                    state = 18;
                    i++;
                } else {
                    if (!correctId(ws->argument1->array)) {
                        syntaxError(inputStart, ws);
                    }
                    if ((ws->bases = newBase(ws->bases, ws->argument1->array, &(ws->pf))) == NULL) {
                        executionError(op, "NEW", ws);
                    }
                    delNum(ws->argument1);
                    ws->argument1 = NULL;
                    state = 1;
                    ungetc(c, stdin);
                }
                break;
            case 18:
                if (c == '$')state = 19;
                else syntaxError(i - 1, ws);
                i++;
                break;
            case 19:
                if (c == '$')state = 20;
                i++;
                break;
            case 20:
                if (c == '$') {
                    state = last;
                    ungetc(' ', stdin);
                } else {
                    state = 19;
                    i++;
                }
                break;
            case 21:
                if (isspace(c)) {
                    state = 9;
                } else if (c == '$') {
                    state = 18;
                    last = 9;
                } else if (isalpha(c)||isDigit(c)) syntaxError(inputStart, ws);
                else syntaxError(i, ws);
                i++;
                break;
            case 22:
                if (isspace(c)) {
                    state = 16;
                } else if (c == '$') {
                    last = 22;
                    state = 18;
                } else if (isalpha(c)||isDigit(c)) syntaxError(inputStart, ws);
                else syntaxError(i, ws);
                i++;
                break;
            case 23:
                if (isspace(c));
                else if (isDigit(c)) {
                    inputStart = i;
                    ws->argument1 = newNum();
                    addToNum(ws->argument1, c);
                    state = 24;
                } else if (c == '$') {
                    last = 23;
                    state = 18;
                } else syntaxError(i, ws);
                i++;
                break;
            case 24:
                if (isDigit(c)) {
                    addToNum(ws->argument1, c);
                    i++;
                } else if (c == '$') {
                    last = 24;
                    state = 18;
                    i++;
                } else {
                    size_t nonTrivial = phfwdNonTrivialCount(ws->pf, ws->argument1->array, (size_t)ws->argument1->taken);
                    delNum(ws->argument1);
                    printf("%zu\n", nonTrivial);
                    state = 1;
                    ungetc(c, stdin);
                }
                break;
            default:
                syntaxError(inputStart, ws);
        }
        c = (char) getchar();
    }
    return state;
}
