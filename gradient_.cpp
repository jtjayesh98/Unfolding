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
// #include "Triangles.cpp"
// #include <GL/freeglut.h>
// #include <GL/glu.h>
// #include "animations.cpp"

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



double spatial_learningrate(Triangle triangle, double max_dist, Point center, double iter_factor, double learning_rate){
  double dist = sq_dist(triangle.centroid, center);
  double retVal;
  if (dist < 0.5*max_dist){
    retVal = 0.1*iter_factor*learning_rate;
  }
  else{
    retVal = iter_factor*learning_rate;
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


std::vector<Triangle> origin_triangles(std::vector<Triangle> triangles){
  std::vector<Triangle> retVal;
  for (int i = 0; i< triangles.size(); i++){
    retVal.push_back(triangles.at(i).origin_triangle());
  }
  return retVal;
}
double cost_calculator(Triangles triangles, std::vector<std::vector<double>> transformations, std::vector<std::vector<double>> original_transformation){
  std::vector<Triangle> transformed_triangles;
  
  for (int i = 0; i < triangles.triangles.size(); i++){
    transformed_triangles.push_back(Triangle(triangles.triangles.at(i).triangle.transform(Transformation(transformations.at(i)).transformation)));
  }
  Triangles cost_triangles = Triangles(transformed_triangles, triangles.edges);
  // std::cout<<calculator(transformed_triangles)<<std::endl;
  triangles = origin_triangles(triangles.triangles);
  double edge_cost = calc_edge(cost_triangles);
  double transformation_cost = norm2change(original_transformation, transformations);
  return edge_cost+transformation_cost;
}

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

std::vector<std::vector<double>> gradient2(Triangles triangles, std::vector<std::vector<double>> transformations, std::vector<std::vector<double>> original_transformation){
  std::vector<std::vector<double>> retVal;
  double change = 0.01;
  double cost = cost_calculator(triangles, transformations, original_transformation);
  for (int i = 0; i < triangles.triangles.size(); i++){
    std::vector<double> row;
    for (int j = 0; j < 3; j++){
      double temp = transformations.at(i).at(j);
      transformations.at(i).at(j) = transformations.at(i).at(j) + change;
      double cost_change = cost_calculator(triangles, transformations, original_transformation) - cost;
      // std::cout<<cost_change<<std::endl;
      row.push_back(cost_change/change);
      transformations.at(i).at(j) = temp;
    }
    retVal.push_back(row);
    row.clear();
  }
  return retVal;
}

// std::vector<std::vector<double>> gradient2(Triangles triangles, std::vector<std::vector<double>> )

std::vector<std::vector<double>> original_transformation(std::vector<Triangle> triangles){
  std::vector<std::vector<double>> retVal;
  for (int i = 0; i < triangles.size(); i++){
    retVal.push_back(triangles.at(i).transformation());
  }
  return retVal;
}

Transformations gradient_descent(std::vector<Triangle> triangles, Transformations transformations, double learning_rate, int iterations){
  std::vector<std::vector<double>> retVal;
  std::vector<std::vector<double>> original_transformation = transformations.vec_transformations;
  for (int i = 0; i < iterations; i++){
    
    std::vector<std::vector<double>> gradients = gradient2(triangles, transformations.vec_transformations, original_transformation);
    
    transformations = transformations.sub_vector(gradients, learning_rate);
    // std::cout<<1<<std::endl;
    // std::cout<<cost_calculator(triangles, transformations.vec_transformations)<<std::endl;
  }  
  return transformations;
}

std::vector<Triangle> test_run2(std::vector<Triangle> triangles, Transformations &transformations, double learning_rate, int iterations){
  double initial_cost = calculator(dual_set(triangles));
  std::ofstream MyExcelFile;
  std::vector<std::vector<double>> original_transformation = transformations.vec_transformations;
  MyExcelFile.open("C:\\test2.csv");
  std::vector<Triangle> triangles1 = triangles;
  for (int i = 0; i < iterations; i++){
    
    std::vector<std::vector<double>> gradients = gradient2(triangles, transformations.vec_transformations, original_transformation);
    
    transformations = transformations.sub_vector(gradients, learning_rate);
    triangles1 = transformations.perform_transformations(triangles1);
    for (int j = 0; j < triangles.size(); j++){
      MyExcelFile<<triangles1.at(j).p1<<","<<triangles1.at(j).p2<<","<<triangles1.at(j).p3;
      MyExcelFile<<","<<" "<<",";
    }
    triangles1 = triangles;
    // std::cout<<cost_calculator(Triangles(triangles), transformations.vec_transformations)<<std::endl;
    MyExcelFile<<std::endl;
    // std::cout<<1<<std::endl;
    // std::cout<<cost_calculator(triangles, transformations.vec_transformations)<<std::endl;
  }
  
  MyExcelFile.close();
  return triangles1;
}


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
    std::vector<std::vector<double>> gradients = gradient2(triangles, transformations.vec_transformations, original_transformation);
    
    transformations = transformations.sub_vector(gradients, learning_rate);
    triangles1 = transformations.perform_transformations(triangles1);
    for (int j = 0; j < triangles.size(); j++){
      MyExcelFile<<triangles1.at(j).p1[0]/10<<","<<triangles1.at(j).p1[1]/10<<","<<triangles1.at(j).p2[0]/10<<","<<triangles1.at(j).p2[1]/10<<","<<triangles1.at(j).p3[0]/10<<","<<triangles1.at(j).p3[1]/10<<",";
    }
    // MyExcelFile<<triangles1.at(0).p1[0]/10<<","<<triangles1.at(0).p1[1]/10<<","<<triangles1.at(0).p2[0]/10<<","<<triangles1.at(0).p2[1]/10<<","<<triangles1.at(0).p3[0]/10<<","<<triangles1.at(0).p3[1]/10;
    triangles1 = triangles;
    // std::cout<<cost_calculator(triangles, transformations.vec_transformations)<<std::endl;
    MyExcelFile<<std::endl;
    std::cout<<1<<std::endl;
    std::cout<<cost_calculator(triangles, transformations.vec_transformations, original_transformation)<<std::endl;
    if (cost_calculator(triangles, transformations.vec_transformations, original_transformation) < 0.00001*initial_cost){ 
      return;
    }
  }
  
  // MyExcelFile.close();
  return;
}

