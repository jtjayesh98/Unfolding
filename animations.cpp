#include <GL/freeglut.h>
#include <GL/glu.h>
#include <iostream>
#include <fstream>
#include <vector>
#include <sstream>
#include <map>

using std::cout; using std::cerr;
using std::endl; using std::string;
using std::ifstream; using std::ostringstream;
using std::istringstream;
using namespace std;
std::map<int, std::vector<string>> csv_contents;

string readFileIntoString(const string& path) {
    auto ss = ostringstream{};
    ifstream input_file(path);
    if (!input_file.is_open()) {
        cerr << "Could not open the file - '"
             << path << "'" << endl;
        exit(EXIT_FAILURE);
    }
    ss << input_file.rdbuf();
    return ss.str();
}
void initialize(){
    glClearColor(0.0, 0.0, 0.0, 1.0);
}


int main(int argc, char** argv)
{
    std::ifstream readData;
    readData.open("transfer.csv");
    double data;
    std::string output;
    output = readFileIntoString("transfer.csv");
    // getline(readData, data);
    // std::map<int, std::vector<string>> csv_contents;
    char delimiter = ',';
    istringstream sstream(output);

    std::vector<string> items;
    string record;

    int counter = 0;
    while (std::getline(sstream, record)) {
        istringstream line(record);
        while (std::getline(line, record, delimiter)) {
            items.push_back(record);
        }
        csv_contents[counter] = items;
        items.clear();
        counter += 1;
    }
    std::cout<<std::stof(csv_contents.at(0).at(0))<<std::endl;
    void resize(int, int);
    void display();
    void draw();
    void draw_triangles();
    void keyboard(unsigned char, int, int);
    void initialize();

    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
    glutInitWindowSize(800, 600);
    glutInitWindowPosition(0, 0);
    glutCreateWindow("Test");

    initialize();

    glutDisplayFunc(draw_triangles);
    glutKeyboardFunc(keyboard);
    glutReshapeFunc(resize);
    
    glutMainLoop();

    return 0;
}



void resize(int width, int height){
    if (height == 0){
        height = 1;
    }
    if (width == 0){
        width = 1;
    }

    glViewport(0, 0, (GLsizei) width, (GLsizei) height);

    glMatrixMode(GL_PROJECTION);

    glLoadIdentity();

    // gluPerspective(45.0, (float)width/height, 0.1, 100.0);
}


void draw_triangles(){
    extern std::map<int, std::vector<string>> csv_contents;
    std::cout<<csv_contents.at(0).at(0)<<std::endl;
    glClear(GL_COLOR_BUFFER_BIT);
    std::cout<<csv_contents.size()<<std::endl;
    for (int i = 0; i < csv_contents.size() ; i++){
        glColor3f(1, 1, 1);
        glBegin(GL_LINE_LOOP);
            glVertex2f(GLfloat(std::stof(csv_contents.at(i).at(0))), GLfloat(std::stof(csv_contents.at(i).at(1))));
            glVertex2f(GLfloat(std::stof(csv_contents.at(i).at(2))), GLfloat(std::stof(csv_contents.at(i).at(3))));
            glVertex2f(GLfloat(std::stof(csv_contents.at(i).at(4))), GLfloat(std::stof(csv_contents.at(i).at(5))));
        glEnd();
        glLoadIdentity();
    }
    glFlush();
}

void draw(){
    glClear(GL_COLOR_BUFFER_BIT);

    glBegin(GL_LINE_LOOP);
        glVertex2f(0.2, 0.2);
        glVertex2f(0, 0);
        glVertex2f(0.4, 0.1);
    glEnd();
    glLoadIdentity();

    glTranslatef(0.2, 0.1, 0);
    glRotatef(45, 0, 0, 1);
    glTranslatef(-0.2, -0.1, 0);

    glColor3f(1, 0, 0);
    glBegin(GL_LINE_LOOP);
        glVertex2f(0.2, 0.2);
        glVertex2f(0, 0);
        glVertex2f(0.4, 0.1);
    glEnd();
    glFlush();
}

void display(){
    glClear(GL_COLOR_BUFFER_BIT);

    glBegin(GL_TRIANGLES);
    glColor3f(1.0, 0.0, 0.0);
    glVertex3f(0.0, 1.0, 0.0);
    glVertex3f(-1.0, -1.0, 0.0);
    glVertex3f(1.0, -1.0, 0.0);

    glEnd();
    glFlush();
}

void keyboard(unsigned char key, int x, int y){
    switch(key){
        case 27: glutLeaveMainLoop();
            break;
        default:
            break;
    }
}