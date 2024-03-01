#include<bits/stdc++.h>
#include "bitmap_image.hpp"
#include <GL/glut.h>
using namespace std;

#define pi (2*acos(0.0))

extern bitmap_image image;

struct Color{
    double r,g,b;
    Color()
    {
        r = g = b = 0.0;
    }

    Color(double r, double g, double b) : r(r), g(g), b(b) {}
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


	// Point updateDirection() {
    //     direction = (camera_pos - looking_pos); // direction = camera_pos - loking
    //     direction = direction / direction.absolute_value(); // normalize
    //     return direction;
    // }

	// Point updateRightVector() {
	// 	right_vector = (up_vector * direction); // right = up x direction
    //     right_vector = right_vector / right_vector.absolute_value();
    //     return right_vector;
	// }

};


// pointlight
struct Light{
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
struct SpotLight{
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


struct Ray{
    Point origin, dir;
    
    Ray(Point origin, Point dir){
        this->origin = origin;
        dir.normalize();
        this->dir = dir;
    }

    // stream
    friend ostream& operator<<(ostream &out, Ray r)
    {
        out << "Origin : " << r.origin << ", Direction : " << r.dir;
        return out;
    }
};

class Object;

extern vector <Light*> lights;
extern vector <SpotLight*> spotlights;
extern vector <Object*> objects;
extern int recursionLevel;

class Object {
public:
		Point reference_point;
		double height, width, length;
		Color color;
		vector <double> coefficients; // ambient, diffuse, specular, reflection coefficients
		int shine; // exponent term of specular component
		
		Object(){
            coefficients = vector <double> (4,0);
		}

		void setColor(Color color){
            this->color = color;
        }

        virtual Color getColorAt(Point point){
            return Color(this->color.r, this->color.g, this->color.b);
        }
    	
		void setShine(int shine){
            this->shine = shine;
        }

		void setCoefficients(vector<double> coefficients){
            this->coefficients = coefficients;
        }   

