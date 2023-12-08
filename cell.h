#ifndef _CELL_H
#define _CELL_H

enum CellType
{
    FOREST = 0,
    LAND = 1,
    CITY = 2,
    ROAD = 3,
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
void setRoadCityDistance(Cell *CellMap);
void setForestBoundary(Cell *cell, double computed_distance);
void setNeighbourDeforestDensity(Cell *CellMap);

#endif