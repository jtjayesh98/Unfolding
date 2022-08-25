#include <iostream>
#include <fstream>
#include <string>

using namespace std;



int main(){
    std::ifstream myfile;
    myfile.open("Test_Mesh.off");   
    std::string read_string;
    myfile>>read_string;
    int num_points = stoi(read_string);
    myfile>>read_string;
    int triangles = stoi(read_string);
    std::cout<<num_points<<std::endl;
    std::cout<<triangles<<std::endl;
    myfile>>read_string;
    for(int i = 0; i < num_points; i++){
        myfile>>read_string;
        double point1 = stod(read_string);
        // std::cout<<point1<<std::endl;
        myfile>>read_string;
        double point2 = stod(read_string);
        // std::cout<<point2<<std::endl;
        myfile>>read_string;
        double point3 = stod(read_string);
        // std::cout<<point3<<std::endl;
    }
    for (int i = 0; i < triangles; i++){
        myfile>>read_string;
        myfile>>read_string;
        int p_index = stoi(read_string);
        std::cout<<p_index<<std::endl;
        myfile>>read_string;
        myfile>>read_string;
    }
    myfile.close();
    return 0;
}