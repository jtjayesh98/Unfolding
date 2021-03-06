#include <CGAL/Simple_cartesian.h>
#include <CGAL/Exact_predicates_exact_constructions_kernel.h>

#include <boost/config.hpp>
#include <boost/version.hpp>

#include <iostream>
#include <fstream>
#include <vector>

#include <CGAL/IO/WKT.h>

//typedef CGAL::Simple_cartesian<CGAL::Gmpq> Kernel;
typedef CGAL::Exact_predicates_exact_constructions_kernel Kernel;

int main(int argc, char* argv[])
{
  typedef CGAL::Point_2<Kernel> Point;
  typedef std::vector<Point> LineString;
  typedef std::vector<LineString> MultiLineString;
  LineString ls;
  {
    std::ifstream is((argc>1)?argv[1]:"data/linestring.wkt");
    CGAL::IO::read_linestring_WKT(is, ls);
    is.close();
  }
  for(Point p : ls)
      std::cout<<p<<std::endl;
  ls.clear();
  MultiLineString mls;
  {
    std::ifstream is((argc>2)?argv[2]:"data/multilinestring.wkt");
    CGAL::IO::read_multi_linestring_WKT(is, mls);
    is.close();
  }
  for(LineString l : mls)
  {
    for(const Point& p : l)
        std::cout<<p<<std::endl;
  }
  return 0;
}
