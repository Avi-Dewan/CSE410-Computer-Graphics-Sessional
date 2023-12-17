#include<math.h>

#define pi (2*acos(0.0))


class Point
{
public:
    double x, y, z;

    Point(){
		
	};
 
    Point(double x, double y, double z){
		this->x = x;
		this->y = y;
		this-> z = z;
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

    double dot(const Point& p) const {
        return (x * p.x + y * p.y + z * p.z);
    }

    double absolute_value() const {
        return sqrt(x*x + y*y + z*z);
    }
};