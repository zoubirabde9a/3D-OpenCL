#include "widget3d.h"
#include <glm/glm.hpp>
#include <glm/gtx/transform.hpp>
#include <QTimer>
#include "picoPng.h"
#include <QDebug>
#include <ctime>
#include "opencl.h"

using namespace ZEngine;

struct ArgStruct
{
    float Scale;
    V3 Offset;
    V3 Rotation;
};

char *OpenCLSrc = R"(

typedef struct V3
{
    float x, y, z;
} V3;

static V3 Multiply(V3 Self, float A)
{
    V3 result = {A * Self.x, A * Self.y, A * Self.z};
    return result;
}

static V3 Add(V3 Self, V3 A)
{
    V3 result = {A.x + Self.x, A.y + Self.y, A.z + Self.z};
    return result;
}

static V3 Subtract(V3 Self, V3 A)
{
    V3 result = {Self.x - A.x, Self.y - A.y, Self.z - A.z};
    return result;
}


typedef struct V2
{
    float x, y;
} V2;

typedef struct Vertex3D
{
    V3 Position;
    char Color[4];
    V2 Uv;
    V3 Normal;
}Vertex3D;


static V2 rotatePoint(float *x, float *y, float angle)
{
    float tmpX = *x;
    float s = sin(angle);
    float c = cos(angle);

    // rotate point
    *x = tmpX * c - *y * s;
    *y = tmpX * s + *y * c;
}

typedef struct ArgStruct
{
    float Scale;
    V3 Offset;
    V3 Rotation;
} ArgStruct;

__kernel void transform(__global Vertex3D *In,
                        __global Vertex3D* Out,
                        int Count,
                        __global ArgStruct* Args)
{
    int i = get_global_id(0);
    if(i < Count)
    {
        Vertex3D Vertex = In[i % 36];
        ArgStruct Arg = Args[i / 36];

        rotatePoint(&(Vertex.Position).x, &(Vertex.Position).z, Arg.Rotation.z);
        rotatePoint(&(Vertex.Normal).x, &(Vertex.Normal).z, Arg.Rotation.z);

        rotatePoint(&Vertex.Position.x, &Vertex.Position.y, Arg.Rotation.x);
        rotatePoint(&(Vertex.Normal.x), &(Vertex.Normal.y), Arg.Rotation.x);

        rotatePoint(&Vertex.Position.z, &Vertex.Position.y, Arg.Rotation.y);
        rotatePoint(&(Vertex.Normal.z), &(Vertex.Normal.y), Arg.Rotation.y);

        Out[i] = Vertex;

        V3 pos = Add((Multiply(Vertex.Position, Arg.Scale)), Arg.Offset);
        Out[i].Position = pos;
    }
}

)";

static QByteArray ReadEntireFile(char *FilePath)
{
    QFile file(FilePath);
    file.open(QIODevice::ReadOnly);
    return file.readAll();
}

static void rotatePoint(float &x, float &y, float angle) {
        float tmpX = x;
        float s = sinf(angle);
        float c = cosf(angle);

        // rotate point
        x = tmpX * c - y * s;
        y = tmpX * s + y * c;
}

