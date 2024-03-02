#include<bits/stdc++.h>
#include "bitmap_image.hpp"
#include <GL/glut.h>
#include "1905120_lights.h"
 
using namespace std;


#define pi (2*acos(0.0))


class Ray{
public:
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


        Point getIntersectionPoint(Ray ray, double t){
            return ray.origin + ray.dir*t;
        }

        void addAmbiance(Color &color, Color intersectionPointColor){
            color.r = intersectionPointColor.r * coefficients[0];
            color.g = intersectionPointColor.g * coefficients[0];
            color.b = intersectionPointColor.b * coefficients[0];
        }

        bool isRayObscured(Point lightPos, Point intersectionPoint, Ray lightRay){

                double t = (intersectionPoint - lightPos).absolute_value();

                // if(t2 < 1e-5) continue;

                bool obscured = false;

                for(Object *obj : objects){

                    double t2 = obj->intersect_T(lightRay, color, 0);

                    if(t2 > 0 && t2 + 1e-5 < t){
                        obscured = true;
                        break;
                    }
                }

                return obscured;
        }

        double getLambertValue( Ray lightRay, Ray normal){
            
            double lambertValue = max(0.0, -lightRay.dir*normal.dir);
            return lambertValue;
        }

        double getPhongValue(Ray ray, Ray lightRay, Ray normal, Point intersectionPoint){
            
            Ray reflection = Ray(intersectionPoint, lightRay.dir - normal.dir*2*(lightRay.dir*normal.dir));
            double phong = max(0.0,-ray.dir*reflection.dir);

            return phong;
        }

        void addDiffuedAndSpecularColor(Color pl_color, Color &color, Color intersectionPointColor, double lambertValue, double phong){
            // lights[i]->color works as the source intensity, Is here -> add diffused and specular color
            
            color.r += ( pl_color.r * coefficients[1] * lambertValue * intersectionPointColor.r
                     + pl_color.r * coefficients[2] * pow(phong,shine) * intersectionPointColor.r);

            color.g += ( pl_color.g * coefficients[1] * lambertValue * intersectionPointColor.g
                     + pl_color.g * coefficients[2] * pow(phong,shine) * intersectionPointColor.g);

            color.b += ( pl_color.b * coefficients[1] * lambertValue * intersectionPointColor.b
                     + pl_color.b * coefficients[2] * pow(phong,shine) * intersectionPointColor.b);

        }


        bool exceedsCutOffAngle(Point lightDir, SpotLight* spotlight) {

            double angle = acos( (lightDir*spotlight->dir )/(lightDir.absolute_value() * spotlight->dir.absolute_value())) * (180.0/pi);
        
            return (fabs(angle) > spotlight->cutoffAngle);
        }

        Ray getReflectedRay(Point intersectionPoint, Ray ray, Ray normal) {

            Point reflectedDir = ray.dir - normal.dir*2*(ray.dir*normal.dir);
            Ray reflectionRay = Ray(intersectionPoint, reflectedDir);

            // to avoid self intersection slightly move the origin of the reflected ray towards the direction of the ray
            reflectionRay.origin = reflectionRay.origin + reflectionRay.dir*1e-5;

            return reflectionRay;
        }

        int getNearestObjectIndexReflection(Ray reflectionRay, Color &color) {

            int nearestObjectIndex = -1;
            double t = -1,tMin = 1e9;

            int noOfObjects = objects.size();

            for(int k=0; k < noOfObjects; k++) {
                t = objects[k]->intersect(reflectionRay, color, 0);
                if(t> 0 && t<tMin) {
                    tMin = t , nearestObjectIndex = k;
                }
            }

            return nearestObjectIndex;
        }

