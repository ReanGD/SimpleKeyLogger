#include "editor/editor.h"

#include "engine/common/path.h"


Editor::Editor(Engine& engine)
    : m_engine(engine)
    , m_generalScene(engine)
    , m_interface(engine) {

}

void Editor::Create() {
    auto& fileManager = FileManager::Get();
    fileManager.AddRootAlias("$tex", std::filesystem::current_path() / "assets" / "textures");

    SetEditorMode(m_editorMode);

    m_interface.Create();
    m_generalScene.Create();
}

void Editor::Update(float deltaTime) {
    auto& window = m_engine.GetWindow();
    WindowInput& wio = window.GetIO();

    if (wio.IsKeyReleasedFirstTime(Key::Escape)) {
        window.Close();
    }

    if (wio.IsKeyReleasedFirstTime(Key::F11)) {
        window.SetFullscreen(!window.IsFullscreen());
    }

    // if (wio.IsKeyReleasedFirstTime(Key::F2)) {
    //     SetEditorMode(!m_editorMode);
    // }

    if (wio.IsKeyReleasedFirstTime(Key::L)) {
        m_engine.SetFillPoligone(!m_engine.IsFillPoligone());
    }

    // if (!m_editorMode) {
    //     auto camera = m_scene.GetCamera();
    //     auto pos = camera->GetPosition();

    //     if (wio.IsKeyReleasedFirstTime(Key::MouseLeft)) {
    //         glm::vec2 coord(0);
    //         glm::vec3 rayWorld = camera->HomogeneousPositionToRay(coord);

    //         glm::vec3 collisionPos;
    //         if (m_engine.GetPhysics().RayTest(pos, pos + rayWorld*100.0f, collisionPos)) {
    //             Mesh sphere;
    //             sphere.Add(GeometryGenerator::CreateSolidSphere(10), *m_materialCube);
    //             auto mat = glm::scale(glm::translate(glm::mat4(1), collisionPos), glm::vec3(0.2f));
    //             sphere.SetModelMatrix(mat);
    //             m_scene.Add(sphere);
    //         }
    //     }

    //     if (wio.IsKeyReleasedFirstTime(Key::MouseRight)) {
    //         Mesh cube;
    //         cube.Add(GeometryGenerator::CreateSolidCube(), *m_materialCube);
    //         auto pcube = std::make_shared<PhysicalBox>(glm::vec3(0.5, 0.5, 0.5), pos, 10);
    //         m_engine.GetPhysics().AddNode(pcube);
    //         cube.SetPhysicalNode(pcube);
    //         m_scene.Add(cube);
    //     }
    // }

    m_generalScene.Update(deltaTime);
}

void Editor::Draw() {
    m_generalScene.Draw();

    // m_fbo->Bind(2000, 2000);
    // glViewport(0, 0, 2000, 2000);
    // glClearColor(0.0f, 0.0f, 1.0f, 1.0f);
    // glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
    // m_scene.Draw();
    // auto fboTex = m_fbo->Unbind();

    // m_interface.Draw(m_editorMode, camera, fboTex, m_scene.GetCountTriangles());
}

void Editor::Destroy() {
    m_generalScene.Destroy();
    m_interface.Destroy();
}

void Editor::SetEditorMode(bool value) {
    m_editorMode = value;
    m_engine.GetWindow().SetCursor(m_editorMode ? CursorType::Arrow : CursorType::Disabled);
    m_engine.GetGui().EnableInput(m_editorMode);
    // m_controller.EnableInput(!m_editorMode);
}
