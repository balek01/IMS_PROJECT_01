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
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <math.h>
#include <time.h>
#include <string.h>
#include "map.h"
#include "cell.h"
#include "utils.h"

int main(int argc, char *const argv[])
{
    int numberOfMonthsSimluated = parseArgs(argc, argv);
    srand((unsigned int)time(NULL));
    Cell *CellMap = initCellMap(map);

    for (int i = 0; i < numberOfMonthsSimluated; i++)
    {
        setDistances(CellMap);
        setNeighbourDeforestDensity(CellMap);
        setProbabiltyOfDeforestationAndUpdateCell(CellMap);
        fprintf(stderr, "%d\n", i + 1);
        fflush(stderr);
    }

    printMap(CellMap);

    free(CellMap);
    return 0;
}
int parseArgs(int argc, char *const argv[])
{

    if (argc < 3)
    {
        fprintf(stderr, "Usage: %s -t <months>\n", argv[0]);
        exit(1);
    }

    if (strcmp(argv[1], "-t") != 0)
    {
        fprintf(stderr, "Usage: %s -t <months>\n", argv[0]);
        exit(1);
    }

    int t_value = atoi(argv[2]);

    if (t_value <= 0)
    {
        fprintf(stderr, "Invalid value for -t. Please enter an integer greater than 0.\n");
        exit(1);
    }
    return t_value;
}
void printMap(Cell *CellMap)
{
    Cell *cell;
    for (int row = 0; row < ROWS; row++)
    {
        for (int col = 0; col < COLS; col++)
        {
            cell = &CellMap[row * COLS + col];
            switch (cell->type)
            {
            case FOREST:
                printf("F");
                break;
            case LAND:
                printf("L");
                break;
            case CITY:
                printf("C");
                break;
            case ROAD:
                printf("R");
                break;
            case DEFORESTED:
                printf("D");
                break;
            default:
                printf("unknown cell type: %d\n", cell->type);
                break;
            }
        }
    }
}
void setProbabiltyOfDeforestationAndUpdateCell(Cell *CellMap)
{
    Cell *cell;
    for (int row = 0; row < ROWS; row++)
    {
        // printf("\n");
        for (int col = 0; col < COLS; col++)
        {
            if (CellMap[row * COLS + col].type == FOREST)
            {

                cell = &CellMap[row * COLS + col];

                double exponent = -(cell->density * CELL_TO_KM - BALANCING_CONSTANT - C_DIST_COFICIENT * cell->city_distance * CELL_TO_KM - R_DIST_COFICIENT * cell->road_distance * CELL_TO_KM - FB_DIST_COFICIENT * cell->forest_boundary_distance * CELL_TO_KM - DEM);
                double denominator = 1.0 + exp(exponent);
                double result = 1.0 / denominator;
                cell->p_deforest = result;

                double randomNum = randomNumber();
                // printf("%.2f \n", randomNum);
                if (randomNum <= cell->p_deforest)
                {
                    // printf("%.2f ", cell->p_deforest);
                    cell->type = DEFORESTED;
                }
            }
        }
    }
}
double randomNumber()
{
    int randomInt = rand() % 1000001;
    return (double)randomInt / 1000000.0;
}
void setNeighbourDeforestDensity(Cell *CellMap)
{
    int count = 0;

    // Define the relative positions of neighbors (assuming 8-connected neighbors)
    int dx[] = {-1, -1, -1, 0, 0, 1, 1, 1};
    int dy[] = {-1, 0, 1, -1, 1, -1, 0, 1};
    double result;

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
                result = 1 + (count / 8.0);
                CellMap[row * COLS + col].density = result;
            }
        }
    }
}

void setDistances(Cell *CellMap)
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
            else if (CellMap[k * COLS + l].type == LAND || CellMap[k * COLS + l].type == DEFORESTED)
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