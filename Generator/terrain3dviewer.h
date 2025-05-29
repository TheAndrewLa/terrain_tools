#ifndef TERRAIN3DVIEWER_H
#define TERRAIN3DVIEWER_H

#pragma once

#include <QOpenGLWidget>
#include <QOpenGLFunctions_3_3_Core>
#include <QOpenGLShaderProgram>
#include <QOpenGLBuffer>
#include <QOpenGLVertexArrayObject>
#include <QMatrix4x4>
#include <QPoint>

#include "Generator/terrain.h"

class Terrain3DViewer : public QOpenGLWidget, protected QOpenGLFunctions_3_3_Core {
    Q_OBJECT
public:
    explicit Terrain3DViewer(QWidget *parent = nullptr);
    ~Terrain3DViewer();

    void setTerrain(const terraingenerator::Terrain<uchar> &terrain,
                    const terraingenerator::Terrain<uchar> &moisture_map);

protected:
    void initializeGL() override;
    void resizeGL(int w, int h) override;
    void paintGL() override;

    void mousePressEvent(QMouseEvent *event) override;
    void mouseMoveEvent(QMouseEvent *event) override;

private:
    enum class Biome : uint32 {
        OCEAN                     = 0x44447A,  // Dark Ocean Blue
        BEACH                     = 0xF0E68C,  // Sand Beige
        SCORCHED                  = 0x555555,  // Medium Gray
        BARE                      = 0x888888,  // Light Gray
        TUNDRA                    = 0xDDDDAF,  // Tundra Beige
        SNOW                      = 0xFFFFFF,  // Pure White
        TEMPERATE_DESERT          = 0xEDC9AF,  // Desert Sand
        SHRUBLAND                 = 0xC4D4A3,  // Light Olive Green
        TAIGA                     = 0x99AE7E,  // Taiga Green
        GRASSLAND                 = 0x88AA54,  // Grassy Green
        TEMPERATE_DECIDUOUS_FOREST= 0x679459,  // Fern Green
        TEMPERATE_RAIN_FOREST     = 0x4F8053,  // Deep Forest Green
        SUBTROPICAL_DESERT        = 0xD2B98B,  // Desert Tan
        TROPICAL_SEASONAL_FOREST  = 0x5A8F4C,  // Desert Tan
        TROPICAL_RAIN_FOREST      = 0x3D6A3D   // Rainforest Green
    };

    static Biome get_biome(const float e, const float m) {
        if (e < 0.15) return Biome::OCEAN;
        if (e < 0.2) return Biome::BEACH;

        if (e > 0.8) {
            if (m < 0.2) return Biome::SCORCHED;
            if (m < 0.4) return Biome::BARE;
            if (m < 0.7) return Biome::TUNDRA;
            return Biome::SNOW;
        }
        if (e > 0.6) {
            if (m < 0.33) return Biome::TEMPERATE_DESERT;
            if (m < 0.66) return Biome::SHRUBLAND;
            return Biome::TAIGA;
        }

        if (e > 0.4) {
            if (m < 0.16) return Biome::TEMPERATE_DESERT;
            if (m < 0.50) return Biome::GRASSLAND;
            if (m < 0.83) return Biome::TEMPERATE_DECIDUOUS_FOREST;
            return Biome::TEMPERATE_RAIN_FOREST;
        }

        if (m < 0.16) return Biome::SUBTROPICAL_DESERT;
        if (m < 0.33) return Biome::GRASSLAND;
        if (m < 0.66) return Biome::TROPICAL_SEASONAL_FOREST;
        return Biome::TROPICAL_RAIN_FOREST;
    }
    struct Color {
        float r, g, b;
    };

    Color colorFromUInt(uint32_t hex) {
        return Color{
            ((hex >> 16) & 0xFF) / 255.0f,
            ((hex >> 8) & 0xFF) / 255.0f,
            (hex & 0xFF) / 255.0f
        };
    }

    void updateBuffers();

    QOpenGLShaderProgram shaderProgram;
    QOpenGLBuffer vertexBuffer{QOpenGLBuffer::VertexBuffer};
    QOpenGLBuffer indexBuffer{QOpenGLBuffer::IndexBuffer};
    QOpenGLVertexArrayObject vao;

    QVector<float> vertices;
    QVector<unsigned int> indices;

    int vertexCount = 0;

    QMatrix4x4 projection;
    QMatrix4x4 modelView;

    float angleX = 30.0f;
    float angleY = -45.0f;

    QPoint lastMousePos;

    int width_ = 0;
    int height_ = 0;
};

#endif // TERRAIN3DVIEWER_H
