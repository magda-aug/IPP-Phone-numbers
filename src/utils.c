/** @file
 * Implementacja interfejsu funkcji pomocniczych dla modułu operacji przekierowywania numerów telefonów
 *
 * @author Magdalena Augustyńska <ma370723@mimuw.edu.pl>
 * @date 01.06.2018
 */

#include <stdbool.h>
#include <stddef.h>
#include <stdlib.h>
#include <string.h>

#include "phone_forward.h"
#include "utils.h"

char *strdup(const char *p) {
    char *np = (char *) malloc(strlen(p) + 1);
    return np ? strcpy(np, p) : np;
}

struct PhoneForwardTrieFromTo *phfwdtrieFromToNew(void) {
    struct PhoneForwardTrieFromTo *phoneForwardTrie = malloc(sizeof(struct PhoneForwardTrieFromTo));
    if (phoneForwardTrie == NULL) return NULL;
    for (int i = 0; i < NUMBER_OF_DIGITS; ++i) {
        phoneForwardTrie->children[i] = NULL;
    }
    phoneForwardTrie->number = NULL;
    return phoneForwardTrie;
}

struct PhoneNumbers *phnumNew(void) {
    struct PhoneNumbers *phoneNumbers = malloc(sizeof(struct PhoneNumbers));
    if (phoneNumbers == NULL) return NULL;
    phoneNumbers->numberOfPhoneNumbers = 0;
    phoneNumbers->arraySize = INIT_SIZE;
    phoneNumbers->array = malloc(sizeof(char *) * INIT_SIZE);
    return phoneNumbers;
}

struct PhoneForwardTrieToFrom *phfwdtrieToFromNew(void) {
    struct PhoneForwardTrieToFrom *phoneForwardTrie = malloc(sizeof(struct PhoneForwardTrieToFrom));
    if (phoneForwardTrie == NULL) return NULL;
    for (int i = 0; i < NUMBER_OF_DIGITS; ++i) {
        phoneForwardTrie->children[i] = NULL;
    }
    phoneForwardTrie->phoneNumbers = phnumNew();
    if (phoneForwardTrie->phoneNumbers == NULL) {
        free(phoneForwardTrie);
        return NULL;
    }
    return phoneForwardTrie;
}

int isDigit(char c) {
    if (((c >= '0') && (c <= '9')) || c == ':' || c == ';') return 1;
    return 0;
}

bool onlyDigits(const char *s) {
    if (s == NULL || strcmp(s, "") == 0)return false;
    while (*s) {
        if (isDigit(*s++) == 0) return false;
    }
    return true;
}

void phfwdTrieToFromDelete(struct PhoneForwardTrieToFrom **pft) {
    if ((*pft) == NULL)return;
    for (int i = 0; i < NUMBER_OF_DIGITS; i++) {
        phfwdTrieToFromDelete(&((*pft)->children[i]));
    }
    phnumDelete((*pft)->phoneNumbers);
    (*pft)->phoneNumbers = NULL;
    free(*pft);
    *pft = NULL;
}

void phfwdTrieFromToDelete(struct PhoneForwardTrieFromTo **pft) {
    if (*pft == NULL)return;
    for (int i = 0; i < NUMBER_OF_DIGITS; i++) {
        phfwdTrieFromToDelete(&((*pft)->children[i]));
    }
    free((*pft)->number);
    (*pft)->number = NULL;
    free(*pft);
    *pft = NULL;
}

bool phnumAdd(struct PhoneNumbers *pnum, char const *num) {
    if (pnum->numberOfPhoneNumbers == pnum->arraySize) {
        pnum->arraySize *= MULTIPLIER;
        char **pnum_new_array;
        pnum_new_array = realloc(pnum->array, sizeof(char *) * pnum->arraySize);
        if (pnum_new_array == NULL) return false;
        pnum->array = pnum_new_array;
    }
    if (num != NULL) {
        char *add = strdup(num);
        if (add != NULL) {
            pnum->array[pnum->numberOfPhoneNumbers] = add;
            pnum->numberOfPhoneNumbers++;
            return true;
        } else {
            return false;
        }
    }
    return true;
}

