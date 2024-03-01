#include "1905120_classes.h"
#include<bits/stdc++.h>
#include "bitmap_image.hpp"

#ifdef __linux__
    #include<GL/glut.h>
#elif WIN32
    #include <windows.h>
    #include <glut.h>
#endif

using namespace std;

#define pi (2*acos(0.0))


// declaration
extern vector<Object*> objects;
extern vector<PointLight> pointLights;
extern vector<SpotLight> spotLights;

int levelOfRevursion;
int noOfPixels;
int noOfObjects;
int noOfPointLights;
int noOfSpotLights;

void loadData() {

    // want to load them from "scene.txt"

    ifstream cin("scene.txt");


    cin >> levelOfRevursion;
    cin >> noOfPixels;

    cin >> noOfObjects;


    double color[3];
    double coEfficients[4];
    int shininess;

    for(int i = 0; i < noOfObjects; i++) {
        string type;
        cin >> type;

        if(type == "sphere") {
            Point center;
            double radius;
            cin >> center.x >> center.y >> center.z >> radius;

            Sphere* sphere = new Sphere(center, radius);

            objects.push_back(sphere);
        }
        else if(type == "triangle") {
            Point a, b, c;
            cin >> a.x >> a.y >> a.z;
            cin >> b.x >> b.y >> b.z;
            cin >> c.x >> c.y >> c.z;

            Triangle* triangle = new Triangle(a, b, c);
            objects.push_back(triangle);

        } else if(type == "general") {

            double A, B, C, D, E, F, G, H, I, J;
            Point reference_point;
            double height, width, length;


            cin >> A >> B >> C >> D >> E >> F >> G >> H >> I >> J;
            cin >> reference_point.x >> reference_point.y >> reference_point.z;
            cin >> height >> width >> length;

            General* general = new General(reference_point, height, width, length);

            general->setQuadricCoefficients(A, B, C, D, E, F, G, H, I, J);
            objects.push_back(general);
        }

        cin >> color[0] >> color[1] >> color[2];
        objects[i]->setColor(color[0], color[1], color[2]);

        cin >> coEfficients[0] >> coEfficients[1] >> coEfficients[2] >> coEfficients[3];
        objects[i]->setCoEfficients(coEfficients[0], coEfficients[1], coEfficients[2], coEfficients[3]);
        
        cin >> shininess;
    }

    Floor* floor = new Floor(1000, 20);
    objects.push_back(floor);

    cin >> noOfPointLights;

    for(int i = 0; i < noOfPointLights; i++) {
        PointLight pointLight;
        cin >> pointLight.light_pos.x >> pointLight.light_pos.y >> pointLight.light_pos.z;
        cin >> pointLight.color[0] >> pointLight.color[1] >> pointLight.color[2];
        pointLights.push_back(pointLight);
    }

    cin >> noOfSpotLights;

    for(int i = 0; i < noOfSpotLights; i++) {
        SpotLight spotLight;
        cin >> spotLight.point_light.light_pos.x >> spotLight.point_light.light_pos.y >> spotLight.point_light.light_pos.z;
        cin >> spotLight.point_light.color[0] >> spotLight.point_light.color[1] >> spotLight.point_light.color[2];
        cin >> spotLight.light_direction.x >> spotLight.light_direction.y >> spotLight.light_direction.z;
        cin >> spotLight.cutoff_angle;
        spotLights.push_back(spotLight);
    }

}



Camera camera(Point(0, 0, 150));

double windowWidth = 500, windowHeight = 500;
double viewAngle = 80;
int imageName = 1;



Point rectify_color(Point point){
	Point ans;

	if(point.x > 1) ans.x = 1;
	else if(point.x < 0) ans.x = 0;
	else ans.x = point.x;

	if(point.y > 1) ans.y = 1;
	else if(point.y < 0) ans.y = 0;
	else ans.y = point.z;

	if(point.z < 0) ans.z = 0;
	else if(point.z > 1) ans.z = 1;
	else ans.z = point.z;

	return ans;
}