        virtual void draw() = 0;
		virtual double intersectHelper(Ray ray, Color &color, int level) = 0;
        virtual Ray getNormal(Point point, Ray incidentRay) = 0;
		virtual double intersect(Ray ray, Color &color, int level)
        {
            double t = intersectHelper(ray, color, level);

            if(t < 0) return -1;
            if(level == 0) return t;

            // find intersection point and it's color
            Point intersectionPoint = ray.origin + ray.dir*t;
            Color colorAtIntersection = getColorAt(intersectionPoint);

            // update color with ambience (thing will become dimmer)
            color.r = colorAtIntersection.r * coefficients[0];
            color.g = colorAtIntersection.g * coefficients[0];
            color.b = colorAtIntersection.b * coefficients[0];

            // cout<< " Lights size " << lights.size() << endl;

            for(int i = 0; i < lights.size(); i++){

                Point lightPosition = lights[i]->pos;
                Point lightDirection = intersectionPoint - lightPosition;
                lightDirection.normalize();
                
                // cast incident ray, from light position to intersection point
                Ray lightRay = Ray(lightPosition, lightDirection);

                // calculate normal at intersectionPoint
                Ray normal = getNormal(intersectionPoint,lightRay);

                /**
                 * @brief check if incedent ray is not obstructed by any other object
                 * 
                 */
                
                double t2 = (intersectionPoint - lightPosition).absolute_value();
                if(t2 < 1e-5) continue;

                bool obscured = false;

                for(Object *obj : objects){
                    double t3 = obj->intersectHelper(lightRay, color, 0);
                    if(t3 > 0 && t3 + 1e-5 < t2){
                        obscured = true;
                        break;
                    }
                }

                if(!obscured){
                    
                    // lambert value
                    double val = max(0.0, -lightRay.dir*normal.dir);
                    
                    // find reflected ray
                    Ray reflection = Ray(intersectionPoint, lightRay.dir - normal.dir*2*(lightRay.dir*normal.dir));
                    double phong = max(0.0,-ray.dir*reflection.dir);
                    
                    // update diffuse and specular components
                    // lights[i]->color works as the source intensity, Is here

                    color.r += lights[i]->color.r * coefficients[1] * val * colorAtIntersection.r;
                    color.r += lights[i]->color.r * coefficients[2] * pow(phong,shine) * colorAtIntersection.r;

                    color.g += lights[i]->color.g * coefficients[1] * val * colorAtIntersection.g;
                    color.g += lights[i]->color.g * coefficients[2] * pow(phong,shine) * colorAtIntersection.g;

                    color.b += lights[i]->color.b * coefficients[1] * val * colorAtIntersection.b;
                    color.b += lights[i]->color.b * coefficients[2] * pow(phong,shine) * colorAtIntersection.b;

                }
            }

            /**
             * @brief same calculation as above, but for spotlights
             * Do until ray cast from light_pos to intersectionPoint exceeds cutoff-angle for the light source
             */

            for(int i = 0; i < spotlights.size(); i++){

                Point lightPosition = spotlights[i]->pointLight.pos;
                Point lightDirection = intersectionPoint - lightPosition;
                lightDirection.normalize();

                double dot = lightDirection*spotlights[i]->dir;
                double angle = acos(dot/(lightDirection.absolute_value()*spotlights[i]->dir.absolute_value())) * (180.0/pi);

                if(fabs(angle)<spotlights[i]->cutoffAngle){

                    Ray lightRay = Ray(lightPosition, lightDirection);
                    Ray normal = getNormal(intersectionPoint,lightRay);
                    
                    Ray reflection = Ray(intersectionPoint, lightRay.dir - normal.dir*2*(lightRay.dir*normal.dir));
                    
                    double t2 = (intersectionPoint - lightPosition).absolute_value();
                    if(t2 < 1e-5) continue;
                    
                    bool obscured = false;
                    
                    for(Object *obj : objects){
                        double t3 = obj->intersectHelper(lightRay, color, 0);
                        if(t3 > 0 && t3 + 1e-5 < t2){
                            obscured = true;
                            break;
                        }
                    }
                    
                    if(!obscured){
                        
                        double phong = max(0.0,-ray.dir*reflection.dir);
                        double val = max(0.0, -lightRay.dir*normal.dir);
                        
                        color.r += spotlights[i]->pointLight.color.r * coefficients[1] * val * colorAtIntersection.r;
                        color.r += spotlights[i]->pointLight.color.r * coefficients[2] * pow(phong,shine) * colorAtIntersection.r;
                        
                        color.g += spotlights[i]->pointLight.color.g * coefficients[1] * val * colorAtIntersection.g;
                        color.g += spotlights[i]->pointLight.color.g * coefficients[2] * pow(phong,shine) * colorAtIntersection.g;
                        
                        color.b += spotlights[i]->pointLight.color.b * coefficients[1] * val * colorAtIntersection.b;
                        color.b += spotlights[i]->pointLight.color.b * coefficients[2] * pow(phong,shine) * colorAtIntersection.b;
                        
                    }
                }
            }

            /**
             * @brief RECURSIVE REFLECTION
             * 
             */

            if(level < recursionLevel){
                // if(level > 1) cout << "Recursion level " << level << endl;

                // find normal at intersectionPoint
                Ray normal = getNormal(intersectionPoint,ray);

                // find reflected ray
                Ray reflectionRay = Ray(intersectionPoint, ray.dir - normal.dir*2*(ray.dir*normal.dir));

                /**
                 * @brief slightly forward from the point 
                 * (by moving the start a little bit towards the reflection direction)
                 * to avoid self intersection
                 * 
                 */
                reflectionRay.origin = reflectionRay.origin + reflectionRay.dir*1e-5;
                

                // find nearest intersection object and do recursive call

                int nearestObjectIndex = -1;
                double t = -1,tMin = 1e9;

                for(int k=0;k<(int)objects.size();k++)
                {
                    t = objects[k]->intersect(reflectionRay,color, 0);
                    if(t> 0 && t<tMin)
                        tMin = t , nearestObjectIndex = k;
                }

                if(nearestObjectIndex != -1)
                {
                    // cout<<"Object "<<nearestObjectIndex<<" intersected"<<endl;

                    Color colorTemp(0,0,0); // refelction color
                    // cout<<"Before Color "<<color.r<<" "<<color.g<<" "<<color.b<<endl;
                    double t = objects[nearestObjectIndex]->intersect(reflectionRay,colorTemp, level+1);

                    // colorTemp will be updated while in the subsequent call
                    // update color using the impact of reflection
                    
                    color.r += colorTemp.r * coefficients[3];
                    color.g += colorTemp.g * coefficients[3];
                    color.b += colorTemp.b * coefficients[3];

                }
            }

            return t;
        }

