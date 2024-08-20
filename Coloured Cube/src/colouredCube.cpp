#include "screenColour.h"
#include "musicPlayer.h"
#include <SDL2/SDL.h>
#include <vector>
#include <algorithm>
#include <cmath>
#include <iostream>



struct Vec3
{
    float x, y, z;

    void rotateX(float angle) {
        float rad = angle;
        float newY = std::cos(rad) * y - std::sin(rad) * z;
        float newZ = std::sin(rad) * y + std::cos(rad) * z;
        y = newY;
        z = newZ;
    }

    void rotateY(float angle) {
        float rad = angle;
        float newX = std::cos(rad) * x + std::sin(rad) * z;
        float newZ = -std::sin(rad) * x + std::cos(rad) * z;
        x = newX;
        z = newZ;
    }

    void rotateZ(float angle) {
        float rad = angle;
        float newX = std::cos(rad) * x - std::sin(rad) * y;
        float newY = std::sin(rad) * x + std::cos(rad) * y;
        x = newX;
        y = newY;
    }

    void rotate(float angleX, float angleY, float angleZ) {
        rotateX(angleX);
        rotateY(angleY);
        rotateZ(angleZ);
    }
};

struct Face {
    std::vector<int> indices;
    float depth;
    SDL_Color color;

    Face(const std::vector<int>& indices, SDL_Color color)
        : indices(indices), color(color), depth(0) {}

    void calculateDepth(const std::vector<Vec3>& points) {
        float totalZ = 0;
        for (int index : indices) {
            totalZ += points[index].z;
        }
        depth = totalZ / indices.size();
    }
};

void line(Screen& screen, float x1, float y1, float x2, float y2)
{
    float dx = x2 - x1;
    float dy = y2 - y1;
    float length = std::sqrt(dx * dx + dy * dy);
    float angle = std::atan2(dy, dx);

    for (float i = 0; i < length; i++) {
        screen.pixel(
            x1 + std::cos(angle) * i,
            y1 + std::sin(angle) * i
        );
    }
}

int main()
{

    // Create and initialize the music player
    MusicPlayer musicPlayer;

    // Play music file in a loop
    if (!musicPlayer.playMusic("Coloured Cube/sounds/ps2_start_up.mp3")) {
        std::cerr << "Failed to play music." << std::endl;
        return -1;
    }


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

    std::vector<Face> faces {
        {{0, 1, 2, 3}, {0, 0, 255, 255}},   // Front face - Red
        {{4, 5, 6, 7}, {0, 0, 255, 255}},   // Back face - Green
        {{0, 1, 5, 4}, {0, 0, 255, 255}},   // Bottom face - Blue
        {{2, 3, 7, 6}, {0, 0, 255, 255}}, // Top face - Yellow
        {{0, 3, 7, 4}, {0, 0, 255, 255}}, // Left face - Magenta
        {{1, 2, 6, 5}, {0, 0, 255, 255}},  // Right face - Cyan
    };

    Vec3 c{0, 0, 0};

    for (auto& p : points)
    {
        c.x += p.x;
        c.y += p.y;
        c.z += p.z;
    }

    c.x /= points.size();
    c.y /= points.size();
    c.z /= points.size();

    while (true)
    {

        screen.clear();

        for (auto& p : points)
        {
            p.x -= c.x;
            p.y -= c.y;
            p.z -= c.z;

            p.rotate(0.02, 0.01, 0.04);

            p.x += c.x;
            p.y += c.y;
            p.z += c.z;
        }

        // Calculate depth for each face
        for (auto& face : faces) {
            face.calculateDepth(points);
        }

        // Sort faces by depth (far to near)
        std::sort(faces.begin(), faces.end(), [](const Face& a, const Face& b) {
            return a.depth > b.depth;
        });

        // Draw faces with color and black outlines
        for (const auto& face : faces)
        {
            std::vector<SDL_FPoint> polygon;

            for (const int index : face.indices)
            {
                polygon.push_back({points[index].x, points[index].y});
            }

            // Draw filled polygon (face)
            screen.drawFilledPolygon(polygon, face.color);

            // Draw black outline
            for (size_t j = 0; j < polygon.size(); ++j)
            {
                size_t next = (j + 1) % polygon.size();
                screen.drawLine(polygon[j].x, polygon[j].y, polygon[next].x, polygon[next].y, {0, 0, 0, 255});
            }
        }

        screen.show();
        screen.clear();
        screen.input();
        SDL_Delay(30);
    }


    return 0;
}
