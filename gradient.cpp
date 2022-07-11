#include <CGAL/Exact_predicates_inexact_constructions_kernel.h>
#include <CGAL/Polygon_2.h>
#include <CGAL/draw_polygon_2.h>
#include <CGAL/draw_polygon_with_holes_2.h>
#include <CGAL/Boolean_set_operations_2.h>
#include <CGAL/intersections.h>
#include <CGAL/Triangle_2.h>
#include <iostream>
#include <typeinfo>
#include <vector>
#include <random>
#include <CGAL/Polygon_mesh_processing/orient_polygon_soup.h>
#include <CGAL/Polygon_mesh_processing/polygon_soup_to_polygon_mesh.h>
#include <CGAL/Aff_transformation_2.h>
#include <cstdlib>
#include <CGAL/squared_distance_2.h>
#include <cmath>
#define 	CGAL_PI   3.14159265358979323846

const double max_dist = 1;

typedef CGAL::Exact_predicates_inexact_constructions_kernel K;
typedef K::Point_2 Point;
typedef CGAL::Polygon_2<K> Polygon_2;
typedef CGAL::Polygon_with_holes_2<K> Polygon_with_holes_2;
typedef std::list<Polygon_with_holes_2> Pwh_list_2;
typedef K::Triangle_2 Triangle_2;
typedef K::Segment_2 Segment_2;

typedef CGAL::Aff_transformation_2<K> Aff_transformation;
typedef CGAL::Vector_2<K> Vector;
typedef CGAL::Direction_2<K> Direction;

using std::cout; using std::endl;


#include "Population.cpp"

std::vector<Transformation> gradient_set(){
  std::vector<Transformation> set;
  Transformation a2 = Transformation(Vector(0.0001, 0), CGAL_PI/10000, 0);
  Transformation a1 = Transformation(Vector(0.0001, 0), CGAL_PI/10000, 1);
  Transformation a3 = Transformation(Vector(0.0001, 0), -CGAL_PI/10000, 0);
  Transformation a4 = Transformation(Vector(0.0001, 0), -CGAL_PI/10000, 1);
  Transformation a5 = Transformation(Vector(0, 0.0001), CGAL_PI/10000, 0);
  Transformation a6 = Transformation(Vector(0, 0.0001), CGAL_PI/10000, 1);
  Transformation a7 = Transformation(Vector(0, 0.0001), -CGAL_PI/10000, 0);
  Transformation a8 = Transformation(Vector(0, 0.0001), -CGAL_PI/10000, 1);
  Transformation a9 = Transformation(Vector(-0.0001, 0), CGAL_PI/10000, 0);
  Transformation a10 = Transformation(Vector(-0.0001, 0), CGAL_PI/10000, 1);
  Transformation a11 = Transformation(Vector(-0.0001, 0), -CGAL_PI/10000, 0);
  Transformation a12 = Transformation(Vector(-0.0001, 0), -CGAL_PI/10000, 1);
  Transformation a13 = Transformation(Vector(0, -0.0001), CGAL_PI/10000, 0);
  Transformation a14 = Transformation(Vector(0, -0.0001), CGAL_PI/10000, 1);
  Transformation a15 = Transformation(Vector(0, -0.0001), -CGAL_PI/10000, 0);
  Transformation a16 = Transformation(Vector(0, -0.0001), -CGAL_PI/10000, 1);
  Transformation a17 = Transformation(Vector(0.0001, 0), 0, 0);
  Transformation a18 = Transformation(Vector(0, 0.0001), 0, 0);
  Transformation a19 = Transformation(Vector(-0.0001, 0), 0, 0);
  Transformation a22 = Transformation(Vector(0, -0.0001), 0, 0);
  Transformation a21 = Transformation(Vector(0, 0), CGAL_PI/10000, 0);
  Transformation a20 = Transformation(Vector(0, 0), -CGAL_PI/10000, 0);

  set.push_back(a1);
  set.push_back(a2);
  set.push_back(a3);
  set.push_back(a4);
  set.push_back(a5);
  set.push_back(a6);
  set.push_back(a7);
  set.push_back(a8);
  set.push_back(a9);
  set.push_back(a10);
  set.push_back(a11);
  set.push_back(a12);
  set.push_back(a13);
  set.push_back(a14);
  set.push_back(a15);
  set.push_back(a16);
  set.push_back(a17);
  set.push_back(a18);
  set.push_back(a19);
  set.push_back(a20);
  set.push_back(a21);
  set.push_back(a22);
  return set;
}

