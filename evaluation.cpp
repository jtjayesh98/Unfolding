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
#define 	CGAL_PI   3.14159265358979323846

typedef CGAL::Exact_predicates_inexact_constructions_kernel K;
typedef K::Point_2 Point;
typedef CGAL::Polygon_2<K> Polygon_2;
typedef CGAL::Polygon_with_holes_2<K> Polygon_with_holes_2;
typedef std::list<Polygon_with_holes_2> Pwh_list_2;
typedef K::Triangle_2 Triangle_2;
typedef K::Segment_2 Segment_2;

typedef CGAL::Aff_transformation_2<K> Transformation;
typedef CGAL::Vector_2<K> Vector;
typedef CGAL::Direction_2<K> Direction;

using std::cout; using std::endl;

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

// '''
// "total_overlap" is currently doing double the work that it needs to do. Efficiency has to be increased here as the cases for the triangles grows, the time taken will grow. 
// '''

double total_overlap(std::vector<Triangle_2> triangles){
  int num_triangles = triangles.size();
  double total_area;
  for (int i = 0; i < num_triangles; i++){
    for (int j = 0; j < num_triangles; j++){
      if (triangles.at(i) != triangles.at(j)){
        total_area = total_area + overlap(triangles.at(i), triangles.at(j));
      }
    }
  }
  return total_area/2;
}

class Transformations{
    public:
        int num_obj;
        std::vector<Transformation> transformations;
        double score;
        void perform_transformations(std::vector<Triangle_2> &triangles);
        double calculate_scores(std::vector<Triangle_2> &triangles);  
        Transformations(std::vector<Transformation> transformations_){
          transformations = transformations_;
          num_obj = transformations_.size();
        }
        Transformations update_score(double score_);                                                                          
};

double Transformations::calculate_scores(std::vector<Triangle_2> &triangles){
    std::vector<Triangle_2> new_set;
    num_obj = this->num_obj;
    for (int i = 0; i < num_obj; i++){
        new_set.push_back(triangles.at(i).transform(this->transformations.at(i)));
    }
    return total_overlap(triangles) - total_overlap(new_set);
}

void Transformations::perform_transformations(std::vector<Triangle_2> &triangles){
    num_obj = this->num_obj;
    for (int i = 0; i < num_obj; i++){
        triangles.at(i) = triangles.at(i).transform(this->transformations.at(i));
    }
}

Transformations Transformations::update_score(double score_){
    this->score = score_;
    return *this;
}


Transformation random_transformation(){
    int type = rand()%2;
    double x_move = (rand()%100);
    double y_move = (rand()%100);
    Transformation a = Transformation(CGAL::TRANSLATION, Vector(x_move/100, y_move/100));
    return a;
}

std::vector<Transformation> create_transformation(int num_obj){
    std::vector<Transformation> retVal;
    for (int i = 0; i < num_obj; i++){
      retVal.push_back(random_transformation());
    }
    return retVal;
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

void Population::populate(){
  for(int i = 0; i < this->pop_size; i++){
    this->transformations.push_back(create_transformation(this->num_obj));
  }
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

// void crossover(Population population, std::vector<double> weights){
//   parents = parent
// }

Transformations crossover(Transformations parent2, Transformations parent1){
  int num_genes = parent1.num_obj;
  std::vector<int> choice;
  for (int i = 0; i< num_genes; i++){
    choice.push_back(rand()%2);
  }
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

void iterator(std::vector<Triangle_2> triangles, int generation_size, int num_gen){
  Population population = Population(triangles.size(), generation_size);
  population.populate();
  double score;
  double min_score;
  std::vector<double> weights;
  std::vector<Transformations> offsprings;
  std::vector<int> parents;
  for (int k = 0; k < num_gen; k++){
    weights.clear();
    min_score = 999999;
    Transformations best_transformation = population.transformations[0];
    for (int i = 0; i < population.transformations.size(); i++){
      Transformations transformation = population.transformations[i];
      score = transformation.calculate_scores(triangles);
      if (score <= min_score){
        min_score = score;
        best_transformation = transformation;
      }
      population.transformations[i] = transformation.update_score(score);
      weights.push_back(1/score);
    }
    best_transformation.perform_transformations(triangles);
    std::cout<<total_overlap(triangles)<<std::endl;
    offsprings.clear();
    for (int i = 0; i < generation_size; i++){
      parents.clear();
      parents = parent_randomizer(weights);
      Transformations parent1 = population.transformations[parents.at(0)];
      Transformations parent2 = population.transformations[parents.at(1)];
      Transformations offspring = crossover(parent1, parent2);
      offsprings.push_back(offspring);
    }
    population = Population(offsprings);
  }
}

int main()
{
  Triangle_2 pgn1 = Triangle_2(Point(0,10), Point(4,10), Point(2, 2));
  Triangle_2 pgn2 = Triangle_2(Point(0,0), Point(4,0), Point(2, 3));
  Triangle_2 pgn3 = Triangle_2(Point(0,0), Point(0,10), Point(5, 5));
  Triangle_2 pgn4 = Triangle_2(Point(1,1), Point(6,7), Point(2,3));
  std::vector<Triangle_2> triangles;
  
  triangles.push_back(pgn1);
  triangles.push_back(pgn2);
  triangles.push_back(pgn3);
  triangles.push_back(pgn4);

  decltype(auto) intR =  CGAL::intersection(pgn1, pgn2);
  std::cout<<overlap(pgn1, pgn2)<<std::endl;
  std::cout<<total_overlap(triangles)<<std::endl;

  Transformation rotate = Transformation(CGAL::ROTATION, sin(CGAL_PI), cos(CGAL_PI));
  Transformation rational_rotate(CGAL::ROTATION,Direction(1,1), 1, 100);
  Transformation translate(CGAL::TRANSLATION, Vector(-1.5, 0));
  Transformation scale(CGAL::SCALING, 3);

  std::vector<Transformation> transformations;
  transformations.push_back(rotate);
  transformations.push_back(translate);
  transformations.push_back(scale);
  transformations.push_back(rational_rotate);
  Transformations t = Transformations(transformations);
  t.perform_transformations(triangles);
  std::cout<<total_overlap(triangles)<<std::endl;  

  iterator(triangles, 1000, 1000);
  return 1;
}