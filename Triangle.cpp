#include <CGAL/Exact_predicates_inexact_constructions_kernel.h>
#include <CGAL/Triangle_2.h>
#include <CGAL/Aff_transformation_2.h>
typedef CGAL::Exact_predicates_inexact_constructions_kernel K;
typedef CGAL::Aff_transformation_2<K> Aff_transformation;
typedef CGAL::Vector_2<K> Vector;
typedef CGAL::Direction_2<K> Direction;



typedef K::Point_2 Point;
typedef K::Triangle_2 Triangle_2;

class Triangle{
    public:
        Point p1;
        Point p2;
        Point p3;
        Point centroid;
        Triangle_2 triangle;
        double area;
        void find_centroid();
        Triangle(Point p1_, Point p2_, Point p3_){
            p1 = p1_;
            p2 = p2_;
            p3 = p3_;
            triangle = Triangle_2(p1_, p2_, p3_);
            area = triangle.area();
            find_centroid();
        }
        Triangle_2 translation(Aff_transformation transformation);
        Triangle_2 rotation(Aff_transformation transformation);
        void update(Triangle_2 triangle_);
        Triangle_2 centered_triangle();
        Triangle_2 move_back(Triangle_2 triangle_);
        Triangle(Triangle_2 triangle_){
            for (int i = 0; i < 3; i++){
                p1 = triangle_[0];
                p2 = triangle_[1];
                p3 = triangle_[2];
                triangle = triangle_;
                area = triangle_.area();
                find_centroid(); 
            }
        }
};

void Triangle::find_centroid(){
    double cx = (this->p1[0] + this->p2[0] + this->p3[0])/3;
    double cy = (this->p1[1] + this->p2[1] + this->p3[1])/3;
    this->centroid = Point(cx, cy);
}

Triangle_2 Triangle::translation(Aff_transformation transformation){
    Triangle_2 retVal = this->triangle.transform(transformation);
    this->update(retVal);
    return retVal;
}

Triangle_2 Triangle::centered_triangle(){
    Point p1_(this->p1[0] - this->centroid[0], this->p1[1] - this->centroid[1]);
    Point p2_(this->p2[0] - this->centroid[0], this->p2[1] - this->centroid[1]);
    Point p3_(this->p3[0] - this->centroid[0], this->p3[1] - this->centroid[1]);
    return Triangle_2(p1_, p2_, p3_);
}

Triangle_2 Triangle::move_back(Triangle_2 triangle_){
    Point p1_(triangle_[0][0] + this->centroid[0], triangle_[0][1] + this->centroid[1]);
    Point p2_(triangle_[1][0] + this->centroid[0], triangle_[1][1] + this->centroid[1]);
    Point p3_(triangle_[2][0] + this->centroid[0], triangle_[2][1] + this->centroid[1]);
    return Triangle_2(p1_, p2_, p3_);
}

void Triangle::update(Triangle_2 triangle_){
    this->p1 = triangle_[0];
    this->p2 = triangle_[1];
    this->p3 = triangle_[2];
    this->triangle = triangle_;
    this->area = triangle.area();
    this->find_centroid();
}

Triangle_2 Triangle::rotation(Aff_transformation transformation){
    Triangle_2 centered_triangle = this->centered_triangle();
    Triangle_2 rotated_triangle = centered_triangle.transform(transformation);
    Triangle_2 retVal = this->move_back(rotated_triangle);
    this->update(retVal);
    return retVal;
}

