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

#ifndef _CELL_H
#define _CELL_H

const double R_DIST_COFICIENT = 5.067;
const double C_DIST_COFICIENT = 1.642;
const double FB_DIST_COFICIENT = 1.983;
const double BALANCING_CONSTANT = 2.2;
const double CELL_TO_KM = 0.12165;
const double DEM = 0.125;

enum CellType
{
    FOREST = 0,
    LAND = 1,
    CITY = 2,
    ROAD = 3,
    DEFORESTED = 4,
};

typedef struct
{
    enum CellType type;
    double city_distance;
    double road_distance;
    double forest_boundary_distance;
    double density;
    double p_deforest;

} Cell;

Cell *initCellMap();
void computeCellsDistance(Cell *CellMap, Cell *cell, int i, int j);
void setDistances(Cell *CellMap);
void setForestBoundary(Cell *cell, double computed_distance);
void setNeighbourDeforestDensity(Cell *CellMap);
void setProbabiltyOfDeforestationAndUpdateCell(Cell *CellMap);

#endif