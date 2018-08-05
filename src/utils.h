/** @file
 * Interfejs modułu funkcji pomocniczych dla modułu operacji przekierowywania numerów telefonów
 *
 * @author Magdalena Augustyńska <ma370723@mimuw.edu.pl>
 * @date 01.06.2018
 */

#ifndef __UTILS_H__
#define __UTILS_H__

#include <stdbool.h>
#include <stddef.h>
#include <stdlib.h>
#include <string.h>

#include "phone_forward.h"

/**
 * Liczba różnych cyfr.
 */
#define NUMBER_OF_DIGITS 12

/**
 * Początkowy rozmiar tablicy w strukturach zawierających tablicę.
 */
#define INIT_SIZE 5

/**
 * Mnożnik użyty do dynamicznej alokacji pamięci.
 */
#define MULTIPLIER 2

/**
 * Struktura drzewiasta do przechowywania numerów zgodnie z prefiksami oraz ich przekierowań.
 */
struct PhoneForwardTrieFromTo {
    char *number; ///< prefiks, na który przekierowujemy numer reprezentowany przez wierzchołek
    struct PhoneForwardTrieFromTo *children[NUMBER_OF_DIGITS]; ///< tablica synów wierzchołka
};

/**
 * Struktura drzewiasta do przechowywania numerów zgodnie z prefiksami oraz przekierowań na dany numer.
 */
struct PhoneForwardTrieToFrom {
    struct PhoneNumbers *phoneNumbers; ///< prefiksy, które zostały przekierowane na numer reprezentowany przez wierzchołek
    struct PhoneForwardTrieToFrom *children[NUMBER_OF_DIGITS]; ///< tablica synów wierzchołka
};

/**
 * Struktura przechowująca numer telefonu.
 * Struktura stworzona na potrzeby modyfikowania na bieżąco numeru telefonu.
 */
struct Num {
    char *array; ///< wskaźnik na tablicę przechowującą znaki napisu
    int size; ///< rozmiar zaalokowanej tablicy
    int taken; ///< liczba zajętych komórek tablicy
};

/** Tworzy kopię napisu.
 * @param p - wskaźnik na napis;
 * @return - wskaźnik na kopię napisu.
 */
char *strdup(const char *p);

/** @brief Tworzy nową strukturę.
 * Tworzy nową strukturę posiadającą jeden wierzchołek i brak przekierowania.
 * @return Wskaźnik na utworzoną strukturę lub NULL, gdy nie udało się
 *         zaalokować pamięci.
 */
struct PhoneForwardTrieFromTo *phfwdtrieFromToNew(void);

/**@brief Tworzy nową strukturę.
 * Tworzy nową strukturę niezawierającą numerów telefonów.
 * @return Wskaźnik na utworzoną strukturę lub NULL, gdy nie udało się
 *         zaalokować pamięci.
 */
struct PhoneNumbers *phnumNew(void);

/** @brief Tworzy nową strukturę.
 * Tworzy nową strukturę posiadającą jeden wierzchołek ze strukturą niezawierającą numerów telefonów.
 * @return Wskaźnik na utworzoną strukturę lub NULL, gdy nie udało się
 *         zaalokować pamięci.
 */
struct PhoneForwardTrieToFrom *phfwdtrieToFromNew(void);

int isDigit(char c);

/** @brief Sprawdza, czy napis jest numerem telefonu.
 * Sprawdza, czy napis reprezentuje poprawny numer telefonu tj. czy jest niepustym ciągiem,
 * składającym się z cyfr 0, 1, 2, 3, 4, 5, 6, 7, 8, 9.
 * @param s - wskaźnik na dowolny napis.
 * @return Wartość @p true, jeśli napis jest poprawnym numerem telefonu.
 *         Wartość @p false, jeśli napis nie jest poprawnym numerem telefonu.
 */
bool onlyDigits(const char *s);

/** @brief Usuwa strukturę.
 * Usuwa strukturę drzewiastą przechowującą odwrotności przekierowań.
 * Usuwa rekurencyjnie synów. Usuwa strukturę przechowującą numery, towarzyszącą każdemu wierzchołkowi.
 * @param pft - wskaźnik na wskaźnik na stukturę przechowującą przekierowania.
 */
void phfwdTrieToFromDelete(struct PhoneForwardTrieToFrom **pft);