		virtual double intersect(Ray ray, Color &color, int level)
        {
            double t = intersect_T(ray, color, level);

            if(level == 0) return t;

            // find intersection point and it's color
            Point intersectionPoint = getIntersectionPoint(ray, t);
            Color intersectionPointColor = getColorAt(intersectionPoint);

            // add ambiance input the color
            addAmbiance(color, intersectionPointColor);

            // for each point light source

            for(int i = 0; i < lights.size(); i++){

                Point lightPos = lights[i]->pos;

                // find direction of light and normalize it
                Point lightDir = intersectionPoint - lightPos;
                lightDir.normalize(); 
                
                // cast ray from light position to intersection point
                Ray lightRay = Ray(lightPos, lightDir);

                // calculate normal at intersectionPoint
                Ray normal = getNormal(intersectionPoint, lightRay);

                
                // check if incedent ray is not obscured by any other object
                // if not, then add the impact of the light source
                if(!isRayObscured(lightPos, intersectionPoint, lightRay)){
                    
                    // lambert value
                    double lambertValue = getLambertValue(lightRay, normal);
                    
                    // phong value
                    double phong = getPhongValue(ray, lightRay, normal, intersectionPoint);
                    // lights[i]->color works as the source intensity, Is here -> add diffused and specular color

                    addDiffuedAndSpecularColor(lights[i]->color, color, intersectionPointColor, lambertValue, phong);
                    
                }
            }

            /* For spotlights 
             * same calculation as above, unless
             *  the ray cast from light_pos to intersectionPoint
             *  exceeds cutoff-angle for the light source
             */

            for(int i = 0; i < spotlights.size(); i++){

                Point lightPos = spotlights[i]->pointLight.pos;

                // find direction of light and normalize it
                Point lightDir = intersectionPoint - lightPos;
                lightDir.normalize(); 

               

                if(exceedsCutOffAngle(lightDir, spotlights[i])) continue;
                
                // cast ray from light position to intersection point
                Ray lightRay = Ray(lightPos, lightDir);

                // calculate normal at intersectionPoint
                Ray normal = getNormal(intersectionPoint, lightRay);

                
                // check if incedent ray is not obscured by any other object
                // if not, then add the impact of the light source
                if(!isRayObscured(lightPos, intersectionPoint, lightRay)){
                    
                    // lambert value
                    double lambertValue = getLambertValue(lightRay, normal);
                    
                    // phong value
                    double phong = getPhongValue(ray, lightRay, normal, intersectionPoint);
                    // spotlights[i]->color works as the source intensity, Is here -> add diffused and specular color

                    addDiffuedAndSpecularColor(spotlights[i]->pointLight.color, color, intersectionPointColor, lambertValue, phong);
                    
                }
            }

            // Recursive Reflection
            /*
                Do the same calculations as camera rays (i.e. the ones cast from
                the eye). The recursion_level (given as input) controls how many times a ray will be reflected when incident upon objects (shapes).
            */

            if(level < recursionLevel){
                // find normal at intersectionPoint
                Ray normal = getNormal(intersectionPoint,ray);

                // find reflected ray
                Ray reflectionRay = getReflectedRay(intersectionPoint, ray, normal);

                // find nearest intersection object and do recursive call

                int nearestObjectIndex = getNearestObjectIndexReflection(reflectionRay, color);

                if(nearestObjectIndex != -1)
                {
        
                    Color colorReflection(0,0,0); // refelction color
                    
                    double t = objects[nearestObjectIndex]->intersect(reflectionRay, colorReflection, level+1);

                    // colorReflection will be updated while input the subsequent call
                    // update color 
                    
                    color.r += colorReflection.r * coefficients[3];
                    color.g += colorReflection.g * coefficients[3];
                    color.b += colorReflection.b * coefficients[3];

                }
            }

            return t;
        }

        virtual void draw() {

        }

		virtual double intersect_T(Ray ray, Color &color, int level) {
            return -1;
        }
        
        virtual Ray getNormal(Point point, Ray incidentRay) = 0;

        // destructor
        virtual ~Object(){
            coefficients.clear();
            coefficients.shrink_to_fit();
        }
};

class Triangle: public Object{
public:
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

        double dot = incidentRay.dir*normal;
        
        if( dot < 0) return Ray(point, -normal);
        else return Ray(point, normal);
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

