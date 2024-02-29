#include "1905120_classes.h"
#include <iostream>
#include <vector>
#include<fstream>

using namespace std;

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
            Vector3D center;
            double radius;
            cin >> center.x >> center.y >> center.z >> radius;

            Sphere* sphere = new Sphere(center, radius);

            objects.push_back(sphere);
        }
        else if(type == "triangle") {
            Vector3D a, b, c;
            cin >> a.x >> a.y >> a.z;
            cin >> b.x >> b.y >> b.z;
            cin >> c.x >> c.y >> c.z;

            Triangle* triangle = new Triangle(a, b, c);
            objects.push_back(triangle);

        } else if(type == "general") {

            double A, B, C, D, E, F, G, H, I, J;
            Vector3D reference_point;
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

    cout << "Data loaded successfully" << endl;
    cout << "levelOfRevursion: " << levelOfRevursion << endl;
    cout << "noOfPixels: " << noOfPixels << endl;
    cout << "noOfObjects: " << noOfObjects << endl;
    cout << "noOfPointLights: " << noOfPointLights << endl;
    cout << "noOfSpotLights: " << noOfSpotLights << endl;

    for(int i = 0; i < noOfObjects; i++) {
        cout << "Object " << i << endl;
        // objects[i]->print();
    }

    for(int i = 0; i < noOfPointLights; i++) {
        cout << "Point Light " << i << endl;
        cout << "Position: " << pointLights[i].light_pos.x << " " << pointLights[i].light_pos.y << " " << pointLights[i].light_pos.z << endl;
        cout << "Color: " << pointLights[i].color[0] << " " << pointLights[i].color[1] << " " << pointLights[i].color[2] << endl;
    }

    for(int i = 0; i < noOfSpotLights; i++) {
        cout << "Spot Light " << i << endl;
        cout << "Position: " << spotLights[i].point_light.light_pos.x << " " << spotLights[i].point_light.light_pos.y << " " << spotLights[i].point_light.light_pos.z << endl;
        cout << "Color: " << spotLights[i].point_light.color[0] << " " << spotLights[i].point_light.color[1] << " " << spotLights[i].point_light.color[2] << endl;
        cout << "Direction: " << spotLights[i].light_direction.x << " " << spotLights[i].light_direction.y << " " << spotLights[i].light_direction.z << endl;
        cout << "Cutoff Angle: " << spotLights[i].cutoff_angle << endl;
    }
}


int main() {

    loadData();

    return 0;
}