#ifndef TRIANGLES_H
#define TRINAGLES_H
#include "Triangle.cpp"
#include <boost/fusion/adapted/std_pair.hpp>
// #include <boost/spirit/include/qi.hpp>
#include <boost/graph/edge_list.hpp>
#include <cmath>

typedef std::pair<int,int> Edge;
typedef std::vector<Edge> EdgeList;
typedef boost::edge_list<EdgeList::iterator> Graph;

bool checkPoint(Point p1, Point p2){
    if (p1[0] == p2[0] && p1[1] == p2[1]){
        return true;
    }
    else{
        return false;
    }
}

bool checkTriangle(Triangle triangle1, Triangle triangle2){
    int check = 0;
    if (checkPoint(triangle1.p1, triangle2.p1)){
        check = check + 1;
    }
    else if (checkPoint(triangle1.p1, triangle2.p2)){
        check = check + 1;
    }
    else if (checkPoint(triangle1.p1, triangle2.p3)){
        check = check + 1;
    }

    if (checkPoint(triangle1.p2, triangle2.p1)){
        check = check + 1;
    }
    else if (checkPoint(triangle1.p2, triangle2.p2)){
        check = check + 1;
    }
    else if (checkPoint(triangle1.p2, triangle2.p3)){
        check = check + 1;
    }
    
    if (checkPoint(triangle1.p3, triangle2.p1)){
        check = check + 1;
    }
    else if (checkPoint(triangle1.p3, triangle2.p2)){
        check = check + 1;
    }
    else if (checkPoint(triangle1.p3, triangle2.p3)){
        check = check + 1;
    }
    
    if (check == 3){
        return true;
    }
    else{
        return false;
    }
}

bool checkEdge(Edge edge1, Edge edge2){
    if (edge1 == edge2){
        return true;
    }
    else if (edge1.first == edge2.second && edge1.second == edge2.first)
    {
        return true;
    }
    else{
        return false;
    }
    
}

double sq_dist(Point p1, Point p2){
    
    double sum = sqrt(pow(p1[0]-p2[0], 2) + pow(p1[1]-p2[1], 2));
    return sum;
}

Point triangles_center(std::vector<Triangle> triangles){
    double cx;
    double cy;
    for (int i = 0; i < triangles.size(); i++){
        cx = cx + triangles.at(i).centroid[0];
        cy = cy + triangles.at(i).centroid[1];
    }
    cx = cx/triangles.size();
    cy = cy/triangles.size();
    return Point(cx, cy);
}

double maxdist(std::vector<Triangle> triangles){
    Point center = triangles_center(triangles);
    double mdist = 0;
    double dist = 0;
    for (int i = 0; i < triangles.size(); i++){
        dist = sq_dist(center, triangles.at(i).centroid);
        if (dist > mdist){
            mdist = dist;
        }
    }
    return mdist;
}

int returnIndex(std::vector<Triangle> triangles, Triangle triangle){
    int index = triangles.size();
    for (int i = 0; i < triangles.size(); i++){
        if (checkTriangle(triangles.at(i), triangle)){
            index = i;
        }
    }
    // return index;
    return index;
}


class Triangles{
    public:
        std::vector<Triangle> triangles;
        std::vector<Edge> edges;
        Point center;
        Triangles(std::vector<Triangle> triangles_, std::vector<Edge> edges_, bool flag){
            this->triangles = triangles_;
            this->edges = edges_;
            if (flag){
                this->center = this->triangles_center(triangles_);    
            }
            // this->center = this->triangles_center(triangles_);
        };
        Triangles(std::vector<Triangle> triangles_, bool flag){
            this->triangles = triangles_;
            if (flag){
                this->center = this->triangles_center(triangles_);
            }
        };
        Point triangles_center(std::vector<Triangle> triangles);
        void update_center();
        void add_edge(Triangle triangle1, Triangle triangle2);
        void add_edge(int index1, int index2);
        bool isedge(Triangle triangle1, Triangle triangle2);
        bool isedge(int index1, int index2);
};

Point Triangles::triangles_center(std::vector<Triangle> triangles){
    double cx;
    double cy;
    for (int i = 0; i < triangles.size(); i++){
        cx = cx + triangles.at(i).centroid[0];
        cy = cy + triangles.at(i).centroid[1];
    }
    cx = cx/triangles.size();
    cy = cy/triangles.size();
    return Point(cx, cy);
}

void Triangles::update_center(){
    this->center = this->triangles_center(this->triangles);
}

void Triangles::add_edge(Triangle triangle1, Triangle triangle2){
    int index1 = returnIndex(this->triangles, triangle1);
    int index2 = returnIndex(this->triangles, triangle2);
    Edge new_edge = Edge(index1, index2);
    this->edges.push_back(new_edge);
}

void Triangles::add_edge(int index1, int index2){
    Edge new_edge = Edge(index1, index2);
    this->edges. push_back(new_edge);
}

bool Triangles::isedge(Triangle triangle1, Triangle triangle2){
    Edge edge = Edge(returnIndex(this->triangles, triangle1), returnIndex(this->triangles, triangle2));
    for (int i = 0; i < this->edges.size(); i++){
        if (checkEdge(edge, this->edges.at(i))){
            return true;
        }
    }
    return false;
}

bool Triangles::isedge(int index1, int index2){
    Edge edge = Edge(index1, index2);
    for(int i = 0; i < this->edges.size(); i++){
        if (checkEdge(edge, this->edges.at(i))){
            return true;
        }
    }
    return false;
}

#endif