static std::vector < Vertex3D > load(char *fileName)
{
    QByteArray fileData = ReadEntireFile(fileName);
    fileData.append('\n');

    enum { Type_Random, Type_Vertex, Type_Texture, Type_Normal, Type_Face };

    std::vector < V3 > vertices;
    std::vector < V2 > textures;
    std::vector < V3 > normals;

    vertices.reserve(1 * 1024 * 1024);
    textures.reserve(1 * 1024 * 1024);
    normals.reserve(1 * 1024 * 1024);


    std::vector < Vertex3D > result;

    int type = Type_Random;
    Vertex3D faceValues[4];
    int faceValuesCount = 0;
    float sign[3];
    float values[3];
    int valuesInt[3];
    int valuesCount = 0;
    float currentValue = 0.f;
    int currentIntValue = 0;
    float currentDivider = 10.f;
    bool beforeComma = true;
    int spaceNumber = 0;

    int offset = 0;

    while(offset < fileData.size())
    {
        char c = fileData.at(offset);

       // qDebug() << offset << ", " << c << " type " << type;

        char nextC = 0;
        if (offset + 1 < fileData.size())
        {
            nextC = fileData.at(offset + 1);
        }

        if (c == '#' || c == 'm' || c == 'o' || c == 'u' || c == 's')
        {
            type = Type_Random;
            while(offset < fileData.size() && (fileData.at(offset) != '\n'))
            {
                offset++;
            }
            continue;
        }

        if (c == 'v')
        {
            if (nextC == 't')
            {
                type = Type_Texture;
            }
            else if (nextC == 'n')
            {
               type = Type_Normal;
            }
            else
            {
                type = Type_Vertex;
            }
            spaceNumber = 0;
            valuesCount = 0;
            currentValue = 0.f;
            beforeComma = true;
            sign[0] = sign[1] = sign[2] = 1.f;
        }
        else if (c == 'f')
        {
            type = Type_Face;
            spaceNumber = 0;
            currentIntValue = 0;
            valuesCount = 0;
            faceValuesCount = 0;
        }

        if (type == Type_Vertex || type == Type_Texture || type == Type_Normal)
        {
            if ((c >= '0' && c <= '9'))
            {
                if (beforeComma)
                {
                    currentValue *= 10;
                    currentValue += c - '0';
                }
                else
                {
                    currentValue += (c - '0') / currentDivider;
                    currentDivider *= 10.f;
                }
            }
            else if (c == '.')
            {
                beforeComma = false;
            }
            else if (c == ' ')
            {
                if (spaceNumber > 0 && spaceNumber < 3)
                {
                    values[valuesCount++] = currentValue;
                    currentValue = 0.f;
                }
                currentDivider = 10.f;
                beforeComma = true;
                spaceNumber++;
            }
            else if (c == '\n')
            {
                values[valuesCount++] = currentValue;
                currentValue = 0.f;

                if (type == Type_Vertex)
                {
                    V3 v = {values[0] * sign[0], values[1] * sign[1], values[2] * sign[2]};
                    vertices.push_back(v);
                }
                else if (type == Type_Texture)
                {
                    V2 v = {values[0] * sign[0], values[1] * sign[1]};
                    textures.push_back(v);
                }
                else if (type == Type_Normal)
                {
                    V3 v = {values[0] * sign[0], values[1] * sign[1], values[2] * sign[2]};
                    normals.push_back(v);
                }
            }
            else if (c == '-')
            {
                sign[valuesCount] = -1.f;
            }
        }
        else if (type == Type_Face)
        {
            if (c >= '0' && c <= '9')
            {
                currentIntValue *= 10;
                currentIntValue += c - '0';
            }
            else if (c == '/' || c == '\\')
            {
                valuesInt[valuesCount++] = currentIntValue;
                currentIntValue = 0;
            }
            else if (c == ' ')
            {
                if ((spaceNumber > 0 && spaceNumber < 4))
                {
                    valuesInt[valuesCount++] = currentIntValue;
                    currentIntValue = 0;

                    V3 position = vertices[valuesInt[0] - 1];
                    V2 texture = textures[valuesInt[1] - 1];
                    V3 normal = normals[valuesInt[2] - 1];
                 //
                    Vertex3D v;
                    v.Position = position;
                    v.Uv = texture;
                    v.Normal = normal;

                    faceValues[faceValuesCount++] = v;
                }

                currentIntValue = 0;
                valuesCount = 0;
                spaceNumber++;
            }
            else if (c == '\n')
            {
                valuesInt[valuesCount++] = currentIntValue;
                currentIntValue = 0;

                V3 position = vertices[valuesInt[0] - 1];
                V2 texture = textures[valuesInt[1] - 1];
                V3 normal = normals[valuesInt[2] - 1];

                Vertex3D v;
                v.Position = position;
                v.Uv = texture;
                v.Normal = normal;

                faceValues[faceValuesCount++] = v;

                currentIntValue = 0;
                valuesCount = 0;
                spaceNumber = 0;

                if (faceValuesCount == 3)
                {
                    result.push_back(faceValues[0]);
                    result.push_back(faceValues[1]);
                    result.push_back(faceValues[2]);
                }
                else if (faceValuesCount == 4)
                {
                    result.push_back(faceValues[0]);
                    result.push_back(faceValues[1]);
                    result.push_back(faceValues[2]);
                    result.push_back(faceValues[2]);
                    result.push_back(faceValues[3]);
                    result.push_back(faceValues[0]);
                }

                 faceValuesCount = 0;
            }
        }

        offset++;
    }

    return result;
}

