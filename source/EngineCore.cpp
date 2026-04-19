#include "EngineCore.h"

void EngineCore::updateResolution(float width, float height)
{
    if (!m_cameraModule)
    {
        m_cameraModule = std::make_unique<gfx::Camera>(width, height);
    }
    else
    {
        m_cameraModule->updateScreenResolution(width, height);
    }
}

void EngineCore::init(QOpenGLExtraFunctions* openGLFunctions)
{
    m_openGLFunctions = openGLFunctions;

    // Detect and load asset paths
    m_AssetRegistryModule = std::make_unique<AssetRegistry>();

    // Load up all the detected textures
    m_textureManagerModule = std::make_unique<gfx::TextureManager>(m_AssetRegistryModule.get(), m_openGLFunctions);

    // Load and compile shaders
    m_shaderManagerModule = std::make_unique<gfx::ShaderManager>(m_AssetRegistryModule.get(), m_openGLFunctions);
    
    // Init mesh manager
    m_sceneModelManagerModule = std::make_unique<gfx::SceneModelManager>(m_AssetRegistryModule.get(), m_openGLFunctions);

    // Init material manager
    m_materialManagerModule = std::make_unique<gfx::MaterialManager>();

    // Init entity manager
    m_entityManagerModule = std::make_unique<EntityManager>();

    // Init Lighting System
    m_lightingSystemModule = std::make_unique<gfx::LightingSystem>();

    // Render init
    m_renderModule = std::make_unique<gfx::Renderer>(openGLFunctions);
    m_renderSystemModule = std::make_unique<gfx::RenderSystem>();

    emit engineInitialised(this);
}

void EngineCore::runStep()
{
    m_renderSystemModule->runRender(m_cameraModule.get(), m_renderModule.get(), m_entityManagerModule.get(), m_lightingSystemModule.get(), m_shaderManagerModule.get());

    if (m_shaderManagerModule->shaderAvailable())
    {
        for (auto& [key, shader] : m_shaderManagerModule->getCompiledMap())
        {
            shader->useProgram();
            shader->updateViewMatrixValue(m_cameraModule.get()->calculateViewMatrix());
            shader->updateProjectionMatrixValue(m_cameraModule.get()->calculateProjectionMatrix());

            m_sceneModelManagerModule->drawScene(shader.get());
            break;
        }
    }
}
