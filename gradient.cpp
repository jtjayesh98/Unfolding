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



std::vector<Transformation> rotational_gradients(){
  std::vector<Transformation> set;
  Transformation a21 = Transformation(Vector(0, 0), CGAL_PI/10000, 0);
  Transformation a22 = Transformation(Vector(0, 0), -CGAL_PI/10000, 0);

  set.push_back(a21);
  set.push_back(a22);
  return set;
}

std::vector<Transformation> translational_gradients(){
  std::vector<Transformation> set;
  Transformation a17 = Transformation(Vector(0.0001, 0), 0, 0);
  Transformation a18 = Transformation(Vector(0, 0.0001), 0, 0);
  Transformation a19 = Transformation(Vector(-0.0001, 0), 0, 0);
  Transformation a22 = Transformation(Vector(0, -0.0001), 0, 0);
  set.push_back(a17);
  set.push_back(a18);
  set.push_back(a19);
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
std::vector<Triangle> origin_triangles(std::vector<Triangle> triangles){
  std::vector<Triangle> retVal;
  for (int i = 0; i< triangles.size(); i++){
    retVal.push_back(triangles.at(i).origin_triangle());
  }
  return retVal;
}

double cost_calculator(std::vector<Triangle> triangles, std::vector<std::vector<double>> transformations){
  std::vector<Triangle_2> transformed_triangles;
  for (int i = 0; i < triangles.size(); i++){
    transformed_triangles.push_back(triangles.at(i).triangle.transform(Transformation(transformations.at(i)).transformation));
  }
  // std::cout<<calculator(transformed_triangles)<<std::endl;
  triangles = origin_triangles(triangles);
  return calculator(transformed_triangles);
}
std::vector<std::vector<double>> gradient2(std::vector<Triangle> triangles, std::vector<std::vector<double>> transformations){
  std::vector<std::vector<double>> retVal;
  double change = 0.01;
  double cost = cost_calculator(triangles, transformations);
  for (int i = 0; i < triangles.size(); i++){
    std::vector<double> row;
    for (int j = 0; j < 3; j++){
      double temp = transformations.at(i).at(j);
      transformations.at(i).at(j) = transformations.at(i).at(j) + change;
      double cost_change = cost_calculator(triangles, transformations) - cost;
      // std::cout<<cost_change<<std::endl;
      row.push_back(cost_change/change);
      transformations.at(i).at(j) = temp;

    }
    retVal.push_back(row);
    row.clear();
  }
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
  // double initial_cost = calculator(dual_set(triangles));
  std::vector<Triangle> retVal;
  Transformations gradients = gradient(triangles);
  for (int i = 0; i<triangles.size(); i++){
    retVal.push_back(gradients.transformations.at(i).scalar_multiplier(learning_rate).perform_transformation(triangles.at(i)));
  }
  return retVal;
}


std::vector<Triangle> single_move(std::vector<Triangle> triangles, Transformation move, int index, double learning_rate){
  std::vector<Triangle> retVal = triangles;
  retVal.at(index) = move.scalar_multiplier(learning_rate).perform_transformation(triangles.at(index));
  return retVal;
}

void test_run(std::vector<Triangle> triangles, int index, int iterations, double learning_rate){
  double initial_cost = calculator(dual_set(triangles));
  std::vector<Transformation> gradients = gradient_set();
  std::ofstream MyExcelFile;
  MyExcelFile.open("run1.csv");
  std::vector<std::vector<Triangle>> triangle_set;
  for (int i = 0; i < gradients.size(); i++){
    triangle_set.push_back(triangles);
  }
  // MyExcelFile << "T1P1, T1P2, T1P3, T1C, T1A, T2P1, T2P2, T2P3, T2C, T2A" << std::endl;
  for(int i = 0; i < iterations; i++){
    for (int j = 0; j < gradients.size(); j++){
      triangle_set.at(j) = single_move(triangle_set.at(j), gradients.at(j), index, learning_rate);
      MyExcelFile << calculator(dual_set(triangle_set.at(j)))<<",";
    }
    MyExcelFile<<std::endl;
    // if (calculator(dual_set(triangles)) < 0.0001*initial_cost){
    //   return;
    // }
  }
  MyExcelFile.close();
  return;
}

void rotation_run(std::vector<Triangle> triangles, int index, int iterations, double learning_rate){
  double initial_cost = calculator(dual_set(triangles));
  std::vector<Transformation> rotgrad = rotational_gradients();
  std::ofstream MyExcelFile;
  MyExcelFile.open("rot.csv");
  std::vector<std::vector<Triangle>> triangle_set;
  for (int i = 0; i < rotgrad.size(); i++){
    triangle_set.push_back(triangles);
  }
  // MyExcelFile << "T1P1, T1P2, T1P3, T1C, T1A, T2P1, T2P2, T2P3, T2C, T2A" << std::endl;
  for(int i = 0; i < iterations; i++){
    for (int j = 0; j < rotgrad.size(); j++){
      triangle_set.at(j) = single_move(triangle_set.at(j), rotgrad.at(j), index, learning_rate);
      MyExcelFile << calculator(dual_set(triangle_set.at(j)))<<",";
    }
    MyExcelFile<<std::endl;
    // if (calculator(dual_set(triangles)) < 0.0001*initial_cost){
    //   return;
    // }
  }
  MyExcelFile.close();
  return;
}


void translation_run(std::vector<Triangle> triangles, int index, int iterations, double learning_rate){
  double initial_cost = calculator(dual_set(triangles));
  std::vector<Transformation> transgrad = translational_gradients();
  std::ofstream MyExcelFile;
  MyExcelFile.open("trans.csv");
  std::vector<std::vector<Triangle>> triangle_set;
  for (int i = 0; i < transgrad.size(); i++){
    triangle_set.push_back(triangles);
  }
  // MyExcelFile << "T1P1, T1P2, T1P3, T1C, T1A, T2P1, T2P2, T2P3, T2C, T2A" << std::endl;
  for(int i = 0; i < iterations; i++){
    for (int j = 0; j < transgrad.size(); j++){
      triangle_set.at(j) = single_move(triangle_set.at(j), transgrad.at(j), index, learning_rate);
      MyExcelFile << calculator(dual_set(triangle_set.at(j)))<<",";
    }
    MyExcelFile<<std::endl;
    // if (calculator(dual_set(triangles)) < 0.0001*initial_cost){
    //   return;
    // }
  }
  MyExcelFile.close();
  return;
}

std::vector<Triangle> iterator(std::vector<Triangle> triangles, int iterations, double learning_rate){
  double initial_cost = calculator(dual_set(triangles));
  std::ofstream MyExcelFile;
  MyExcelFile.open("C:\\test.csv");

  MyExcelFile << "T1P1, T1P2, T1P3, T1C, T1A, T2P1, T2P2, T2P3, T2C, T2A" << std::endl;
  for(int i = 0; i < iterations; i++){
    triangles = gradient_move(triangles, learning_rate);
    for (int i = 0; i<triangles.size(); i++){
      if (i == triangles.size() - 1){
        MyExcelFile << triangles.at(i).p1 << ", "<< triangles.at(i).p2 << ", "<< triangles.at(i).p3 << ", "<< triangles.at(i).centroid << ", "<< triangles.at(i).area;
      }
      else{
        MyExcelFile << triangles.at(i).p1 << ", "<< triangles.at(i).p2 << ", "<< triangles.at(i).p3 << ", "<< triangles.at(i).centroid << ", "<< triangles.at(i).area<< ", ";
      }
      
    }
    MyExcelFile<<std::endl;
    if (calculator(dual_set(triangles)) < 0.0001*initial_cost){
      return triangles;
    }
  }
  MyExcelFile.close();
  return triangles;
}

void test_run2(std::vector<Triangle> triangles, int iterations, double learning_rate){
  double initial_cost = calculator(dual_set(triangles));
  std::ofstream MyExcelFile;
  MyExcelFile.open("C:\\test2.csv");

  for(int i = 0; i < iterations; i++){
    triangles = gradient_move(triangles, learning_rate);
    MyExcelFile << calculator(dual_set(triangles));
    MyExcelFile<<std::endl;
    if (calculator(dual_set(triangles)) < 0.0001*initial_cost){
      return;
    }
  }
  MyExcelFile.close();
  return;
}

std::vector<std::vector<double>> original_transformation(std::vector<Triangle> triangles){
  std::vector<std::vector<double>> retVal;
  for (int i = 0; i < triangles.size(); i++){
    retVal.push_back(triangles.at(i).transformation());
  }
  return retVal;
}



Transformations gradient_descent(std::vector<Triangle> triangles, Transformations transformations, double learning_rate, int iterations){
  std::vector<std::vector<double>> retVal;
  
  for (int i = 0; i < iterations; i++){
    
    std::vector<std::vector<double>> gradients = gradient2(triangles, transformations.vec_transformations);
    
    transformations = transformations.sub_vector(gradients, learning_rate);
    // std::cout<<1<<std::endl;
    std::cout<<cost_calculator(triangles, transformations.vec_transformations)<<std::endl;
  }  
  return transformations;
}

double norm2change(std::vector<std::vector<double>> original, std::vector<std::vector<double>> transformed){
  double sum = 0;
  for (int i = 0; i < original.size(); i++){
    for (int j = 0; j < 3; j++){
      sum = sum + pow(original.at(i).at(j) - transformed.at(i).at(j), 2.00);
    }
  }
  return double(sqrt(sum));
}

int main()
{
  Triangle test = Triangle(Point(0, 0), Point(0, 1), Point(1, 1));
  Triangle pgn1 = Triangle(Point(0,10), Point(4,10), Point(2, 2));
  Triangle pgn2 = Triangle(Point(0,0), Point(4,0), Point(2, 3));
  Triangle pgn3 = Triangle(Point(0,0), Point(0,10), Point(5, 5));
  Triangle pgn4 = Triangle(Point(1,1), Point(6,7), Point(2,3));
  Triangle pgn5 = Triangle(Point(-1, -3), Point(-4, -5), Point(0,1));

  std::vector<Triangle> triangles;
  triangles.push_back(test);
  triangles.push_back(pgn1);
  triangles.push_back(pgn2);
  triangles.push_back(pgn3);
  triangles.push_back(pgn4);
  triangles.push_back(pgn5);


  std::vector<std::vector<double>> transformations = original_transformation(triangles);
  
  triangles = origin_triangles(triangles);
  
  Transformations transformations1 = Transformations(transformations);
  
  transformations1 = gradient_descent(triangles, transformations1, 0.001, 10000);

  std::cout<<norm2change(transformations, transformations1.vec_transformations)<<std::endl;



  return 1;
}