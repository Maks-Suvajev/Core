#include "Test.h"

Test::Test(TestObjects testInitObjects)
{
    testObjects.entityManager   = nullptr;
    testObjects.materialManager = nullptr;
    testObjects.textureManager  = nullptr;
    testObjects.shaderManager   = nullptr;
    testObjects.meshManager     = nullptr;
    testObjects.lightingSystem     = nullptr;


    testObjects.entityManager   = testInitObjects.entityManager;
    testObjects.materialManager = testInitObjects.materialManager;
    testObjects.textureManager  = testInitObjects.textureManager;
    testObjects.shaderManager   = testInitObjects.shaderManager;
    testObjects.meshManager     = testInitObjects.meshManager;
    testObjects.lightingSystem  = testInitObjects.lightingSystem;


    printTestInput();
}

void Test::createEntity()
{
    Entity newEntity = testObjects.entityManager->generateNewEntity();

    initMeshComponent(newEntity);
    initMaterialComponent(newEntity);
    initTransformComponent(newEntity);

    // New entity for each light component
    Entity lightEntity1 = testObjects.entityManager->generateNewEntity();
    testObjects.lightingSystem->addDirectionalLight(testObjects.entityManager, lightEntity1, initDirectionalLight());

    Entity lightEntity2 = testObjects.entityManager->generateNewEntity();
    testObjects.lightingSystem->addSpotLight(testObjects.entityManager, lightEntity2, initSpotLight());

    Entity lightEntity3 = testObjects.entityManager->generateNewEntity();
    testObjects.lightingSystem->addPointLight(testObjects.entityManager, lightEntity3, initPointLight());

    testObjects.entityManager->printActiveEntityIDs();
    testObjects.entityManager->printActiveEntityComponents(lightEntity1);
    testObjects.entityManager->printActiveEntityComponents(lightEntity2);
    testObjects.entityManager->printActiveEntityComponents(lightEntity3);


}

void Test::initTestData()
{
    printCurrentAssets();

    // Populate mesh manager and material manager
    initMeshData();
    initMaterialData();

    createEntity();

    printCurrentAssets();

}

void Test::initMeshData()
{
    gfx::MeshData newMesh;
    newMesh.vertices = std::vector<float>(std::begin(gfx::cubeVertices), std::end(gfx::cubeVertices));
    newMesh.numVertices = 36U;

    testObjects.meshManager->addMesh("cube", std::move(newMesh));

}

void Test::initLight()
{


}

gfx::SpotLight Test::initSpotLight()
{
    glm::vec3 specular{1.0f};
    glm::vec3 diffuse = specular * glm::vec3(0.5f);
    glm::vec3 ambient = diffuse * glm::vec3(0.2f);

    gfx::SpotLight spotLight;
    spotLight.lightColours.ambient = ambient;
    spotLight.lightColours.diffuse = diffuse;
    spotLight.lightColours.specular = specular;
    
    return spotLight;
}

gfx::PointLight Test::initPointLight()
{
    // Light colours
    glm::vec3 specular{1.0f};
    glm::vec3 diffuse = specular * glm::vec3(0.5f);
    glm::vec3 ambient = diffuse * glm::vec3(0.2f);

    // pos
    glm::vec3 position = glm::vec3(1.2f, 1.0, 2.0f);

    // fall off function
    float constant = 1.0f;
    float linear = 0.09f;
    float quadratic = 0.032f;

    gfx::PointLight pointLight;
    pointLight.lightColours.ambient = ambient;
    pointLight.lightColours.diffuse = diffuse;
    pointLight.lightColours.specular = specular;
    pointLight.position = position;
    pointLight.constants.constant = constant;
    pointLight.constants.linear = linear;
    pointLight.constants.quadratic = quadratic;

    return pointLight;
}

