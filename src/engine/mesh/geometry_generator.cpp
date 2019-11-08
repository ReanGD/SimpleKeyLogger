#include "engine/mesh/geometry_generator.h"

#include <glm/gtc/matrix_transform.hpp>
#include "engine/common/exception.h"


std::shared_ptr<Lines> GeometryGenerator::CreateLine(const glm::vec3& from, const glm::vec3& to) {
    VertexBuffer vertexBuffer(sizeof(VertexP) * 2);
    VertexP* vb = static_cast<VertexP*>(vertexBuffer.Lock());
    vb[0].Position	= from;
    vb[1].Position	= to;
    if (!vertexBuffer.Unlock()) {
        throw EngineError("can't unlock vertex buffer in GeometryGenerator::CreateLine");
    }

    return std::make_shared<Lines>(VertexP::vDecl, vertexBuffer);
}

std::shared_ptr<Geometry> GeometryGenerator::CreateSolidCube() {
    VertexPNTC vb[24];
    vb[ 0].Position	= glm::vec3(-0.5f,-0.5f,-0.5f);
    vb[ 1].Position	= glm::vec3(-0.5f, 0.5f,-0.5f);
    vb[ 2].Position	= glm::vec3( 0.5f, 0.5f,-0.5f);
    vb[ 3].Position	= glm::vec3( 0.5f,-0.5f,-0.5f);
    vb[ 4].Position	= glm::vec3( 0.5f,-0.5f, 0.5f);
    vb[ 5].Position	= glm::vec3( 0.5f, 0.5f, 0.5f);
    vb[ 6].Position	= glm::vec3(-0.5f, 0.5f, 0.5f);
    vb[ 7].Position	= glm::vec3(-0.5f,-0.5f, 0.5f);
    vb[ 8].Position	= glm::vec3(-0.5f,-0.5f, 0.5f);
    vb[ 9].Position	= glm::vec3(-0.5f, 0.5f, 0.5f);
    vb[10].Position	= glm::vec3(-0.5f, 0.5f,-0.5f);
    vb[11].Position	= glm::vec3(-0.5f,-0.5f,-0.5f);
    vb[12].Position	= glm::vec3( 0.5f,-0.5f,-0.5f);
    vb[13].Position	= glm::vec3( 0.5f, 0.5f,-0.5f);
    vb[14].Position	= glm::vec3( 0.5f, 0.5f, 0.5f);
    vb[15].Position	= glm::vec3( 0.5f,-0.5f, 0.5f);
    vb[16].Position	= glm::vec3(-0.5f,-0.5f, 0.5f);
    vb[17].Position	= glm::vec3(-0.5f,-0.5f,-0.5f);
    vb[18].Position	= glm::vec3( 0.5f,-0.5f,-0.5f);
    vb[19].Position	= glm::vec3( 0.5f,-0.5f, 0.5f);
    vb[20].Position	= glm::vec3(-0.5f, 0.5f,-0.5f);
    vb[21].Position	= glm::vec3(-0.5f, 0.5f, 0.5f);
    vb[22].Position	= glm::vec3( 0.5f, 0.5f, 0.5f);
    vb[23].Position	= glm::vec3( 0.5f, 0.5f,-0.5f);

    for(int i=0,j=0; i<6; ++i) {
        vb[j++].TexCoord = glm::vec2(0.0f,1.0f);
        vb[j++].TexCoord = glm::vec2(0.0f,0.0f);
        vb[j++].TexCoord = glm::vec2(1.0f,0.0f);
        vb[j++].TexCoord = glm::vec2(1.0f,1.0f);
    }

    for(int i=0; i<8; ++i) {
        float zn = (i<4) ? -1.0f : 1.0f;
        vb[i+ 0].Normal = glm::vec3(0.0f, 0.0f,  zn);
        vb[i+ 8].Normal = glm::vec3(zn,   0.0f, 0.0f);
        vb[i+16].Normal = glm::vec3(0.0f, zn,   0.0f);
    }

// 	CAABB box;
    for(int i=0; i<24; ++i)	{
// 		box.Add(LockVB[i].Position);
        vb[i].Tangent = glm::vec3(0.0f,1.0f,0.0f);
    }
    VertexBuffer vertexBuffer(vb, sizeof(vb));

    uint16_t ib[12 * 3];
    for(uint16_t i=0,j=0;i<6;++i) {
        uint16_t sm = i*4;
        ib[j++]=sm; ib[j++]=sm+1; ib[j++]=sm+2;
        ib[j++]=sm; ib[j++]=sm+2; ib[j++]=sm+3;
    }
    IndexBuffer indexBuffer(ib, sizeof(ib));

    return std::make_shared<Geometry>(VertexPNTC::vDecl, vertexBuffer, indexBuffer);
}