void run(Triangles triangles, Transformations &transformations, double learning_rate, int iterations){
  double initial_cost = calc_edge(triangles);
  std::vector<Triangle> triangles1 = triangles.triangles;
  std::ofstream MyExcelFile;
  std::vector<std::vector<double>> original_transformation = transformations.vec_transformations;
  MyExcelFile.open("triangle0.csv");
  for (int i = 0; i < iterations; i++){
    if (i == iterations*0.5){
      learning_rate = learning_rate*0.9;
    }
    std::vector<std::vector<double>> gradients = gradient2(triangles.triangles, transformations.vec_transformations, original_transformation);  
    transformations = transformations.sub_vector(gradients, learning_rate);
    triangles1 = transformations.perform_transformations(triangles.triangles);
    for (int j = 0; j < triangles.triangles.size(); j++){
      MyExcelFile<<triangles1.at(j).p1[0]/10<<","<<triangles1.at(j).p1[1]/10<<","<<triangles1.at(j).p2[0]/10<<","<<triangles1.at(j).p2[1]/10<<","<<triangles1.at(j).p3[0]/10<<","<<triangles1.at(j).p3[1]/10<<",";
    }
    triangles1 = triangles.triangles;
    if(cost_calculator(triangles, transformations.vec_transformations, original_transformation) < initial_cost*0.00001){
      MyExcelFile.close();
      return;
    }
    MyExcelFile<<std::endl;
    std::cout<<cost_calculator(triangles, transformations.vec_transformations, original_transformation)<<std::endl;
  }
  MyExcelFile.close();
}





void populations_writer(Population population){
  std::ofstream MyFile;
  MyFile.open("population.csv");
  for (int i = 0; i < population.transformations.size(); i++){
    for (int j = 0; j < population.transformations.at(i).vec_transformations.size(); j++){
      for (int k = 0; k < 3; k ++){
        MyFile << population.transformations.at(i).vec_transformations.at(j).at(k)<<",";
      }
    }
    MyFile<<std::endl;
  }
  MyFile.close();
}

Population populator(Triangles triangles, int population_size){
  std::vector<Triangle> triangles1 = triangles.triangles;
  Transformations run_transformations = Transformations(original_transformation(triangles.triangles));
  triangles = Triangles(origin_triangles(triangles.triangles));
  std::ofstream MyExcelFile;
  MyExcelFile.open("population.csv");
  Population retVal = Population(triangles.triangles.size()*3, population_size);
  for (int i = 0; i < population_size; i++){
    Transformations transformations1 = run_transformations;
    test_run3(triangles1, transformations1, 0.0002, 1);    
    retVal.add_transformations(transformations1);
    // std::cout<<cost_calculator(triangles, transformations1.vec_transformations)<<std::endl;
  }
  populations_writer(retVal);
  return retVal;
}


