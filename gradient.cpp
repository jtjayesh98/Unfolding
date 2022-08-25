#include <CGAL/Exact_predicates_inexact_constructions_kernel.h>
#include <CGAL/Polygon_2.h>
#include <CGAL/Surface_mesh.h>
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
#include <CGAL/Surface_mesh/IO/OFF.h>
#include <CGAL/Surface_mesh/Surface_mesh.h>
#include <cstdlib>
#include <CGAL/draw_surface_mesh.h>


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
typedef CGAL::Vector_3<K> Vector_3;
typedef CGAL::Direction_2<K> Direction;
// typedef CGAL::Surface_mesh<Point> Surface_mesh;

typedef CGAL::Surface_mesh<K::Point_3> Surface_mesh;

using std::cout; using std::endl;


#include "Population.cpp"

// Moving all triangles to the origin
std::vector<Triangle> origin_triangles(std::vector<Triangle> triangles){
  std::vector<Triangle> retVal;
  for (int i = 0; i< triangles.size(); i++){
    retVal.push_back(triangles.at(i).origin_triangle());
  }
  return retVal;
}

double norm2change(std::vector<std::vector<double>> original, std::vector<std::vector<double>> transformed){
  double sum = 0;
  for (int i = 0; i < original.size(); i++){
    for (int j = 0; j < 3; j++){
      if (j == 0 || j==1){
        sum = sum + abs(original.at(i).at(j) - transformed.at(i).at(j));
      }
      else{
        sum = sum + 10*abs(original.at(i).at(j) - transformed.at(i).at(j));
      }
      // sum = sum + pow(original.at(i).at(j) - transformed.at(i).at(j), 2.00);
    }
  }
  return sum;
}

double calc_edge(Triangles triangles){
  int num_triangles = triangles.triangles.size();
  double total_area = 0;
  for (int i = 0; i < num_triangles; i++){
    for (int j = 0; j < num_triangles; j++){
      if (triangles.triangles.at(i).triangle != triangles.triangles.at(j).triangle){
        total_area = total_area + cost(triangles.triangles.at(i).triangle, triangles.triangles.at(j).triangle, triangles.isedge(triangles.triangles.at(i), triangles.triangles.at(j)));
      }
    }
  }
  return total_area/2;
}

double cost_calculator(Triangles triangles, std::vector<std::vector<double>> transformations, std::vector<std::vector<double>> original_transformation){
  std::vector<Triangle> transformed_triangles;
  
  for (int i = 0; i < triangles.triangles.size(); i++){
    transformed_triangles.push_back(Triangle(triangles.triangles.at(i).triangle.transform(Transformation(transformations.at(i)).transformation)));
  }
  Triangles cost_triangles = Triangles(transformed_triangles, triangles.edges, false);
  // std::cout<<calculator(transformed_triangles)<<std::endl;
  triangles = Triangles(origin_triangles(triangles.triangles), false);
  double edge_cost = calc_edge(cost_triangles);
  double transformation_cost = norm2change(original_transformation, transformations);
  return edge_cost+transformation_cost;
}


double spatial_learningrate(std::vector<double> transformation, double max_dist, double iter_factor, double learning_rate){
  // std::cout<<triangle.centroid[0]<<std::endl;Tut03CPUPositionOffset
  double dist = sqrt(pow(transformation[1], 2)+ pow(transformation[0], 2));
  // std::cout<<dist<<std::endl;
  double retVal;
  if (dist < 0.5*max_dist){
    retVal = 0.5*(iter_factor)*learning_rate;
  }
  else{
    retVal = (iter_factor)*learning_rate;
  }
  return retVal;
}


std::vector<std::vector<double>> gradient2(Triangles triangles, std::vector<std::vector<double>> transformations, std::vector<std::vector<double>> original_transformation, double iter_factor, double learningrate){
  std::vector<std::vector<double>> retVal;
  double change = 0.01;
  double element = 0;
  double maxd = maxdist(triangles.triangles);
  // std::cout<<maxd<<std::endl;
  double cost = cost_calculator(triangles, transformations, original_transformation);
  for (int i = 0; i < triangles.triangles.size(); i++){
    std::vector<double> row;
    for (int j = 0; j < 3; j++){
      double temp = transformations.at(i).at(j);
      transformations.at(i).at(j) = transformations.at(i).at(j) + change;
      double cost_change = cost_calculator(triangles, transformations, original_transformation) - cost;
      if (cost_change > 1){
        cost_change = 0;
      }
      element = spatial_learningrate(transformations.at(i), maxd, iter_factor, learningrate)*cost_change/change;
      if (j == 2){
        element = element * 0.1;
      }
      row.push_back(element);
      transformations.at(i).at(j) = temp;
    }
    retVal.push_back(row);
    row.clear();
  }
  return retVal;
}


// READING FUNCTION
std::vector<Triangle> off_reader(std::string filename){
  std::vector<Point> points;
  std::vector<Triangle> retVal;
  std::ifstream myfile;
  myfile.open(filename);
  std::string read_string;
  myfile>>read_string;
  int num_points = stoi(read_string);
  myfile>>read_string;
  int triangles = stoi(read_string);
  myfile>>read_string;
  for(int i = 0; i < num_points; i++){
    myfile>>read_string;
    double px = stod(read_string);
    myfile>>read_string;
    double py = stod(read_string);
    myfile>>read_string;
    double pz = stod(read_string);
    points.push_back(Point(px, py));
  }
  for (int i = 0; i < triangles; i++){
    myfile>>read_string;
    myfile>>read_string;
    int p1_index = stoi(read_string);
    myfile>>read_string;
    int p2_index = stoi(read_string);
    myfile>>read_string;
    int p3_index = stoi(read_string);
    retVal.push_back(Triangle(points.at(p1_index), points.at(p2_index), points.at(p3_index)));
  }

  myfile.close();
  return retVal;
}


