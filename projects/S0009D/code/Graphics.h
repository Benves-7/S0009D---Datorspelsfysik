#pragma once

#include "Vector4D.h"
#include "GraphicsNode.h"
#include <vector>
#include <fstream>

namespace CoreGraphics
{
    struct Nvx2Header
    {
        unsigned int magic;
        unsigned int numGroups;
        unsigned int numVertices;
        unsigned int vertexWidth;
        unsigned int numIndices;
        unsigned int numEdges;
        unsigned int vertexComponentMask;
    };

    struct Nvx2Group
    {
        unsigned int firstVertex;
        unsigned int numVertices;
        unsigned int firstTriangle;
        unsigned int numTriangles;
        unsigned int firstEdge;
        unsigned int numEdges;
    };

    struct PrimitiveGroup
    {
        unsigned int numVerticies;
        unsigned int baseIndex;
        unsigned int numIndicies;
        unsigned int primitiveTopology;
    };

    enum SemanticName
    {
        Position = 0,
        Normal = 1,
        TexCoord1 = 2,
        Tangent = 3,
        Binormal = 4,
        Color = 5,
        TexCoord2 = 6,
        SkinWeights = 7,
        SkinJIndices = 8,

        Invalid,
    };

    enum Format
    {
        Float,      //> one-component float, expanded to (float, 0, 0, 1)
        Float2,     //> two-component float, expanded to (float, float, 0, 1)
        Float3,     //> three-component float, expanded to (float, float, float, 1)
        Float4,     //> four-component float
        UByte4,     //> four-component unsigned byte
        Byte4,		//> four-component signed byte
        Short2,     //> two-component signed short, expanded to (value, value, 0, 1)
        Short4,     //> four-component signed short
        UByte4N,    //> four-component normalized unsigned byte (value / 255.0f)
        Byte4N,		//> four-component normalized signed byte (value / 127.0f)
        Short2N,    //> two-component normalized signed short (value / 32767.0f)
        Short4N,    //> four-component normalized signed short (value / 32767.0f)
    };

    enum N2VertexComponent
    {
        N2Coord = (1 << 0),      // 3 floats
        N2Normal = (1 << 1),      // 3 floats
        N2NormalB4N = (1 << 2),      // 4 unsigned bytes, normalized
        N2Uv0 = (1 << 3),      // 2 floats
        N2Uv0S2 = (1 << 4),      // 2 shorts, 4.12 fixed point
        N2Uv1 = (1 << 5),      // 2 floats
        N2Uv1S2 = (1 << 6),      // 2 shorts, 4.12 fixed point
        N2Uv2 = (1 << 7),      // 2 floats
        N2Uv2S2 = (1 << 8),      // 2 shorts, 4.12 fixed point
        N2Uv3 = (1 << 9),      // 2 floats
        N2Uv3S2 = (1 << 10),     // 2 shorts, 4.12 fixed point
        N2Color = (1 << 11),     // 4 floats
        N2ColorUB4N = (1 << 12),     // 4 unsigned bytes, normalized
        N2Tangent = (1 << 13),     // 3 floats
        N2TangentB4N = (1 << 14),     // 4 unsigned bytes, normalized
        N2Binormal = (1 << 15),     // 3 floats
        N2BinormalB4N = (1 << 16),     // 4 unsigned bytes, normalized
        N2Weights = (1 << 17),     // 4 floats
        N2WeightsUB4N = (1 << 18),     // 4 unsigned bytes, normalized
        N2JIndices = (1 << 19),     // 4 floats
        N2JIndicesUB4 = (1 << 20),     // 4 unsigned bytes

        N2NumVertexComponents = 21,
        N2AllComponents = ((1 << N2NumVertexComponents) - 1),
    };
} // namespace CoreGraphics

using namespace std; 
using namespace CoreGraphics;
struct vertexComponent
{
    SemanticName name;
    unsigned int index;
    Format format;
};

class Graphics
{
public:
    Graphics() {}
    ~Graphics() {}

