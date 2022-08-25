#include <CGAL/Exact_predicates_inexact_constructions_kernel.h>
#include <CGAL/Polygon_2.h>
#include <iostream>
#include <CGAL/draw_polygon_2.h>
#include <CGAL/Surface_mesh/IO/OFF.h>
#include <CGAL/Surface_mesh/Surface_mesh.h>
#include <CGAL/draw_surface_mesh.h>

typedef CGAL::Exact_predicates_inexact_constructions_kernel K;
typedef K::Point_2 Point;
typedef CGAL::Polygon_2<K> Polygon_2;
typedef CGAL::Surface_mesh<K::Point_3> Surface_mesh;
// const NamedParameters &np
using std::cout; using std::endl;
int main()
{
  Surface_mesh sm;
  std::ifstream myfile;
  myfile.open("./test.off");
  // CGAL::IO::read_OFF(myfile, sm);
  CGAL::IO::read_OFF(myfile, sm);
  myfile.close();
  Point points1[] = { Point(0,0), Point(5.1,0), Point(1,1)};
  Point points2[] = { Point(0,0), Point(6,0), Point(2, 2)};
  Polygon_2 pgn1(points1, points1+4);
  Polygon_2 pgn2(points2, points2+4);
  // check if the polygon is simple.
  // CGAL::draw(pgn1);
  CGAL::draw(sm);
  return 0;
}