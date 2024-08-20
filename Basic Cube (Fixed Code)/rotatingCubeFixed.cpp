#include "screen.h"
#include <random>
#include <numeric>
#include <cmath>

struct Vec3
{
    float x, y, z;

    // Rotate the point around the X-axis
    void rotateX(float angle) {
        float rad = angle;
        float newY = std::cos(rad) * y - std::sin(rad) * z;
        float newZ = std::sin(rad) * y + std::cos(rad) * z;
        y = newY;
        z = newZ;
    }

    // Rotate the point around the Y-axis
    void rotateY(float angle) {
        float rad = angle;
        float newX = std::cos(rad) * x + std::sin(rad) * z;
        float newZ = -std::sin(rad) * x + std::cos(rad) * z;
        x = newX;
        z = newZ;
    }

    // Rotate the point around the Z-axis
    void rotateZ(float angle) {
        float rad = angle;
        float newX = std::cos(rad) * x - std::sin(rad) * y;
        float newY = std::sin(rad) * x + std::cos(rad) * y;
        x = newX;
        y = newY;
    }

    // Rotate around all three axes
    void rotate(float angleX, float angleY, float angleZ) {
        rotateX(angleX);
        rotateY(angleY);
        rotateZ(angleZ);
    }
};

struct Connection
{
    int a,b;
};

void line(Screen& screen, float x1, float y1, float x2, float y2)
{
    float dx = x2 - x1;
    float dy = y2 - y1;
    float length = std::sqrt(dx * dx + dy * dy);
    float angle = std::atan2(dy,dx);

    for(float i = 0; i < length; i++ )
    {
        screen.pixel(
                        x1 + std::cos(angle) * i,
                        y1 + std::sin(angle) * i
        );
    }
}

int main()
{
    Screen screen;

    std::vector<Vec3> points {
        {100,100,100},
        {200,100,100},
        {200,200,100},
        {100,200,100},
        
        {100,100,200},
        {200,100,200},
        {200,200,200},
        {100,200,200}
    };

    std::vector<Connection> connections
    {
        {0,4},
        {1,5},
        {2,6},
        {3,7},

        {0,1},
        {1,2},
        {2,3},
        {3,0},

        {4,5},
        {5,6},
        {6,7},
        {7,4}
    };

    Vec3 c{0,0,0};
    
    for(auto& p : points)
    {
        c.x += p.x;
        c.y += p.y;
        c.z += p.z;
    }

    c.x /= points.size();  
    c.y /= points.size();
    c.z /= points.size();

    while(true)
    {   
        for(auto& p : points)
        {
            // Translate points to origin
            p.x -= c.x;
            p.y -= c.y;
            p.z -= c.z;

            // Rotate the points
            p.rotate(0.03, 0.0, 0.01);

            // Translate points back to original position
            p.x += c.x;
            p.y += c.y;
            p.z += c.z;

            // Draw the point
            screen.pixel(p.x, p.y);
        }

        for(auto& conn : connections)
        {
            line(screen, points[conn.a].x,
                         points[conn.a].y,
                         points[conn.b].x,
                         points[conn.b].y);
        }

        screen.show();
        screen.clear();
        screen.input();
        SDL_Delay(30);
    }

    return 0;
}
