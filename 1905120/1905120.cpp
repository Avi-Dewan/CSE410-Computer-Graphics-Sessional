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

		glColor3f(1.0, 1.0, 1.0);

		glVertex3f( 100,0,0);
		glVertex3f(-100,0,0);

		glVertex3f(0,-100,0);
		glVertex3f(0, 100,0);


		glVertex3f(0,0, 100);
		glVertex3f(0,0,-100);
	}glEnd();
}

vector<Point> generatePoints(int segments, double radius) {

    vector<Point> points;

    for(int i = 0; i <= segments; i++) {

        Point point;
        point.x = radius * cos((i *1.0/ segments) * 2 * pi);
        point.y = radius * sin((i*1.0 / segments) * 2 * pi);

        points.push_back(point);
    }

    return points;
}

void circle2D(double radius, int segments) {

    int i;
    glColor3f(1, 1, 1);

    //generate points

    vector<Point> points = generatePoints(segments, radius);
    
    //draw segments using generated points
    for(i = 0; i < segments; i++) {
        glBegin(GL_LINES);
        {
            glVertex3f(points[i].x, points[i].y, 0);
            glVertex3f(points[i+1].x, points[i+1].y, 0);
        }
        glEnd();
    }
}

void square3D(double i) {
    double a = 5;

    glBegin(GL_QUADS);{
		glVertex3f( a+i, a+i,-80);
		glVertex3f( a+i,-a-i,-80);
		glVertex3f(-a-i,-a-i,-80);
		glVertex3f(-a-i, a+i,-80);
	}glEnd();
} 

void sixSqures(double radius, double a, double shift) {

     for(int i = 0; i < 6; i++) {
         glColor3f(1, 1, 1);
       

        glPushMatrix();
        glRotatef(i*30, 0, 0, 1);
        // glTranslatef()
        square3D(i*10*radius);
		glPopMatrix();

    }
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
	axes3D();
    
    glPushMatrix();
    circle2D(30, 100);
    glPopMatrix();
    
    glPushMatrix();
	glRotatef(rotation, 0, 0,1);
    // square3D();
    sixSqures(rad, currentSide, side);

	glPopMatrix();

    // glFlush();
    //ADD this line in the end --- if you use double buffer (i.e. GL_DOUBLE)
	glutSwapBuffers();

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