static V3 normalize(V3 in)
{
    V3 result;
    float d = sqrtf(in.x * in.x + in.y * in.y + in.z * in.z);
    result.x = in.x / d;
    result.y = in.y / d;
    result.z = in.z / d;
}

static void transform(Vertex3D *Out, const std::vector < Vertex3D > &in,
                                          float scale, V3 offset, V3 rotation)
{
    for(int i = 0; i < in.size(); i++)
    {
        Vertex3D vertex = in[i];
        Out[i] = vertex;
        rotatePoint(vertex.Position.x, vertex.Position.z, rotation.z);
        rotatePoint(Out[i].Normal.x, Out[i].Normal.z, rotation.z);

        rotatePoint(vertex.Position.x, vertex.Position.y, rotation.x);
        rotatePoint(Out[i].Normal.x, Out[i].Normal.y, rotation.x);

        rotatePoint(vertex.Position.z, vertex.Position.y, rotation.y);
        rotatePoint(Out[i].Normal.z, Out[i].Normal.y, rotation.y);

        //vertex.Normal = normalize(vertex.Normal);
        Out[i].Position = (vertex.Position * scale) + offset;
    }

}

static int loadTexture(char *path)
{
    unsigned int texture;
    QFile file(path);
    file.open((QIODevice::ReadOnly));
    QByteArray byteArray = file.readAll();
    const unsigned char* imageData = (const unsigned char*)byteArray.data();
    size_t imageSize = byteArray.size();
    std::vector < unsigned char > outImage;
    unsigned long imageWidth = 0, imageHeight = 0;
    int result = decodePNG(outImage, imageWidth, imageHeight, imageData, imageSize, true);
    if (result != 0) {
        printf("failed to decode PNG\n");
    }

    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_2D, texture);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, imageWidth, imageHeight, 0, GL_RGBA, GL_UNSIGNED_BYTE, outImage.data());
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glGenerateMipmap(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, 0);

    return texture;
}


static int loadNormalTexture(char *path)
{
    unsigned int texture;
    QFile file(path);
    file.open((QIODevice::ReadOnly));
    QByteArray byteArray = file.readAll();
    const unsigned char* imageData = (const unsigned char*)byteArray.data();
    size_t imageSize = byteArray.size();
    std::vector < unsigned char > outImage;
    unsigned long imageWidth = 0, imageHeight = 0;
    int result = decodePNG(outImage, imageWidth, imageHeight, imageData, imageSize, true);
    if (result != 0) {
        printf("failed to decode PNG\n");
    }

    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_2D, texture);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, imageWidth, imageHeight, 0, GL_RGBA, GL_UNSIGNED_BYTE, outImage.data());
    glGenerateMipmap(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, 0);

    return texture;
}

Widget3D::Widget3D(QWidget* parent) : QGLWidget(parent)
{
    QTimer *timer = new QTimer(this);
    connect(timer, SIGNAL(timeout()), this, SLOT(update()));
    timer->start(10);

    offsetX = 0.f;
    offsetY = 1.f;
    offsetZ = 0.f;

    rotateX = 0.0f;//-2.1f;
    rotateY = 0.f;
    rotateZ = 0.f;

    cameraDistance = 0.1f;

    pitch = 20;

    cameraPosition = glm::vec3(-0.20, 2.28, -6.5);

    Forward = glm::vec3(0.f, 0.f, 1.f);
    Up = glm::vec3(0.f, 1.f, 0.f);


    positions = new glm::vec3[BoxCount];
    for(int i = 0; i < BoxCount; i++)
    {
        positions[i] = glm::vec3(0.3f * i, 0.3f * i, 0.3f * i);
    }

    treeModel = load(":/tree.obj");
    humanModel = load(":/MaleLow.obj");
    cubeModel = load(":/cube.obj");
    qDebug() << cubeModel.size();

    Transform transform = {};
    transform.Position = {-2.0, 0.2, 0};
    transform.Scale = 0.15f;
    transform.Rotation = {0.0f, 0, 0.f};
    transform.Velocity = {};
    transform.RotationSpeed = {0.00f, 0, 0.01f};
    transform.ScalDiff = 0.000f;
    transform.Vertices = &treeModel;
    objects.push_back(transform);


    for(int i = 0; i < 256 * 16; i++)
    {

        float random1 = ((rand() % 1000) - (rand() % 1000)) / 10000.f;
        float random2 = ((rand() % 1000) - (rand() % 1000)) / 10000.f;
        transform = {};
        transform.Position = {2.0, 1.2, 0};
        transform.Scale = 0.1f;
        transform.Rotation = {0.0f, 0, 0.f};
        transform.Velocity = {random2, random1, 0.00f};
        transform.RotationSpeed = {0.05f, 0.001, 0.01f};
        transform.ScalDiff = 0.000f;
        transform.Vertices = &cubeModel;
        particles.push_back(transform);
    }

    for(int i = 0; i < 1; i++)
    {
        Transform transform = {};
        transform.Position = {0, 1.0 * i, 0};
        transform.Scale = 0.1f;
        transform.Rotation = {0.0f, 0, 0.f};
        transform.Velocity = {0.001f * i, 0.001f * i, 0.f};
        transform.RotationSpeed = {0.f, 0, 0.00f};
        if (i == 1)
        {
            transform.ScalDiff = 0.0001f;
            transform.Vertices = &treeModel;
        }
        else
        {
            transform.ScalDiff = 0.f;
            transform.Vertices = &humanModel;
        }

        objects.push_back(transform);
    }

}