//  ADD ANY UNCOMMENTED SECTION HERE, MAINTAINING THE ORDER

int main(int argc, char** argv)
{

  
  // return 0;
  // std::ofstream transferData;
  // transferData.open("triangle0.csv");

  Triangle test = Triangle(Point(0, 0), Point(0, 1), Point(1, 1));
  Triangle pgn1 = Triangle(Point(0,10), Point(4,10), Point(2, 2));
  Triangle pgn2 = Triangle(Point(0,0), Point(4,0), Point(2, 3));
  Triangle pgn3 = Triangle(Point(0,0), Point(0,10), Point(5, 5));
  Triangle pgn4 = Triangle(Point(1,1), Point(6,7), Point(2,3));
  Triangle pgn5 = Triangle(Point(-1, -3), Point(-4, -5), Point(0,1));
  Triangle pgn6 = Triangle(Point(1, 3), Point(-4, -5), Point(0,1));
  Triangle pgn7 = Triangle(Point(0,0), Point(5,5), Point(0,5));
  std::vector<Triangle> triangles_list = off_reader("Test_Mesh.off");
  Triangles triangles = Triangles(triangles_list);
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
  
  triangles = Triangles(origin_triangles(triangles.triangles));
  Transformations transformations1 = Transformations(transformations);
  
  
  
  test_run3(triangles, transformations1, 0.0002, 100);
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



// The following functions were created for an earlier run with a different interpretation of the problem. It works but has been commented out for clarity of the problem.


// std::vector<Transformation> gradient_set(){
//   std::vector<Transformation> set;
//   Transformation a2 = Transformation(Vector(0.0001, 0), CGAL_PI/10000, 0);
//   Transformation a1 = Transformation(Vector(0.0001, 0), CGAL_PI/10000, 1);
//   Transformation a3 = Transformation(Vector(0.0001, 0), -CGAL_PI/10000, 0);
//   Transformation a4 = Transformation(Vector(0.0001, 0), -CGAL_PI/10000, 1);
//   Transformation a5 = Transformation(Vector(0, 0.0001), CGAL_PI/10000, 0);
//   Transformation a6 = Transformation(Vector(0, 0.0001), CGAL_PI/10000, 1);
//   Transformation a7 = Transformation(Vector(0, 0.0001), -CGAL_PI/10000, 0);
//   Transformation a8 = Transformation(Vector(0, 0.0001), -CGAL_PI/10000, 1);
//   Transformation a9 = Transformation(Vector(-0.0001, 0), CGAL_PI/10000, 0);
//   Transformation a10 = Transformation(Vector(-0.0001, 0), CGAL_PI/10000, 1);
//   Transformation a11 = Transformation(Vector(-0.0001, 0), -CGAL_PI/10000, 0);
//   Transformation a12 = Transformation(Vector(-0.0001, 0), -CGAL_PI/10000, 1);
//   Transformation a13 = Transformation(Vector(0, -0.0001), CGAL_PI/10000, 0);
//   Transformation a14 = Transformation(Vector(0, -0.0001), CGAL_PI/10000, 1);
//   Transformation a15 = Transformation(Vector(0, -0.0001), -CGAL_PI/10000, 0);
//   Transformation a16 = Transformation(Vector(0, -0.0001), -CGAL_PI/10000, 1);
//   Transformation a17 = Transformation(Vector(0.0001, 0), 0, 0);
//   Transformation a18 = Transformation(Vector(0, 0.0001), 0, 0);
//   Transformation a19 = Transformation(Vector(-0.0001, 0), 0, 0);
//   Transformation a22 = Transformation(Vector(0, -0.0001), 0, 0);
//   Transformation a21 = Transformation(Vector(0, 0), CGAL_PI/10000, 0);
//   Transformation a20 = Transformation(Vector(0, 0), -CGAL_PI/10000, 0);

//   set.push_back(a1);
//   set.push_back(a2);
//   set.push_back(a3);
//   set.push_back(a4);
//   set.push_back(a5);
//   set.push_back(a6);
//   set.push_back(a7);
//   set.push_back(a8);
//   set.push_back(a9);
//   set.push_back(a10);
//   set.push_back(a11);
//   set.push_back(a12);
//   set.push_back(a13);
//   set.push_back(a14);
//   set.push_back(a15);
//   set.push_back(a16);
//   set.push_back(a17);
//   set.push_back(a18);
//   set.push_back(a19);
//   set.push_back(a20);
//   set.push_back(a21);
//   set.push_back(a22);
//   return set;
// }



// std::vector<Transformation> rotational_gradients(){
//   std::vector<Transformation> set;
//   Transformation a21 = Transformation(Vector(0, 0), CGAL_PI/10000, 0);
//   Transformation a22 = Transformation(Vector(0, 0), -CGAL_PI/10000, 0);

//   set.push_back(a21);
//   set.push_back(a22);
//   return set;
// }

// std::vector<Transformation> translational_gradients(){
//   std::vector<Transformation> set;
//   Transformation a17 = Transformation(Vector(0.0001, 0), 0, 0);
//   Transformation a18 = Transformation(Vector(0, 0.0001), 0, 0);
//   Transformation a19 = Transformation(Vector(-0.0001, 0), 0, 0);
//   Transformation a22 = Transformation(Vector(0, -0.0001), 0, 0);
//   set.push_back(a17);
//   set.push_back(a18);
//   set.push_back(a19);
//   set.push_back(a22);
//   return set;
// }

// double move_cost(int index, std::vector<Triangle> triangles, Transformation transformation){
//   double org_cost = calculator(dual_set(triangles));
//   triangles.at(index) = transformation.perform_transformation(triangles.at(index));
//   double new_cost = calculator(dual_set(triangles));
//   // std::cout<<(new_cost - org_cost)<<std::endl;
//   return new_cost - org_cost;
// }
// double cost_calculator(std::vector<Triangle> triangles, std::vector<std::vector<double>> transformations){
//   std::vector<Triangle_2> transformed_triangles;
//   for (int i = 0; i < triangles.size(); i++){
//     transformed_triangles.push_back(triangles.at(i).triangle.transform(Transformation(transformations.at(i)).transformation));
//   }
//   // std::cout<<calculator(transformed_triangles)<<std::endl;
//   triangles = origin_triangles(triangles);
//   return calculator(transformed_triangles);
// }
// Transformations gradient(std::vector<Triangle> triangles){
//   std::vector<Transformation> gradients;
//   std::vector<Transformation> grad_move = gradient_set();
  
//   // std::cout<<grad_move.size()<<std::endl;
//   for (int i = 0; i < triangles.size(); i++){
//     double min_cost = 9999999999;
//     int index = 0;
//     for (int j = 0; j< grad_move.size(); j++){
//       double cost = move_cost(i, triangles, grad_move.at(j));
//       if (cost < min_cost){
//         min_cost = cost;
//         index = j;
//         // std::cout<<index<<std::endl;
//       }
//       // std::cout<<std::endl;
//     }
//     gradients.push_back(grad_move.at(index));
//     // std::cout<<std::endl;    
//   }
//   Transformations retVal = Transformations(gradients);
//   return retVal;
// }




// // double test_cost(std::vector<)

// double test_cost(std::vector<Triangle> triangles, int index_, int trindex){
//   std::vector<double> gradients;
//   std::vector<Transformation> grad_move = gradient_set();
//   double cost = move_cost(trindex, triangles, grad_move.at(index_));
//   return cost;
// }

// std::vector<Triangle> gradient_move(std::vector<Triangle> triangles, double learning_rate){
//   // double initial_cost = calculator(dual_set(triangles));
//   std::vector<Triangle> retVal;
//   Transformations gradients = gradient(triangles);
//   for (int i = 0; i<triangles.size(); i++){
//     retVal.push_back(gradients.transformations.at(i).scalar_multiplier(learning_rate).perform_transformation(triangles.at(i)));
//   }
//   return retVal;
// }


// std::vector<Triangle> single_move(std::vector<Triangle> triangles, Transformation move, int index, double learning_rate){
//   std::vector<Triangle> retVal = triangles;
//   retVal.at(index) = move.scalar_multiplier(learning_rate).perform_transformation(triangles.at(index));
//   return retVal;
// }

// void test_run(std::vector<Triangle> triangles, int index, int iterations, double learning_rate){
//   double initial_cost = calculator(dual_set(triangles));
//   std::vector<Transformation> gradients = gradient_set();
//   std::ofstream MyExcelFile;
//   MyExcelFile.open("run1.csv");
//   std::vector<std::vector<Triangle>> triangle_set;
//   for (int i = 0; i < gradients.size(); i++){
//     triangle_set.push_back(triangles);
//   }
//   // MyExcelFile << "T1P1, T1P2, T1P3, T1C, T1A, T2P1, T2P2, T2P3, T2C, T2A" << std::endl;
//   for(int i = 0; i < iterations; i++){
//     for (int j = 0; j < gradients.size(); j++){
//       triangle_set.at(j) = single_move(triangle_set.at(j), gradients.at(j), index, learning_rate);
//       MyExcelFile << calculator(dual_set(triangle_set.at(j)))<<",";
//     }
//     MyExcelFile<<std::endl;
//     // if (calculator(dual_set(triangles)) < 0.0001*initial_cost){
//     //   return;
//     // }
//   }
//   MyExcelFile.close();
//   return;
// }

// void rotation_run(std::vector<Triangle> triangles, int index, int iterations, double learning_rate){
//   double initial_cost = calculator(dual_set(triangles));
//   std::vector<Transformation> rotgrad = rotational_gradients();
//   std::ofstream MyExcelFile;
//   MyExcelFile.open("rot.csv");
//   std::vector<std::vector<Triangle>> triangle_set;
//   for (int i = 0; i < rotgrad.size(); i++){
//     triangle_set.push_back(triangles);
//   }
//   // MyExcelFile << "T1P1, T1P2, T1P3, T1C, T1A, T2P1, T2P2, T2P3, T2C, T2A" << std::endl;
//   for(int i = 0; i < iterations; i++){
//     for (int j = 0; j < rotgrad.size(); j++){
//       triangle_set.at(j) = single_move(triangle_set.at(j), rotgrad.at(j), index, learning_rate);
//       MyExcelFile << calculator(dual_set(triangle_set.at(j)))<<",";
//     }
//     MyExcelFile<<std::endl;
//     // if (calculator(dual_set(triangles)) < 0.0001*initial_cost){
//     //   return;
//     // }
//   }
//   MyExcelFile.close();
//   return;
// }

// void translation_run(std::vector<Triangle> triangles, int index, int iterations, double learning_rate){
//   double initial_cost = calculator(dual_set(triangles));
//   std::vector<Transformation> transgrad = translational_gradients();
//   std::ofstream MyExcelFile;
//   MyExcelFile.open("trans.csv");
//   std::vector<std::vector<Triangle>> triangle_set;
//   for (int i = 0; i < transgrad.size(); i++){
//     triangle_set.push_back(triangles);
//   }
//   // MyExcelFile << "T1P1, T1P2, T1P3, T1C, T1A, T2P1, T2P2, T2P3, T2C, T2A" << std::endl;
//   for(int i = 0; i < iterations; i++){
//     for (int j = 0; j < transgrad.size(); j++){
//       triangle_set.at(j) = single_move(triangle_set.at(j), transgrad.at(j), index, learning_rate);
//       MyExcelFile << calculator(dual_set(triangle_set.at(j)))<<",";
//     }
//     MyExcelFile<<std::endl;
//     // if (calculator(dual_set(triangles)) < 0.0001*initial_cost){
//     //   return;
//     // }
//   }
//   MyExcelFile.close();
//   return;
// }

// std::vector<Triangle> iterator(std::vector<Triangle> triangles, int iterations, double learning_rate){
//   double initial_cost = calculator(dual_set(triangles));
//   std::ofstream MyExcelFile;
//   MyExcelFile.open("C:\\test.csv");

//   MyExcelFile << "T1P1, T1P2, T1P3, T1C, T1A, T2P1, T2P2, T2P3, T2C, T2A" << std::endl;
//   for(int i = 0; i < iterations; i++){
//     triangles = gradient_move(triangles, learning_rate);
//     for (int i = 0; i<triangles.size(); i++){
//       if (i == triangles.size() - 1){
//         MyExcelFile << triangles.at(i).p1 << ", "<< triangles.at(i).p2 << ", "<< triangles.at(i).p3 << ", "<< triangles.at(i).centroid << ", "<< triangles.at(i).area;
//       }
//       else{
//         MyExcelFile << triangles.at(i).p1 << ", "<< triangles.at(i).p2 << ", "<< triangles.at(i).p3 << ", "<< triangles.at(i).centroid << ", "<< triangles.at(i).area<< ", ";
//       }
      
//     }
//     MyExcelFile<<std::endl;
//     if (calculator(dual_set(triangles)) < 0.0001*initial_cost){
//       return triangles;
//     }
//   }
//   MyExcelFile.close();
//   return triangles;
// }

// void test_run2(std::vector<Triangle> triangles, int iterations, double learning_rate){
//   double initial_cost = calculator(dual_set(triangles));
//   std::ofstream MyExcelFile;
//   MyExcelFile.open("C:\\test2.csv");

//   for(int i = 0; i < iterations; i++){
//     triangles = gradient_move(triangles, learning_rate);
//     MyExcelFile << calculator(dual_set(triangles));
//     MyExcelFile<<std::endl;
//     if (calculator(dual_set(triangles)) < 0.0001*initial_cost){
//       return;
//     }
//   }
//   MyExcelFile.close();
//   return;
// }
// 

