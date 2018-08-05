/** @file
 * Interfejs modułu udostępniającego operacje na numerach telefonów przez interfejs tekstowy
 *
 * @author Magdalena Augustyńska <ma370723@mimuw.edu.pl>
 * @date 01.06.2018
 */

#ifndef __PARSER_H__
#define __PARSER_H__

#include <stdlib.h>
#include <stdio.h>
#include <ctype.h>

#include "phone_forward.h"
#include "base.h"

/** @brief Zwalnia pamięć.
 * Zwalnia pamięć zaalokowaną podczas działania programu.
 * @param ws - wskaźnik na strukturę roboczą, w której znajdują się wszystkie struktury w użyciu podczas działania programu.
 */
void freeMemory(struct WorkingStruct *ws);

/** @brief Kończy działanie programu.
 * Zwalnia zaalokowaną pamięć i kończy działanie programu z błędem.
 * @param ws - wskaźnik na strukturę roboczą, w której znajdują się wszystkie struktury w użyciu podczas działania programu.
 */
void end(struct WorkingStruct *ws);

/** @brief Obsługuje błąd składniowy.
 * Wypisuje komunikat o błędzie składniowym na standardowe wyjście diagnostyczne i kończy działanie programu.
 * @param signNumber - numer pierwszego znaku, który nie daje się zinterpretować jako poprawne wejście;
 * @param ws - wskaźnik na strukturę roboczą, w której znajdują się wszystkie struktury w użyciu podczas działania programu.
 */
void syntaxError(int signNumber, struct WorkingStruct *ws);

/** @brief Obsługuje błąd wykonania.
 * Wypisuje komunikat o błędzie wykonania na standardowe wyjście diagnostyczne i kończy działanie programu.
 * @param signNumber - numer pierwszego znaku operatora, którego wykonanie spowodowało błąd;
 * @param op - napis reprezentujący nazwę operatora, którego wykonanie spowodowało błąd;
 * @param ws - wskaźnik na strukturę roboczą, w której znajdują się wszystkie struktury w użyciu podczas działania programu.
 */
void executionError(int signNumber, char *op, struct WorkingStruct *ws);

/** @brief Obsługuje błąd końca danych.
 * Wypisuje komunikat o niespodziewanym końcu danych wejściowych na standardowe wyjście diagnostyczne i kończy działanie programu.
 * @param ws - wskaźnik na strukturę roboczą, w której znajdują się wszystkie struktury w użyciu podczas działania programu.
 */
void eofError(struct WorkingStruct *ws);

/** @brief Wypisuje przekierowania.
 * Wypisuje przekierowanie na numer znajdujący się w strukturze roboczej przekazanej jako argument.
 * Numer ten jest wskazywany przez wskaźnik @p argument1.
 * Aktualna baza przekierowań jest wskazywana przez wskaźnik @p pf w strukturze roboczej.
 * @param signNumber - numer znaku operatora "?";
 * @param ws - wskaźnik na strukturę roboczą, w której znajdują się wszystkie struktury w użyciu podczas działania programu.
 */
void printPhfwdReverse(int signNumber, struct WorkingStruct *ws);

/** @brief Wypisuje przekierowania.
 * Wypisuje przekierowanie z numeru znajdującego się w strukturze roboczej przekazanej jako argument.
 * Numer ten jest wskazywany przez wskaźnik @p argument1.
 * Aktualna baza przekierowań jest wskazywana przez wskaźnik @p pf w strukturze roboczej.
 * @param signNumber - numer znaku operatora "?";
 * @param ws - wskaźnik na strukturę roboczą, w której znajdują się wszystkie struktury w użyciu podczas działania programu.
 */
void printPhfwdGet(int signNumber, struct WorkingStruct *ws);

/** @brief Dodaje przekierowanie.
 * Dodaje przekierowanie z numeru @p argument1 na numer @p argument2 znajdujących się w strukturze roboczej, do bazy wskazywanej przez @p pf.
 * @param signNumber - numer znaku operatora ">";
 * @param ws - wskaźnik na strukturę roboczą, w której znajdują się wszystkie struktury w użyciu podczas działania programu.
 */
void add(int signNumber, struct WorkingStruct *ws);

/** @brief Sprawdza poprawność identyfikatora.
 * Sprawdza, czy identyfikator nie jest jednym z zastrzeżonych napisów.
 * @param c - wskaźnik na napis.
 * @return Wartość @p true, jeśli napis nie jest zastrzeżonym identyfikatorem.
 *         Wartość @p false w przeciwnym przypadku.
 */
bool correctId(char *c);

/** @brief Czyta wejście.
 * Czyta dane ze standardowego wejścia, wyniki wypisuje na standardowe wyjście, a informacje o błędach na standardowe wyjście diagnostyczne.
 * @param ws - wskaźnik na strukturę roboczą, w której znajdują się wszystkie struktury w użyciu podczas działania programu;
 * @return Numer stanu, w którym program zakończył działanie.
 */
int parse(struct WorkingStruct *ws);

#endif /* __PARSER_H__ */