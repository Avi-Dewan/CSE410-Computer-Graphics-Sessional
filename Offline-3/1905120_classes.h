#ifndef CLASSES_H
#define CLASSES_H   

#include<vector>
#include<math.h>
#include<iostream>
#include<iomanip>

#ifdef __linux__
    #include<GL/glut.h>
#elif WIN32
    #include <windows.h>
    #include <glut.h>
#endif

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

    friend std::ostream& operator<<(std::ostream& os, const Point p);


    double & operator[](int i){
        if(i == 0) return x;
        if(i == 1) return y;
        if(i == 2) return z;

        cerr << "Point index out of range" << endl;
        return x;
    }
};


ostream &operator<<(ostream &os, Point p){
    os << fixed << setprecision(7) << p.x << " " << p.y << " " << p.z;
    return os;
}


class Matrix{
    int row;
    int col;
    vector<vector<double>> mat;

    void makeIdentityMatrix(){
        for(int i = 0; i < row; i++){
            for(int j = 0; j < col; j++){
                if(i == j) mat[i][j] = 1;
                else mat[i][j] = 0;
            }
        }
    }

    void makeZeroMatrix(){
        for(int i = 0; i < row; i++){
            for(int j = 0; j < col; j++){
                mat[i][j] = 0;
            }
        }
    }
public:
    Matrix(int row=3, int col=3)
        : row(row), col(col),mat(row, vector<double>(col))
    {
    }

    Matrix(Point p1, Point p2, Point p3)
        : Matrix()
    {
        makeIdentityMatrix();
        setPoint(0, p1);
        setPoint(1, p2);
        setPoint(2, p3);
    }

    void setPoint(int col, Point p){
        for(int i = 0; i < row; i++){
            mat[i][col] = p[i];
        }
    }

		double determinant(){
			double det = 0;

 			// Calculate determinant using specific formula
  		det = mat[0][0] * (mat[1][1] * mat[2][2] - mat[1][2] * mat[2][1])
        - mat[0][1] * (mat[1][0] * mat[2][2] - mat[1][2] * mat[2][0])
        + mat[0][2] * (mat[1][0] * mat[2][1] - mat[1][1] * mat[2][0]);

  		return det;
		}

    Matrix operator*(Matrix mat){
        Matrix ans;
        for(int i=0; i< this->row; i++){
            for(int j=0; j< mat.col; j++){
                ans.mat[i][j] = 0;
                for(int k = 0; k< mat.row; k++){
                    ans.mat[i][j] += this->mat[i][k] * mat.mat[k][j];
                }
            }
        }
        
        return ans;
    }

    Matrix transpose(){
        Matrix ans;
        for(int i=0; i< row; i++){
            for(int j=0; j< col; j++){
                ans.mat[i][j] = mat[j][i];
            }
        }

        return ans;
    }

    friend ostream& operator<<(ostream& o,const Matrix m){
        o << fixed << showpoint ;
        for(int i = 0; i < m.row; i++){
            for(int j = 0; j < m.col; j++){
                o << setprecision(7) << m.mat[i][j] << " ";
            }
            o << endl;
        }
        return o;
    }
};


class Camera {
public:
    Point camera_pos;
    Point looking_pos;
    Point up_vector;
    Point right_vector;
    Point direction;

    Camera(Point p) {
        camera_pos = p;
        looking_pos = Point(0, 0, 0);
        up_vector = Point(0, 1, 0);
        updateDirection();
		updateRightVector();
    }

    void look() {
        gluLookAt(
            camera_pos.x, camera_pos.y, camera_pos.z,
            looking_pos.x, looking_pos.y, looking_pos.z,
            up_vector.x, up_vector.y, up_vector.z
        );
    }

    void forward() {
        direction = updateDirection();
        camera_pos = camera_pos - direction * 12; // camera speed = 12
    }

    void backward() {
        direction = updateDirection();
        camera_pos = camera_pos + direction * 12;
    }

    void up() {
        camera_pos = camera_pos + up_vector * 12;
        looking_pos = looking_pos + up_vector * 12;
    }

    void down() {
        camera_pos = camera_pos - up_vector * 12;
        looking_pos = looking_pos - up_vector * 12;
    }

    void right() {
		right_vector = updateRightVector();
        camera_pos = camera_pos - right_vector * 12;
    }

    void left() {
		right_vector = updateRightVector();
        camera_pos = camera_pos + right_vector * 12;
    }

    void lookLeft() {
        looking_pos = looking_pos - right_vector * 2; // rotate speed = 2
        updateDirection();
		updateRightVector();
    }

    void lookRight() {
        looking_pos = looking_pos + right_vector * 2;
        updateDirection();
		updateRightVector();
    }

    void lookUp() {
        looking_pos = looking_pos + up_vector * 2;
        updateDirection();
		updateRightVector();
    }

    void lookDown() {
        looking_pos = looking_pos - up_vector * 2;
        updateDirection();
		updateRightVector();
    }


