#include<iostream>
#include<fstream>
#include<vector>
#include<stack>

#include "Matrix.cpp"
#include "bitmap_image.hpp"


using namespace std;

void generateBitmapImage(double screenWidth, double screenHeight, vector<vector<int>> frame_buffer) {
    
    bitmap_image image(screenWidth, screenHeight);
    
    for(int y = 0; y < screenHeight; y++){
        for(int x = 0; x < screenWidth; x++){
            int color = frame_buffer[y][x];
            image.set_pixel(x, y, palette_colormap[color]);
        }
    }

    image.save_image("out.bmp");

}

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
    ofstream zBuffer_out( "z_buffer.txt" );

    double screenWidth, screenHeight;

    config_in >> screenWidth >> screenHeight;
    config_in.close();

    // cout << screenWidth << screenHeight << endl;

    vector<vector<double>> z_buffer(screenHeight+1, vector<double>(screenWidth+1, 2));
    vector<vector<int>> frame_buffer(screenHeight+1, vector<int>(screenWidth+1, 49));

    double dx = 2 / screenWidth, dy = 2 / screenHeight;

    for(Triangle &triangle: triangles) {

        Boundary boundary = triangle.getBoundary();

        int color = rand() % 50;

        for(int y = boundary.upper / dy; y >= boundary.lower/dy-1; y-- ) {
            for(int x = boundary.left/dx - 1; x <= boundary.right/dx + 1; x++) {
                
                // check if inside triangle or not

                Point lamda = triangle.getBarycentricCoordinates(x*dx, y*dy);
    
                if(lamda.x >= -1e-9 && lamda.y >= -1e-9 && lamda.z >= -1e-9) {
                   
                    // inside triangle

                    int row = screenHeight - ( y+screenHeight/2 );
                    int col = x + screenWidth/2;

                    if(row < 0 || row >= screenHeight) continue;
                    if(col < 0 || col >= screenWidth) continue;

                    // cout << x*dx << " " << y*dy << endl;

                    //cout << "zvalue" << endl;

                    double zValue = triangle.get_zValue(x*dx, y*dy);

                    // cout << zValue << endl;
                    
                    if(zValue > 1 || zValue < -1) continue;
                    
                    if(zValue < z_buffer[row][col]) {
                        z_buffer[row][col] = zValue;
                        frame_buffer[row][col] = color;
                    }

                }

            }
        }

    }

    for (int i = 0; i < screenHeight+1; i++) {
        for (int j = 0; j < screenWidth+1; j++) {
            if (z_buffer[i][j] < 1.1) {
                zBuffer_out << fixed << setprecision(6) << z_buffer[i][j] << "\t";
            }
        }
        zBuffer_out << endl;
    }

    zBuffer_out.close();

    // bitmap image 

    generateBitmapImage(screenWidth, screenHeight, frame_buffer);
    

}