    void loadMesh(char* fileName)
    {
        std::ifstream file;
        file.open(fileName, std::ifstream::in | std::ifstream::binary);
        file.seekg(0, file.end);
        unsigned int length = file.tellg();
        file.seekg(0, file.beg);
        char* ptr = new char[length];
        file.read(ptr, length);
        file.close();

        Nvx2Header* h = (Nvx2Header*)ptr;
        header = h;
        header->numIndices *= 3;

        numGroups = h->numGroups;
        numVertices = h->numVertices;
        vertexWidth = h->vertexWidth;
        numIndices = h->numIndices;
        numEdges = h->numEdges;
        vertexComponentMask = h->vertexComponentMask;
        groupDataSize = 6 * sizeof(unsigned int) * numGroups;                          // Nvx2Group contains 6 unsigned int.
        vertexDataSize = numVertices * vertexWidth * sizeof(GLfloat);                   // selfexplaned.
        indexDataSize = 3 * sizeof(int) * numIndices;                                  // numIndices is a group of 3 int.

        vertexDataPtr = ((uchar*)ptr) + groupDataSize;
        indexDataPtr = ((uchar*)vertexDataPtr) + vertexDataSize;

        ptr += sizeof(Nvx2Header);

        Nvx2Group* g = (Nvx2Group*)ptr;
        for (int i = 0; i < (size_t)numGroups; i++)
        {
            PrimitiveGroup p;
            p.numVerticies = g->numVertices;
            p.baseIndex = g->firstTriangle * 3;
            p.numIndicies = g->numTriangles * 3;
            p.primitiveTopology = 4;
            primGroups.push_back(p);
            g++;
        }

        for (int i = 0; i < N2NumVertexComponents; i++)
        {
            SemanticName sem;
            Format fmt;
            size_t index = 0;

            if (vertexComponentMask & (1 << i))
            {
                switch (1 << i)
                {
                case N2Coord:
                    sem = SemanticName::Position;
                    fmt = Format::Float3;
                    break;
                case N2Normal:
                    sem = SemanticName::Normal;
                    fmt = Format::Float3;
                    break;
                case N2NormalB4N:
                    sem = SemanticName::Normal;
                    fmt = Format::Byte4N;
                    break;
                case N2Uv0:
                    sem = SemanticName::TexCoord1;
                    fmt = Format::Float2;
                    index = 0;
                    break;
                case N2Uv0S2:
                    sem = SemanticName::TexCoord1;
                    fmt = Format::Short2;
                    index = 0;
                    break;
                case N2Uv1:
                    sem = SemanticName::TexCoord2;
                    fmt = Format::Float2;
                    index = 1;
                    break;
                case N2Uv1S2:
                    sem = SemanticName::TexCoord2;
                    fmt = Format::Short2;
                    index = 1;
                    break;
                case N2Color:
                    sem = SemanticName::Color;
                    fmt = Format::Float4;
                    break;
                case N2ColorUB4N:
                    sem = SemanticName::Color;
                    fmt = Format::UByte4N;
                    break;
                case N2Tangent:
                    sem = SemanticName::Tangent;
                    fmt = Format::Float3;
                    break;
                case N2TangentB4N:
                    sem = SemanticName::Tangent;
                    fmt = Format::Byte4N;
                    break;
                case N2Binormal:
                    sem = SemanticName::Binormal;
                    fmt = Format::Float3;
                    break;
                case N2BinormalB4N:
                    sem = SemanticName::Binormal;
                    fmt = Format::Byte4N;
                    break;
                case N2Weights:
                    sem = SemanticName::SkinWeights;
                    fmt = Format::Float4;
                    break;
                case N2WeightsUB4N:
                    sem = SemanticName::SkinWeights;
                    fmt = Format::UByte4N;
                    break;
                case N2JIndices:
                    sem = SemanticName::SkinJIndices;
                    fmt = Format::Float4;
                    break;
                case N2JIndicesUB4:
                    sem = SemanticName::SkinJIndices;
                    fmt = Format::UByte4;
                    break;
                default:
                    sem = SemanticName::Position;
                    fmt = Format::Float3;
                    break;
                }
                vertexComponent vertexC;
                vertexC.name = sem;
                vertexC.index = index;
                vertexC.format = fmt;

                vertexComponents.push_back(vertexC);
            }
        }
    }

    void draw()
    {
        //glUseProgram(0);
        glDrawElements(GL_TRIANGLES, numIndices, GL_UNSIGNED_INT, 0);
    }

    void setupMesh()
    {
        generateHandels();
        setupBuffers();
        bindBuffers(vertexDataPtr, vertexDataSize, indexDataPtr, indexDataSize);
        loadMeshBuffers();
    }