    void tiltCounterClockwise() {

        Point vxp = direction * up_vector;
        Point vxpvxp = direction * vxp;

        double angle = 9; // rotate Angle

        up_vector = up_vector + vxp * sin(angle * pi/180) + vxpvxp * (1 - cos(angle * pi/180));

        updateDirection();
		updateRightVector();
    }

    void tiltClockwise() {


        Point vxp = direction * up_vector;
        Point vxpvxp = direction * vxp;

        double angle = -9; // rotate Angle

        up_vector = up_vector + vxp * sin(angle * pi/180) + vxpvxp * (1 - cos(angle * pi/180));

        updateDirection();
		updateRightVector();;
    }

    void upWithoutReferenceChange() {
        camera_pos = camera_pos + up_vector * 12; // camera speed = 12
    }

    void downWithoutReferenceChange() {
        camera_pos = camera_pos - up_vector * 12;
    }

	Point updateDirection() {
        direction = (camera_pos - looking_pos); // direction = camera_pos - loking
        direction = direction / direction.absolute_value(); // normalize
        return direction;
    }

	Point updateRightVector() {
		right_vector = (up_vector * direction); // right = up x direction
        right_vector = right_vector / right_vector.absolute_value();
        return right_vector;
	}

};




class PointLight {
public:
    Point light_pos;
    double color[3];
};

class SpotLight {
public:
    PointLight point_light;
    Point light_direction;
    double cutoff_angle;
};



class Ray {
public:
    Point start;
    Point dir;

    Ray(Point start, Point dir) {
        this->start = start;
        this->dir = dir;
        this->start.normalize();
        this->dir.normalize();
    }
};




class Object {
public:
    Point reference_point; 
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

    virtual double intersect(Ray *r, Point *color, int level) {
        return this->getIntersectT(r, color, level);
    }

    virtual double getIntersectT(Ray *r, Point *color, int level) {
        return -1.0;
    }


};

class Sphere : public Object {
public:
    Sphere(Point center, double radius) {
        reference_point = center;
        length = radius;
    }

    vector<vector<Point>> generatePoints(double rad, int stacks, int slices) {

        vector<vector<Point>> points;

        double stack_angle = (double)(pi/2) / stacks;
        double slice_angle = (double)(2*pi) / slices;

        for(int i = 0; i <= stacks; i++) {
            double h = rad*sin(i*stack_angle);
            double r = rad*cos(i*stack_angle);

            vector<Point> stack_points;

            for(int j = 0; j  <= slices; j++) {
                Point p(0, 0, 0);
                p.x = r*cos(j*slice_angle);
                p.y = r*sin(j*slice_angle);
                p.z = h;
                stack_points.push_back(p);
            }

            points.push_back(stack_points);

        }
        return points;
    }

    void sphere3D(double rad, int stacks, int slices) {

        vector<vector<Point>> points = generatePoints(rad, stacks, slices);

        for(int i = 0; i < stacks; i++) {
            for(int j = 0; j < slices; j++) {

                // Upper Hemisphere
                glBegin(GL_QUADS);{
                        glVertex3f(points[i][j].x, points[i][j].y, points[i][j].z);
                        glVertex3f(points[i][j+1].x, points[i][j+1].y, points[i][j+1].z);
                        glVertex3f(points[i+1][j+1].x, points[i+1][j+1].y, points[i+1][j+1].z);
                        glVertex3f(points[i+1][j].x, points[i+1][j].y, points[i+1][j].z);
                }glEnd();

                // Lower Hemisphere
                glBegin(GL_QUADS);{
                        glVertex3f(points[i][j].x, points[i][j].y, -points[i][j].z);
                        glVertex3f(points[i][j+1].x, points[i][j+1].y, -points[i][j+1].z);
                        glVertex3f(points[i+1][j+1].x, points[i+1][j+1].y, -points[i+1][j+1].z);
                        glVertex3f(points[i+1][j].x, points[i+1][j].y, -points[i+1][j].z);
                }glEnd();
            }
        }
        
    }


    void draw() {
        // write codes for drawing sphere
        glPushMatrix();
        glColor3f(color[0], color[1], color[2]);
        glTranslatef(reference_point.x, reference_point.y, reference_point.z); // translate the sphere to the center
        sphere3D(length, 20, 20);
        glPopMatrix();
    }

    double getIntersectT(Ray *r, Point *color, int level) override{
        // source : https://www.bluebill.net/circle_ray_intersection.html

        Point u = reference_point - r->start;
        Point u1 = r->dir * u.dot(r->dir);
        double d = (u - u1).absolute_value();

        if( d > this->length) return -1;

        double m = sqrt(this->length * this->length - d * d);
        double len_u1 = u1.absolute_value();

        *color = Point(this->color[0], this->color[1], this->color[2]);
        return len_u1 - m;
    }    


};

class Triangle : public Object {
public:
    Point a, b, c;

    Triangle(Point a, Point b, Point c) {
        this->a = a;
        this->b = b;
        this->c = c;
    }

    void draw() {

        glColor3f(color[0], color[1], color[2]);
        glBegin(GL_TRIANGLES);
        {
            glVertex3f(a.x, a.y, a.z);
            glVertex3f(b.x, b.y, b.z);
            glVertex3f(c.x, c.y, c.z);
        }
        glEnd();
    }


