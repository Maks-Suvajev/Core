#ifndef TEST_H
#define TEST_H

#include "MeshManager.h"
#include "MaterialManager.h"
#include "TextureManager.h"
#include "ShaderManager.h"
#include "EntityManager.h"
#include "LightingSystem.h"

// Vertices and test positions for cube
#include "Cube.h"

// Components
#include "MeshTypes.h"
#include "PhysicsTypes.h"
#include "LightTypes.h"

struct TestObjects
{
    gfx::MeshManager*       meshManager;
    gfx::MaterialManager*   materialManager;
    gfx::TextureManager*    textureManager;
    gfx::ShaderManager*     shaderManager;
    EntityManager*          entityManager;
    gfx::LightingSystem*    lightingSystem;
};

class Test
{
    public:
        Test(TestObjects testObjects);
        void initTestData();


    private:
        // System modules
        TestObjects testObjects;

        // init data
        gfx::MaterialProperties initGenericCubeMaterial();
        gfx::MaterialProperties initLightCubeMaterial();
        gfx::Transform          initCubeTransform();

        void                    initLight();
        gfx::SpotLight          initSpotLight();
        gfx::PointLight         initPointLight();
        gfx::DirectionalLight   initDirectionalLight();

        // Manager data init
        void initMeshData();
        void initMaterialData();

        // Entity init
        void createEntity();

        // Component init
        void initMeshComponent(Entity entity);
        void initMaterialComponent(Entity entity);
        void initTransformComponent(Entity entity);

        // printing
        void printCurrentAssets();
        void printTestInput();
};


#endif