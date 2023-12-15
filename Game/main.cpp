#include <SFML/Graphics.hpp>
#include <cmath>
#include <cstring>
#include <iostream>
#include <vector>

class Player {
    public:
        int x, y;

        void init(int x, int y) {
            x, y = x, y;
        }
};

void multiplyMatrixVector(std::vector<float>& i, std::vector<float>& o, float m[4][4]);
void drawLine(float x1, float y1, float x2, float y2);
void drawTriangle(float x1, float y1, float x2, float y2, float x3, float y3);

sf::RenderWindow window(sf::VideoMode(720, 480), "PewPew Live Mesh Editor");
int main()
{
    int screenWidth = 720;
    int screenHeight = 480;
    sf::Image windowIcon;
    windowIcon.loadFromFile("Game Logo/Logo.png");
    window.setIcon(1024, 1024, windowIcon.getPixelsPtr());
    sf::CircleShape shape(100.f);
    sf::Color color(0, 255, 255);
    shape.setFillColor(color);
    sf::Clock clock;

    float fNear = 0.1f;
    float fFar = 1000.0f;
    float fFOV = 60.0f;
    float fAspectRatio = (float) screenHeight / (float) screenWidth;
    float fFOVRad = 1.0f / tanf(fFOV * 0.5f / 180.0f * 3.14159f);
    float fTimeElapsed = 0.0f;

    float matProj[4][4];
    matProj[0][0] = fAspectRatio * fFOVRad;
    matProj[1][1] = fFOVRad;
    matProj[2][2] = fFar / (fFar - fNear);
    matProj[3][2] = (-fFar * fNear) / (fFar * fNear);
    matProj[2][3] = 1.0f;
    matProj[3][3] = 0.0f;

    std::vector<std::vector<std::vector<float>>> meshCube = {
        {{0.0f, 0.0f, 0.0f}, {0.0f, 1.0f, 0.0f}, {1.0f, 1.0f, 0.0f}},  //South
        {{0.0f, 0.0f, 0.0f}, {1.0f, 1.0f, 0.0f}, {1.0f, 0.0f, 0.0f}},

        {{1.0f, 0.0f, 0.0f}, {1.0f, 1.0f, 0.0f}, {1.0f, 1.0f, 1.0f}}, //East
        {{1.0f, 0.0f, 0.0f}, {1.0f, 1.0f, 1.0f}, {1.0f, 0.0f, 1.0f}},

        {{1.0f, 0.0f, 1.0f}, {1.0f, 1.0f, 1.0f}, {0.0f, 1.0f, 1.0f}},//North
        {{1.0f, 0.0f, 1.0f}, {0.0f, 1.0f, 1.0f}, {0.0f, 1.0f, 1.0f}},

        {{0.0f, 0.0f, 1.0f}, {0.0f, 1.0f, 1.0f}, {0.0f, 1.0f, 0.0f}}, //West
        {{0.0f, 0.0f, 1.0f}, {0.0f, 1.0f, 0.0f}, {0.0f, 0.0f, 0.0f}},

        {{0.0f, 1.0f, 0.0f}, {0.0f, 1.0f, 1.0f}, {1.0f, 1.0f, 1.0f}}, //Top
        {{0.0f, 1.0f, 0.0f}, {1.0f, 1.0f, 1.0f}, {1.0f, 1.0f, 0.0f}},

        {{1.0f, 0.0f, 1.0f}, {0.0f, 0.0f, 1.0f}, {0.0f, 0.0f, 0.0f}},
        {{1.0f, 0.0f, 1.0f}, {0.0f, 0.0f, 0.0f}, {1.0f, 0.0f, 0.0f}}
    };

    while (window.isOpen())
    {
        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                window.close();
        }

        sf::Vector2u screenDimensions = window.getSize();
        screenWidth = screenDimensions.x;
        screenHeight = screenDimensions.y;

        window.clear();
        fTimeElapsed = (float)clock.getElapsedTime().asSeconds();

        float matRotZ[4][4]; float matRotX[4][4];

        matRotZ[0][0] = cosf(fTimeElapsed / 2);
        matRotZ[0][1] = sinf(fTimeElapsed / 2);
        matRotZ[1][0] = -sinf(fTimeElapsed / 2);
        matRotZ[1][1] = cosf(fTimeElapsed / 2);
        matRotZ[2][2] = 1.0f;
        matRotZ[3][3] = 1.0f;

        matRotX[0][0] = 1.0F;
        matRotX[1][1] = cosf(fTimeElapsed);
        matRotX[1][2] = sinf(fTimeElapsed);
        matRotX[2][1] = -sinf(fTimeElapsed);
        matRotX[2][2] = cosf(fTimeElapsed);
        matRotX[3][3] = 1.0f;

        
        int size_mesh = sizeof(meshCube) / sizeof(meshCube[0]);
        for (int i = 0; i < 12; i++) {
            auto triangle = meshCube[i];
            std::vector<std::vector<float>> projTri = {{0, 0, 0}, {0, 0, 0}, {0, 0, 0} };
            std::vector<std::vector<float>> translTri = {{0, 0, 0}, {0, 0, 0}, {0, 0, 0} };
            std::vector<std::vector<float>> rotTriZ = {{0, 0, 0}, {0, 0, 0}, {0, 0, 0} };
            std::vector<std::vector<float>> rotTriX = {{0, 0, 0}, {0, 0, 0}, {0, 0, 0} };

            multiplyMatrixVector(triangle.at(0), rotTriZ.at(0), matRotZ);
            multiplyMatrixVector(triangle.at(1), rotTriZ.at(1), matRotZ);
            multiplyMatrixVector(triangle.at(2), rotTriZ.at(2), matRotZ);

            multiplyMatrixVector(rotTriZ.at(0), rotTriX.at(0), matRotX);
            multiplyMatrixVector(rotTriZ.at(1), rotTriX.at(1), matRotX);
            multiplyMatrixVector(rotTriZ.at(2), rotTriX.at(2), matRotX);

            translTri = rotTriX;

            translTri.at(0).at(2) += 3.0f;
            translTri.at(1).at(2) += 3.0f;
            translTri.at(2).at(2) += 3.0f;

            multiplyMatrixVector(translTri.at(0), projTri.at(0), matProj);
            multiplyMatrixVector(translTri.at(1), projTri.at(1), matProj);
            multiplyMatrixVector(translTri.at(2), projTri.at(2), matProj);

            projTri.at(0).at(0) += 1.0f; projTri.at(0).at(1) += 1.0f;
            projTri.at(1).at(0) += 1.0f; projTri.at(1).at(1) += 1.0f;
            projTri.at(2).at(0) += 1.0f; projTri.at(2).at(1) += 1.0f;

            projTri.at(0).at(0) *= 0.5f * (float)screenWidth; projTri.at(0).at(1) *= 0.5f * (float)screenHeight; 
            projTri.at(1).at(0) *= 0.5f * (float)screenWidth; projTri.at(1).at(1) *= 0.5f * (float)screenHeight; 
            projTri.at(2).at(0) *= 0.5f * (float)screenWidth; projTri.at(2).at(1) *= 0.5f * (float)screenHeight; 
            
            drawTriangle(projTri.at(0).at(0), projTri.at(0).at(1), projTri.at(1).at(0), projTri.at(1).at(1), projTri.at(2).at(0), projTri.at(2).at(1) );
        }

        window.display();
    }

    return 0;
}