void Widget3D::initializeGL(){
    glewInit();

    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LESS);

    m_program.compileShaders(":/vertexShader_3D.vert", ":/fragmentShader_3D.frag");
    m_program.addAttribute("vertexPosition");
    m_program.addAttribute("vertexColor");
    m_program.addAttribute("vertexUV");
    m_program.addAttribute("vertexNormal");
    m_program.linkShaders();

    // Generate the VAO if it isn't already generated
    if (m_vao == 0) {
        glGenVertexArrays(1, &m_vao);
    }

    // Bind the VAO. All subsequent opengl calls will modify it's state.
    glBindVertexArray(m_vao);

    //G enerate the VBO if it isn't already generated
    if (m_vbo == 0) {
        glGenBuffers(1, &m_vbo);
    }
    glBindBuffer(GL_ARRAY_BUFFER, m_vbo);

    //Tell opengl what attribute arrays we need
    glEnableVertexAttribArray(0);
    glEnableVertexAttribArray(1);
    glEnableVertexAttribArray(2);
    glEnableVertexAttribArray(3);

    //This is the position attribute pointer
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex3D), (void*)offsetof(Vertex3D, Position));
    //This is the color attribute pointer
    glVertexAttribPointer(1, 4, GL_UNSIGNED_BYTE, GL_TRUE, sizeof(Vertex3D), (void*)offsetof(Vertex3D, Color));
    //This is the UV attribute pointer
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex3D), (void*)offsetof(Vertex3D, Uv));

    glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex3D), (void*)offsetof(Vertex3D, Normal));

    glBindVertexArray(0);

   /* {
        QFile file(":/texture.png");
        file.open((QIODevice::ReadOnly));
        QByteArray byteArray = file.readAll();
        const unsigned char* imageData = (const unsigned char*)byteArray.data();
        size_t imageSize = byteArray.size();
        std::vector < unsigned char > outImage;
        unsigned long imageWidth = 0, imageHeight = 0;
        int result = decodePNG(outImage, imageWidth, imageHeight, imageData, imageSize, true);
        if (result != 0) {
            std::cout << "failed to decode PNG" << std::endl;
        }
        glGenTextures(1, &m_texture);
        glBindTexture(GL_TEXTURE_2D, m_texture);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, imageWidth, imageHeight, 0, GL_RGBA, GL_UNSIGNED_BYTE, outImage.data());
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
        glGenerateMipmap(GL_TEXTURE_2D);
        glBindTexture(GL_TEXTURE_2D, 0);
    }*/

    m_texture = 0;
    m_texture = loadTexture(":/plaster_material.png");
}
void Widget3D::updateWidget(){

}

