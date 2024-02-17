#include<iostream>
#include<algorithm>
#include "Point.cpp"
#include "Boundary.cpp"

using namespace std;

class Triangle {
private:
public:
    Point p1, p2, p3;

    Triangle() {
        
    }

    Triangle(Point p1, Point p2, Point p3):p1(p1), p2(p2), p3(p3) {
    
    }

    double area_xy() {  //2D area of traingle in xy plane
        Point v1 = p2 - p1;
        Point v2 = p3 - p1;

        Point v3 = v1*v2;

        return v3.z;
    }


    Point getBarycentricCoordinates(double x, double y) {

        Point p = Point(x, y, 0);

        Triangle t1(p2, p3, p);
        Triangle t2(p3, p1, p);
        Triangle t3(p1, p2, p);

        Point bary_coords(t1.area_xy()/area_xy(), t2.area_xy()/area_xy(), t3.area_xy()/area_xy());

        // cout << bary_coords << endl;

        return bary_coords;

    }

    double get_zValue(double x, double y) {
        Point lamda = getBarycentricCoordinates(x, y);

        // cout << lamda << endl;
        // cout << p1.z << " " << p2.x << " " << p3.z << endl;

        //cout << lamda.x * p1.z  << " " <<  lamda.y * p2.z  << " " << lamda.z * p3.z << endl;

        double zValue = (lamda.x * p1.z) + (lamda.y * p2.z) + (lamda.z * p3.z);

        //cout << zValue << endl;

        return zValue;
    }

    Boundary getBoundary(){

        double right = max({p1.x, p2.x, p3.x});

        double left = min({p1.x, p2.x, p3.x});

        double upper = max({p1.y, p2.y, p3.y});

        double lower = min({p1.y, p2.y, p3.y});

        Boundary boundary(upper, lower, left, right);
        boundary.clipEdges();

        return boundary;
    }




    friend std::ostream& operator<<(ostream& os, const Triangle t);
};


ostream& operator<<(ostream& os, const Triangle t){
        os << t.p1 << endl;
        os << t.p2 << endl;
        os << t.p3 << endl;
        return os;
}