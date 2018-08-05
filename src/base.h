/** @file
 * Interfejs klasy przechowującej bazy przekierowań
 *
 * @author Magdalena Augustyńska <ma370723@mimuw.edu.pl>
 * @date 01.06.2018
 */

#ifndef __BASE_H__
#define __BASE_H__

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdbool.h>

#include "utils.h"

/** Struktura przechowująca bazy przekierowań.
 * Struktura drzewiasta - wierzchołek reprezentuje bazę oraz jej identyfikator.
 */
struct BaseNode {
    char *id; ///< identyfikator bazy przekierowań
    struct PhoneForward *pf; ///< wskaźnik na bazę przekierowań
    struct BaseNode *left; ///< wskaźnik na lewego syna; poddrzewo zawierające przekierowania o identyfikatorze mniejszym wg porządku leksykograficznego
    struct BaseNode *right; ///< wskaźnik na prawego syna; poddrzewo zawierające przekierowania o identyfikatorze większym wg porządku leksykograficznego
};

/**
 * Struktura robocza przechowująca elementy potrzebne do czytania zdefiniowanego w zadaniu języka programowania.
 */
struct WorkingStruct {
    struct PhoneForward *pf; ///< wskaźnik na aktualną bazę przekierowań
    struct BaseNode *bases; ///< wskaźnik na strukturę drzewiastą przechowującą bazy przekierowań
    struct Num *argument1; ///< struktura reprezentująca napis do przechowywania argumentu funkcji - identyfikatora bazy lub numeru telefonu
    struct Num *argument2; ///< struktura reprezentująca napis do przechowywania drugiego argumentu funkcji - identyfikatora bazy lub numeru telefonu
};

/** @brief Tworzy strukturę.
 * Tworzy nową strukturę roboczą zawierającą niezainicjalizowane struktury.
 * @return Wskaźnik na utworzoną strukturę.
 */
struct WorkingStruct *newWorkingStruct();

/** @brief Usuwa strukturę.
 * Usuwa strukturę drzewiastą przechowującą bazy przekierowań.
 * @param n - wskaźnik na strukturę do usunięcia.
 */
void delNode(struct BaseNode *n);

/** @brief Dodaje bazę.
 * Dodaje wierzchołek reprezentujący bazę przekierowań do struktury drzewiastej.
 * @param n - wskaźnik na korzeń drzewa przechowującego bazy przekierowań;
 * @param id - napis reprezentujący identyfikator bazy, którą dodajemy;
 * @param pf - wskaźnik na wskaźnik na aktualną bazę przekierowań;
 * @return Wskaźnik na korzeń zmodyfikowanego drzewa.
 */
struct BaseNode *newBase(struct BaseNode *n, char *id, struct PhoneForward **pf);

/** @brief Usuwa wierzchołek.
 * Usuwa z drzewa wierzchołek o podanym identyfikatorze.
 * @param n - wskaźnik na korzeń drzewa przechowującego bazy przekierowań;
 * @param id - napis reprezentujący identyfikator bazy, którą usuwamy;
 * @param pf - wskaźnik na wskaźnik na aktualną bazę przekierowań;
 * @param correct - parametr informujący o tym, czy próbowano usunąć bazę nieobecną w drzewie - modyfikowany w takim przypadku.
 * @return Wskaźnik na korzeń zmodyfikowanego drzewa.
 */
struct BaseNode *delBase(struct BaseNode *n, char *id, struct PhoneForward **pf, bool *correct);

/** @brief Znajduje wierzchołek.
 * Pomocnicza funkcja do usuwania wierzchołka z drzewa baz przekierowań. Znajduje wierzchołek, który jest wstawiany w miejsce usuwanego
 * i usuwa krawędź między nim a rodzicem.
 * @param n - wskaźnik na korzeń przeszukiwanego poddrzewa.
 * @return Wskaźnik na znaleziony wierzchołek drzewa
 */
struct BaseNode *findMinimumOnTheRight(struct BaseNode *n);

#endif /* __BASE_H__ */
