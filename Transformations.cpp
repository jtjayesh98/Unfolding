#include <CGAL/Aff_transformation_2.h>
#include <CGAL/Exact_predicates_inexact_constructions_kernel.h>
#include <CGAL/Triangle_2.h>
#include <iostream>
#include <typeinfo>
#include <vector>
#include <random>
#include "Transformation.cpp"
#include "functions.cpp"
class Transformations{
    public:
        int num_obj;
        std::vector<Transformation> transformations;
        std::vector<std::vector<double>> vec_transformations;
        double score;
        std::vector<Triangle> perform_transformations(std::vector<Triangle> &triangles);
        double calculate_scores(std::vector<Triangle> &triangles);
        double calculate_scores2(std::vector<Triangle> &triangles);  
        Transformations(std::vector<Transformation> transformations_){
          transformations = transformations_;
          num_obj = transformations_.size();
          for (int i = 0; i < transformations_.size(); i++){
            vec_transformations.push_back(transformations_.at(i).vecForm());
          }
        }
        Transformations(std::vector<std::vector<double>> transformations_){
          vec_transformations = transformations_;
          for (int i = 0; i < transformations_.size(); i++){
            transformations.push_back(Transformation(transformations_.at(i)));
          }
          num_obj = transformations_.size();
        }
        Transformations update_score(double score_);
        void mutator();
        Transformations sub_vector(std::vector<std::vector<double>> gradients, double learning_rate);
};

Transformations Transformations::sub_vector(std::vector<std::vector<double>> gradients, double learning_rate){
  std::vector<std::vector<double>> vec_transformations_;
  
  for (int i = 0; i < gradients.size(); i++){
    std::vector<double> row;
    for (int j = 0; j < 3; j++){
      row.push_back(this->vec_transformations.at(i).at(j) - gradients.at(i).at(j));
    }
    vec_transformations_.push_back(row);
    row.clear();
  }
  return Transformations(vec_transformations_);
}

std::vector<Triangle_2> dual_set(std::vector<Triangle> &triangles){
  std::vector<Triangle_2> retVal;
  for (int i= 0; i< triangles.size(); i++){
    retVal.push_back(triangles.at(i).triangle);
  }
  return retVal;
}

void Transformations::mutator(){
  // std::vector<Transformation> transformations = this->transformations;
  for (int i = 0; i < this->transformations.size(); i++){
    this->transformations.at(i).mutate();
  }
}

double Transformations::calculate_scores(std::vector<Triangle> &triangles){
    std::vector<Triangle> new_set;
    num_obj = this->num_obj;
    for (int i = 0; i < num_obj; i++){
        new_set.push_back(this->transformations.at(i).perform_transformation(triangles.at(i)));
    }
    return total_overlap(dual_set(triangles)) - total_overlap(dual_set(new_set));
}

double Transformations::calculate_scores2(std::vector<Triangle> &triangles){
    std::vector<Triangle> new_set;
    num_obj = this->num_obj;
    for (int i = 0; i < num_obj; i++){
        new_set.push_back(this->transformations.at(i).perform_transformation(triangles.at(i)));
    } 
    return calculator(dual_set(triangles)) - calculator(dual_set(new_set));
}

std::vector<Triangle> Transformations::perform_transformations(std::vector<Triangle> &triangles){
    num_obj = this->num_obj;
    std::vector<Triangle> retVal;
    for (int i = 0; i < this->transformations.size(); i++){
      retVal.push_back(this->transformations.at(i).perform_transformation(triangles.at(i)));
    }
    return retVal;
}

Transformations Transformations::update_score(double score_){
    this->score = score_;
    return *this;
}







// void crossover(Population population, std::vector<double> weights){
//   parents = parent
// }

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