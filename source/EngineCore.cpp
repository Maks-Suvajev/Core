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

// Preset shaders for testing - GUI will make it more flexible
static const std::vector<ShaderProgramFilenameStrings> shaderFilenames
{
    {"lightSource", "vertexShaderLightTest.vs", "fragmentShaderLightSource.fs"},
    {"normalObject", "vertexShaderLightTest.vs", "fragmentShaderLightTest.fs"}
};

void EngineCore::initTestModule()
{
    TestObjects testObjects;
    testObjects.textureManager  = m_textureManagerModule.get();
    testObjects.shaderManager   = m_shaderManagerModule.get(); 
    testObjects.entityManager   = m_entityManagerModule.get();
    testObjects.materialManager = m_materialManagerModule.get();
    testObjects.meshManager     = m_meshManagerModule.get();
    testObjects.lightingSystem   = m_lightingSystemModule.get();

    m_testModule = std::make_unique<Test>(testObjects);
}

void EngineCore::init(QOpenGLExtraFunctions* openGLFunctions)
{
    m_openGLFunctions = openGLFunctions;

    // Detect and load asset paths
    m_gfxAssetRegistryModule = std::make_unique<gfx::GfxAssetRegistry>();

    // Load up all the detected textures
    m_textureManagerModule = std::make_unique<gfx::TextureManager>(m_gfxAssetRegistryModule.get(), m_openGLFunctions);

    // Get the shader paths (from test data)
    std::vector<ShaderProgramFilePaths> shaderSources = m_gfxAssetRegistryModule->loadShaderPathSet(shaderFilenames);

    // Load and compile shaders
    m_shaderManagerModule = std::make_unique<gfx::ShaderManager>(shaderSources, m_openGLFunctions);
    
    // Init mesh manager
    m_meshManagerModule = std::make_unique<gfx::MeshManager>(m_openGLFunctions);

    // Init material manager
    m_materialManagerModule = std::make_unique<gfx::MaterialManager>();

    // Init entity manager
    m_entityManagerModule = std::make_unique<EntityManager>();

    // Init Lighting System
    m_lightingSystemModule = std::make_unique<gfx::LightingSystem>();

    initTestModule();

    m_testModule->initTestData();

    m_renderModule = std::make_unique<gfx::Renderer>(openGLFunctions);
    m_renderSystemModule = std::make_unique<gfx::RenderSystem>();

    emit engineInitialised(this);
}

void EngineCore::runStep()
{
    m_renderSystemModule->runRender(m_cameraModule.get(), m_renderModule.get(), m_entityManagerModule.get(), m_lightingSystemModule.get(), m_shaderManagerModule.get());
}
