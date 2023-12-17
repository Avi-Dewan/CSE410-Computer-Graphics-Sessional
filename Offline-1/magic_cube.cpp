#include<bits/stdc++.h>
#include "Camera.cpp"


#ifdef __linux__
    #include<GL/glut.h>
#elif WIN32
    #include <windows.h>
    #include <glut.h>
#endif

using namespace std;


#define pi (2*acos(0.0))

const double side = 50;
const double circleRadius = side /sqrt(3);

double rad = 0;
double currentSide = side;
int counter = 0;
int rotation = 0;

Camera camera(Point(0, -80, 40));


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





void square2D(double a) {
    glBegin(GL_QUADS);{
		glVertex3f( a, a,0);
		glVertex3f( a,-a,0);
		glVertex3f(-a,-a,0);
		glVertex3f(-a, a,0);
	}glEnd();
}

void triangle_upper(double a) {

    glBegin(GL_TRIANGLES);{
            glVertex3f(a, 0, 0);
            glVertex3f(0, a, 0);
            glVertex3f(0, 0, a);
    }glEnd();

}

void triangle_lower(double a) {

    glBegin(GL_TRIANGLES);{
        glVertex3f(a, 0, 0);
        glVertex3f(0, a, 0);
        glVertex3f(0, 0, -a);
    }glEnd();  

}

void octaHedron(double radius, double a, double shift) {

    for(int i = 0; i < 4; i++) {

        if(i%2) glColor3f(1, 0, 1);
        else glColor3f(0, 1, 1);

        glPushMatrix();
        glRotatef(i*90, 0, 0, 1);
		glTranslatef(radius/sqrt(3), radius/sqrt(3), radius/sqrt(3));
        triangle_upper(a);
		glPopMatrix();

    }

    for(int i = 0; i < 4; i++) {
        if(i%2) glColor3f(0, 1, 1);
        else glColor3f(1, 0, 1);

        glPushMatrix();
        glRotatef(i*90, 0, 0, 1);
		glTranslatef(radius/sqrt(3), radius/sqrt(3), -radius/sqrt(3));
        triangle_lower(a);
		glPopMatrix();

    }

}


vector<vector<Point>> generateSpherePartPoints(int s) {

    vector<vector<Point>> points;

    int row_points = 1 << s + 1; // 2^n + 1

    double theta, angle_in_degree;

	for(int i = 0; i < row_points; i++) {

		vector<Point> stack_points;

        // Latitudal angle and normal of latitudal plan alnog Z axis
        angle_in_degree = (double) (45.0f - 90.0f * i / (row_points - 1));
        theta = angle_in_degree * (pi/180); 
        Point lat_normal = Point(-sin(theta), cos(theta), 0);

		for(int j = 0; j < row_points; j++) {
			
			// Longitudinal angle and normal of latitudal plan alnog Y axis
            angle_in_degree = (double) (-45.0f + 90.0f * j / (row_points - 1));
            theta = angle_in_degree * (pi/180); 
            Point long_normal = Point(-sin(theta), 0, -cos(theta));

            // direction vector of intersected line, n1 x n2
            Point dir = (long_normal*lat_normal).normalize();

            stack_points.push_back(dir);
		}

		points.push_back(stack_points);
	}

    return points;
}

void twoSphereParts_inOneAxis(double radius, int s, double shift) {
   
    vector<vector<Point>> points = generateSpherePartPoints(s);

    int row_points = points.size();

    
    glPushMatrix();
	glTranslatef(shift, 0,0);
	glScalef(radius, radius, radius);

    for(int i = 0; i < row_points -1; i++){
		for(int j = 0; j < row_points - 1; j++){
			glBegin(GL_QUADS);{
				glVertex3f(points[i][j].x, points[i][j].y, points[i][j].z);
				glVertex3f(points[i][j+1].x, points[i][j+1].y, points[i][j+1].z);
				glVertex3f(points[i+1][j+1].x, points[i+1][j+1].y, points[i+1][j+1].z);
				glVertex3f(points[i+1][j].x, points[i+1][j].y, points[i+1][j].z);
			}glEnd();
		}
	}
	
	glPopMatrix();

	glPushMatrix();
	glTranslatef(-shift, 0, 0);
	glScalef(radius, radius, radius);
	glRotatef(180, 0, 0, 1);

	for(int i = 0; i < row_points -1; i++){
		for(int j = 0; j < row_points - 1; j++){
			glBegin(GL_QUADS);{
				glVertex3f(points[i][j].x, points[i][j].y, points[i][j].z);
				glVertex3f(points[i][j+1].x, points[i][j+1].y, points[i][j+1].z);
				glVertex3f(points[i+1][j+1].x, points[i+1][j+1].y, points[i+1][j+1].z);
				glVertex3f(points[i+1][j].x, points[i+1][j].y, points[i+1][j].z);
			}glEnd();
		}
	}

	glPopMatrix();
    
} 

