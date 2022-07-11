// #include "Transformation.cpp"
#include "Transformations.cpp"

Transformation random_transformation(){
    int type = rand()%2;
    double x_move = (rand()%100);
    double y_move = (rand()%100);
    Transformation a = Transformation(Vector(x_move/100, y_move/100), 0, type);
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

Population new_generation(std::vector<Transformations> offsprings){
  return Population(offsprings);
}
