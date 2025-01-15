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
std::vector<std::vector<int>> find_boundary(std::vector<std::vector<int>> cell,
                                            std::vector<std::vector<int>> nucleus);

/**
 * @brief Find the force on the nucleus due to the outer boundary of the cell
 *        Note: This method assumes an equal force is exerted on all points on the outer boundary of the cell.
 *
 * @param cell 2D array where 1 is the cell and 0 is everything else
 * @param nucleus 2D array where 1 is the nucleus and 0 is everything else
 *
 * @return 2D double array of the force on each pixel on nucleus
 */
std::vector<std::vector<double>> find_nucleus_force(std::vector<std::vector<int>> cell, 
                                                    std::vector<std::vector<int>> nucleus);

/**
 * @brief Find the force on the nucleus due to the pixels with applied force
 *
 * @param cell 2D array where 1 is the cell and 0 is everything else
 * @param nucleus 2D array where 1 is the nucleus and 0 is everything else
 * @param force 2D array containing the force exerted on the nucleus due to each pixel
 *
 * @return 2D double array of the force on each pixel on nucleus
 */
std::vector<std::vector<double>> find_nucleus_force(std::vector<std::vector<int>> cell, 
                                                    std::vector<std::vector<int>> nucleus, 
                                                    std::vector<std::vector<double>> force);

/**
 * @brief Find the nucleus centroid
 *
 * @param nucleus nucleus
 *
 * @return vector of 2 elements (x, y) of the centroid
 */
std::vector<double> find_nucleus_centroid(std::vector<std::vector<int>> nucleus);

/**
 * @brief Find the force vector on the nucleus
 *
 * @param nucleus 2D array where 1 is the nucleus and 0 is anything else
 * @param force 2D array of the force exerted on each pixel on the outer surface of the nucleus
 *
 * @return vector of 2 elements (x, y) of the net force on the nucleus
 */
std::vector<double> find_force_vector(std::vector<std::vector<int>> nucleus,
                                      std::vector<std::vector<double>> force);

/**
 * @brief Create a coord and distance pair
 *
 * @param y y coordinate
 * @param x x coordinate
 * @param d distance
 */
inline std::pair<int, std::pair<int, int>> make_coord(int y, int x, int d);

/**
  * @brief Output the array as a csv at filepath
  *
  * @param filepath string of the filepath for the csv
  * @param array array to export
  */
void export_csv(std::string filepath, std::vector<std::vector<int>> array);

/**
  * @brief Output the array as a csv at filepath
  *
  * @param filepath string of the filepath for the csv
  * @param array array to export
  */
void export_csv(std::string filepath, std::vector<std::vector<double>> array);
} // namespace nucleusforce

#endif