    virtual double intersect_T(Ray ray, Color &color, int level){

        Matrix mat1 = Matrix(
            a.x - ray.origin.x, a.x - c.x, ray.dir.x,
            a.y - ray.origin.y, a.y - c.y, ray.dir.y,
            a.z - ray.origin.z, a.z - c.z, ray.dir.z);

        Matrix mat2 = Matrix(
            a.x - b.x, a.x - ray.origin.x, ray.dir.x,
            a.y - b.y, a.y - ray.origin.y, ray.dir.y,
            a.z - b.z, a.z - ray.origin.z, ray.dir.z);

        Matrix mat3 = Matrix(
            a.x - b.x, a.x - c.x, a.x - ray.origin.x,
            a.y - b.y, a.y - c.y, a.y - ray.origin.y,
            a.z - b.z, a.z - c.z, a.z - ray.origin.z);

        Matrix mat = Matrix (
            a.x - b.x, a.x - c.x, ray.dir.x,
            a.y - b.y, a.y - c.y, ray.dir.y,
            a.z - b.z, a.z - c.z, ray.dir.z);

        double determin = mat.determinant();

        if(fabs(determin) < 1e-5) return -1;

        double beta =  mat1.determinant() / determin;
        double gamma = mat2.determinant() / determin;
        double t = mat3.determinant() / determin;

        if (beta + gamma < 1 && beta > 0 && gamma > 0 && t > 0){
            return t;
        }
        else{
            return -1;
        }
    }

    friend istream& operator>>(istream &input, Triangle &t)
    {
        input >> t.a >> t.b >> t.c; // 3 vertices
        input >> t.color.r >> t.color.g >> t.color.b; // colors
        for(int i = 0; i < 4; i++) input >> t.coefficients[i];
        input >> t.shine;
        return input;
    }
};


class General : public Object{
public:
    double A,B,C,D,E,F,G,H,I,J;

    General(){

    }

    virtual void draw(){
        return;
    }

    virtual Ray getNormal(Point point, Ray incidentRay)
    {
        double nomal_x = 2*A*point.x + D*point.y + E*point.z + G;
        double nomal_y = 2*B*point.y + D*point.x + F*point.z + H;
        double nomal_z = 2*C*point.z + E*point.x + F*point.y + I;

        Point normal(nomal_x, nomal_y, nomal_z);

        return Ray(point, normal);
    }

    bool checkOk(Point point)
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


    virtual double intersect_T(Ray ray, Color &color, int level){

        double x0 = ray.origin.x;
        double y0 = ray.origin.y;
        double z0 = ray.origin.z;

        double x1 = ray.dir.x;
        double y1 = ray.dir.y;
        double z1 = ray.dir.z;

        double a = A*x1*x1 + B*y1*y1 + C*z1*z1 + D*x1*y1 + E*x1*z1 + F*y1*z1;
        double b = 2*A*x0*x1 + 2*B*y0*y1 + 2*C*z0*z1 + D*(x0*y1 + x1*y0) + E*(x0*z1 + x1*z0) + F*(y0*z1 + y1*z0) + G*x1 + H*y1 + I*z1;
        double c = A*x0*x0 + B*y0*y0 + C*z0*z0 + D*x0*y0 + E*x0*z0 + F*y0*z0 + G*x0 + H*y0 + I*z0 + J;

        double discriminant = b*b - 4*a*c;

        if(discriminant < 0) return -1;

        if(fabs(a) < 1e-5) return -c/b;
        
        double t1 = (-b - sqrt(discriminant))/(2*a);
        double t2 = (-b + sqrt(discriminant))/(2*a);

        if(t1 < 0 && t2 < 0) return -1;
        if(t2<t1) swap(t1,t2);
        if(t1 > 0) {
            Point intersectionPoint = getIntersectionPoint(ray, t1);

            if(checkOk(intersectionPoint)){
                return t1;
            }
        }
        if(t2 > 0) {
            Point intersectionPoint = getIntersectionPoint(ray, t2);
            if(checkOk(intersectionPoint)){
                return t2;
            }
        }

        return -1;

    }
    
