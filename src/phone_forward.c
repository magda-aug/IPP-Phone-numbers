/** @file
 * Implementacja modułu operacji przekierowywania numerów telefonów
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

struct PhoneForward *phfwdNew(void) {
    struct PhoneForward *phoneForward = malloc(sizeof(struct PhoneForward));
    if (phoneForward == NULL) return NULL;
    phoneForward->trieFromTo = phfwdtrieFromToNew();
    phoneForward->trieToFrom = phfwdtrieToFromNew();
    if (phoneForward->trieFromTo == NULL || phoneForward->trieToFrom == NULL) {
        free(phoneForward);
        return NULL;
    }
    return phoneForward;
}

void phfwdDelete(struct PhoneForward *pf) {
    if (pf == NULL) return;
    phfwdTrieFromToDelete(&(pf->trieFromTo));
    phfwdTrieToFromDelete(&(pf->trieToFrom));
    free(pf);
}

bool phfwdAdd(struct PhoneForward *pf, char const *num1, char const *num2) {
    if (!onlyDigits(num1) || !onlyDigits(num2))return false;
    if (strcmp(num1, num2) == 0)return false;
    struct PhoneForwardTrieFromTo *tmp1 = pf->trieFromTo;
    struct PhoneForwardTrieToFrom *tmp2 = pf->trieToFrom;
    size_t length = strlen(num1);
    char letter;
    for (size_t i = 0; i < length; i++) {
        letter = num1[i];
        if (tmp1->children[(letter - '0')] == NULL) {
            tmp1->children[(letter - '0')] = phfwdtrieFromToNew();
            if (tmp1->children[(letter - '0')] == NULL)return false;
        }
        tmp1 = tmp1->children[(letter - '0')];
    }
    if (tmp1->number != NULL) {
        onePhfwdRemove(tmp2, tmp1->number, num1);
        free(tmp1->number);
        tmp1->number = NULL;
    }
    tmp1->number = strdup(num2);

    length = strlen(num2);
    for (size_t i = 0; i < length; i++) {
        letter = num2[i];
        if (tmp2->children[(letter - '0')] == NULL) {
            tmp2->children[(letter - '0')] = phfwdtrieToFromNew();
            if (tmp2->children[(letter - '0')] == NULL)return false;
        }
        tmp2 = tmp2->children[(letter - '0')];
    }
    return phnumAdd(tmp2->phoneNumbers, num1);
}

void phfwdRemove(struct PhoneForward *pf, char const *num) {
    if (!onlyDigits(num))return;
    struct PhoneForwardTrieFromTo *tmp1 = pf->trieFromTo;
    struct PhoneForwardTrieFromTo *tmp1Prev = NULL;
    struct PhoneForwardTrieToFrom *tmp2 = pf->trieToFrom;
    size_t length = strlen(num);
    char letter = 0;
    size_t i;
    for (i = 0; i < length; i++) {
        letter = num[i];
        if (tmp1->children[(letter - '0')] == NULL) {
            break;
        }
        tmp1Prev = tmp1;
        tmp1 = tmp1->children[(letter - '0')];
    }
    if (i != length)
        return;
    struct Num *vector = newNum();
    for (i = 0; num[i] != '\0'; i++) {
        addToNum(vector, num[i]);
    }
    traverse(tmp1, tmp2, vector);
    phfwdTrieFromToDelete(&tmp1);
    if (tmp1Prev != NULL && letter != 0) {
        tmp1Prev->children[(letter - '0')] = NULL;
    }
    delNum(vector);
}

struct PhoneNumbers const *phfwdGet(struct PhoneForward *pf, char const *num) {
    struct PhoneNumbers *res = phnumNew();
    if (res == NULL)
        return NULL;
    res->numberOfPhoneNumbers++;
    if (!onlyDigits(num)) {
        res->array[0] = NULL;
        return res;
    }
    size_t oldPrefix = 0;
    size_t length = strlen(num);
    struct PhoneForwardTrieFromTo *tmp = pf->trieFromTo;
    char letter;
    char *max = NULL;
    size_t i;
    size_t counter = 0;
    for (i = 0; i < length; i++) {
        letter = num[i];
        if (tmp->children[(letter - '0')] == NULL) {
            break;
        } else {
            tmp = tmp->children[(letter - '0')];
            counter++;
            if (tmp->number) {
                max = tmp->number;
                oldPrefix = counter;
            }
        }
    }

    if (max == NULL) res->array[0] = strdup(num);
    else {
        size_t newPrefix = strlen(max);
        size_t newLength = strlen(num) - oldPrefix + newPrefix;
        char *fwd = malloc(sizeof(char) * (newLength + 1));
        for (i = 0; i < newPrefix; i++) {
            fwd[i] = max[i];
        }
        for (i = newPrefix; i < newLength; i++) {
            fwd[i] = num[i - newPrefix + oldPrefix];
        }
        fwd[newLength] = 0;
        res->array[0] = fwd;
    }
    return res;
}

struct PhoneNumbers const *phfwdReverse(struct PhoneForward *pf, char const *num) {
    struct PhoneNumbers *res = phnumNew();
    if (res == NULL)return NULL;
    if (!onlyDigits(num)) {
        res->array[0] = NULL;
        res->numberOfPhoneNumbers++;
        return res;
    }
    size_t length = strlen(num);
    struct PhoneForwardTrieToFrom *tmp = pf->trieToFrom;
    char letter;
    size_t i, newPrefix, newLength;
    for (i = 0; i < length; i++) {
        letter = num[i];
        if (tmp->children[(letter - '0')] == NULL) {
            break;
        }
        tmp = tmp->children[(letter - '0')];
        for (int j = 0; j < tmp->phoneNumbers->numberOfPhoneNumbers; j++) {
            newPrefix = strlen(tmp->phoneNumbers->array[j]);
            newLength = strlen(num) - (i + 1) + newPrefix;
            char *fwd = malloc(sizeof(char) * (newLength + 1));
            for (size_t k = 0; k < newPrefix; ++k) {
                fwd[k] = (tmp->phoneNumbers->array[j])[k];
            }
            for (size_t k = newPrefix; k < newLength; ++k) {
                fwd[k] = num[k - newPrefix + i + 1];
            }
            fwd[newLength] = 0;
            if (phnumAddLexic(res, fwd) == false)return NULL;
        }
    }
    phnumAddLexic(res, strdup(num));
    return res;
}

char const *phnumGet(struct PhoneNumbers const *pnum, size_t idx) {
    if (pnum == NULL)return NULL;
    if (idx > (size_t) pnum->numberOfPhoneNumbers - 1)return NULL;
    return pnum->array[idx];
}

size_t phfwdNonTrivialCount(struct PhoneForward *pf, char const *set, size_t len) {
    if (pf == NULL || !onlyDigits(set) || len == 0)return 0;
}