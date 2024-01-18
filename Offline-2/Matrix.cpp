#include<iostream>
#include<vector>
#include "Triangle.cpp"

using namespace std;

const double PI = 2*acos(0.0);

class Matrix{
    vector<vector<double>> matrix;
public:

    void makeNull() {
        for(int i = 0; i < 4; i++) {
            for(int j = 0; j < 4; j++ ) {
                matrix[i][j] = 0;
            } 
        }
    }

    void makeIdentity() {
        for(int i = 0; i < 4; i++) {
            for(int j = 0; j < 4; j++ ) {
                if(i == j) matrix[i][j] = 1;
                else matrix[i][j] = 0;
            } 
        }
    }

    

    Matrix() {
        matrix = vector<vector<double>>(4, vector<double>(4, 0.00));
    }

    Matrix operator*(Matrix m) {

        Matrix product;

        for (int i=0; i < 4; i++) {
            for (int j = 0; j < 4; j++) {
                for (int k = 0; k < 4; k++) {
                    product.matrix[i][j] += matrix[i][k] * m.matrix[k][j];
                }
            }
        }
        return product;
    }

    vector<vector<double>> pointsToTransposedMatrix(const Point& p1, const Point& p2, const Point& p3) {
        vector<vector<double>> mat(4, vector<double>(3));

        // Populate the matrix with coordinates of the points
        mat[0] = {p1.x, p2.x, p3.x};
        mat[1] = {p1.y, p2.y, p3.y};
        mat[2] = {p1.z, p2.z, p3.z};
        mat[3] = {p1.t, p2.t, p3.t};

        return mat;
    }

    vector<vector<double>> matrixMult(vector<vector<double>> t) {
        if (matrix[0].size() != t.size()) {
            cout << "Error: Incompatible matrix sizes for multiplication." << endl;
            return {};
        }

        // Initialize the result matrix with zeros
        vector<vector<double>> result(4, vector<double>(3, 0.0));

        // Perform matrix multiplication
        for (int i = 0; i < 4; ++i) {
            for (int j = 0; j < 3; ++j) {
                for (int k = 0; k < t.size(); ++k) {
                    result[i][j] += matrix[i][k] * t[k][j];
                }
            }
        }

        return result;
    }

    Triangle operator*(Triangle t) {

        // make a 4 x 3 matrix from triangle t
        vector<vector<double>> mat_t = pointsToTransposedMatrix(t.p1, t.p2, t.p3);
        // mat * t = a 4 x 3 matrix
        vector<vector<double>> mat_mul = matrixMult(mat_t);

        Point p1(mat_mul[0][0], mat_mul[1][0], mat_mul[2][0], mat_mul[3][0]);
        Point p2(mat_mul[0][1], mat_mul[1][1], mat_mul[2][1], mat_mul[3][1]);
        Point p3(mat_mul[0][2], mat_mul[1][2], mat_mul[2][2], mat_mul[3][2]);

        p1 = p1 / p1.t;
        p2 = p2 / p2.t;
        p3 = p3 / p3.t;

        Triangle new_triangle(p1, p2, p3);

        return new_triangle;
    }

    //    Point operator*(Point point){
    //     Point ans;
    //     for(int i=0; i< 4; i++){
    //         ans[i] = 0;
    //         for(int j = 0; j < 4; j++){
    //             ans[i] += this->matrix[i][j] * point[j];
    //         }
    //     }

    //     ans = ans / ans.t;
        
    //     return ans;
    // }

    // Triangle operator*(Triangle t){
    //     Triangle ans;
    //     ans.p1= *this * t.p1;
    //     ans.p2 = *this * t.p2;
    //     ans.p3 = *this * t.p3;
        
    //     return ans;
    // }    

    Matrix transpose(){
        Matrix ans;
        for(int i=0; i< 4; i++){
            for(int j=0; j< 4; j++){
                ans.matrix[i][j] = matrix[j][i];
            }
        }

        return ans;
    }

    static Matrix getIdentityMatrix() {
        Matrix m;
        m.makeIdentity();
        return m;
    }

    static Matrix getTranslationMatrix(Point t) {
        Matrix m;
        m.makeIdentity();

        m.matrix[0][3] = t.x;
        m.matrix[1][3] = t.y;
        m.matrix[2][3] = t.z;

        return m;

    }

    static Matrix getScalingMatrix(Point s) {
        Matrix m;

        m.matrix[0][0] = s.x;
        m.matrix[1][1] = s.y;
        m.matrix[2][2] = s.z;
        m.matrix[3][3] = 1;

        return m;

    }

    static Point Rodrigues(Point x, Point a, double angle) {

        double angleInRad = angle * (PI/180);

        double dot_value = x.dot(a);
        Point cross_point = a*x;

        return x*cos(angleInRad) + (a*dot_value)*(1-cos(angleInRad)) + cross_point*sin(angleInRad);
        
    }

    static Matrix getRotationMatrix(double angle, Point axis) {
        Matrix m;

        axis.normalize();

        Point i(1,0,0);
        Point j(0,1,0);
        Point k(0,0,1);


        Point c1 = Rodrigues(i, axis, angle);
        Point c2 = Rodrigues(j, axis, angle);
        Point c3 = Rodrigues(k, axis, angle);

        m.matrix[0][0] = c1.x;
        m.matrix[0][1] = c2.x;
        m.matrix[0][2] = c3.x;

        m.matrix[1][0] = c1.y;
        m.matrix[1][1] = c2.y;
        m.matrix[1][2] = c3.y;
        
        m.matrix[2][0] = c1.z;
        m.matrix[2][1] = c2.z;
        m.matrix[2][2] = c3.z;
       
        m.matrix[3][3] = 1;

        return m;
    }

    static Matrix getViewRotationMatix(Point r, Point u, Point l) {
        Matrix m;

        m.matrix[0][0] = r.x;
        m.matrix[0][1] = r.y;
        m.matrix[0][2] = r.z;

        m.matrix[1][0] = u.x;
        m.matrix[1][1] = u.y;
        m.matrix[1][2] = u.z;
        
        m.matrix[2][0] = -l.x;
        m.matrix[2][1] = -l.y;
        m.matrix[2][2] = -l.z;
       
        m.matrix[3][3] = 1;

        return m;
    }

    static Matrix getProjectionMatrix(double t, double r, double near, double far) {
        Matrix m;

        m.matrix[0][0] = near/r;
        m.matrix[1][1] = near/t;
        m.matrix[2][2] = -(far+near) / (far-near);
        m.matrix[2][3] = -(2*far*near) / (far-near);
        m.matrix[3][2] = -1;

        return m;
    }

    
};

