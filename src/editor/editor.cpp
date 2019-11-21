#include "editor/editor.h"

#include <spdlog/spdlog.h>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/random.hpp>

#include "engine/api/gl.h"
#include "engine/common/path.h"
#include "engine/common/exception.h"
#include "engine/heightmap/heightmap.h"
#include "engine/mesh/geometry_generator.h"
#include "engine/material/texture_manager.h"


Editor::Editor(Engine& engine)
    : m_engine(engine)
    , m_interface(engine) {

}

void Editor::Create() {
    m_interface.Create();
    SetEditorMode(m_editorMode);

    auto& texManager = TextureManager::Get();
    auto& fileManager = FileManager::Get();
    fileManager.AddRootAlias("$tex", std::filesystem::current_path() / "assets" / "textures");

    auto camera = m_scene.GetCamera();
    camera->SetViewParams(glm::vec3(-10, 20, 0), glm::vec3(1, 0, 0));
    m_controller.AttachCamera(camera);

    auto shaderTex = Shader::Create("vertex_old", "fragment_tex");
    auto shaderLandscape = Shader::Create("vertex_landscape", "fragment_tex");
    auto shaderTexLight = Shader::Create("vertex", "fragment_tex_light");

    m_declCamera = shaderTexLight->GetUBDecl("ubCamera");
    if (!m_declCamera) {
        throw EngineError("Сouldn't found uniform block '{}' in shader", "ubCamera");
    }

    m_ubCamera = std::make_shared<UniformBuffer>(m_declCamera->GetSize());
    m_ubCamera->setUniform(m_declCamera->GetOffset("uProjMatrix"), camera->GetProjMatrix());
    m_ubCamera->setUniform(m_declCamera->GetOffset("uViewMatrix"), camera->GetViewMatrix());
    m_ubCamera->Sync();

    GLuint index = 0;
    glUniformBlockBinding(shaderTexLight->GetHandle(), m_declCamera->GetIndex(), index);
    m_ubCamera->Bind(index);

    auto shaderClrLight = Shader::Create("vertex_old", "fragment_clr_light");
    auto shaderLine = Shader::Create("vertex_line", "fragment_clr");
    auto shaderSprite = Shader::Create("vertex_sprite", "fragment_clr");
    auto shaderNormals = Shader::Create("vertex_normals", "geometry_normals", "fragment_normals");

    auto texture = texManager.Load("$tex/brice.jpg");
    auto groundTex = texManager.Load("$tex/ground.jpg");
    auto heightmapTex = texManager.Load("$tex/heightmap2.jpg");

    // auto heightmapBody = Heightmap().Load("$tex/heightmap2.jpg", error);
    // if (!heightmapBody) {
    //     return false;
    // }
    // m_engine.GetPhysics().AddNode(heightmapBody);

    Material materialTex(shaderTex);
    materialTex.SetBaseTexture(0, texture);

    Material materialSprite(shaderSprite);
    materialSprite.SetBaseColor(glm::vec3(0.4, 0.7, 0.1));

    m_materialCube = std::make_shared<Material>(shaderClrLight);
    m_materialCube->SetBaseColor(glm::vec3(0.9, 0.1, 0.1));

    Material materialGround(shaderTex);
    materialGround.SetBaseTexture(0, groundTex);

    Material materialLandscape(shaderLandscape);
    materialLandscape.SetBaseTexture(0, heightmapTex);
    m_materialLine = std::make_shared<Material>(shaderLine);
    m_materialLine->SetBaseColor(glm::vec3(0.9f, 0.9f, 0.1f));

    m_materialNormals = std::make_shared<Material>(shaderNormals);
    m_materialNormals->SetBaseColor(glm::vec3(0.9f, 0.9f, 0.4f));

    for (size_t i=0; i!=10; ++i) {
        Mesh cube;
        cube.Add(GeometryGenerator::CreateSolidCube(), materialTex);
        auto pcube = std::make_shared<PhysicalBox>(glm::vec3(0.5, 0.5, 0.5), glm::linearRand(glm::vec3(0, 25, -7), glm::vec3(12, 200, 7)), glm::linearRand(1, 100));
        m_engine.GetPhysics().AddNode(pcube);
        cube.SetPhysicalNode(pcube);
        m_scene.Add(cube);
    }

    Mesh plane;
    plane.Add(GeometryGenerator::CreateSolidPlane(2048, 2048, 1.0f, 1.0f), materialLandscape);
    auto matModel = glm::scale(glm::mat4(1.0), glm::vec3(2048., 1.0f, 2048.));
    plane.SetModelMatrix(matModel);
    m_scene.Add(plane);

    Mesh sprite;
    sprite.Add(GeometryGenerator::CreateSolidPlane(2, 2, 1.0f, 1.0f), materialSprite);
    m_scene.Add(sprite);

    m_line = GeometryGenerator::CreateLine(glm::vec3(0), glm::vec3(10.0f));

    const auto meshCntX = 1;
    const auto meshCntZ = 1;
    Material materialSphere(shaderTexLight);
    materialSphere.SetBaseColor(glm::vec3(0.6f, 0.1f, 0.1f));
    materialSphere.SetBaseTexture(0, groundTex);
    auto sphereGeom = GeometryGenerator::CreateSolidSphere(100);
    for(auto i=0; i!=meshCntX; ++i) {
        float posX = 1.5f * static_cast<float>(i) - static_cast<float>(meshCntX) * 1.5f / 2.0f;
        for(auto j=0; j!=meshCntZ; ++j) {
            Mesh mesh;
            mesh.Add(sphereGeom, materialSphere);

            float posZ = 2.5f * static_cast<float>(j) - static_cast<float>(meshCntZ) * 2.5f / 2.0f;
            matModel = glm::translate(glm::mat4(1.0), glm::vec3(posX, 0.51f, posZ));
            matModel = glm::scale(matModel, glm::vec3(1.0f, 1.0f, 2.0f));
            mesh.SetModelMatrix(matModel);
            m_scene.Add(mesh);
        }
    }

    m_fbo = std::make_shared<Framebuffer>();
}