        // destructor
        virtual ~Object(){
            coefficients.clear();
            coefficients.shrink_to_fit();
        }
};

struct General : public Object{
    double A,B,C,D,E,F,G,H,I,J;

    General(){

    }

    virtual void draw(){
        return;
    }

    virtual Ray getNormal(Point point, Ray incidentRay)
    {
        Point dir(2*A*point.x + D*point.y + E*point.z + G,
               2*B*point.y + D*point.x + F*point.z + H,
               2*C*point.z + E*point.x + F*point.y + I);

        return Ray(point, dir);
    }

    bool ok(Point point)
    {
        if(fabs(length) > 1e-5){
            if(point.x < reference_point.x) return false;
            if(point.x > reference_point.x + length) return false;
        }
        

        if(fabs(width) > 1e-5){
            if(point.y < reference_point.y) return false;
            if(point.y > reference_point.y + width) return false;
        }
        

        if(fabs(height) > 1e-5){
            if(point.z < reference_point.z) return false;
            if(point.z > reference_point.z + height) return false;
        }
    
        return true;
    }


    virtual double intersectHelper(Ray ray, Color &color, int level){

        double X0 = ray.origin.x;
        double Y0 = ray.origin.y;
        double Z0 = ray.origin.z;

        double X1 = ray.dir.x;
        double Y1 = ray.dir.y;
        double Z1 = ray.dir.z;

        double C0 = A*X1*X1 + B*Y1*Y1 + C*Z1*Z1 + D*X1*Y1 + E*X1*Z1 + F*Y1*Z1;
        double C1 = 2*A*X0*X1 + 2*B*Y0*Y1 + 2*C*Z0*Z1 + D*(X0*Y1 + X1*Y0) + E*(X0*Z1 + X1*Z0) + F*(Y0*Z1 + Y1*Z0) + G*X1 + H*Y1 + I*Z1;
        double C2 = A*X0*X0 + B*Y0*Y0 + C*Z0*Z0 + D*X0*Y0 + E*X0*Z0 + F*Y0*Z0 + G*X0 + H*Y0 + I*Z0 + J;

        double discriminant = C1*C1 - 4*C0*C2;
        if(discriminant < 0) return -1;
        if(fabs(C0) < 1e-5) {
            return -C2/C1;
        }
        double t1 = (-C1 - sqrt(discriminant))/(2*C0);
        double t2 = (-C1 + sqrt(discriminant))/(2*C0);

        if(t1 < 0 && t2 < 0) return -1;

        // cout<<"t1 "<<t1<<" t2 "<<t2<<endl;

        if(t2<t1) swap(t1,t2);

        if(t1 > 0) {
            // cout<<"t1 "<<t1<<endl;
            Point intersectionPoint = ray.origin + ray.dir*t1;
            if(ok(intersectionPoint)){
                return t1;
            }
        }
        if(t2 > 0) {
            // cout<<"t2 "<<t2<<endl;
            Point intersectionPoint = ray.origin + ray.dir*t2;
            if(ok(intersectionPoint)){
                return t2;
            }
        }

        return -1;

    }
    
    // input stream
    friend istream& operator>>(istream &in, General &g)
    {
        in >> g.A >> g.B >> g.C >> g.D >> g.E >> g.F >> g.G >> g.H >> g.I >> g.J;
        in >> g.reference_point >> g.length >> g.width >> g.height;

        in >> g.color.r >> g.color.g >> g.color.b; // color
        for(int i = 0; i < 4; i++) in >> g.coefficients[i];
        in >> g.shine;
        return in;
    }

};

double determinant(double ara[3][3]){
	double v1 = ara[0][0] * (ara[1][1] * ara[2][2] - ara[1][2] * ara[2][1]);
	double v2 = ara[0][1] * (ara[1][0] * ara[2][2] - ara[1][2] * ara[2][0]);
	double v3 = ara[0][2] * (ara[1][0] * ara[2][1] - ara[1][1] * ara[2][0]);
	return v1 - v2 + v3;
}


struct Triangle: public Object
{
    Point a,b,c;