double move_cost(int index, std::vector<Triangle> triangles, Transformation transformation){
  double org_cost = calculator(dual_set(triangles));
  triangles.at(index) = transformation.perform_transformation(triangles.at(index));
  double new_cost = calculator(dual_set(triangles));
  // std::cout<<(new_cost - org_cost)<<std::endl;
  return new_cost - org_cost;
}

double change(Triangle triangle, Transformation transformation){
  Triangle transformed = transformation.perform_transformation(triangle);
  return cost(triangle.triangle, transformed.triangle, true);
}

Transformations gradient(std::vector<Triangle> triangles){
  std::vector<Transformation> gradients;
  std::vector<Transformation> grad_move = gradient_set();
  
  // std::cout<<grad_move.size()<<std::endl;
  for (int i = 0; i < triangles.size(); i++){
    double min_cost = 9999999999;
    int index = 0;
    for (int j = 0; j< grad_move.size(); j++){
      double cost = move_cost(i, triangles, grad_move.at(j));
      if (cost < min_cost){
        min_cost = cost;
        index = j;
        // std::cout<<index<<std::endl;
      }
      // std::cout<<std::endl;
    }
    gradients.push_back(grad_move.at(index));
    // std::cout<<std::endl;    
  }
  Transformations retVal = Transformations(gradients);
  return retVal;
}

// double test_cost(std::vector<)

double test_cost(std::vector<Triangle> triangles, int index_, int trindex){
  std::vector<double> gradients;
  std::vector<Transformation> grad_move = gradient_set();
  double cost = move_cost(trindex, triangles, grad_move.at(index_));
  return cost;
}

std::vector<Triangle> gradient_move(std::vector<Triangle> triangles, double learning_rate){
  std::vector<Triangle> retVal;
  Transformations gradients = gradient(triangles);
  for (int i = 0; i<triangles.size(); i++){
    retVal.push_back(gradients.transformations.at(i).scalar_multiplier(learning_rate).perform_transformation(triangles.at(i)));
  }
  return retVal;
}

void iterator(std::vector<Triangle> triangles, int iterations){
  for(int i = 0; i < iterations; i++){

  }
}

int main()
{
  Triangle test = Triangle(Point(0, 0), Point(0, 1), Point(1, 1));
  Triangle pgn1 = Triangle(Point(0,10), Point(4,10), Point(2, 2));
  Triangle pgn2 = Triangle(Point(0,0), Point(4,0), Point(2, 3));
  // Triangle pgn3 = Triangle(Point(0,0), Point(0,10), Point(5, 5));
  // Triangle pgn4 = Triangle(Point(1,1), Point(6,7), Point(2,3));
  // Triangle pgn5 = Triangle(Point(-1, -3), Point(-4, -5), Point(0,1));

  std::vector<Triangle> triangles;
  triangles.push_back(pgn1);
  triangles.push_back(pgn2);
  // triangles.push_back(pgn3);
  // triangles.push_back(pgn4);
  // triangles.push_back(pgn5);
  for (int i = 0; i < 10; i++){
    std::cout<<calculator(dual_set(triangles))<<std::endl;
    triangles = gradient_move(triangles, 100);
    std::cout<<calculator(dual_set(triangles))<<std::endl;
    std::cout<<std::endl;
  }
  
  // Transformations gradients = gradient(triangles);
  // std::cout<<":"<<std::endl;
  // Point p(1, 0);
  // Point q(-1, -1);
  // // std::cout<<<<std::endl;
  // for (int i = 0; i < gradients.transformations.size(); i++){
  //   std::cout<<"Vector Movement: "<<gradients.transformations.at(i).retVector()<<std::endl;
  //   std::cout<<"Rotation by: "<<gradients.transformations.at(i).retRot()<<std::endl;
  //   std::cout<<"Order is: "<<gradients.transformations.at(i).retOrder()<<std::endl;
  //   std::cout<<std::endl;
  // }



  return 1;
}