void Editor::Update(float deltaTime) {
    auto& window = m_engine.GetWindow();
    WindowInput& wio = window.GetIO();

    m_scene.Update();
    if (wio.IsKeyReleasedFirstTime(Key::Escape)) {
        window.Close();
    }

    if (wio.IsKeyReleasedFirstTime(Key::F11)) {
        window.SetFullscreen(!window.IsFullscreen());
    }

    if (wio.IsKeyReleasedFirstTime(Key::F2)) {
        SetEditorMode(!m_editorMode);
    }

    if (wio.IsKeyReleasedFirstTime(Key::L)) {
        m_engine.SetFillPoligone(!m_engine.IsFillPoligone());
    }

    if (wio.IsKeyReleasedFirstTime(Key::N)) {
        m_showNormals = !m_showNormals;
    }

    if (m_editorMode) {
        if (wio.IsKeyReleasedFirstTime(Key::MouseLeft)) {
            glm::vec2 coord;
            wio.GetHomogeneousClipCursorPosition(coord.x, coord.y);
            auto camera = m_scene.GetCamera();
            glm::vec3 pos = camera->GetPosition();
            glm::vec3 rayWorld = camera->HomogeneousPositionToRay(coord);

            auto& vertexBuffer = m_line->GetVertexBuffer();
            VertexP* vb = static_cast<VertexP*>(vertexBuffer.Lock());
            vb[0].Position	= pos;
            vb[1].Position	= pos + rayWorld * 10.0f;
            if (!vertexBuffer.Unlock()) {
                throw std::runtime_error("Can't unlock vertex buffer in GeometryGenerator::CreateLine");
            }
        }
    } else {
        auto camera = m_scene.GetCamera();
        auto pos = camera->GetPosition();

        if (wio.IsKeyReleasedFirstTime(Key::MouseLeft)) {
            glm::vec2 coord(0);
            glm::vec3 rayWorld = camera->HomogeneousPositionToRay(coord);

            glm::vec3 collisionPos;
            if (m_engine.GetPhysics().RayTest(pos, pos + rayWorld*100.0f, collisionPos)) {
                Mesh sphere;
                sphere.Add(GeometryGenerator::CreateSolidSphere(10), *m_materialCube);
                auto mat = glm::scale(glm::translate(glm::mat4(1), collisionPos), glm::vec3(0.2f));
                sphere.SetModelMatrix(mat);
                m_scene.Add(sphere);
            }
        }

        if (wio.IsKeyReleasedFirstTime(Key::MouseRight)) {
            Mesh cube;
            cube.Add(GeometryGenerator::CreateSolidCube(), *m_materialCube);
            auto pcube = std::make_shared<PhysicalBox>(glm::vec3(0.5, 0.5, 0.5), pos, 10);
            m_engine.GetPhysics().AddNode(pcube);
            cube.SetPhysicalNode(pcube);
            m_scene.Add(cube);
        }
    }

    m_controller.Update(wio, deltaTime);
}

void Editor::Draw() {
    auto camera = m_scene.GetCamera();

    m_ubCamera->setUniform(m_declCamera->GetOffset("uProjMatrix"), camera->GetProjMatrix());
    m_ubCamera->setUniform(m_declCamera->GetOffset("uViewMatrix"), camera->GetViewMatrix());
    m_ubCamera->Sync();

    GLuint index = 0;
    m_ubCamera->Bind(index);

    m_scene.Draw();
    m_fbo->Bind(2000, 2000);
    glViewport(0, 0, 2000, 2000);
    glClearColor(0.0f, 0.0f, 1.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
    m_scene.Draw();
    auto fboTex = m_fbo->Unbind();
    if (m_showNormals) {
        m_scene.DrawWithMaterial(*m_materialNormals);
    }

    glm::mat4 matModel(1.0f);
    m_materialLine->Bind(camera, matModel, matModel);
    m_line->Bind();
    m_line->Draw();
    m_line->Unbind();
    m_materialLine->Unbind();

    m_interface.Draw(m_editorMode, camera, fboTex, m_scene.GetCountTriangles());
}

void Editor::Destroy() {
    m_interface.Destroy();

    m_materialLine.reset();
    m_materialNormals.reset();
    m_line.reset();
    m_fbo.reset();
}

void Editor::SetEditorMode(bool value) {
    m_editorMode = value;
    m_engine.GetWindow().SetCursor(m_editorMode ? CursorType::Arrow : CursorType::Disabled);
    m_engine.GetGui().EnableInput(m_editorMode);
    m_controller.EnableInput(!m_editorMode);
}
