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
#include "AssetRegistry.h"
#include "TextureManager.h"
#include "SceneModelManager.h"
#include "MaterialManager.h"

// Entities
#include "EntityManager.h"

// Components
#include "ComponentManager.h"

// Systems
#include "RenderSystem.h" 
#include "Renderer.h"

#include "ShaderTypes.h"


class EngineCore : public QObject
{
    Q_OBJECT

    signals:
        void engineInitialised(EngineCore* engine);

    public:
        EngineCore() = default;
        void init(QOpenGLExtraFunctions* openGLFunctions);
        void runStep();
        void updateResolution(float width, float height);

        // Data access for frontend QT models
        gfx::TextureManager* getTextureManager()
        {
            return m_textureManagerModule.get();
        }

        gfx::ShaderManager* getShaderManager()
        {
            return m_shaderManagerModule.get();
        }

        gfx::SceneModelManager* getSceneModelManager()
        {
            return m_sceneModelManagerModule.get();
        }


        EntityManager* getEntityManager()
        {
            return m_entityManagerModule.get();
        }
        
    private:

        std::unique_ptr<gfx::Camera>            m_cameraModule;
        
        // Asset management
        std::unique_ptr<AssetRegistry>          m_AssetRegistryModule; // loads paths for different asset types
        std::unique_ptr<gfx::ShaderManager>     m_shaderManagerModule; // Manages compiling, storing and accessing shaders
        std::unique_ptr<gfx::TextureManager>    m_textureManagerModule; // Manages loading and storing textures
        std::unique_ptr<gfx::SceneModelManager> m_sceneModelManagerModule; // Manages loading and storing models
        std::unique_ptr<gfx::MaterialManager>   m_materialManagerModule; // Manages storing different material configurations

        // Systems
        std::unique_ptr<gfx::Renderer>          m_renderModule; // Renders renderable components
        std::unique_ptr<gfx::RenderSystem>      m_renderSystemModule; // Syncs rendering, material, mesh and light 
        std::unique_ptr<gfx::LightingSystem>    m_lightingSystemModule;

        // Components and Entities
        std::unique_ptr<EntityManager>          m_entityManagerModule; // Manages entities. their components and lifetimes

        QOpenGLExtraFunctions*                  m_openGLFunctions;
        
};


#endif


