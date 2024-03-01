#include<bits/stdc++.h>
#include <GL/glut.h>
using namespace std;
#include "1905120_classes.h"
#include "bitmap_image.hpp"

#define pi (2*acos(0.0))

double cameraHeight;
double cameraAngle;
double angle;

int recursionLevel;
int imageHeight,imageWidth;
bitmap_image image;

vector <Object*> objects;
vector <Light*> lights;
vector <SpotLight*> spotlights;

int imageCount = 1;
int numSegments;

double windowWidth = 500, windowHeight = 500;
double viewAngle = 80;

// camera
Camera camera;


// draw axes
void drawAxes()
{
	glColor3f(1.0, 1.0, 1.0);
	glBegin(GL_LINES);
	{
		glVertex3f( 100,0,0);
		glVertex3f(-100,0,0);

		glVertex3f(0,-100,0);
		glVertex3f(0, 100,0);

		glVertex3f(0,0, 100);
		glVertex3f(0,0,-100);
	}
	glEnd();
	
}

int getNearestObjectIndex(Ray ray, Color &color) {
	double tMin = -1;
	double nearestObjectIndex = -1, t;

	int noOfObjects = objects.size();

	for(int k=0; k < noOfObjects; k++) {
		t = objects[k]->intersect(ray, color, 0);
		if(t > 0 && (nearestObjectIndex == -1 || t<tMin)) {
			tMin = t , nearestObjectIndex = k;
		}
	}

	return nearestObjectIndex;
}

void maxminColor(Color &color) {

	if(color.r > 1) color.r = 1;
	if(color.g > 1) color.g = 1;
	if(color.b > 1) color.b = 1;

	if(color.r < 0) color.r = 0;
	if(color.g < 0) color.g = 0;
	if(color.b < 0) color.b = 0;
}

void capture()
{
	cout<<"Capturing Image...."<<endl;

	image = bitmap_image(imageWidth, imageHeight);

	// initialize bitmap image and set background color to black
	for(int i=0; i < imageWidth; i++) {
		for(int j=0; j < imageHeight; j++) {
			image.set_pixel(i, j, 0, 0, 0);
		}
	}
	
	double planeDistance = (windowHeight / 2.0) / tan(((pi/2.0) * viewAngle) / (180.0));

	Point topLeft = camera.pos + (camera.look_vector * planeDistance) + (camera.up_vector * (windowHeight / 2.0)) - (camera.right_vector * (windowWidth / 2.0));

	double du = windowWidth*1.0 / imageWidth;
	double dv = windowHeight*1.0 / imageHeight;

	// Choose middle of the grid cell
	topLeft = topLeft + (camera.right_vector * 0.5 * du ) - (camera.up_vector * 0.5 * dv );

	int nearestObjectIndex = -1;

	for(int i = 0; i < imageWidth; i++) {
		for(int j = 0; j < imageHeight; j++)
		{
			// calculate current pixel
			Point pixel = topLeft + (camera.right_vector * du * i) - (camera.up_vector * dv * j);
			// cast ray from EYE to (curPixel-eye) direction
			Ray ray(camera.pos ,pixel-camera.pos);
			Color color;

			// find nearest object
			nearestObjectIndex = getNearestObjectIndex(ray, color);

			if(nearestObjectIndex != -1){
				color = Color(0,0,0);
				double t = objects[nearestObjectIndex]->intersect(ray,color, 1);
				maxminColor(color);
				image.set_pixel(i, j, 255*color.r, 255*color.g, 255*color.b);
			}
		}
	}

	image.save_image("Output_1"+to_string(imageCount++)+".bmp");
	
	cout<<"Image Saaved"<<endl;		
}

void keyboardHandler(unsigned char key, int x,int y){

	switch(key){
        case '0':
            capture();
            break;
		case '1':
			camera.lookLeft();
			break;
		
		case '2':
			camera.lookRight();
			break;

		case '3':
			camera.lookUp();
			break;
		
		case '4':
			camera.lookDown();
			break;

		case '5':
			camera.tiltCounterClockwise();
			break;

		case '6':
			camera.tiltClockwise();
			break;

		// case 'w':
		// 	camera.upWithoutReferenceChange();
		// 	break;
		// case 's':
		// 	camera.downWithoutReferenceChange();
		// 	break;
		
		default:
			break;
	}
}



