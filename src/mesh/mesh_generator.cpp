#include "mesh/mesh_generator.h"


Mesh MeshGenerator::CreateSolidCube() {
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

    return Mesh(VertexPNTC::vDecl, vertexBuffer, indexBuffer);
}
