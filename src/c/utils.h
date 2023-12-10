/**
 *
 * @file main.c
 *
 * @brief Project: IMS CA in ecology
 *
 * @authors xbalek02 Miroslav Bálek, xdobes22 Kristián Dobeš
 *
 *
 *
 */

#ifndef _UTILS_H
#define _UTILS_H

#include "cell.h"

double randomNumber();
void printMap(Cell *CellMap);
int parseArgs(int argc, char *const argv[]);
#endif