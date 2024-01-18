#include<math.h>
#include<iostream>
#include<iomanip>

#define pi (2*acos(0.0))

using namespace std;

class Point
{
public:
    double x, y, z, t;

    Point(){
		this->t = 1;
	};
 
    Point(double x, double y, double z, double t = 1){
		this->x = x;
		this->y = y;
		this->z = z;
        this->t = t;
	}

    // Overloaded addition operator
    Point operator +(const Point& p) const {
        return Point(x + p.x, y + p.y, z + p.z);
    }

    // Overloaded subtraction operator
    Point operator -(const Point& p) const {
        return Point( x - p.x, y - p.y, z - p.z);
    }

    // Overloaded division operator
    Point operator /(double div) const {
        return Point(x / div, y / div, z / div);
    }

    // Overloaded multiplication operator
    Point operator *(double multiplier) const {
        return Point(x * multiplier, y * multiplier, z * multiplier);
    }
	
	Point operator *(const Point& p) const {
        return Point(
            y * p.z - z * p.y,
            z * p.x - x * p.z,
            x * p.y - y * p.x
        );
    }

    Point operator -(){
        return Point(-this->x, -this->y, -this->z);
    }

    double dot(const Point& p) const {
        return (x * p.x + y * p.y + z * p.z);
    }

    double absolute_value() const {
        return sqrt(x*x + y*y + z*z);
    }

    void normalize() {
        double abs = absolute_value();
        x = x / abs;
        y = y / abs;
        z = z / abs;
    }

    double & operator[](int i){
        if(i == 0) return x;
        if(i == 1) return y;
        if(i == 2) return z;
        if(i == 3) return t;

        cerr << "Point index out of range" << endl;
        return x;
    }


    friend std::ostream& operator<<(std::ostream& os, const Point p);

};



ostream &operator<<(ostream &os, Point p){
    os << fixed << setprecision(7) << p.x << " " << p.y << " " << p.z;
    return os;
}