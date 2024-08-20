#ifndef SCREEN_COLOUR_H
#define SCREEN_COLOUR_H

#include <SDL2/SDL.h>
#include <vector>
#include <algorithm>

class Screen
{
    SDL_Event e;
    SDL_Window* window;
    SDL_Renderer* renderer;
    std::vector<SDL_FPoint> points;

public:
    Screen()
    {
        SDL_Init(SDL_INIT_VIDEO);
        SDL_CreateWindowAndRenderer(
            400*2, 300*2, 0, &window, &renderer);
        SDL_RenderSetScale(renderer, 2, 2);
    }

    void pixel(float x, float y)
    {
        points.emplace_back(x,y);
    }

    void clear()
    {
        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
        SDL_RenderClear(renderer);
    }

    void show()
    {
        SDL_RenderPresent(renderer);
    }

    void input()
    {
        while(SDL_PollEvent(&e))
        {
            if(e.type == SDL_QUIT)
            {
                SDL_Quit();
                exit(0);
            }
        }
    }

    

    void drawLine(float x1, float y1, float x2, float y2, SDL_Color color)
    {
        SDL_SetRenderDrawColor(renderer, color.r, color.g, color.b, color.a);
        SDL_RenderDrawLineF(renderer, x1, y1, x2, y2);
    }

    void drawFilledPolygon(const std::vector<SDL_FPoint>& vertices, SDL_Color color)
    {
        SDL_SetRenderDrawColor(renderer, color.r, color.g, color.b, color.a);

        // Find the bounding box of the polygon
        float minX = vertices[0].x, maxX = vertices[0].x;
        float minY = vertices[0].y, maxY = vertices[0].y;

        for (const auto& vertex : vertices) {
            minX = std::min(minX, vertex.x);
            maxX = std::max(maxX, vertex.x);
            minY = std::min(minY, vertex.y);
            maxY = std::max(maxY, vertex.y);
        }

        // Loop over the bounding box and draw the polygon using the even-odd rule
        for (float y = minY; y <= maxY; ++y) {
            std::vector<float> intersections;

            for (size_t i = 0; i < vertices.size(); ++i) {
                const auto& v1 = vertices[i];
                const auto& v2 = vertices[(i + 1) % vertices.size()];

                if ((v1.y <= y && v2.y > y) || (v2.y <= y && v1.y > y)) {
                    float x = v1.x + (y - v1.y) * (v2.x - v1.x) / (v2.y - v1.y);
                    intersections.push_back(x);
                }
            }

            std::sort(intersections.begin(), intersections.end());

            for (size_t i = 0; i < intersections.size(); i += 2) {
                if (i + 1 < intersections.size()) {
                    SDL_RenderDrawLineF(renderer, intersections[i], y, intersections[i + 1], y);
                }
            }
        }
    }
};
#endif