#ifndef CLASSES_H
#define CLASSES_H   

#include<vector>


using namespace std;



class Vector3D {
public:
    double x, y, z; // should have x, y, z
};

class Object {
public:
    Vector3D reference_point; 
    double height, width, length;
    double color[3];
    double coEfficients[4]; // ambient, diffuse, specular, reflection coefficients
    int shine; // exponent term of specular component

    Object() {}
    virtual void draw() {}

    void setColor(double r, double g, double b) {
        color[0] = r;
        color[1] = g;
        color[2] = b;
    }

    void setShine(int s) {
        shine = s;
    }


    void setCoEfficients(double ambient, double diffuse, double specular, double reflection) {
        coEfficients[0] = ambient;
        coEfficients[1] = diffuse;
        coEfficients[2] = specular;
        coEfficients[3] = reflection;
    }
};

class Sphere : public Object {
public:
    Sphere(Vector3D center, double radius) {
        reference_point = center;
        length = radius;
    }

    void draw() {
        // write codes for drawing sphere
    }
};

class Triangle : public Object {
public:
    Vector3D a, b, c;

    Triangle(Vector3D a, Vector3D b, Vector3D c) {
        this->a = a;
        this->b = b;
        this->c = c;
    }

    void draw() {
        // write codes for drawing triangle
    }
};


// general
// 1 1 1 0 0 0 -20 -20 -20 200	- A B C D E F G H I J
// 0 0 0 0 0 5	- cube reference point, length, width, height (0 indicates no clipping along this dimension)
// 0.0 0.0 1.0	- color
// 0.4 0.2 0.1 0.3	- ambient, diffuse, specular, recursive reflection coefficient
// 3		- shininess

class General : public Object {
public:

    double A, B, C, D, E, F, G, H, I, J;

    General(Vector3D reference_point, double height, double width, double length) {
        this->reference_point = reference_point;
        this->height = height;
        this->width = width;
        this->length = length;
    }

    void setQuadricCoefficients(double A, double B, double C, double D, double E, double F, double G, double H, double I, double J) {
        this->A = A;
        this->B = B;
        this->C = C;
        this->D = D;
        this->E = E;
        this->F = F;
        this->G = G;
        this->H = H;
        this->I = I;
        this->J = J;
    }

    

    void draw() {
        // write codes for drawing general objects
    }
};

class PointLight {
public:
    Vector3D light_pos;
    double color[3];
};

class SpotLight {
public:
    PointLight point_light;
    Vector3D light_direction;
    double cutoff_angle;
};

class Floor : public Object {
public:
    Floor(double floorWidth, double tileWidth) {
        reference_point = {-floorWidth/2, -floorWidth/2, 0};
        length = tileWidth;
    }

    void draw() {
        // write codes for drawing a checkerboard-like floor with alternate colors on adjacent tiles
    }
};


vector<Object*> objects;
vector<PointLight> pointLights;
vector<SpotLight> spotLights;

#endif