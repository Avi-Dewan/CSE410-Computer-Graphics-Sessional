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

void line2D_upper(int nSqre, double length) {

	double transelateAmount = 2*length;

	for(int i = 0; i < nSqre; i++) {
		glPushMatrix();
		glColor3f(1.0, 1.0, 1.0);
		glRotatef(45, 0, 0, 1);
		glTranslatef(i*transelateAmount, i*transelateAmount, 0);
		square2D(length);
		glPopMatrix();
	}

}

void line2D_lower(int nSqre, double length) {

	double transelateAmount = 2*length;

	for(int i = -(nSqre-1); i < 0; i++) {
		glPushMatrix();
		glColor3f(1.0, 1.0, 1.0);
		glRotatef(45, 0, 0, 1);
		glTranslatef(i*transelateAmount, i*transelateAmount, 0);
		square2D(length);
		glPopMatrix();
	}
}

void grid2D(int nSquare, double length) {
	
	double half_diagonal = length*sqrt(2);

	for(int i = 0; i < nSquare; i++) {
		glPushMatrix();
		glColor3f(1.0, 1.0, 1.0);
		glTranslatef(half_diagonal + i*2*half_diagonal, 0, 0);
		line2D_upper(nSquare, length);
		line2D_lower(nSquare, length);
		glPopMatrix();
	}

	for(int i = -(nSquare-1); i <= 0; i++) {
		glPushMatrix();
		glColor3f(1.0, 1.0, 1.0);
		glTranslatef(-half_diagonal + i*2*half_diagonal, 0, 0);
		line2D_upper(nSquare, length);
		line2D_lower(nSquare, length);
		glPopMatrix();
	}
}


void drawBoundary(double a) {

	glBegin(GL_QUADS);{
		glVertex3f(a, a, 0);
		glVertex3f(a, -a, 0);
		glVertex3f(a, -a, 5);
		glVertex3f(a, a, 5);
	}glEnd();

	glBegin(GL_QUADS);{
		glVertex3f(a, -a, 0);
		glVertex3f(-a, -a, 0);
		glVertex3f(-a, -a, 5);
		glVertex3f(a, -a, 5);
	}glEnd();

	glBegin(GL_QUADS);{
		glVertex3f(-a, -a, 0);
		glVertex3f(-a, a, 0);
		glVertex3f(-a, a, 5);
		glVertex3f(-a, -a, 5);
	}glEnd();

	glBegin(GL_QUADS);{
		glVertex3f(a, a, 0);
		glVertex3f(-a, a, 0);
		glVertex3f(-a, a, 5);
		glVertex3f(a, a, 5);
	}glEnd();

}


