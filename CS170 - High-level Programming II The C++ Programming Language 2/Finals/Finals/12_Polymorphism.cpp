#include "12_Polymorphism.h"

//g++ -Wall -Wextra -Werror -Wconversion -pedantic -std=c++11 -o Test.exe Test.cpp

/*
#include <iostream>
using namespace std;

class Shape
{
public:
    Shape()
    {
        cout << "Constructor Shape" << endl;
    }

    virtual void draw() //if no virtual, only prints the base instead of derived classes
    {
        cout << "Draw Shape" << endl;
    }

    virtual ~Shape() //if no virtual, only free the base instead of all derived classed and lastly based. will have mem leak.
    {
        cout << "Destructor Shape" << endl;
    }
};

class Circle : public Shape
{
public:
    Circle()
    {
        cout << "Constructor Circle" << endl;
    }

    void draw()
    {
        Shape::draw();  //state base class draw, if not will not base class: draw Shape->
        cout << "Draw Circle" << endl;  //draw Circle
    }

    ~Circle()
    {
        cout << "Destructor Circle" << endl;
    }
};

class Polygon : public Shape
{
public:
    Polygon()
    {
        cout << "Constructor Polygon" << endl;
    }

    void draw()
    {
        Shape::draw(); //state base class draw, if not will not base class: Shape draw->
        cout << "Draw Polygon" << endl; //draw Polygon->
    }

    ~Polygon()
    {
        cout << "Destructor Polygon" << endl;
    }
};

class Triangle : public Polygon
{
public:
    Triangle()
    {
        cout << "Constructor Triangle" << endl;
    }

    void draw()
    {
        Polygon::draw(); //state base class draw, if not will not base class: draw Shape->draw Polygon->
        cout << "Draw Triangle" << endl;        //draw Triangle
    }

    ~Triangle()
    {
        cout << "Destructor Triangle" << endl;
    }
};

int main() {

    Shape* shapes[] = { new Circle(), new Triangle() };
    
    shapes[0]->draw();
    shapes[1]->draw();
    delete shapes[0];
    delete shapes[1];
    return 0;
}
*/