void multiplyMatrixVector(std::vector<float>& i, std::vector<float>& o, float m[4][4]) {
    o.at(0) = i.at(0) * m[0][0] + i.at(1) * m[1][0] + i.at(2) * m[2][0] + m[3][0];
    o.at(1) = i.at(0) * m[0][1] + i.at(1) * m[1][1] + i.at(2) * m[2][1] + m[3][1];
    o.at(2) = i.at(0) * m[0][2] + i.at(1) * m[1][2] + i.at(2) * m[2][2] + m[3][2];

    float w = i.at(0) * m[0][3] + i.at(1) * m[1][3] + i.at(2) * m[2][3] + m[3][3];

    if (w != 0.0f) {
        o.at(0) /= w; o.at(1) /= w; o.at(2) /= w;
    }
}

void drawLine(float x1, float y1, float x2, float y2) {
    sf::VertexArray lines(sf::LinesStrip, 2);

    lines[0].position = sf::Vector2f(x1, y1);
    lines[1].position = sf::Vector2f(x2, y2);

    window.draw(lines);
}

void drawTriangle(float x1, float y1, float x2, float y2, float x3, float y3) {
    drawLine(x1, y1, x2, y2);
    drawLine(x2, y2, x3, y3);
    drawLine(x3, y3, x1, y1);
}