    virtual double getIntersectT(Ray *r, Point *color, int level){
        Point col1 = a - b;
        Point col2 = a - c;
        Point col3 = r->dir;
        Point col4 = a - r->start;

        Matrix mat = Matrix(col1, col2, col3);
        Matrix mat1 = Matrix(col4, col2, col3);
        Matrix mat2 = Matrix(col1, col4, col3);
        Matrix mat3 = Matrix(col1, col2, col4);

        double det = mat.determinant();
        if(fabs(det) < 1e-5) return -1;

        double beta = mat1.determinant() / det;
        double gamma = mat2.determinant() / det;
        double t = mat3.determinant() / det;

        if( beta + gamma < 1 && beta > -.00001 && gamma > -.00001 && t > .00001){
        *color = Point(this->color[0], this->color[1], this->color[2]);
        return t;
        }
        else return -1;
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

    General(Point reference_point, double height, double width, double length) {
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


    virtual double getIntersectT(Ray *r, Point *color, int level){
        double x0 = r->start.x;
        double y0 = r->start.y;
        double z0 = r->start.z;

        double x1 = r->dir.x;
        double y1 = r->dir.y;
        double z1 = r->dir.z;

        double a = A*x1*x1 + B*y1*y1 + C*z1*z1 + D*x1*y1 + E*x1*z1 + F*y1*z1;
        double b = 2*A*x0*x1 + 2*B*y0*y1 + 2*C*z0*z1 + D*(x0*y1 + x1*y0) + E*(x0*z1 + x1*z0) + F*(y0*z1 + y1*z0) + G*x1 + H*y1 + I*z1;
        double c = A*x0*x0 + B*y0*y0 + C*z0*z0 + D*x0*y0 + E*x0*z0 + F*y0*z0 + G*x0 + H*y0 + I*z0 + J;

        if(fabs(a) < 0) return -c/b; // not quardatice
        double discriminant = b*b - 4 * a * c;

        // cout << b << " " << a << " " << c << endl;
        // cout << discriminant << endl;
        if(discriminant < 0) return -1;
        else if(fabs(discriminant) < 1e5) return -b/2/a;
        else {
        double det_sqrt = sqrt(discriminant);
        double t1 = (-b + det_sqrt) / 2 / a;
        double t2 = (-b - det_sqrt) / 2 / a;

        // cout << t1 << " " << t2 << endl;
        if (t1 < 0 && t2 < 0) return -1;
        if(t1 > t2) swap(t1, t2);

        *color = Point(this->color[0], this->color[1], this->color[2]);
        if(t1 > 0) return t1;
        else return t2;
        }
    }

    

    void draw() {
        // write codes for drawing general objects
    }
};


class Floor : public Object {
public:
    Floor(double floorWidth, double tileWidth) {
        reference_point = Point(-floorWidth/2, -floorWidth/2, 0);
        length = tileWidth;
    }

    void square2D(double a) {
        glBegin(GL_QUADS);{
            glVertex3f( a, a,0);
            glVertex3f( a,-a,0);
            glVertex3f(-a,-a,0);
            glVertex3f(-a, a,0);
        }glEnd();
    }

    void line2D_upper(int nSqre, double length) {

        double transelateAmount = 2*length;

        for(int i = 0; i < nSqre; i++) {
            glPushMatrix();
            glColor3f(1.0, 1.0, 1.0);
            glRotatef(45, 0, 0, 1);
            glTranslatef(i*transelateAmount, i*transelateAmount, 0);
            square2D(length);
            glPopMatrix();
        }

    }

    void line2D_lower(int nSqre, double length) {

        double transelateAmount = 2*length;

        for(int i = -(nSqre-1); i < 0; i++) {
            glPushMatrix();
            glColor3f(1.0, 1.0, 1.0);
            glRotatef(45, 0, 0, 1);
            glTranslatef(i*transelateAmount, i*transelateAmount, 0);
            square2D(length);
            glPopMatrix();
        }
    }

    void grid2D(int nSquare, double length) {
        
        double half_diagonal = length*sqrt(2);

        for(int i = 0; i < nSquare; i++) {
            glPushMatrix();
            glColor3f(1.0, 1.0, 1.0);
            glTranslatef(half_diagonal + i*2*half_diagonal, 0, 0);
            line2D_upper(nSquare, length);
            line2D_lower(nSquare, length);
            glPopMatrix();
        }

        for(int i = -(nSquare-1); i <= 0; i++) {
            glPushMatrix();
            glColor3f(1.0, 1.0, 1.0);
            glTranslatef(-half_diagonal + i*2*half_diagonal, 0, 0);
            line2D_upper(nSquare, length);
            line2D_lower(nSquare, length);
            glPopMatrix();
        }
    }

    void draw() {
        // write codes for drawing a checkerboard-like floor with alternate colors on adjacent tiles
        int noOfSquares = -reference_point.x / length / 2;
        grid2D(noOfSquares, length);
    }
};






vector<Object*> objects;
vector<PointLight> pointLights;
vector<SpotLight> spotLights;

#endif