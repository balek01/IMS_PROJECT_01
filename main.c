/**
 *
 * @file main.c
 *
 * @brief Project: IMS CA in ecology
 *
 * @author xbalek02 Miroslav Bálek
 *
 *
 *
 */
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <math.h>
#include "map.h"
#include "cell.h"
#include "utils.h"
#include "main.h"

int main(int argc, char *argv[])
{

    Cell *CellMap = initCellMap(map);

    setRoadCityDistance(CellMap);
    setNeighbourDeforestDensity(CellMap);
    // setProbabiltyOfDeforestation(CellMap);

    printf("done \n");

    free(CellMap);
    return 0;
}

void setNeighbourDeforestDensity(Cell *CellMap)
{
    int count = 0;

    // Define the relative positions of neighbors (assuming 8-connected neighbors)
    int dx[] = {-1, -1, -1, 0, 0, 1, 1, 1};
    int dy[] = {-1, 0, 1, -1, 1, -1, 0, 1};

    for (int row = 0; row < ROWS; row++)
    {

        for (int col = 0; col < COLS; col++)
        {
            count = 0;
            if (CellMap[row * COLS + col].type == FOREST)
            {

                for (int i = 0; i < 8; ++i)
                {
                    int newRow = row + dx[i];
                    int newCol = col + dy[i];

                    // Check if the new position is within bounds
                    if (newRow >= 0 && newRow < ROWS && newCol >= 0 && newCol < COLS)
                    {

                        // Check if the neighbor has the target value 'F'
                        if (CellMap[newRow * COLS + newCol].type != FOREST)
                        {
                            count++;
                        }
                    }
                }
                CellMap[row * COLS + col].density = count;
            }
        }
    }
}
void setRoadCityDistance(Cell *CellMap)
{
    Cell *cell;
    for (int i = 0; i < ROWS; i++)
    {
        // printf("\n");
        for (int j = 0; j < COLS; j++)
        {

            cell = &CellMap[i * COLS + j];
            if (cell->type == FOREST)
            {

                computeCellsDistance(CellMap, cell, i, j);
                // printf("%f ", cell->forest_boundary_distance);
            }
        }
    }
}

void computeCellsDistance(Cell *CellMap, Cell *cell, int x, int y)
{
    double computed_distance = 0;
    for (int k = 0; k < ROWS; k++)
    {
        for (int l = 0; l < COLS; l++)
        {
            computed_distance = sqrt(((x - k) * (x - k)) + ((y - l) * (y - l)));

            if (CellMap[k * COLS + l].type == ROAD)
            {
                if ((cell->road_distance == -1) || (cell->road_distance > computed_distance))
                {
                    cell->road_distance = computed_distance;
                }
                setForestBoundary(cell, computed_distance);
            }
            else if (CellMap[k * COLS + l].type == CITY)
            {

                if ((cell->city_distance == -1) || cell->city_distance > computed_distance)
                {
                    cell->city_distance = computed_distance;
                }
                setForestBoundary(cell, computed_distance);
            }
            else if (CellMap[k * COLS + l].type == LAND)
            {

                setForestBoundary(cell, computed_distance);
            }
        }
    }
}

void setForestBoundary(Cell *cell, double computed_distance)
{
    if ((cell->forest_boundary_distance == -1) || cell->forest_boundary_distance > computed_distance)
    {
        cell->forest_boundary_distance = computed_distance;
    }
}

Cell *initCellMap()
{
    Cell *CellMap = malloc(ROWS * COLS * sizeof(Cell));
    if (CellMap == NULL)
    {
        perror("Memory allocation failed.");
        exit(1);
    }
    for (int i = 0; i < ROWS; i++)
    {
        for (int j = 0; j < COLS; j++)
        {

            CellMap[i * COLS + j].city_distance = -1;
            CellMap[i * COLS + j].road_distance = -1;
            CellMap[i * COLS + j].forest_boundary_distance = -1;
            switch (map[i][j])
            {
            case 'L':
                CellMap[i * COLS + j].type = LAND;
                break;
            case 'C':
                CellMap[i * COLS + j].type = CITY;
                break;
            case 'R':
                CellMap[i * COLS + j].type = ROAD;
                break;
            case 'F':
                CellMap[i * COLS + j].type = FOREST;
                break;

            default:
                printf("INVALID STATE IN MAP %c", map[i][j]);
                break;
            }
        }
    }

    return CellMap;
}