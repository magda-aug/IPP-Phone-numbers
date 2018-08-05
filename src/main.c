/** @file
 * Funkcja main modułu udostępniającego operacje na numerach telefonów przez interfejs tekstowy
 *
 * @author Magdalena Augustyńska <ma370723@mimuw.edu.pl>
 * @date 01.06.2018
 */

#include <stdlib.h>

#include "base.h"
#include "parser.h"

/** @brief Przeprowadza program.
 * Czyta dane ze standardowego wejścia, wyniki wypisuje na standardowe wyjście, a informacje o błędach na standardowe wyjście diagnostyczne.
 * @return Wartość 0, jeśli program przetworzył wszystkie dane wejściowe i zakończył się poprawnie.
 *         W przeciwnym przypadku kończy działanie programu i nic nie zwraca.
 */
int main() {
    struct WorkingStruct *w = newWorkingStruct();
    int endState = parse(w);
    if (endState != 1)eofError(w);
    freeMemory(w);
    return 0;
}
