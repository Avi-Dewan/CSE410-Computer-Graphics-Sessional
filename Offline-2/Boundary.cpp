#include<algorithm>
#include<iostream>

using namespace std;

class Boundary {
public:
    double upper, lower, left, right;

    Boundary(double upper, double lower, double left, double right) {
        this->upper = upper;
        this->lower = lower;
        this->right = right;
        this->left = left;
    }

    //clip edges

    void clipEdges() {
        // clip edges
        upper = min(1., upper);
        lower = max(-1., lower);
        left = max(-1., left);
        right = min(1., right);
    }
};