    Triangle(){

    }

    Triangle(Point a, Point b, Point c)
    {
        this->a = a;
        this->b = b;
        this->c = c;
    }

    virtual Ray getNormal(Point point, Ray incidentRay)
    {
        Point normal = (b-a)^(c-a);
        normal.normalize();
        
        if(incidentRay.dir*normal < 0){
            return Ray(point, -normal);
        }
        else{
            return Ray(point, normal);
        }
    }

    virtual void draw(){
        glColor3f(color.r, color.g, color.b);
        glBegin(GL_TRIANGLES);
        {
            glVertex3f(a.x, a.y, a.z);
            glVertex3f(b.x, b.y, b.z);
            glVertex3f(c.x, c.y, c.z);
        }
        glEnd();
    }

    virtual double intersectHelper(Ray ray, Color &color, int level){

        double betaMat[3][3] = {
				{a.x - ray.origin.x, a.x - c.x, ray.dir.x},
				{a.y - ray.origin.y, a.y - c.y, ray.dir.y},
				{a.z - ray.origin.z, a.z - c.z, ray.dir.z}
			};
			double gammaMat[3][3] = {
				{a.x - b.x, a.x - ray.origin.x, ray.dir.x},
				{a.y - b.y, a.y - ray.origin.y, ray.dir.y},
				{a.z - b.z, a.z - ray.origin.z, ray.dir.z}
			};
			double tMat[3][3] = {
				{a.x - b.x, a.x - c.x, a.x - ray.origin.x},
				{a.y - b.y, a.y - c.y, a.y - ray.origin.y},
				{a.z - b.z, a.z - c.z, a.z - ray.origin.z}
			};
			double AMat[3][3] {
				{a.x - b.x, a.x - c.x, ray.dir.x},
				{a.y - b.y, a.y - c.y, ray.dir.y},
				{a.z - b.z, a.z - c.z, ray.dir.z}
			};

        double Adet = determinant(AMat);
        double beta = determinant(betaMat) / Adet;
        double gamma = determinant(gammaMat) / Adet;
        double t = determinant(tMat) / Adet;

        if (beta + gamma < 1 && beta > 0 && gamma > 0 && t > 0){
            return t;
        }
        else{
            return -1;
        }
    }

    // input stream
    friend istream& operator>>(istream &in, Triangle &t)
    {
        in >> t.a >> t.b >> t.c; // 3 vertices
        in >> t.color.r >> t.color.g >> t.color.b; // color
        for(int i = 0; i < 4; i++) in >> t.coefficients[i];
        in >> t.shine;
        return in;
    }
};

struct Sphere : public Object{

        Sphere(){
        }

		Sphere(Point center, double radius){
			reference_point = center;
			length = radius;
		}

        virtual Ray getNormal(Point point, Ray incidentRay){
            return Ray(point, point - reference_point);
        }

		virtual void draw(){
            int stacks = 30;
			int slices = 20;

			Point points[100][100];
			int i, j;
			double h, r;
			// generate points
			for (i = 0; i <= stacks; i++)
			{
				h = length * sin(((double)i / (double)stacks) * (pi / 2));
				r = length * cos(((double)i / (double)stacks) * (pi / 2));
				for (j = 0; j <= slices; j++)
				{
					points[i][j].x = r * cos(((double)j / (double)slices) * 2 * pi);
					points[i][j].y = r * sin(((double)j / (double)slices) * 2 * pi);
					points[i][j].z = h;
				}
			}
			//draw quads using generated points
			for (i = 0; i < stacks; i++)
			{
				glPushMatrix();
				glTranslatef(reference_point.x, reference_point.y, reference_point.z);
				glColor3f(color.r, color.g, color.b);
				for (j = 0; j < slices; j++)
				{
					glBegin(GL_QUADS);
					{
						//upper hemisphere
						glVertex3f(points[i][j].x, points[i][j].y, points[i][j].z);
						glVertex3f(points[i][j + 1].x, points[i][j + 1].y, points[i][j + 1].z);
						glVertex3f(points[i + 1][j + 1].x, points[i + 1][j + 1].y, points[i + 1][j + 1].z);
						glVertex3f(points[i + 1][j].x, points[i + 1][j].y, points[i + 1][j].z);
						//lower hemisphere
						glVertex3f(points[i][j].x, points[i][j].y, -points[i][j].z);
						glVertex3f(points[i][j + 1].x, points[i][j + 1].y, -points[i][j + 1].z);
						glVertex3f(points[i + 1][j + 1].x, points[i + 1][j + 1].y, -points[i + 1][j + 1].z);
						glVertex3f(points[i + 1][j].x, points[i + 1][j].y, -points[i + 1][j].z);
					}
					glEnd();
				}
				glPopMatrix();
			}
		}

