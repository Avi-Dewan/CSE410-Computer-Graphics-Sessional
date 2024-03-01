// pointlight
#include "1905120_utils.h"

class Light{
public:
    
    Point pos;
    Color color;

    void draw()
    {
        glPointSize(5);
        glBegin(GL_POINTS);
        glColor3f(color.r, color.g, color.b);
        glVertex3f(pos.x, pos.y, pos.z);
        glEnd();
    }

    // input stream
    friend istream& operator>>(istream &in, Light &l)
    {
        in >> l.pos.x >> l.pos.y >> l.pos.z;
        in >> l.color.r >> l.color.g >> l.color.b;
        return in;
    }

};

// spotlight
class SpotLight{
public:
    Light pointLight;
    Point dir;
    double cutoffAngle; // this is different from the spotlight

    void draw()
    {
        Color color = pointLight.color;
        Point pos = pointLight.pos;

        glPointSize(15);
        glBegin(GL_POINTS);
        glColor3f(color.r, color.g, color.b);
        glVertex3f(pos.x, pos.y, pos.z);
        glEnd();
    }

    // input stream
    friend istream& operator>>(istream &in, SpotLight &l)
    {
        in >> l.pointLight.pos;
        in >> l.pointLight.color.r >> l.pointLight.color.g >> l.pointLight.color.b;
        in >> l.dir;
        in >> l.cutoffAngle;
        return in;
    }

};