vector<vector<Point>> generatePoints(double rad, int stacks, int slices) {

    vector<vector<Point>> points;

	double stack_angle = (double)(pi/2) / stacks;
	double slice_angle = (double)(2*pi) / slices;

	for(int i = 0; i <= stacks; i++) {
		double h = rad*sin(i*stack_angle);
		double r = rad*cos(i*stack_angle);

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

void sphere3D(double rad, int stacks, int slices) {

    vector<vector<Point>> points = generatePoints(rad, stacks, slices);


	// for(int i = 0; i < points.size(); i++) {
	// 	for(int j = 0; j < points[i].size(); j++) {
	// 		cout << points[i][j].x << " " << points[i][j].y << " |";
	// 	}

	// 	cout << endl;
 	// }

	for(int i = 0; i < stacks; i++) {
		
		bool isRed = true;

		for(int j = 0; j < slices; j++) {

			// Upper Hemisphere
			if(isRed) glColor3f(1, 0, 0);
			else glColor3f(0, 1, 0);

			glBegin(GL_QUADS);{
					glVertex3f(points[i][j].x, points[i][j].y, points[i][j].z);
					glVertex3f(points[i][j+1].x, points[i][j+1].y, points[i][j+1].z);
					glVertex3f(points[i+1][j+1].x, points[i+1][j+1].y, points[i+1][j+1].z);
					glVertex3f(points[i+1][j].x, points[i+1][j].y, points[i+1][j].z);
			}glEnd();

			// Lower Hemisphere

			if(!isRed) glColor3f(1, 0, 0);
			else glColor3f(0, 1, 0);

			glBegin(GL_QUADS);{
					glVertex3f(points[i][j].x, points[i][j].y, -points[i][j].z);
					glVertex3f(points[i][j+1].x, points[i][j+1].y, -points[i][j+1].z);
					glVertex3f(points[i+1][j+1].x, points[i+1][j+1].y, -points[i+1][j+1].z);
					glVertex3f(points[i+1][j].x, points[i+1][j].y, -points[i+1][j].z);
			}glEnd();

			 if(j%3 == 0) isRed = !isRed;
		}
	}
	
}

class Sphere{

public:
	int stacks;
	int slices;

	Point pos; // poistion
	double rad; // radius
	double v; // v =  velocity
	int theta; // thetha = angle
	int rotate_counter;

	double boundary;
	int mode;

	Sphere(int rad, double velocity, int theta, double boundary) {
		this->stacks = 20;
		this->slices = 24;

		this->pos = Point(0, 0, 0);
		this->rad = rad;
		this->v = velocity;
		this->theta = theta;
		this->rotate_counter = 0;

		this->boundary = boundary;
		this->mode = 0;
	}


	void drawSphere(){

		glPushMatrix();
		glTranslatef(pos.x, pos.y, pos.z);
		glRotatef(-rotate_counter%360, sin(theta* pi/180),-cos(theta*pi/180),0);
		sphere3D(rad, stacks, slices);
		glPopMatrix();

	}

	void drawArrow(){
		
		glColor3f(0,0,1);
		
		glPushMatrix();

		glTranslatef(pos.x, pos.y, pos.z);
		glRotatef(theta , 0, 0, 1);
		glLineWidth(5);

		glBegin(GL_LINES);{
			glVertex3f(0,0,0);
			glVertex3f(10, 0,0);
		}glEnd();

		glBegin(GL_TRIANGLES);{
			glVertex3f(10,0,0);
			glVertex3f(8, 2, 0);
			glVertex3f(8, -2, 0);
		}glEnd();
	
		glLineWidth(1);
		glPopMatrix();
	}	

	void draw(){
		drawSphere();
		drawArrow();
	}

	
	bool isOutsideBoundary() {
		double x = pos.x;
		double y = pos.y;
		return (( x+rad >= boundary) || (x-rad <= -boundary) || (y+rad >= boundary ) || (y-rad <= -boundary));
	}

	void forward(){

		if(isOutsideBoundary()){
			// theta = theta + 180;

			double v_x = cos(theta*(pi/180));
			double v_y = sin(theta*(pi/180));

			if(pos.x+rad >= boundary || pos.x-rad <= -boundary) {
				v_x = -v_x; 
			} else {
				v_y = - v_y;
			}

			double angle = atan(v_y / v_x) * (180/pi);

			if(v_x > 0 ) theta = angle; 
			else theta = 180 + angle;
		}

		double angle = theta* pi/180;

		pos = pos + Point(v * cos(angle), v * sin(angle), 0);

		rotate_counter+=10;
	}

	void backward() {

		if(isOutsideBoundary()){
			// theta = theta + 180;

			double v_x = cos(theta*(pi/180));
			double v_y = sin(theta*(pi/180));

			if(pos.x+rad >= boundary || pos.x-rad <= -boundary) {
				v_x = -v_x; 
			} else {
				v_y = - v_y;
			}

			double angle = atan(v_y / v_x) * (180/pi);

			if(v_x > 0 ) theta = angle;
			else theta = 180 + angle;
		}

		double angle = theta* pi/180;

		pos = pos + Point(-v * cos(angle), -v * sin(angle), 0);

		rotate_counter-=10;
	}

	bool isManual(){
		return mode == 0;
	}

};


Sphere sphere(3, .3, 60, 5*6); // a= 5*square_length
Camera camera(Point(10, -70, 50));


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


	// grid

	glPushMatrix();
	glTranslatef(0,0,-3);
	// square2D(5);
	// line2D(10, 5);
	grid2D(30, 6);
	glPopMatrix();


	// boundary 

	glPushMatrix();
	glColor3f(1, 0, 0);
	glTranslatef(0,0,-3);
	glRotatef(45,0,0,1);
	drawBoundary(5*6); // 5 * square_length
	glPopMatrix();


	// sphere3D(3, 20, 24);


	glPushMatrix();
	glRotatef(45, 0, 0, 1);
	sphere.draw();
	glPopMatrix();


    // glFlush();
    //ADD this line in the end --- if you use double buffer (i.e. GL_DOUBLE)

	glutSwapBuffers();
}



void idle() {
        
    // if(animate) {
    //     counter++;
    // }

	if(!sphere.isManual())
		sphere.forward();

    glutPostRedisplay();
}


void keyboardHandler(unsigned char key, int x,int y){

	switch(key){

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
			sphere.theta = (sphere.theta + 20) % 360;
			break;

		case 'l': case 'L':
			sphere.theta = (sphere.theta - 20 + 360)%360;
			break;

		case 'i': case 'I':	
			if(sphere.isManual()){
				sphere.forward();
			}
			break;

		case 'k': case 'K':
			if(sphere.isManual()){
				sphere.backward();
			}
			break;

		case ' ':
			sphere.mode = 1 - sphere.mode;
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
	gluPerspective(80,	1,	1,	1000.0);
	//field of view in the Y (vertically)
	//aspect ratio that determines the field of view in the X direction (horizontally)
	//near distance
	//far distance
}




int main(int argc, char **argv) {


    // making a window

    glutInit(&argc, argv);
    glutInitWindowPosition(0, 0);
    glutInitWindowSize(600, 600);

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