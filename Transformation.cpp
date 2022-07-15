#include <CGAL/Aff_transformation_2.h>
#include <CGAL/Exact_predicates_inexact_constructions_kernel.h>
#include <CGAL/Triangle_2.h>
#include <iostream>
#include <typeinfo>
#include <vector>
#include <random>
#include "Triangle.cpp"
typedef CGAL::Exact_predicates_inexact_constructions_kernel K;
typedef K::Triangle_2 Triangle_2;
typedef CGAL::Aff_transformation_2<K> Aff_transformation;
typedef CGAL::Vector_2<K> Vector;
typedef CGAL::Direction_2<K> Direction;
//Somethings are redundunt in the class Transformation. I am nowhere using Aff_transformation anymore and it can be deleted now, that I have custom rotation class.
class Transformation{
  public:
    int type = 0;
    Aff_transformation translation;
    Aff_transformation rotation;
    Aff_transformation transformation;
    Vector translation_vector;
    double rho = 0;
    Transformation(Vector vector_, double rho_, int order){
      translation_vector = vector_;
      rho = rho_;
      type = order%2;
      translation = Aff_transformation(CGAL::TRANSLATION, vector_);
      rotation = Aff_transformation(CGAL::ROTATION, sin(rho_), cos(rho_));
      if (order%2 == 0){
        transformation = translation*rotation;
      }
      else{
        transformation = rotation*translation;
      }
    }
    Aff_transformation retTransform();
    void mutate();
    Vector retVector();
    double retRot();
    Triangle perform_transformation(Triangle triangle);
    int retOrder();
    Aff_transformation retTranslation();
    Aff_transformation retRotation();
    Transformation scalar_multiplier(double multiplier);
    void update(Transformation transformation_);
};

void Transformation::update(Transformation transformation_){
  this->translation_vector = transformation_.translation_vector;
  this->rho = transformation_.rho;
  this->type = transformation_.type;
  this->translation = transformation_.translation;
  this->rotation = transformation_.rotation;
  this->transformation = transformation_.transformation;
}

Transformation Transformation::scalar_multiplier(double multiplier){
  this->translation_vector = this->translation_vector*multiplier;
  this->rho = this->rho*multiplier;
  Transformation retVal = Transformation(this->translation_vector, this->rho, this->type);
  this->update(retVal);
  return retVal;
}
Aff_transformation Transformation::retTranslation(){
  return this->translation;
}

Aff_transformation Transformation::retRotation(){
  return this->rotation;
}

Triangle Transformation::perform_transformation(Triangle triangle){
    Triangle_2 retVal;
    if (this->type == 0){
      Triangle_2 translated = triangle.translation(this->retTranslation());
      retVal = triangle.rotation(this->retRotation());
    }
    else if(this->type == 1){
      // std::cout<<triangle.triangle<<std::endl;
      Triangle_2 rotated = triangle.rotation(this->retRotation());
      // std::cout<<rotated<<std::endl;
      retVal = triangle.translation(this->retTranslation());
      
    }
    return Triangle(retVal);
} 

int Transformation::retOrder(){
  return this->type;
}
Aff_transformation Transformation::retTransform(){
  return this->transformation;
}

double Transformation::retRot(){
  return this->rho;
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