std::shared_ptr<Geometry> GeometryGenerator::CreateSolidSphere(uint16_t cntVertexCircle) {
    cntVertexCircle = glm::min(cntVertexCircle, uint16_t(363));
    uint16_t plg = cntVertexCircle/2 - 1;

    // CAABB box;
    float B = -glm::half_pi<float>();
    float stepB = glm::pi<float>() / float(plg + 1);
    float stepA = glm::two_pi<float>() / float(cntVertexCircle - 1);

    uint32_t ind = 1;
    uint16_t vertexCnt = plg*cntVertexCircle + 2;
    auto* vb = new VertexPNTC[vertexCnt];
    for(auto ix=0; ix!=plg; ++ix) {
        B += stepB;
        float Vy = glm::sin(B);
        float rad = glm::cos(B);
        float tv = (1.0f - Vy) / 2.0f;

        float A = 0.0f;
        for(auto iy=0; iy!=cntVertexCircle; iy++) {
            float s = glm::sin(A);
            float c = glm::cos(A);

            vb[ind].Position = glm::vec3(rad * c, Vy, rad * s) * 0.5f;
            vb[ind].TexCoord = glm::vec2(A / glm::two_pi<float>(), tv);
            vb[ind].Normal   = glm::normalize(vb[ind].Position);
            vb[ind].Tangent  = glm::vec3(0.0f, 1.0f, 0.0f);
            // box.Add(vb[ind].Position);
            ind++;
            A+=stepA;
        }
    }

    vb[0]			= VertexPNTC{glm::vec3(0.0f,-0.5f,0.0f), glm::vec3(0.0f,-1.0f,0.0f), glm::vec3(0.0f,1.0f,0.0f), glm::vec2(0.5f,1.0f)};
    vb[vertexCnt-1]	= VertexPNTC{glm::vec3(0.0f, 0.5f,0.0f), glm::vec3(0.0f, 1.0f,0.0f), glm::vec3(0.0f,1.0f,0.0f), glm::vec2(0.5f,0.0f)};
    // box.Add(vb[0].Position);
    // box.Add(vb[VertexCnt-1].Position);


    ind=0;
    size_t indexCnt = 6*(cntVertexCircle-1)*plg;
    uint16_t* ib = new uint16_t[indexCnt];
    for(uint16_t ix=0; ix!=plg-1; ++ix) {
        uint16_t z1,z2,z3,z4;
        z1=ix*cntVertexCircle+1;	z2=z1+1;
        z3=z1+cntVertexCircle;		z4=z2+cntVertexCircle;
        for(auto iy=0; iy!=cntVertexCircle-1; iy++) {
            ib[ind++]=z1;	ib[ind++]=z3;	ib[ind++]=z4;
            ib[ind++]=z1;	ib[ind++]=z4;	ib[ind++]=z2;
            z1++; z2++; z3++; z4++;
        }
    }

    uint16_t iy = cntVertexCircle*(plg-1);
    for(uint16_t ix=1; ix!=cntVertexCircle; ix++) {
        ib[ind++]=ix;       ib[ind++]=ix+1;   ib[ind++]=0;
        ib[ind++]=iy+ix+1;  ib[ind++]=iy+ix;  ib[ind++]=vertexCnt-1;
    }


    VertexBuffer vertexBuffer(vb, vertexCnt * sizeof(VertexPNTC));
    delete []vb;

    IndexBuffer indexBuffer(ib, indexCnt * sizeof(*ib));
    delete []ib;

    return std::make_shared<Geometry>(VertexPNTC::vDecl, vertexBuffer, indexBuffer);
}

template<class T>
std::shared_ptr<Geometry> CreateSolidPlane(uint32_t cntXSides, uint32_t cntZSides, float scaleTextureX, float scaleTextureZ) {
    // CAABB box;
    uint32_t ind = 0;
    uint32_t vertexCnt = (cntXSides+1)*(cntZSides+1);
    auto* vb = new VertexPNTC[vertexCnt];
    for(uint32_t i=0; i!=cntXSides + 1; ++i) {
        float tu = static_cast<float>(i)/static_cast<float>(cntXSides);
        for(uint32_t j=0; j!=cntZSides + 1; ++j) {
            float tv = static_cast<float>(j)/static_cast<float>(cntZSides);
            vb[ind].Position	= glm::vec3(tu-0.5f, 0.0f, tv-0.5f);
            vb[ind].Normal		= glm::vec3(0.0f,    1.0f, 0.0f);
            vb[ind].Tangent		= glm::vec3(0.0f,    1.0f, 0.0f);
            vb[ind].TexCoord	= glm::vec2(scaleTextureX*tu, scaleTextureZ*tv);
            // box.Add(vb[ind].Position);
            ++ind;
        }
    }


    ind = 0;
    uint32_t indexCnt = cntXSides*cntZSides*6;
    T* ib = new T[indexCnt];
    for(T i=0; i!=cntXSides; ++i) {
        for(T j=0; j!=cntZSides; ++j) {
            T z1 = i * static_cast<T>(cntXSides + 1) + j;
            T z2 = z1 + static_cast<T>(cntZSides + 1);
            ib[ind++] = z1;
            ib[ind++] = z1+1;
            ib[ind++] = z2;
            ib[ind++] = z2;
            ib[ind++] = z1+1;
            ib[ind++] = z2+1;
        }
    }


    VertexBuffer vertexBuffer(vb, vertexCnt * sizeof(VertexPNTC));
    delete []vb;

    IndexBuffer indexBuffer(ib, indexCnt * sizeof(T));
    delete []ib;

    return std::make_shared<Geometry>(VertexPNTC::vDecl, vertexBuffer, indexBuffer);
}

std::shared_ptr<Geometry> GeometryGenerator::CreateSolidPlane(uint32_t cntXSides, uint32_t cntZSides, float scaleTextureX, float scaleTextureZ) {
    cntXSides = glm::max(cntXSides, uint32_t(2));
    cntZSides = glm::max(cntZSides, uint32_t(2));

    if (cntXSides*cntZSides*6 <= std::numeric_limits<uint16_t>::max()) {
        return ::CreateSolidPlane<uint16_t>(cntXSides, cntZSides, scaleTextureX, scaleTextureZ);
    } else {
        return ::CreateSolidPlane<uint32_t>(cntXSides, cntZSides, scaleTextureX, scaleTextureZ);
    }
}