void specialKeyListener(int key, int x,int y){
	switch(key){

		case GLUT_KEY_DOWN:	
			camera.backward();
			break;

		case GLUT_KEY_UP:		
			camera.forward();
			break;

		case GLUT_KEY_RIGHT:
			camera.right();
			break;

		case GLUT_KEY_LEFT:
			camera.left();
			break;

		case GLUT_KEY_PAGE_UP:
			camera.up();
			break;

		case GLUT_KEY_PAGE_DOWN:
			camera.down();
			break;

		case GLUT_KEY_INSERT:
			break;

		case GLUT_KEY_HOME:
			break;

		case GLUT_KEY_END:
			break;

		default:
			break;
	}
}



void display(){

	 // printf("We are in display dunction, counter: %d\n", ++counter);

	//clear the display
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glClearColor(0,0,0,0);	//color black
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glMatrixMode(GL_MODELVIEW);

	glLoadIdentity();

    camera.look();

	glMatrixMode(GL_MODELVIEW);

	//add objects


    for (int i=0; i<objects.size(); i++){
		Object *object = objects[i];
		object->draw();
	}

	for (int i=0; i<lights.size(); i++){
		lights[i]->draw();
	}

	for(int i=0;i<spotlights.size();i++){
		SpotLight* l = spotlights[i];
		// cout<<"Spotlight "<<spotlight<<endl
		spotlights[i]->draw();
	}

	glutSwapBuffers();
}


void idle(){
	//codes for any changes in Models, Camera
	glutPostRedisplay();
}

void loadData()
{
	ifstream in("scene.txt");
	in >> recursionLevel >> imageHeight;

	imageWidth = imageHeight;

	int objCount;
	in >> objCount;

	for(int i=0;i<objCount;i++)
	{
		string objType;
		in >> objType;

		Object *obj;

		if(objType == "sphere"){
			obj = new Sphere();
			in >> *((Sphere *)obj);
		}
		else if(objType == "triangle"){
			obj = new Triangle();
			in >> *((Triangle *)obj);
		}
		else if(objType == "general"){
			// obj = new Plane();
			obj = new General();
			in >> *((General *)obj);
		}
		else{
			cout<<objType<<" is not a valid object type"<<endl;
		}
		objects.push_back(obj);
	}

	int lightCount;
	in >> lightCount;

	for(int i=0;i<lightCount;i++){
		Light *light = new Light();
		in >> *light;
		lights.push_back(light);
	}

	int spotlightCount;
	in >> spotlightCount;

	for(int i=0;i<spotlightCount;i++){
		SpotLight *spotlight = new SpotLight();
		in >> *spotlight;
		spotlights.push_back(spotlight);
	}

        
	Object *floor;
	floor = new Floor(400, 10);
	floor->setColor(Color(0.5, 0.5, 0.5));

	vector <double> coefficients = {0.4, 0.2, 0.2, 0.2};

	floor->setCoefficients(coefficients);
	
	objects.push_back(floor);
	
	
}

void init(){
	//codes for initialization

	cameraHeight=150.0;
	cameraAngle=1.0;
	angle=0;
	numSegments = 36;

	loadData();
	

	//clear the screen
	glClearColor(0,0,0,0);

	/************************
	/ set-up projection here
	************************/
	//load the PROJECTION matrix
	glMatrixMode(GL_PROJECTION);

	//initialize the matrix
	glLoadIdentity();

	//give PERSPECTIVE parameters
	gluPerspective(80,	1,	1,	1000.0);
	//field of view in the Y (vertically)
	//aspect ratio that determines the field of view in the X direction (horizontally)
	//near distance
	//far distance
}

int main(int argc, char **argv){

	
	glutInit(&argc,argv);
	glutInitWindowSize(500, 500);
	glutInitWindowPosition(0, 0);
	glutInitDisplayMode(GLUT_DEPTH | GLUT_DOUBLE | GLUT_RGB);	//Depth, Double buffer, RGB color

	glutCreateWindow("Offline-3: Ray Tracing");

	init();

	glEnable(GL_DEPTH_TEST);	//enable Depth Testing

	glutDisplayFunc(display);	//display callback function
	glutIdleFunc(idle);		

	glutKeyboardFunc(keyboardHandler);
	glutSpecialFunc(specialKeyListener);

	glutMainLoop();		//The main loop of OpenGL

	objects.clear();
	objects.shrink_to_fit();

	lights.clear();
	lights.shrink_to_fit();

	spotlights.clear();
	spotlights.shrink_to_fit();

	return 0;
}