#include "terrain3dviewer.h"
#include <QMouseEvent>

Terrain3DViewer::Terrain3DViewer(QWidget *parent)
    : QOpenGLWidget(parent) {
    setMinimumSize(400, 400);
}

Terrain3DViewer::~Terrain3DViewer() {
    makeCurrent();
    vertexBuffer.destroy();
    indexBuffer.destroy();
    vao.destroy();
    doneCurrent();
}

void Terrain3DViewer::setTerrain(const terraingenerator::Terrain<uchar> &terrain, const terraingenerator::Terrain<uchar> &moisture_map) {
    width_ = terrain.width();
    height_ = terrain.height();

    vertices.clear();
    indices.clear();

    //Создаем вершины (x,y,z), y - высота
    for (int y = 0; y < height_; ++y) {
        for (int x = 0; x < width_; ++x) {
            float heightVal = terrain[x][y];

            // e и m — нормализованные высота и влажность [0..1]
            float e = 1.0 * heightVal / 255;
            float m = 1.0 * moisture_map[x][y]/ 255;  // пример влажности, надо иметь такой массив

            Biome b = biome(e, m);
            Color c = colorFromUInt(static_cast<uint32_t>(b));

            vertices.push_back(float(x));     // x
            vertices.push_back(heightVal);    // y
            vertices.push_back(float(y));     // z

            vertices.push_back(c.r);
            vertices.push_back(c.g);
            vertices.push_back(c.b);      // z
        }
    }

    // Индексы для треугольников (две треугольных клетки на квадрат)
    for (int y = 0; y < height_ - 1; ++y) {
        for (int x = 0; x < width_ - 1; ++x) {
            int i = y * width_ + x;

            indices.push_back(i);
            indices.push_back(i + 1);
            indices.push_back(i + width_);

            indices.push_back(i + 1);
            indices.push_back(i + width_ + 1);
            indices.push_back(i + width_);
        }
    }

    vertexCount = indices.size();

    // // Обновляем буферы на GPU
    updateBuffers();

    update();
}

void Terrain3DViewer::updateBuffers() {
    if (!context()) {
        qWarning() << "No OpenGL context!";
        return;
    }

    makeCurrent();

    if (!vao.isCreated()) {
        if (!vao.create()) {
            qWarning() << "Failed to create VAO!";
            doneCurrent();
            return;
        }
    }
    vao.bind();

    if (!vertexBuffer.isCreated()) {
        if (!vertexBuffer.create()) {
            qWarning() << "Failed to create vertex buffer!";
            vao.release();
            doneCurrent();
            return;
        }
    }
    vertexBuffer.bind();
    vertexBuffer.allocate(vertices.constData(), vertices.size() * sizeof(float));

    if (!indexBuffer.isCreated()) {
        if (!indexBuffer.create()) {
            qWarning() << "Failed to create index buffer!";
            vertexBuffer.release();
            vao.release();
            doneCurrent();
            return;
        }
    }
    indexBuffer.bind();
    indexBuffer.allocate(indices.constData(), indices.size() * sizeof(unsigned int));

    if (!shaderProgram.isLinked()) {
        qWarning() << "Shader program not linked!";
        indexBuffer.release();
        vertexBuffer.release();
        vao.release();
        doneCurrent();
        return;
    }
    shaderProgram.bind();

    int posLoc = shaderProgram.attributeLocation("a_position");
    if (posLoc == -1) {
        qWarning() << "Attribute 'a_position' not found!";
        shaderProgram.release();
        indexBuffer.release();
        vertexBuffer.release();
        vao.release();
        doneCurrent();
        return;
    }
    shaderProgram.enableAttributeArray(posLoc);
    shaderProgram.setAttributeBuffer(posLoc, GL_FLOAT, 0, 3, 3 * sizeof(float));

    int colorLoc = shaderProgram.attributeLocation("a_color");
    shaderProgram.enableAttributeArray(colorLoc);
    shaderProgram.setAttributeBuffer(colorLoc, GL_FLOAT, 3 * sizeof(float), 3, 6 * sizeof(float));

    vao.release();
    vertexBuffer.release();
    indexBuffer.release();
    shaderProgram.release();


    doneCurrent();
}


void Terrain3DViewer::initializeGL() {
    initializeOpenGLFunctions();

    glEnable(GL_DEPTH_TEST);
    glClearColor(0.3f, 0.3f, 0.3f, 1.0f);

    // Компилируем шейдеры
    if (!shaderProgram.addShaderFromSourceCode(QOpenGLShader::Vertex,
                                               R"(#version 330 core
layout(location = 0) in vec3 a_position;
layout(location = 1) in vec3 a_color;

out vec3 v_color;

uniform mat4 modelViewProjection;

void main() {
    gl_Position = modelViewProjection * vec4(a_position, 1.0);
    v_color = a_color;
}
    )")) {
        qFatal("Vertex shader compilation failed");
    }

    if (!shaderProgram.addShaderFromSourceCode(QOpenGLShader::Fragment,
                                               R"(#version 330 core
in vec3 v_color;
out vec4 fragColor;

void main() {
    fragColor = vec4(v_color, 1.0);
}
    )")) {
        qFatal("Fragment shader compilation failed");
    }

    if (!shaderProgram.link()) {
        qFatal("Shader program link failed");
    }
}

void Terrain3DViewer::resizeGL(int w, int h) {
    glViewport(0, 0, w, h);
    projection.setToIdentity();
    projection.perspective(45.0f, float(w) / float(h), 0.1f, 1000.0f);
}

void Terrain3DViewer::paintGL() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    modelView.setToIdentity();
    modelView.translate(0, -5.0f, -std::max(width_, height_) * 1.5f);
    modelView.rotate(angleX, 1, 0, 0);
    modelView.rotate(angleY, 0, 1, 0);
    modelView.translate(-width_ / 2.0f, 0, -height_ / 2.0f);

    shaderProgram.bind();

    shaderProgram.setUniformValue("u_projection", projection);
    shaderProgram.setUniformValue("u_modelView", modelView);

    vao.bind();
    glDrawElements(GL_TRIANGLES, vertexCount, GL_UNSIGNED_INT, nullptr);
    vao.release();

    shaderProgram.release();
}

void Terrain3DViewer::mousePressEvent(QMouseEvent *event) {
    lastMousePos = event->pos();
}

void Terrain3DViewer::mouseMoveEvent(QMouseEvent *event) {
    int dx = event->position().x() - lastMousePos.x();
    int dy = event->position().y() - lastMousePos.y();

    if (event->buttons() & Qt::LeftButton) {
        angleX += dy * 0.5f;
        angleY += dx * 0.5f;
        update();
    }
    lastMousePos = event->pos();
}
