#include<bits/stdc++.h>
#include "bitmap_image.hpp"
#include <GL/glut.h>
using namespace std;

#define pi (2*acos(0.0))


class Color{
public:
    double r,g,b;

    Color(double r = 0, double g = 0, double b = 0){
        this->r = r;
        this->g = g;
        this->b = b;
    }
};

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

    double operator*(const Point& p) const { // dot product
        return (x * p.x + y * p.y + z * p.z);
    }
	
	Point operator ^(const Point& p) const { // cross product
        return Point(
            y * p.z - z * p.y,
            z * p.x - x * p.z,
            x * p.y - y * p.x
        );
    }

    Point operator -(){
        return Point(-this->x, -this->y, -this->z);
    }

    double dot(const Point& p) const { // dot product
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

    friend std::ostream& operator<<(std::ostream& os, const Point p);
    friend std::istream& operator>>(std::istream& is, Point &p);
};

istream &operator>>(istream &is, Point &p){
    is >> p.x >> p.y >> p.z;
    return is;
}

ostream &operator<<(ostream &os, Point p){
    os << fixed << setprecision(7) << p.x << " " << p.y << " " << p.z;
    return os;
}

class Matrix {
public:
    double mat[3][3];

    Matrix() {
    }
    //9 values and populate them in the matrix
    Matrix(double a1, double a2, double a3, double b1, double b2, double b3, double c1, double c2, double c3) {
        mat[0][0] = a1;
        mat[0][1] = a2;
        mat[0][2] = a3;
        mat[1][0] = b1;
        mat[1][1] = b2;
        mat[1][2] = b3;
        mat[2][0] = c1;
        mat[2][1] = c2;
        mat[2][2] = c3;
    }

    double determinant() {
        return mat[0][0] * (mat[1][1] * mat[2][2] - mat[1][2] * mat[2][1]) - mat[0][1] * (mat[1][0] * mat[2][2] - mat[1][2] * mat[2][0]) + mat[0][2] * (mat[1][0] * mat[2][1] - mat[1][1] * mat[2][0]);
    }

};

class Camera {
public:
    Point pos;
    Point look_vector;
    Point up_vector;
    Point right_vector;
    Point direction;

    void rotateVectorWRT_AxisByTheta(Point &v1, Point &axis, double theta)
    {
        v1 = v1*cos(theta) + (axis^v1)*sin(theta);
    }

    Camera() {
        pos = Point(190, 110, 20);

        up_vector = Point(0, 0, 1);
        look_vector = Point(-1 / sqrt(2), -1 / sqrt(2), 0);
        right_vector = Point(-1 / sqrt(2), 1 / sqrt(2), 0);
    }

    void look() {
        gluLookAt(
            pos.x, pos.y, pos.z,
            pos.x + look_vector.x, pos.y + look_vector.y, pos.z + look_vector.z,
            up_vector.x, up_vector.y, up_vector.z
        );
    }

    void forward() {
        pos = pos + look_vector * 3;
    }

    void backward() {
        pos = pos - look_vector * 3;
    }

    void up() {
        pos = pos + up_vector * 3;
    }

    void down() {
        pos = pos - up_vector * 3;
    }

    void right() {
		pos = pos + right_vector * 3;
    }

    void left() {
		pos = pos - right_vector * 3;
    }

    void lookLeft() {
        rotateVectorWRT_AxisByTheta(right_vector, up_vector, pi/180);
        rotateVectorWRT_AxisByTheta(look_vector, up_vector, pi/180);
    }

    void lookRight() {
        rotateVectorWRT_AxisByTheta(right_vector, up_vector, -pi/180);
        rotateVectorWRT_AxisByTheta(look_vector, up_vector, -pi/180);
    }

    void lookUp() {
       rotateVectorWRT_AxisByTheta(up_vector, right_vector, pi/180);
       rotateVectorWRT_AxisByTheta(look_vector, right_vector, pi/180);
    }

    void lookDown() {
       rotateVectorWRT_AxisByTheta(up_vector, right_vector, -pi/180);
       rotateVectorWRT_AxisByTheta(look_vector, right_vector, -pi/180);
    }


    void tiltCounterClockwise() {
        rotateVectorWRT_AxisByTheta(right_vector, look_vector, -pi/180);
        rotateVectorWRT_AxisByTheta(up_vector, look_vector, -pi/180);
    }

    void tiltClockwise() {
        rotateVectorWRT_AxisByTheta(right_vector, look_vector, pi/180);
        rotateVectorWRT_AxisByTheta(up_vector, look_vector, pi/180);
    }
};