void rotate_color_faces(int face) {
    
    if(face == 0) {
		glColor3f(1,0,0);
    } else if(face == 1) {
        glRotatef(90,0,1,0);
		glColor3f(0,0,1);
    } else {
        glRotatef(90,0,0,1);
		glColor3f(0,1,0);
    }

}


void sphereSixParts(double radius, double shift) {
    for(int i = 0; i < 3; i++) {

        rotate_color_faces(i);
		twoSphereParts_inOneAxis(radius, 6, shift);

    }
}


vector<Point> generateCylinderPoints(double rad, int slices) {

    vector<Point> points;
    double angle_in_degree, theta;

    for(int i = 0; i <= slices; i++){
        angle_in_degree = -45 +  (double)i / slices * 90 ;
        theta = angle_in_degree * (pi/180);
        points.push_back(Point(rad*cos(theta), rad*sin(theta) , 0));
    }

    return points;
}

void cylinder(double rad, double height, int slices) {

    if(height <= 0) return;

    vector<Point> points = generateCylinderPoints(rad, slices);

    for(int i = 0; i < slices; i++) {
        glBegin(GL_QUADS);{
            glVertex3f(points[i].x, points[i].y, -height);
            glVertex3f(points[i].x, points[i].y, height);
            glVertex3f(points[i+1].x, points[i+1].y, height);
            glVertex3f(points[i+1].x, points[i+1].y, -height);
        }glEnd();
    }

}

void draw12Cylinders(double side, double radius, int slices) {

    double height = (side)*sqrt(2) ;
    double centerCylinder = (side)/2;

    int j;
    
    glColor3f(1,1,0);

    for(int i = 0; i < 12; i++) {

        if( i < 4) {

            // drawing the cylinder

            j = i;
            glPushMatrix();
            glRotatef(j*90, 0,1,0);
            glTranslatef(centerCylinder, 0, -centerCylinder);
            glRotatef(45, 0, 1, 0);
            cylinder(radius, height/2, slices);
            glPopMatrix();
        

        } else if( i < 8) {

            glPushMatrix();
            glRotatef(90, 1,0,0);

            // drawing the cylinder

            j = i - 4;
            glPushMatrix();
            glRotatef(j*90, 0,1,0);
            glTranslatef(centerCylinder, 0, -centerCylinder);
            glRotatef(45, 0, 1, 0);
            cylinder(radius, height/2, slices);
            glPopMatrix();


            glPopMatrix();

        } else {

            glPushMatrix();
            glRotatef(90, 0,0,1);

            // drawing the cylinder

            j = i - 8;
            glPushMatrix();
            glRotatef(j*90, 0,1,0);
            glTranslatef(centerCylinder, 0, -centerCylinder);
            glRotatef(45, 0, 1, 0);
            cylinder(radius, height/2, slices);
            glPopMatrix();


            glPopMatrix();
        }
    }
}


void init() {


	//codes for initialization

    counter = 0;
	rad = counter / 100.0 * circleRadius;
	currentSide = side - counter / 100.0 * side;

	//clear the screen
	glClearColor(0,0,0,0);

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


    // axes(); 
	// axes3D();

    glPushMatrix();
	glRotatef(rotation, 0, 0,1);
    octaHedron(rad, currentSide, side);
    sphereSixParts(rad, currentSide);
    draw12Cylinders(currentSide, rad, 24);
	glPopMatrix();

    // glFlush();
    //ADD this line in the end --- if you use double buffer (i.e. GL_DOUBLE)
	glutSwapBuffers();

}



void idle() {
    glutPostRedisplay();
}


void keyboardHandler(unsigned char key, int x,int y){

	switch(key){

        case ',': case '<':
            counter = min(counter + 5, 100);
            break;

        case '.': case '>':
            counter = max(counter - 5, 0);
            break;
		
		case 'a': case 'A':
			rotation = (rotation - 2 + 360) % 360;
			break;

		case 'd': case 'D':
			rotation = (rotation + 2) % 360;
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
		
		default:
			break;
	}

    rad = counter / 100.0 * circleRadius;
	currentSide = side - counter / 100.0 * side;
}



void specialKeyHandler(int key, int x,int y){
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



int main(int argc, char **argv) {


    // making a window

    glutInit(&argc, argv);
    glutInitWindowPosition(0, 0);
    glutInitWindowSize(600, 600);

	glutInitDisplayMode(GLUT_DEPTH | GLUT_DOUBLE | GLUT_RGB);	//Depth, Double buffer, RGB color
    glutCreateWindow("Second OpenGL project");

    init();
    
	glEnable(GL_DEPTH_TEST);	//enable Depth Testing

    glutDisplayFunc(display);

    glutKeyboardFunc(keyboardHandler);
	glutSpecialFunc(specialKeyHandler);

    glutIdleFunc(idle);

    
    glutMainLoop();
     
    return 0;
}