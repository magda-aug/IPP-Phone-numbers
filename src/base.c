/** @file
 * Implementacja interfejsu klasy przechowującej bazy przekierowań
 *
 * @author Magdalena Augustyńska <ma370723@mimuw.edu.pl>
 * @date 01.06.2018
 */

#include <stdlib.h>
#include <string.h>

#include "utils.h"
#include "base.h"

void delNode(struct BaseNode *n) {
    if (n == NULL)return;
    delNode(n->left);
    delNode(n->right);
    free(n->id);
    if (n->pf != NULL) {
        phfwdDelete(n->pf);
        n->pf = NULL;
    }
    free(n);
}

struct BaseNode *newBase(struct BaseNode *n, char *id, struct PhoneForward **pf) {
    if (n == NULL) {
        n = malloc(sizeof(struct BaseNode));
        if (n == NULL)
            return NULL;
        n->id = strdup(id);
        n->pf = phfwdNew();
        *pf = n->pf;
        n->left = NULL;
        n->right = NULL;
        return n;
    } else if (strcmp(id, n->id) > 0) {
        n->right = newBase(n->right, id, pf);
    } else if (strcmp(id, n->id) < 0) {
        n->left = newBase(n->left, id, pf);
    } else *pf = n->pf;
    return n;
}

struct BaseNode *findMinimumOnTheRight(struct BaseNode *n) {
    struct BaseNode *parent = n;
    n = n->right;
    int i = 0;
    while (n->left != NULL) {
        i = 1;
        parent = n;
        n = n->left;
    }
    if(i) parent->left = n->right;
    else parent->right = n->right;
    return n;
}

struct BaseNode *delBase(struct BaseNode *n, char *id, struct PhoneForward **pf, bool *correct) {
    if (n == NULL) {
        *correct = false;
        return NULL;
    } else if (strcmp(id, n->id) < 0)
        n->left = delBase(n->left, id, pf, correct);
    else if (strcmp(id, n->id) > 0)
        n->right = delBase(n->right, id, pf, correct);
    else {
        if (n->left == NULL && n->right == NULL) {
            free(n->id);
            n->id = NULL;
            if (*pf == n->pf)*pf = NULL;
            if (n->pf) {
                phfwdDelete(n->pf);
                n->pf = NULL;
            }
            free(n);
            n = NULL;
        } else if (n->left == NULL) {
            free(n->id);
            n->id = NULL;
            if (*pf == n->pf)*pf = NULL;
            if (n->pf) {
                phfwdDelete(n->pf);
                n->pf = NULL;
            }
            n = n->right;
        } else if (n->right == NULL) {
            free(n->id);
            n->id = NULL;
            if (*pf == n->pf)*pf = NULL;
            if (n->pf) {
                phfwdDelete(n->pf);
                n->pf = NULL;
            }
            n = n->left;
        } else {
            struct BaseNode *temp = findMinimumOnTheRight(n);
            free(n->id);
            n->id = NULL;
            if (*pf == n->pf)*pf = NULL;
            if (n->pf) {
                phfwdDelete(n->pf);
                n->pf = NULL;
            }
            n->id = temp->id;
            n->pf = temp->pf;
            free(temp);
        }
    }
    return n;
}

struct WorkingStruct *newWorkingStruct() {
    struct WorkingStruct *newWS = malloc(sizeof(struct WorkingStruct));
    newWS->pf = NULL;
    newWS->bases = NULL;
    newWS->argument1 = NULL;
    newWS->argument2 = NULL;
    return newWS;
}