    void generateHandels()
    {
        glGenBuffers(1, &VBO);
        glGenBuffers(1, &EBO);

        glGenVertexArrays(1, &VAO);
    }
    void setupBuffers()
    {
        glGenBuffers(1, &VBO);
        glBindBuffer(GL_ARRAY_BUFFER, VBO);

        glGenBuffers(1, &EBO);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);

    }
    void bindBuffers(const void* vb, unsigned int vbSize, const void* ib, unsigned int ibSize)
    {
        glBindBuffer(GL_ARRAY_BUFFER, VBO);
        glBufferData(GL_ARRAY_BUFFER, vbSize, vb, GL_STATIC_DRAW);

        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, ibSize, ib, GL_STATIC_DRAW);

        glBindVertexArray(VAO);
    }
    void loadMeshBuffers()
    {
        int offset = 0;
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 10 * sizeof(GLfloat), (void*)0);
        offset += sizeof(GLfloat) * 3;
        glEnableVertexAttribArray(1);
        glVertexAttribPointer(1, 4, GL_BYTE, GL_FALSE, 10 * sizeof(GLfloat), (void*)offset);
        offset += sizeof(GLbyte) * 4;
        glEnableVertexAttribArray(2);
        glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 10 * sizeof(GLfloat), (void*)offset);
        offset += sizeof(GLfloat) * 2;
        glEnableVertexAttribArray(3);
        glVertexAttribPointer(3, 4, GL_BYTE, GL_FALSE, 10 * sizeof(GLfloat), (void*)offset);
        offset += sizeof(GLbyte) * 4;
        glEnableVertexAttribArray(4);
        glVertexAttribPointer(4, 4, GL_BYTE, GL_TRUE, 10 * sizeof(GLfloat), (void*)offset);
        offset += sizeof(GLbyte) * 4;
        glEnableVertexAttribArray(5);
        glVertexAttribPointer(5, 4, GL_UNSIGNED_BYTE, GL_TRUE, 10 * sizeof(GLfloat), (void*)offset);
        offset += sizeof(GLbyte) * 4;
        glEnableVertexAttribArray(6);
        glVertexAttribPointer(6, 4, GL_UNSIGNED_BYTE, GL_FALSE, 10 * sizeof(GLfloat), (void*)offset);
        glBindVertexArray(0);
        //int offset = 0;
        //glEnableVertexAttribArray(0);
        //glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 10 * sizeof(GLfloat), (void*)0);
        //offset += sizeof(GLfloat) * 3;
        //glEnableVertexAttribArray(1);
        //glVertexAttribPointer(1, 4, GL_BYTE, GL_FALSE, 10 * sizeof(GLbyte), (void*)offset);
        //offset += sizeof(GLbyte) * 4;
        //glEnableVertexAttribArray(2);
        //glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 10 * sizeof(GLfloat), (void*)offset);
        //offset += sizeof(GLfloat) * 2;
        //glEnableVertexAttribArray(3);
        //glVertexAttribPointer(3, 4, GL_BYTE, GL_FALSE, 10 * sizeof(GLbyte), (void*)offset);
        //offset += sizeof(GLbyte) * 4;
        //glEnableVertexAttribArray(4);
        //glVertexAttribPointer(4, 4, GL_BYTE, GL_TRUE, 10 * sizeof(GLbyte), (void*)offset);
        //offset += sizeof(GLbyte) * 4;
        //glEnableVertexAttribArray(5);
        //glVertexAttribPointer(5, 4, GL_UNSIGNED_BYTE, GL_TRUE, 10 * sizeof(GLbyte), (void*)offset);
        //offset += sizeof(GLbyte) * 4;
        //glEnableVertexAttribArray(6);
        //glVertexAttribPointer(6, 4, GL_UNSIGNED_BYTE, GL_FALSE, 10 * sizeof(GLbyte), (void*)offset);
        //glBindVertexArray(0);
    }

    void unbindBuffers()
    {
        glBindVertexArray(0);
        glBindBuffer(GL_ARRAY_BUFFER, 0);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
    }


    // ------------------------------------------------------------------------

    GraphicsNode gNode;

    Nvx2Header* header;

    vector<PrimitiveGroup> primGroups;
    vector<vertexComponent> vertexComponents;

    void* groupDataPtr;
    void* vertexDataPtr;
    void* indexDataPtr;

    size_t groupDataSize;
    size_t vertexDataSize;
    size_t indexDataSize;

    unsigned int numGroups;
    unsigned int numVertices;
    unsigned int vertexWidth;
    unsigned int numIndices;
    unsigned int numEdges;
    unsigned int vertexComponentMask;

    uint32 EBO;
    uint32 VBO;
    uint32 VAO;
};