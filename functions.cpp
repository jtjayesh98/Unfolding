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
#include <fstream>

#define 	CGAL_PI   3.14159265358979323846


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





double distance (Triangle_2 triangle1, Triangle_2 triangle2){
  double dist = squared_distance(triangle1, triangle2);
  return dist;
}
// '''
// "overlap" is not taking into consideration if the triangles don't intersect. Need to run it and see what happens if they dont intersect.
// '''
//Edit: Taken care of the edge case where there is no intersect. 

double overlap(Triangle_2 triangle1, Triangle_2 triangle2){
  double overlap_area; //Overlap area between two triangles
  decltype(auto) intR = CGAL::intersection(triangle1, triangle2);
  if (!intR){
    return 0;
  }
  if (const Triangle_2* s = boost::get<Triangle_2>(&*intR)){
    overlap_area = s->area();
  }
  else if(const Point* p = boost::get<Point>(&*intR)){
    overlap_area = 0;
  }
  else if(const Segment_2* s = boost::get<Segment_2>(&*intR)){
    overlap_area = 0;
  }
  else if(const std::vector<Point>* v = boost::get<std::vector<Point>>(&*intR)){
    int count = v->size();
    Polygon_2 p;
    for(int i = 0; i < count; i++){
      Point a = Point(v->at(i));
      p.push_back(a);
    }
    overlap_area = p.area();
  }
  return double(overlap_area);
}

double cost(Triangle_2 triangle1, Triangle_2 triangle2, bool connected){
  // std::cout<<2<<std::endl;
  double overlap_area; //Overlap area between two triangles
  double sq_distance = distance(triangle1, triangle2);
  decltype(auto) intR = CGAL::intersection(triangle1, triangle2);
  if (!intR){
    if(connected){
      // std::cout<<"Case 1"<<std::endl;
      return double(sq_distance);
    }
    else{
      return 0;
    }
  }
  if (const Triangle_2* s = boost::get<Triangle_2>(&*intR)){
    overlap_area = s->area();
  }
  else if(const Point* p = boost::get<Point>(&*intR)){
    overlap_area = 0;
  }
  else if(const Segment_2* s = boost::get<Segment_2>(&*intR)){
    overlap_area = 0;
  }
  else if(const std::vector<Point>* v = boost::get<std::vector<Point>>(&*intR)){
    int count = v->size();
    Polygon_2 p;
    for(int i = 0; i < count; i++){
      Point a = Point(v->at(i));
      p.push_back(a);
    }
    overlap_area = p.area();
  }
  return 10.00*double(overlap_area);
}

double sum_distance(std::vector<Triangle_2> triangles){
  int num_triangles = triangles.size();
  double total_area = 0;
  for (int i = 0; i < num_triangles; i++){
    for (int j = 0; j < num_triangles; j++){
      if (triangles.at(i) != triangles.at(j)){
        total_area = total_area + distance(triangles.at(i), triangles.at(j));
      }
    }
  }
  return total_area/2;
}

double calculator(std::vector<Triangle_2> triangles){
  
  int num_triangles = triangles.size();
  double total_area = 0;
  for (int i = 0; i < num_triangles; i++){
    for (int j = 0; j < num_triangles; j++){
      if (triangles.at(i) != triangles.at(j)){
        total_area = total_area + cost(triangles.at(i), triangles.at(j), true);
      }
    }
  }
  return total_area/2;
}


// '''
// "total_overlap" is currently doing double the work that it needs to do. Efficiency has to be increased here as the cases for the triangles grows, the time taken will grow. 
// '''

double total_overlap(std::vector<Triangle_2> triangles){
  int num_triangles = triangles.size();
  double total_area = 0;
  for (int i = 0; i < num_triangles; i++){
    for (int j = 0; j < num_triangles; j++){
      if (triangles.at(i) != triangles.at(j)){
        total_area = total_area + overlap(triangles.at(i), triangles.at(j));
      }
    }
  }
  return total_area/2;
}

std::vector<int> parent_randomizer(std::vector<double> weights){
  std::random_device rd;
  std::mt19937 gen(rd());
  std::discrete_distribution<> d(weights.begin(), weights.end());\
  int parent1 = d(gen);
  int parent2 = d(gen);
  std::vector<int> parentsID;
  parentsID.push_back(parent1);
  parentsID.push_back(parent2);
  return parentsID;
}