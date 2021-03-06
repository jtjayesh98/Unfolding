#include <CGAL/Exact_predicates_inexact_constructions_kernel.h>

#include <CGAL/grid_simplify_point_set.h>
#include <CGAL/IO/read_points.h>

#include <vector>
#include <fstream>

// types
typedef CGAL::Exact_predicates_inexact_constructions_kernel Kernel;
typedef Kernel::Point_3 Point;

int main(int argc, char*argv[])
{
  const std::string fname = (argc>1) ? argv[1] : CGAL::data_file_path("points_3/oni.pwn");

  // Reads a point set file in points[].
  std::vector<Point> points;
  if(!CGAL::IO::read_points(fname, std::back_inserter(points)))
  {
    std::cerr << "Error: cannot read file " << fname << std::endl;
    return EXIT_FAILURE;
  }

  // simplification by clustering using erase-remove idiom
  double cell_size = 0.001;
  points.erase(CGAL::grid_simplify_point_set(points, cell_size), points.end());

  // Optional: after erase(), use Scott Meyer's "swap trick" to trim excess capacity
  std::vector<Point>(points).swap(points);

  return EXIT_SUCCESS;
}
