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
#include "functions.cpp"
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


class Transformation{
  public:
    int type = 0;
    Aff_transformation transformation;
    Vector translation_vector;
    Transformation(Vector vector_){
      translation_vector = vector_;
      transformation = Aff_transformation(CGAL::TRANSLATION, vector_);
    }
    Aff_transformation retTransform();
    void mutate();
    Vector retVector();
};

Transformation random_transformation(){
    int type = rand()%2;
    double x_move = (rand()%100);
    double y_move = (rand()%100);
    Transformation a = Transformation(Vector(x_move/100, y_move/100));
    return a;
}

Aff_transformation Transformation::retTransform(){
  return this->transformation;
}

Vector Transformation::retVector(){
  return this->translation_vector;
}

void Transformation::mutate(){
  Vector vec = this->translation_vector;
  double x;
  double y;
  
  x = vec.hx();
  y = vec.hy();
  if (rand()%4 == 0){
    x = x*110/100;
    y = y*110/100;
  }
  else if (rand()%4 == 1){
    x = x*90/100;
    y = y*110/100;
  }
  else if (rand()%4 == 2){
    x = x*110/100;
    y = y*90/100;
  }
  else if(rand()%4 == 3){
    x = x*90/100;
    y = y*90/100;
  }
  Aff_transformation new_transformation = Aff_transformation(CGAL::TRANSLATION, Vector(x, y));
  this->translation_vector = Vector(x, y); 
  this->transformation = new_transformation;
}

class Transformations{
    public:
        int num_obj;
        std::vector<Transformation> transformations;
        double score;
        void perform_transformations(std::vector<Triangle_2> &triangles);
        double calculate_scores(std::vector<Triangle_2> &triangles);
        double calculate_scores2(std::vector<Triangle_2> &triangles);  
        Transformations(std::vector<Transformation> transformations_){
          transformations = transformations_;
          num_obj = transformations_.size();
        }
        Transformations update_score(double score_);
        void mutator();
};

void Transformations::mutator(){
  // std::vector<Transformation> transformations = this->transformations;
  for (int i = 0; i < this->transformations.size(); i++){
    this->transformations.at(i).mutate();
  }
}



void Transformations::perform_transformations(std::vector<Triangle_2> &triangles){
    num_obj = this->num_obj;
    for (int i = 0; i < num_obj; i++){
        triangles.at(i) = triangles.at(i).transform(this->transformations.at(i).retTransform());
    }
}

Transformations Transformations::update_score(double score_){
    this->score = score_;
    return *this;
}







class Population{
    public:
      int num_obj;
      int pop_size;
      std::vector<Transformations> transformations;
      Population(int num_obj_, int pop_size_){
        num_obj = num_obj_;
        pop_size = pop_size_;
      }
      Population(std::vector<Transformations> transformations_){
        transformations = transformations_;
        num_obj = transformations_.at(0).transformations.size();
        pop_size = transformations_.size();
      }
      void populate();
};

std::vector<Transformation> create_transformation(int num_obj){
    std::vector<Transformation> retVal;
    for (int i = 0; i < num_obj; i++){
      retVal.push_back(random_transformation());
    }
    return retVal;
}

void Population::populate(){
  for(int i = 0; i < this->pop_size; i++){
    this->transformations.push_back(create_transformation(this->num_obj));
  }
}

Transformations crossover(Transformations parent2, Transformations parent1){
  int mutation = rand()%100;
  if (mutation < 5){
    int parent = rand()%2;
    if (parent == 0){
      parent1.mutator();
    }
    else if (parent == 1){
      parent2.mutator();
    }
  }
  int num_genes = parent1.num_obj;
  std::vector<Transformation> offspring_genes;
  for (int i = 0; i < num_genes; i++){
    int choice = rand()%2;
    if (choice == 0){
      offspring_genes.push_back(parent1.transformations.at(i));
    }
    else if (choice == 1){
      offspring_genes.push_back(parent2.transformations.at(i));
    }
  }
  Transformations offspring = Transformations(offspring_genes);
  return offspring;
}

Population new_generation(std::vector<Transformations> offsprings){
  return Population(offsprings);
}