    // input stream
    friend istream& operator>>(istream &input, General &gen)
    {
        input >> gen.A >> gen.B >> gen.C >> gen.D >> gen.E >> gen.F >> gen.G >> gen.H >> gen.I >> gen.J;

        input >> gen.reference_point >> gen.length >> gen.width >> gen.height;

        input >> gen.color.r >> gen.color.g >> gen.color.b; 

        for(int i = 0; i < 4; i++) input >> gen.coefficients[i];

        input >> gen.shine;

        return input;
    }
   

};

class Sphere : public Object{
public:
        Sphere(){
        }

		Sphere(Point center, double radius){
			reference_point = center;
			length = radius;
		}

        virtual Ray getNormal(Point point, Ray incidentRay){
            Point normal = point - reference_point;
            return Ray(point, normal);
        }
        
        vector<vector<Point>> generatePoints(int stacks, int slices) {

            vector<vector<Point>> points;

            double stack_angle = (double)(pi/2) / stacks;
            double slice_angle = (double)(2*pi) / slices;

            for(int i = 0; i <= stacks; i++) {
                double h = length*sin(i*stack_angle);
                double r = length*cos(i*stack_angle);

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


		virtual void draw(){
            int stacks = 30;
			int slices = 20;

            vector<vector<Point>> points = generatePoints(stacks, slices);
			
			//draw quads using generated points

			for (int i = 0; i < stacks; i++) {

				glPushMatrix();
				glTranslatef(reference_point.x, reference_point.y, reference_point.z);
				glColor3f(color.r, color.g, color.b);

				for (int j = 0; j < slices; j++) {
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

        virtual double intersect_T(Ray ray, Color &color, int level){

            ray.origin = ray.origin - reference_point; // adjust ray origin
            
            double a = 1;
            double b = 2 * (ray.dir * ray.origin);
            double c = (ray.origin * ray.origin) - (length*length);

            double discriminant = b*b-4 *a*c;

            double t = -1;
            
            if(discriminant > 0){
                
                if(fabs(a) < 1e-5){

                    t = -c/b;

                } else {

                    double t1 = (-b - sqrt(discriminant)) / (2 * a);
                    double t2 = (-b + sqrt(discriminant)) / (2 * a);

                    if(t2<t1) swap(t1, t2);

                    if (t1 > 0){
                        t = t1;
                    } else if (t2 > 0){
                        t = t2;
                    } else { 
                        t = -1;
                    }
                }
            }

            return t;
        }

        // input stream
        friend std::istream& operator>>(std::istream& input, Sphere& s){
            input >> s.reference_point >> s.length; // center and radius

            input >> s.color.r >> s.color.g >> s.color.b; // color

            for(int i = 0; i < 4; i++) input >> s.coefficients[i];

            input >> s.shine;

            return input;

        }
};

class Floor : public Object {
public:
    int noTiles;

    Floor(){
        int floorWidth = 400, tileWidth = 10;

        noTiles = floorWidth / tileWidth;

        reference_point = Point(-floorWidth / 2, -floorWidth / 2, 0);
        length = tileWidth;
    }

    virtual Color getColorAt(Point point){

        int X = (point.x - reference_point.x) / length;
		int Y = (point.y - reference_point.y) / length;

        if(X<0 || X >= noTiles || Y < 0 || Y >= noTiles){
            return Color(0,0,0);
        }

		if (((X + Y) % 2) == 0) {
			return Color(1,1,1);
		} 
		
        return Color(0,0,0);
		
    }

    virtual Ray getNormal(Point point, Ray incidentRay){

        if(incidentRay.dir.z > 0) return Ray(point, Point(0, 0, 1));
        else return Ray(point, Point(0, 0, -1));
    }

    void setColor(int x) {
        if (( x% 2) == 0) glColor3f(1, 1, 1);
		else glColor3f(0, 0, 0);
    }

    virtual void draw(){

        for (int i = 0; i < noTiles; i++) {

			for (int j = 0; j < noTiles; j++){

				setColor(i+j);

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

    virtual double intersect_T(Ray ray, Color &color, int level){
        Point normal = Point(0, 0, 1);
        double dotP = normal * ray.dir;
        
        if (fabs(dotP) < 1e-5) return -1;

        double t = -(normal * ray.origin) / dotP;
        return t;
    }
};