bool Widget3D::event(QEvent *event)
{
    if (event->type() == QEvent::KeyPress) {
        QKeyEvent *ke = static_cast<QKeyEvent *>(event);
        if (ke->key() == Qt::Key_Tab) {
            return true;
        } else if (ke->key() == Qt::Key_Up)
        {
            cameraPosition.z += 0.01;
        }
        else if (ke->key() == Qt::Key_Down)
        {
            cameraPosition.z -= 0.01;
        }
        else if (ke->key() == Qt::Key_Right)
        {
            cameraPosition.x -= 0.01;
        }
        else if (ke->key() == Qt::Key_Left)
        {
            cameraPosition.x += 0.01;
        }
        else if (ke->key() == Qt::Key_Z)
        {
            cameraPosition.y -= 0.01;
        }
        else if (ke->key() == Qt::Key_S)
        {
            cameraPosition.y += 0.01;
        }
        else if (ke->key() == Qt::Key_Minus)
        {
           // scale -= scale * 0.01;
            cameraDistance -= 0.1;
        }
        else if (ke->key() == Qt::Key_Plus)
        {
            //scale += scale * 0.01;

            cameraDistance += 0.1;
        }
        else if (ke->key() == Qt::Key_0)
        {
           // pitch += 1.1;

            rotateX += 0.1;
        }
        else if (ke->key() == Qt::Key_1)
        {
           // pitch -= 1.1;
            rotateX -= 0.1;
        }
    }
    return QWidget::event(event);
}

static void drawBox(float X, float Y, float Z, float HalfDims, uint32_t color, Vertex3D* verticies)
{
    float XMinusHalfDims = X - HalfDims;
    float XPlusHalfDims = X + HalfDims;

    float YMinusHalfDims = Y - HalfDims;
    float YPlusHalfDims = Y + HalfDims;

    float ZMinusHalfDims = Z - HalfDims;
    float ZPlusHalfDims = Z + HalfDims;

     verticies[0] = {XMinusHalfDims,YMinusHalfDims, ZMinusHalfDims, color, 0, 1};
     verticies[1] = {XMinusHalfDims,YMinusHalfDims, ZPlusHalfDims, color, 0, 1};
     verticies[2] = {XMinusHalfDims, YPlusHalfDims, ZPlusHalfDims, color, 0, 1};

     verticies[3] = {XPlusHalfDims, YPlusHalfDims,ZMinusHalfDims, color, 0, 1};
     verticies[4] = {XMinusHalfDims,YMinusHalfDims,ZMinusHalfDims, color, 0, 1};
     verticies[5] = {XMinusHalfDims, YPlusHalfDims,ZMinusHalfDims, color, 0, 1};

     verticies[6] = {XPlusHalfDims,YMinusHalfDims, ZPlusHalfDims, color, 0, 1};
     verticies[7] = {XMinusHalfDims,YMinusHalfDims,ZMinusHalfDims, color, 0, 1};
     verticies[8] = {XPlusHalfDims,YMinusHalfDims,ZMinusHalfDims, color, 0, 1};

     verticies[9] = {XPlusHalfDims, YPlusHalfDims,ZMinusHalfDims, color, 0, 1};
     verticies[10] = {XPlusHalfDims,YMinusHalfDims,ZMinusHalfDims, color, 0, 1};
     verticies[11] = {XMinusHalfDims,YMinusHalfDims,ZMinusHalfDims, color, 0, 1};

     verticies[12] = {XMinusHalfDims,YMinusHalfDims,ZMinusHalfDims, color, 0, 1};
     verticies[13] = {XMinusHalfDims, YPlusHalfDims, ZPlusHalfDims, color, 0, 1};
     verticies[14] = {XMinusHalfDims, YPlusHalfDims,ZMinusHalfDims, color, 0, 1};

     verticies[15] = {XPlusHalfDims,YMinusHalfDims, ZPlusHalfDims, color, 0, 1};
     verticies[16] = {XMinusHalfDims,YMinusHalfDims, ZPlusHalfDims, color, 0, 1};
     verticies[17] = {XMinusHalfDims,YMinusHalfDims,ZMinusHalfDims, color, 0, 1};

     verticies[18] = {XMinusHalfDims, YPlusHalfDims, ZPlusHalfDims, color, 0, 1};
     verticies[19] = {XMinusHalfDims,YMinusHalfDims, ZPlusHalfDims, color, 0, 1};
     verticies[20] = {XPlusHalfDims,YMinusHalfDims, ZPlusHalfDims, color, 0, 1};

     verticies[21] = {XPlusHalfDims, YPlusHalfDims, ZPlusHalfDims, color, 0, 1};
     verticies[22] = {XPlusHalfDims,YMinusHalfDims,ZMinusHalfDims, color, 0, 1};
     verticies[23] = {XPlusHalfDims, YPlusHalfDims,ZMinusHalfDims, color, 0, 1};

     verticies[24] = {XPlusHalfDims,YMinusHalfDims,ZMinusHalfDims, color, 0, 1};
     verticies[25] = {XPlusHalfDims, YPlusHalfDims, ZPlusHalfDims, color, 0, 1};
     verticies[26] = {XPlusHalfDims,YMinusHalfDims, ZPlusHalfDims, color, 0, 1};

     verticies[27] = {XPlusHalfDims, YPlusHalfDims, ZPlusHalfDims, color, 0, 1};
     verticies[28] = {XPlusHalfDims, YPlusHalfDims,ZMinusHalfDims, color, 0, 1};
     verticies[29] = {XMinusHalfDims, YPlusHalfDims,ZMinusHalfDims, color, 0, 1};

     verticies[30] = {XPlusHalfDims, YPlusHalfDims, ZPlusHalfDims, color, 0, 1};
     verticies[31] = {XMinusHalfDims, YPlusHalfDims,ZMinusHalfDims, color, 0, 1};
     verticies[32] = {XMinusHalfDims, YPlusHalfDims, ZPlusHalfDims, color, 0, 1};

     verticies[33] = {XPlusHalfDims, YPlusHalfDims, ZPlusHalfDims, color, 0, 1};
     verticies[34] = {XMinusHalfDims, YPlusHalfDims, ZPlusHalfDims, color, 0, 1};
     verticies[35] = {XPlusHalfDims,YMinusHalfDims, ZPlusHalfDims, color, 0, 1};

}

