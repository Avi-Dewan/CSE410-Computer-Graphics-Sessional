#include<iostream>
#include<fstream>
#include<vector>
#include<stack>

#include "Matrix.cpp"

using namespace std;

int main() {

    ifstream scene_in("scene.txt");


    Point eye, look, up;
    double  fovY, aspectRatio, near, far;
    vector<Triangle> triangles;
    stack<Matrix> stack_of_matrix;

    scene_in >> eye.x >> eye.y >> eye.z;
    scene_in >> look.x >> look.y >> look.z;
    scene_in >> up.x >> up.y >> up.z;
    scene_in >>  fovY >> aspectRatio >> near >> far;


    ofstream stage1_out( "stage1.txt" );
    ofstream stage2_out( "stage2.txt" );
    ofstream stage3_out( "stage3.txt" );
    
    Matrix M = Matrix::getIdentityMatrix(); 
    stack_of_matrix.push(M);

    string command;

    while (true)
    {
        scene_in >> command;

        if(command == "end") break;

        else if(command == "triangle") {
            Point p1, p2, p3;

            scene_in >> p1.x >> p1.y >> p1.z;
            scene_in >> p2.x >> p2.y >> p2.z;
            scene_in >> p3.x >> p3.y >> p3.z;



            Triangle triangle(p1, p2, p3);

            triangle = M*triangle;

            stage1_out << triangle << endl;

            triangles.push_back(triangle);

        } else if(command == "translate") {

            Point t;
            scene_in >> t.x >> t.y >> t.z;

            Matrix T = Matrix::getTranslationMatrix(t);
            
            M = M*T;

        } else if(command == "scale") {
            Point s;
            scene_in >> s.x >> s.y >> s.z;

            Matrix S = Matrix::getScalingMatrix(s);

            M = M*S;

        } else if(command == "rotate") {
            double angle;
            Point axes;

            scene_in >> angle;
            scene_in >> axes.x >> axes.y >> axes.z;

            Matrix R = Matrix::getRotationMatrix(angle, axes);
            
            M = M*R;

        } else if(command == "push") {
            stack_of_matrix.push(M);
        } else if(command == "pop") {
            M = stack_of_matrix.top();
            stack_of_matrix.pop();
        }
    }

    scene_in.close();
    stage1_out.close();

    // stage 2

    Point l = (look - eye);
    l.normalize();
    Point r = l*up;
    r.normalize();
    Point u = r*l;

    // translate eye/camera to origin
    Matrix T_eye2origin = Matrix::getTranslationMatrix(-eye);
    Matrix viewRotation = Matrix::getViewRotationMatix(r, u, l);

    Matrix viewTransformation = viewRotation * T_eye2origin;

    for(Triangle &triangle: triangles) {
        triangle = viewTransformation*triangle;
        stage2_out << triangle << endl;
    }

    stage2_out.close();


    // stage 3

    double fovX = fovY*aspectRatio;
    double t = near*tan(fovY/2* (PI/180));
    double rr = near*tan(fovX/2* (PI/180));

    Matrix projectionMatrix = Matrix::getProjectionMatrix(t, rr, near, far);

    for(Triangle &triangle: triangles) {
        triangle = projectionMatrix*triangle;
        stage3_out << triangle << endl;
    }

    stage3_out.close();

    // zBuffer
    ifstream config_in("config.txt");
    ofstream zBuffer_out( "zBuffer.txt" );

    double screenWidth, screenHeight;

    config_in >> screenWidth >> screenHeight;
    config_in.close();

    vector<vector<double> > z_buffer(screenHeight+1, vector<double>(screenWidth+1, 2));
    vector<vector<int> > frame_buffer(screenHeight+1, vector<int>(screenWidth+1, 49));

    
}