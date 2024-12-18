#ifndef NUCLEUS_FORCE_H
#define NUCLEUS_FORCE_H

#include <vector>
#include <string>

namespace nucleusforce {
/**
  * @brief Find the distance from any point on the nucleus to all points in the cell
  *
  * @param cell 2D array where 1 is the cell and 0 is everything else
  * @param nucleus 2D array where 1 is the nucleus and 0 is everything else
  */
std::vector<std::vector<int>> find_dist(std::vector<std::vector<int>> cell,
                                        std::vector<std::vector<int>> nucleus);

/**
  * @brief Find the outer boundary of the cell
  *
  * @param cell 2D array where 1 is the cell and 0 is everything else
  */
std::vector<std::vector<int>> find_boundary(std::vector<std::vector<int>> cell);

/**
 * @brief Find the force on the nucleus due to the outer boundary of the cell
 *        Note: This method assumes an equal force is exerted on all points on the outer boundary of the cell.
 *
 * @param cell 2D array where 1 is the cell and 0 is everything else
 */
std::vector<std::vector<int>> find_nucleus_force(std::vector<std::vector<int>> cell);

/**
 * @brief Find the force on the nucleus due to the outer boundary of the cell
 *
 * @param cell 2D array where 1 is the cell and 0 is everything else
 * @param force 2D array containing the force exerted on the nucleus due to each pixel
 */
std::vector<std::vector<int>> find_nucleus_force(std::vector<std::vector<int>> cell, std::vector<std::vector<int>> force);

/**
  * @brief Output the array as a csv at filepath
  *
  * @param filepath string of the filepath for the csv
  * @param array array to export
  */
void export_csv(std::string filepath, std::vector<std::vector<int>> array);
} // namespace nucleusforce

#endif