        virtual double intersectHelper(Ray ray, Color &color, int level){

            ray.origin = ray.origin - reference_point; // adjust ray origin
            
            double a = 1;
            double b = 2 * (ray.dir*ray.origin);
            double c = (ray.origin*ray.origin) - (length*length);

            

            double discriminant = pow(b, 2) - 4 * a * c;
            double t = -1;
            if (discriminant < 0){
                t = -1;
            }
            else{
                
                if(fabs(a) < 1e-5)
                {
                    t = -c/b;
                    return t;
                }

                double t1 = (-b - sqrt(discriminant)) / (2 * a);
                double t2 = (-b + sqrt(discriminant)) / (2 * a);

                if(t2<t1) swap(t1, t2);

                if (t1 > 0){
                    t = t1;
                }
                else if (t2 > 0){
                    t = t2;
                }
                else{
                    t = -1;
                }
            }

            return t;
            // if(level == 0) return t;
            
            // Point intersectionPoint = ray.origin + ray.dir * t;
            // Point normal = intersectionPoint - reference_point;
        }

        // input stream
        friend std::istream& operator>>(std::istream& in, Sphere& s){
            in >> s.reference_point >> s.length; // center and radius
            in >> s.color.r >> s.color.g >> s.color.b; // color
            for(int i = 0; i < 4; i++) in >> s.coefficients[i];
            in >> s.shine;
            return in;
        }
};

struct Floor : public Object{

    int tiles;

    Floor(){
        tiles = 1;
    }

    Floor(int floorWidth,int tileWidth){
        tiles = floorWidth / tileWidth;
        reference_point = Point(-floorWidth / 2, -floorWidth / 2, 0);
        length = tileWidth;
    }

    virtual Color getColorAt(Point point){

        int tileX = (point.x - reference_point.x) / length;
		int tileY = (point.y - reference_point.y) / length;

        if(tileX<0 || tileX>=tiles || tileY<0 || tileY>=tiles){
            return Color(0,0,0);
        }

		if (((tileX + tileY) % 2) == 0)
		{
			return Color(1,1,1);
		}
		else
		{
            // cout<<"Black"<<endl;
			return Color(0,0,0);
		}
    }

    virtual Ray getNormal(Point point, Ray incidentRay){
        if(incidentRay.dir.z > 0) return Ray(point, Point(0, 0, 1));
        else return Ray(point, Point(0, 0, -1));
    }

    virtual void draw(){
        for (int i = 0; i < tiles; i++)
		{
			for (int j = 0; j < tiles; j++)
			{
				if (((i + j) % 2) == 0) glColor3f(1, 1, 1);
				else glColor3f(0, 0, 0);

				glBegin(GL_QUADS);
				{
					glVertex3f(reference_point.x + i * length, reference_point.y + j * length, 0);
					glVertex3f(reference_point.x + (i + 1) * length, reference_point.y + j * length, 0);
					glVertex3f(reference_point.x + (i + 1) * length, reference_point.y + (j + 1) * length, 0);
					glVertex3f(reference_point.x + i * length, reference_point.y + (j + 1) * length, 0);
				}
				glEnd();
			}
		}
    }

    virtual double intersectHelper(Ray ray, Color &color, int level){
        Point normal = Point(0, 0, 1);
        double dotP = normal * ray.dir;
        
        if (round(dotP * 100) == 0)
			return -1;

        double t = -(normal * ray.origin) / dotP;

        Point p = ray.origin + ray.dir * t;

        if(p.x <= reference_point.x || p.x >= abs(reference_point.x) && p.y <= reference_point.y && p.y >= abs(reference_point.y)){
            return -1;
        }
        
        return t;
    }
};