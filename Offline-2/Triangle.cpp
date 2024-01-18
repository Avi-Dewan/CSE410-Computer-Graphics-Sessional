#include<iostream>
#include "Point.cpp"

using namespace std;

class Triangle {
private:
public:
    Point p1, p2, p3;

    Triangle() {
        
    }

    Triangle(Point p1, Point p2, Point p3):p1(p1), p2(p2), p3(p3) {
    
    }

    friend std::ostream& operator<<(ostream& os, const Triangle t);
};


ostream& operator<<(ostream& os, const Triangle t){
        os << t.p1 << endl;
        os << t.p2 << endl;
        os << t.p3 << endl;
        return os;
}