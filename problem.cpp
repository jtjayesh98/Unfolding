#include "populator.cpp"
// #include "functions.cpp"

double Transformations::calculate_scores(std::vector<Triangle_2> &triangles){
    std::vector<Triangle_2> new_set;
    num_obj = this->num_obj;
    for (int i = 0; i < num_obj; i++){
        new_set.push_back(triangles.at(i).transform(this->transformations.at(i).retTransform()));
    } 
    return total_overlap(triangles) - total_overlap(new_set);
}

double Transformations::calculate_scores2(std::vector<Triangle_2> &triangles){
    std::vector<Triangle_2> new_set;
    num_obj = this->num_obj;
    for (int i = 0; i < num_obj; i++){
        new_set.push_back(triangles.at(i).transform(this->transformations.at(i).retTransform()));
    } 
    return calculator(triangles) - calculator(new_set);
}