void Widget3D::paintGL(){
    updateWidget();

    glViewport(0, 0, this->size().width(), this->size().height());
    glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);
    glClearColor(1.0f, 0.1f, 0.1f, 1.0f);

    m_program.use();

    /*printf("pitch %f\n", pitch);

    float horizontalDistance = cameraDistance * cos(glm::radians(pitch));
    float verticalDistance = cameraDistance * sin(glm::radians(pitch));

    float Theta = 45.f + angleAroundPosition;
    float OffsetX = horizontalDistance * sin(glm::radians(Theta));
    float OffsetZ = horizontalDistance * cos(glm::radians(Theta));

    printf("offset X %f      offsetY %f\n", OffsetX, OffsetZ);
    float CameraX = cameraPosition.X - OffsetX;
    float CameraY = cameraPosition.Y + verticalDistance;
    float CameraZ = cameraPosition.Z  - OffsetZ;*/


  //  glm::mat4 projectionMatrix = glm::perspective(45.f, 1.0f, 0.0001f, 100.0f);
    float Aspect = (float)this->size().width() / (float)this->size().height();
    glm::mat4 projectionMatrix =  glm::perspective(70.f, Aspect, 0.0001f, 100.0f) *
    glm::lookAt(cameraPosition, cameraPosition + Forward, Up);
    //glm::mat4 projectionMatrix = glm::ortho(0.f, 1.f, 0.f, 1.f);
    static float i = 0;
    i += 0.001f;


   /* projectionMatrix = glm::rotate(projectionMatrix, (GLfloat)rotateX, glm::vec3(0, 1, 0));
    projectionMatrix = glm::rotate(projectionMatrix, (GLfloat)rotateY, glm::vec3(0, 0, 1));
    projectionMatrix = glm::rotate(projectionMatrix, (GLfloat)rotateZ, glm::vec3(1, 0, 0));

    projectionMatrix = glm::translate(projectionMatrix, glm::vec3(CameraX, CameraY, CameraZ));*/

  //  projectionMatrix = glm::scale(projectionMatrix, glm::vec3(0.02, 0.02, 0.02) + scale);

    GLint pUniform = m_program.getUniformLocation("P");
    glUniformMatrix4fv(pUniform, 1, GL_FALSE, &projectionMatrix[0][0]);


    pUniform = m_program.getUniformLocation("lightPos");

    static V3 lightPos = {0, 0, -4.9f};
    QPoint p = QCursor::pos();
    p = mapFromGlobal(p);

    lightPos.x = (((float)this->size().width() - (float)p.x()) / (float)this->size().width()) * 14.f - 7.0f;
    lightPos.y = (((float)this->size().height() - (float)p.y()) / (float)this->size().height()) * 14.f - 7.0f;

   /* static float diff = 0.1f;
    lightPos.x += diff;
    if (lightPos.x > 7.f || lightPos.x < -7.f)
    {
        diff = -diff;
    }*/
    glUniform3f(pUniform, lightPos.x, lightPos.y,  lightPos.z);

    pUniform = m_program.getUniformLocation("mySampler");
    glUniform1i(pUniform, 0);

   /* pUniform = m_program.getUniformLocation("Normals");
    glUniform1i(pUniform, 1);*/

    int begin = clock();

    int N = 0;
    for(int i = 0; i < objects.size(); i++)
    {
        Transform tr = objects[i];
        N += tr.Vertices->size();
    }

    for(int i = 0; i < particles.size(); i++)
    {
        Transform tr = particles[i];
        N += tr.Vertices->size();
    }

    Vertex3D *Vertices = (Vertex3D *)malloc(sizeof(Vertex3D) * (N));
    Vertex3D *ptr = Vertices;
    std::vector < Vertex3D > v;

    for(int i = 0; i < objects.size(); i++)
    {
        Transform &tr = objects[i];
        if (v.size() < tr.Vertices->size())
        {
            v.resize(tr.Vertices->size());
        }
        tr.Position = tr.Position + tr.Velocity;
        tr.Rotation = tr.Rotation + tr.RotationSpeed;
        tr.Scale += tr.ScalDiff;
        transform(v.data(), *tr.Vertices, tr.Scale, tr.Position, tr.Rotation);
        memcpy(ptr, v.data(), sizeof(Vertex3D) * tr.Vertices->size());
        ptr += tr.Vertices->size();
    }

    ArgStruct *argsA = new ArgStruct[particles.size() * 36];
    for(int i = 0; i < particles.size(); i++)
    {
        Transform &tr = particles[i];
        if (v.size() < tr.Vertices->size())
        {
            v.resize(tr.Vertices->size());
        }
        tr.Position = tr.Position + tr.Velocity;
        tr.Rotation = tr.Rotation + tr.RotationSpeed;
        tr.Scale += tr.ScalDiff;

        argsA[i].Scale = tr.Scale;
        argsA[i].Offset = tr.Position;
        argsA[i].Rotation = tr.Rotation;

        if (updateMethodGpu == false)
        {
            transform(v.data(), *tr.Vertices, tr.Scale, tr.Position, tr.Rotation);
            memcpy(ptr, v.data(), sizeof(Vertex3D) * tr.Vertices->size());
            ptr += tr.Vertices->size();
        }
    }
    if (updateMethodGpu == true)
    {
         executeOpenCLFunction(OpenCLSrc, "transform",
                              sizeof(Vertex3D), sizeof(ArgStruct), 36, particles.size() * 36,
                             cubeModel.data(), ptr, argsA);
    }
    delete argsA;


    qDebug() << "time to update " << clock() - begin;
    begin = clock();

    // Bind our VBO
    glBindBuffer(GL_ARRAY_BUFFER, m_vbo);
    // Orphan the buffer (for speed)
    glBufferData(GL_ARRAY_BUFFER, (N) * sizeof(Vertex3D), nullptr, GL_DYNAMIC_DRAW);
    // Upload the data
    glBufferSubData(GL_ARRAY_BUFFER, 0, (N) * sizeof(Vertex3D), Vertices);

    free(Vertices);

    // Unbind the VBO
    glBindBuffer(GL_ARRAY_BUFFER, 0);


    glBindVertexArray(m_vao);

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, m_texture);

    glDrawArrays(GL_TRIANGLES, 0, N);

    glBindVertexArray(0);

    qDebug() << "time to draw " << clock() - begin;

  m_program.unuse();


  m_frames++;

  if (m_frames >= 300)
  {
      particles.clear();
      for(int i = 0; i < 256 * 16; i++)
      {

          float random1 = ((rand() % 1000) - (rand() % 1000)) / 10000.f;
          float random2 = ((rand() % 1000) - (rand() % 1000)) / 10000.f;
          Transform transform = {};
          transform.Position = {2.0, 1.2, 0};
          transform.Scale = 0.1f;
          transform.Rotation = {0.0f, 0, 0.f};
          transform.Velocity = {random2, random1, 0.00f};
          transform.RotationSpeed = {0.05f, 0.001, 0.01f};
          transform.ScalDiff = 0.000f;
          transform.Vertices = &cubeModel;
          particles.push_back(transform);
      }
      m_frames = 0;
  }
}

