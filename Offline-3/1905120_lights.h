// pointlight
#include "1905120_utils.h"

class Light{
public:
    
    Point pos;
    Color color;

    void draw()
    {
        glPointSize(8);
        glBegin(GL_POINTS);
        glColor3f(color.r, color.g, color.b);
        glVertex3f(pos.x, pos.y, pos.z);
        glEnd();
    }

    
    friend istream& operator>>(istream &input, Light &lt)
    {
        input >> lt.pos.x >> lt.pos.y >> lt.pos.z;
        input >> lt.color.r >> lt.color.g >> lt.color.b;
        return input;
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

        glPointSize(18);

        glBegin(GL_POINTS);
        glColor3f(color.r, color.g, color.b);
        glVertex3f(pos.x, pos.y, pos.z);
        glEnd();
    }

    // input stream
    friend istream& operator>>(istream &input, SpotLight &spt)
    {
        input >> spt.pointLight.pos;
        input >> spt.pointLight.color.r >> spt.pointLight.color.g >> spt.pointLight.color.b;
        input >> spt.dir;
        input >> spt.cutoffAngle;
        return input;
    }

};