int idxInPhNum(struct PhoneNumbers *pnum, const char *num) {
    for (int i = 0; i < pnum->numberOfPhoneNumbers; i++) {
        if (strcmp(pnum->array[i], num) == 0) {
            return i;
        }
    }
    return -1;
}

void onePhfwdRemove(struct PhoneForwardTrieToFrom *pft, const char *num1, const char *num2) {
    if (num1 == NULL || num2 == NULL)
        return;
    size_t length = strlen(num1);
    char letter;
    struct PhoneForwardTrieToFrom *tmp = pft;
    for (size_t i = 0; i < length; i++) {
        letter = num1[i];
        if (tmp->children[(letter - '0')] == NULL)
            return;
        tmp = tmp->children[(letter - '0')];
    }
    int idx = idxInPhNum(tmp->phoneNumbers, num2);
    if (idx == tmp->phoneNumbers->numberOfPhoneNumbers - 1 && idx != -1) {
        free(tmp->phoneNumbers->array[idx]);
        tmp->phoneNumbers->array[idx] = NULL;
    } else if (idx != -1) {
        free(tmp->phoneNumbers->array[idx]);
        for (int i = idx; i < tmp->phoneNumbers->numberOfPhoneNumbers - 1; i++) {
            tmp->phoneNumbers->array[i] = tmp->phoneNumbers->array[i + 1];
        }
        tmp->phoneNumbers->array[tmp->phoneNumbers->numberOfPhoneNumbers - 1] = NULL;
    }
    tmp->phoneNumbers->numberOfPhoneNumbers--;
}

struct Num *newNum() {
    struct Num *newNum = malloc(sizeof(struct Num));
    if (newNum == NULL)
        return NULL;
    newNum->size = INIT_SIZE;
    newNum->array = malloc(sizeof(char) * newNum->size);
    newNum->array[0] = '\0';
    newNum->taken = 1;
    return newNum;
}

void addToNum(struct Num *n, char add) {
    if (n->size == n->taken) {
        n->size *= MULTIPLIER;
        char *new_array = realloc(n->array, sizeof(char) * n->size);
        if (new_array != NULL)n->array = new_array;
        else
            exit(1);
    }
    n->array[n->taken] = '\0';
    n->array[n->taken - 1] = add;
    n->taken++;
}

void cutLast(struct Num *n) {
    if (n->taken > 1) {
        n->array[n->taken - 2] = '\0';
        n->taken--;
    }
}

void delNum(struct Num *n) {
    if (n != NULL) {
        free(n->array);
        free(n);
    }
}

void traverse(struct PhoneForwardTrieFromTo *tmp1, struct PhoneForwardTrieToFrom *tmp2, struct Num *n) {
    if (tmp1 != NULL && tmp1->number != NULL) {
        onePhfwdRemove(tmp2, tmp1->number, n->array);
    }
    if (tmp1 == NULL)return;
    for (int i = 0; i < NUMBER_OF_DIGITS; i++) {
        addToNum(n, (char) (i + '0'));
        traverse(tmp1->children[i], tmp2, n);
        cutLast(n);
    }
}

bool phnumAddLexic(struct PhoneNumbers *pnum, char *num) {
    if (pnum->numberOfPhoneNumbers == pnum->arraySize) {
        pnum->arraySize *= MULTIPLIER;
        char **new_array = realloc(pnum->array, sizeof(char *) * pnum->arraySize);
        if (new_array == NULL)
            return false;
        pnum->array = new_array;
    }
    int idx = -1;
    for (int i = 0; i < pnum->numberOfPhoneNumbers; i++) {
        if (strcmp((pnum->array)[i], num) >= 0) {
            if (strcmp((pnum->array)[i], num) == 0) {
                free(num);
                return true;
            }
            idx = i;
            break;
        }
    }
    if (idx == -1)
        (pnum->array)[pnum->numberOfPhoneNumbers] = num;
    else {
        for (int i = pnum->numberOfPhoneNumbers; i > idx; i--) {
            (pnum->array)[i] = (pnum->array)[i - 1];
        }
        (pnum->array)[idx] = num;
    }
    pnum->numberOfPhoneNumbers++;
    return true;
}