/** @brief Usuwa strukturę.
 * Usuwa strukturę drzewiastą przechowującą przekierowania.
 * Usuwa rekurencyjnie synów. Usuwa numer, na który jest przekierowywany numer reprezentowany przez dany wierzchołek.
 * @param pft - wskaźnik na wskaźnik na stukturę przechowującą przekierowania.
 */
void phfwdTrieFromToDelete(struct PhoneForwardTrieFromTo **pft);

/** @brief Dodaje numer.
 * Dodaje napis reprezentujący numer telefonu do struktury przechowującej numery.
 * @param pnum  - wskaźnik na strukturę przechowującą numery telefonów;
 * @param num - wskaźnik na napis reprezentujący numer telefonu.
 * @return Wartość @p true, jeśli numer został dodany.
 *         Wartość @p false, jeśli nie udało
 *         się zaalokować pamięci.
 */
bool phnumAdd(struct PhoneNumbers *pnum, char const *num);

/** Szuka numeru.
 * Szuka indeksu, pod którym znajduje się numer telefonu w tablicy przechowywanej przez strukturę.
 * @param pnum - wskaźnik na strukturę przechowującą numery telefonów;
 * @param num - wskaźnik na napis reprezentujący numer telefonu.
 * @return Liczba całkowita reprezentująca indeks w tablicy, pod którym znajduje się numer lub
 *         -1, gdy nie znaleziono podanego numeru.
 */
int idxInPhNum(struct PhoneNumbers *pnum, const char *num);

/**@brief Usuwa przekierowanie.
 * Usuwa przekierowanie z drzewa odwrotności przekierowań wszystkich numerów mających prefiks @p num2 na numery,
 * w których ten prefiks zamieniono odpowiednio na prefiks @p num1.
 * @param pft - struktura drzewiasta przechowująca odwrotności przekierowań;
 * @param num1 - wskaźnik na napis reprezentujący prefiks numerów, na które
 *               jest wykonywane przekierowanie;
 * @param num2 – wskaźnik na napis reprezentujący prefiks numerów przekierowywanych.
 */
void onePhfwdRemove(struct PhoneForwardTrieToFrom *pft, const char *num1, const char *num2);

/** @brief Tworzy strukturę.
 * Tworzy nową strukturę reprezentującą numer telefonu.
 * @return Wskaźnik na strukturę reprezentującą numer lub NULL, gdy nie udało się zaalokować pamięci.
 */
struct Num *newNum();

/** @brief Dodaje znak.
 * Modyfikuje napis przechowywany przez strukturę dodając znak na końcu napisu.
 * @param n - wskaźnik na strukturę przechowującą napis;
 * @param add - znak, który ma być dodany.
 */
void addToNum(struct Num *n, char add);

/** @brief Usuwa znak.
 * Usuwa ostatni znak z napisu reprezentowangeo przez strukturę.
 * @param n - wskaźnik na strukturę przechowującą napis.
 */
void cutLast(struct Num *n);

/** @brief Usuwa strukturę.
 *  Usuwa strukturę przechowującą numer telefonu.
 * @param n - wskaźnik na strukturę przechowującą numer telefonu.
 */
void delNum(struct Num *n);

/** @brief Usuwa przekierowania.
 * Szuka i usuwa wszystkie przekierowania w drzewie odwrotności przekierowań @p tmp2, które są zawarte w
 * drzewie przekierowań @p tmp1.
 * @param tmp1 - wskaźnik na drzewo przekierowań;
 * @param tmp2 - wskaźnik na drzewo odwrotności przekierowań;
 * @param n - wskaźnik na strukturę przechowującą numer telefonu.
 */
void traverse(struct PhoneForwardTrieFromTo *tmp1, struct PhoneForwardTrieToFrom *tmp2, struct Num *n);

/**@brief Dodaje numer.
 * Dodaje napis reprezentujący numer do tablicy znajdującej się w strukturze, która przechowuje numery telefonów,
 * zachowując porządek leksykograficzny w tablicy.
 * @param pnum - wskaźnik na strukturę przechowującą numery telefonów;
 * @param num - wskaźnik na napis.
 * @return Wartość @p true, jeśli numer został dodany.
 *         Wartość @p false, jeśli nie udało
 *         się zaalokować pamięci.
 */
bool phnumAddLexic(struct PhoneNumbers *pnum, char *num);

#endif /* __UTILS_H__ */
