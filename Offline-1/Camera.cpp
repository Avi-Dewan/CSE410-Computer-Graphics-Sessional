#include "Point.cpp"

#ifdef __linux__
    #include<GL/glut.h>
#elif WIN32
    #include <windows.h>
    #include <glut.h>
#endif

class Camera {
public:
    Point camera_pos;
    Point looking_pos;
    Point up_vector;
    Point right_vector;
    Point direction;

    Camera(Point p) {
        camera_pos = p;
        looking_pos = Point(0, 0, 0);
        up_vector = Point(0, 0, 1);
        updateDirection();
		updateRightVector();
    }

    void look() {
        gluLookAt(
            camera_pos.x, camera_pos.y, camera_pos.z,
            looking_pos.x, looking_pos.y, looking_pos.z,
            up_vector.x, up_vector.y, up_vector.z
        );
    }

    void forward() {
        direction = updateDirection();
        camera_pos = camera_pos - direction * 12; // camera speed = 12
    }

    void backward() {
        direction = updateDirection();
        camera_pos = camera_pos + direction * 12;
    }

    void up() {
        camera_pos = camera_pos + up_vector * 12;
        looking_pos = looking_pos + up_vector * 12;
    }

    void down() {
        camera_pos = camera_pos - up_vector * 12;
        looking_pos = looking_pos - up_vector * 12;
    }

    void right() {
		right_vector = updateRightVector();
        camera_pos = camera_pos - right_vector * 12;
    }

    void left() {
		right_vector = updateRightVector();
        camera_pos = camera_pos + right_vector * 12;
    }

    void lookLeft() {
        looking_pos = looking_pos - right_vector * 2; // rotate speed = 2
        updateDirection();
		updateRightVector();
    }

    void lookRight() {
        looking_pos = looking_pos + right_vector * 2;
        updateDirection();
		updateRightVector();
    }

    void lookUp() {
        looking_pos = looking_pos + up_vector * 2;
        updateDirection();
		updateRightVector();
    }

    void lookDown() {
        looking_pos = looking_pos - up_vector * 2;
        updateDirection();
		updateRightVector();
    }


    void tiltCounterClockwise() {

        Point vxp = direction * up_vector;
        Point vxpvxp = direction * vxp;

        double angle = 9; // rotate Angle

        up_vector = up_vector + vxp * sin(angle * pi/180) + vxpvxp * (1 - cos(angle * pi/180));

        updateDirection();
		updateRightVector();
    }

    void tiltClockwise() {


        Point vxp = direction * up_vector;
        Point vxpvxp = direction * vxp;

        double angle = -9; // rotate Angle

        up_vector = up_vector + vxp * sin(angle * pi/180) + vxpvxp * (1 - cos(angle * pi/180));

        updateDirection();
		updateRightVector();;
    }

    void upWithoutReferenceChange() {
        camera_pos = camera_pos + up_vector * 12; // camera speed = 12
    }

    void downWithoutReferenceChange() {
        camera_pos = camera_pos - up_vector * 12;
    }

	Point updateDirection() {
        direction = (camera_pos - looking_pos).normalize(); // direction = camera_pos - loking

        return direction;
    }

	Point updateRightVector() {
		right_vector = (up_vector * direction).normalize(); // right = up x direction

        return right_vector;
	}

};