gfx::DirectionalLight Test::initDirectionalLight()
{
    glm::vec3 specular{1.0f};
    glm::vec3 diffuse = specular * glm::vec3(0.5f);
    glm::vec3 ambient = diffuse * glm::vec3(0.2f);
    glm::vec3 direction = glm::vec3(-0.2f, -1.0f, -0.3f);

    gfx::DirectionalLight directionalLight;
    directionalLight.direction = direction;
    directionalLight.lightColours.specular = specular;
    directionalLight.lightColours.diffuse = diffuse;
    directionalLight.lightColours.ambient = ambient;

    return directionalLight;
}

gfx::MaterialProperties Test::initLightCubeMaterial()
{
    gfx::MaterialProperties initProperties;

    // shader
    initProperties.shader = testObjects.shaderManager->getShaderID("lightSource");

    initProperties.shininess = 0.0f;
    //initProperties.isLightSource = true;

    return initProperties;
}

gfx::MaterialProperties Test::initGenericCubeMaterial()
{
    gfx::MaterialProperties initProperties;

    // shader
    initProperties.shader = testObjects.shaderManager->getShaderID("normalObject");

    // lighting texture
    initProperties.lightingTextures.diffuse = testObjects.textureManager->getTextureID("container2.png");
    initProperties.lightingTextures.specular = testObjects.textureManager->getTextureID("container2_specular.png");

    initProperties.shininess = 1024.0f;
    //initProperties.isLightSource = false;

    return initProperties;
}


void Test::initMaterialData()
{
    testObjects.materialManager->addMaterial("generic cube", std::move(initGenericCubeMaterial()));
    testObjects.materialManager->addMaterial("light cube", std::move(initLightCubeMaterial()));

}

void Test::initMeshComponent(Entity entity)
{
    auto meshPtr = testObjects.meshManager->getMesh("cube");   
    
    if (meshPtr == nullptr)
    {
        return;    
    }
 
    testObjects.entityManager->addComponentData<gfx::GpuHandles>(entity, meshPtr->getGPUHandles());
}

void Test::initMaterialComponent(Entity entity)
{
    auto materialPtr = testObjects.materialManager->getMaterial("generic cube");

    if (materialPtr == nullptr)
    {
        return;
    }

    testObjects.entityManager->addComponentData<gfx::MaterialProperties>(entity, materialPtr->getMaterialProperties());
}

gfx::Transform Test::initCubeTransform()
{
    gfx::Transform newTransform;

    newTransform.position = glm::vec3{0.0f, 0.0f, 0.0f};
    newTransform.rotation = glm::vec3{0.0f, 0.0f, 0.0f};
    newTransform.scaleFactors = glm::vec3{1.0f, 1.0f, 1.0f};

    return newTransform;
}

void Test::initTransformComponent(Entity entity)
{
    testObjects.entityManager->addComponentData<gfx::Transform>(entity, initCubeTransform());
}

void Test::printCurrentAssets()
{
    if (testObjects.textureManager != nullptr)
    {
        testObjects.textureManager->printAllTextures();
    }
    else
    {
        std::cout << "Can't print textures, pointer is null." << std::endl;
    }

    if (testObjects.shaderManager != nullptr)
    {
        testObjects.shaderManager->printAllShaderPrograms();
    }
    else
    {
        std::cout << "Can't print shaders, pointer is null." << std::endl;
    }
}

void Test::printTestInput()
{
    if (testObjects.entityManager == nullptr)
    {
        std::cout << "ERROR::Entity manager is nullptr." << std::endl;
    }

    if (testObjects.materialManager == nullptr)
    {
        std::cout << "ERROR::Material manager is nullptr." << std::endl;
    }

    if (testObjects.textureManager == nullptr)
    {
        std::cout << "ERROR::Texture manager is nullptr." << std::endl;
    }

    if (testObjects.shaderManager == nullptr)
    {
        std::cout << "ERROR::Shader manager is nullptr." << std::endl;
    }

    if (testObjects.meshManager == nullptr)
    {
        std::cout << "ERROR::Mesh manager is nullptr." << std::endl;
    }

    if (testObjects.lightingSystem == nullptr)
    {
        std::cout << "ERROR::Mesh manager is nullptr." << std::endl;
    }

    std::cout << "If no errors above, all pointers are valid." << std::endl;
}