// Finding the original transformations
std::vector<std::vector<double>> original_transformation(std::vector<Triangle> triangles){
  std::vector<std::vector<double>> retVal;
  for (int i = 0; i < triangles.size(); i++){
    retVal.push_back(triangles.at(i).transformation());
  }
  return retVal;
}




// Iterator

void test_run3(Triangles triangles_, Transformations &transformations, double learning_rate, int iterations){
  std::vector<Triangle> triangles = triangles_.triangles;
  Point center = triangles_.center;
  double mdist = maxdist(triangles);
  double initial_cost = calculator(dual_set(triangles));
  std::vector<std::vector<double>> original_transformation = transformations.vec_transformations;
  std::ofstream MyExcelFile;
  MyExcelFile.open("triangle0.csv");
  std::vector<Triangle> triangles1 = triangles;
  for (int i = 0; i < iterations; i++){
    if (i == iterations*0.5){
      learning_rate = learning_rate*0.9;
    }
    std::vector<std::vector<double>> gradients = gradient2(triangles_, transformations.vec_transformations, original_transformation, iterations-i/iterations, learning_rate);
    
    transformations = transformations.sub_vector(gradients, learning_rate);
    triangles1 = transformations.perform_transformations(triangles1);
    for (int j = 0; j < triangles.size(); j++){
      MyExcelFile<<triangles1.at(j).p1[0]/10<<","<<triangles1.at(j).p1[1]/10<<","<<triangles1.at(j).p2[0]/10<<","<<triangles1.at(j).p2[1]/10<<","<<triangles1.at(j).p3[0]/10<<","<<triangles1.at(j).p3[1]/10<<",";
    }
    // MyExcelFile<<triangles1.at(0).p1[0]/10<<","<<triangles1.at(0).p1[1]/10<<","<<triangles1.at(0).p2[0]/10<<","<<triangles1.at(0).p2[1]/10<<","<<triangles1.at(0).p3[0]/10<<","<<triangles1.at(0).p3[1]/10;
    triangles1 = triangles;
    // std::cout<<cost_calculator(triangles, transformations.vec_transformations)<<std::endl;
    MyExcelFile<<std::endl;
    // std::cout<<1<<std::endl;
    std::cout<<cost_calculator(triangles_, transformations.vec_transformations, original_transformation)<<std::endl;
    if (cost_calculator(triangles_, transformations.vec_transformations, original_transformation) < 0.00001*initial_cost){ 
      return;
    }
  }
  
  // MyExcelFile.close();
  return;
}


int main(int argc, char** argv)
{
  Triangle pgn2 = Triangle(Point(0,0), Point(4,0), Point(2, 3));
  Triangle pgn3 = Triangle(Point(0,0), Point(0,10), Point(5, 5));
  Triangle pgn4 = Triangle(Point(1,1), Point(6,7), Point(2,3));
  Triangle pgn5 = Triangle(Point(-1, -3), Point(-4, -5), Point(0,1));
  Triangle pgn6 = Triangle(Point(1, 3), Point(-4, -5), Point(0,1));
  Triangle pgn7 = Triangle(Point(0,0), Point(5,5), Point(0,5));
  std::vector<Triangle> triangles_list = off_reader("Test_Mesh.off");
  Triangles triangles = Triangles(triangles_list, true);
  std::cout<<triangles.triangles_center(triangles_list)<<std::endl;
  // std::vector<Triangle> triangles;
  // std::cout<<triangles.size()<<std::endl;
  // triangles.push_back(test);
  // triangles.push_back(pgn1);
  // triangles.push_back(pgn2);
  // triangles.push_back(pgn3);
  // triangles.push_back(pgn4);
  // triangles.push_back(pgn5);
  // triangles.push_back(pgn6);
  // triangles.push_back(pgn7);
  
  // Population pop = populator(triangles, 1);


  std::vector<std::vector<double>> transformations = original_transformation(triangles.triangles);
  
  triangles = Triangles(origin_triangles(triangles.triangles), false);
  Transformations transformations1 = Transformations(transformations);
  
  
  
  test_run3(triangles, transformations1, 0.0002, 40);
  std::cout<<std::endl;
  // output = test_run3(triangles, transformations1,0.0003, 100);
  // output = test_run3(triangles, transformations1, 0.0001, 100);

  // transformations1 = gradient_descent(triangles, transformations1, 0.0001, 10000);
  // output = transformations1.perform_transformations(triangles);
  // for (int i = 0; i < triangles.size(); i++){
  //   transferData<<output.at(i).p1[0]/10<<","<<output.at(i).p1[1]/10<<","<<output.at(i).p2[0]/10<<","<<output.at(i).p2[1]/10<<","<<output.at(i).p3[0]/10<<","<<output.at(i).p3[1]/10<<std::endl;
  // }

  // std::cout<<norm2change(transformations, transformations1.vec_transformations)<<std::endl;



  return 0;
}