void capture(){

  cout << "capturing" << endl;
  
  // initialize bitmap image and set background color
  bitmap_image image(noOfPixels, noOfPixels);

  for(int i = 0; i < noOfPixels; i++) {
		for(int j=0;j<noOfPixels;j++){
			image.set_pixel(i, j, 0, 0, 0);
    }
  }

  double planeDistance = ( windowHeight / 2.0 ) / tan(viewAngle*(pi/180)/ 2.0);
  
  Point topleft = camera.camera_pos + camera.direction * planeDistance ;
                  - camera.right_vector * (windowWidth/2.0)
                  + camera.up_vector * (windowHeight / 2.0);

  double du = windowWidth / noOfPixels;
  double dv = windowHeight / noOfPixels;


  // choose middle of the grid cell
  topleft = topleft + camera.right_vector * (0.5*du) - camera.up_vector * (0.5*dv);

  for(int i = 0; i < noOfPixels;i++){
    for(int j = 0; j < noOfPixels; j++){
      Point curPixel = topleft + camera.right_vector * (du * i) - camera.up_vector * dv * j;

      Ray ray(camera.camera_pos, (curPixel - camera.camera_pos));
			
      Point color(0,0,0);

      double tMin = -1;
      Object *nearest = nullptr;
      for(auto i: objects){
        double t  = i->intersect(&ray, &color, 0);
        if(t > 0 && (nearest == nullptr || t < tMin)){
          tMin = t;
          nearest = i;
        }
      }
      if(nearest != nullptr){
        double t = nearest->intersect(&ray, &color, 1);
				color = rectify_color(color);
        image.set_pixel(i, j, 255*color.x, 255*color.y, 255*color.z);
      }

    }
  }

  image.save_image("outputs/Output_1"+to_string(imageName)+".bmp");
  // imageName++;
	cout << "saved" << endl;
}



void axes3D()
{
	
	glBegin(GL_LINES);{

		glColor3f(1.0, 0.0, 0.0);

		glVertex3f( 100,0,0);
		glVertex3f(-100,0,0);

		glVertex3f(0,-100,0);
		glVertex3f(0, 100,0);

		glColor3f(1.0, 1.0, 0.0);

		glVertex3f(0,0, 100);
		glVertex3f(0,0,-100);
	}glEnd();
}





void display() {

    // printf("We are in display dunction, counter: %d\n", ++counter);

	//clear the display
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glClearColor(0,0,0,0);	//color black
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glMatrixMode(GL_MODELVIEW);

	glLoadIdentity();

    camera.look();

	glMatrixMode(GL_MODELVIEW);

	// cout << camera.camera_pos.x << " " << camera.camera_pos.y << " " << camera.camera_pos.z << endl;
    

	// axes3D();


    for(int i =0; i < objects.size(); i++) {
        objects[i]->draw();
    }


	glutSwapBuffers();
}



void idle() {
        
    // if(animate) {
    //     counter++;
    // }

	// if(!sphere.isManual())
	// 	sphere.forward();

    glutPostRedisplay();
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

		case 'w':
			camera.upWithoutReferenceChange();
			break;
		case 's':
			camera.downWithoutReferenceChange();
			break;

		case 'j': case 'J':
			// sphere.theta = (sphere.theta + 20) % 360;
			break;

		case 'l': case 'L':
			// sphere.theta = (sphere.theta - 20 + 360)%360;
			break;

		case 'i': case 'I':	
			// if(sphere.isManual()){
			// 	sphere.forward();
			// }
			break;

		case 'k': case 'K':
			// if(sphere.isManual()){
			// 	sphere.backward();
			// }
			break;

		case ' ':
			// sphere.mode = 1 - sphere.mode;
			break;
		
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



void init() {


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
	gluPerspective(viewAngle,	1,	1,	1000.0);
	//field of view in the Y (vertically)
	//aspect ratio that determines the field of view in the X direction (horizontally)
	//near distance
	//far distance
}




int main(int argc, char **argv) {

    loadData();


    // making a window

    glutInit(&argc, argv);
    glutInitWindowPosition(0, 0);
    glutInitWindowSize(windowWidth, windowHeight);

	glutInitDisplayMode(GLUT_DEPTH | GLUT_DOUBLE | GLUT_RGB);	//Depth, Double buffer, RGB color
    glutCreateWindow("First OpenGL project");

    init();
    
	glEnable(GL_DEPTH_TEST);	//enable Depth Testing

    glutDisplayFunc(display);

    glutKeyboardFunc(keyboardHandler);
	glutSpecialFunc(specialKeyListener);

    glutIdleFunc(idle);

    
    glutMainLoop();
     
    return 0;
}
