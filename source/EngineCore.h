#ifndef ENGINE_CORE_H
#define ENGINE_CORE_H

// STL
#include <memory> 
#include <vector>
#include <string>

// OS access 


// QT
#include <QOpenGLExtraFunctions>
#include "QObject"

// Resource managers
#include "ShaderManager.h"
#include "GfxAssetRegistry.h"
#include "TextureManager.h"
#include "MeshManager.h"
#include "MaterialManager.h"

// Entities
#include "EntityManager.h"

// Components
#include "ComponentManager.h"

// Systems
#include "RenderSystem.h" 
#include "Renderer.h"

#include "ShaderTypes.h"

// Test data loader
#include "Test.h"

class EngineCore : public QObject
{
    Q_OBJECT

    signals:
        void engineInitialised(EngineCore* engine);

    public:
        EngineCore() = default;
        void init(QOpenGLExtraFunctions* openGLFunctions);
        void runStep();
        void initTestModule(); // Pass resources to test module - required before GUI implmented.
        void updateResolution(float width, float height);

        // Data access for frontend QT models
        gfx::TextureManager* getTextureManager()
        {
            return m_textureManagerModule.get();
        }

    private:

        std::unique_ptr<gfx::Camera>            m_cameraModule;
        
        // Asset management
        std::unique_ptr<gfx::GfxAssetRegistry>  m_gfxAssetRegistryModule; // loads and stores paths for different assets
        std::unique_ptr<gfx::ShaderManager>     m_shaderManagerModule; // Manages compiling, storing and accessing shaders
        std::unique_ptr<gfx::TextureManager>    m_textureManagerModule; // Manages loading and storing textures
        std::unique_ptr<gfx::MeshManager>       m_meshManagerModule; // Manages loading and storing Meshes
        std::unique_ptr<gfx::MaterialManager>   m_materialManagerModule; // Manages storing different material configurations

        // Systems
        std::unique_ptr<gfx::Renderer>          m_renderModule; // Renders renderable components
        std::unique_ptr<gfx::RenderSystem>      m_renderSystemModule; // Syncs rendering, material, mesh and light 
        std::unique_ptr<gfx::LightingSystem>    m_lightingSystemModule;

        // Components and Entities
        std::unique_ptr<EntityManager>          m_entityManagerModule; // Manages entities. their components and lifetimes

        // Test - Will be replaced by GUI
        std::unique_ptr<Test>                   m_testModule;

        QOpenGLExtraFunctions*                  m_openGLFunctions;
};


#endif


