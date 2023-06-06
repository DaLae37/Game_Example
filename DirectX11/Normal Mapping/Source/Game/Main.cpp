#include "Common.h"

#include "Game/Game.h"
#include "Cube/RotatingCube.h"
#include "Cube/Cube.h"
#include "Light/RotatingPointLight.h"

INT WINAPI wWinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance, _In_ LPWSTR lpCmdLine, _In_ INT nCmdShow)
{

	std::unique_ptr<Game> game = std::make_unique<Game>(L"Normal Mapping");

    std::shared_ptr<VertexShader> vertexShader = std::make_shared<VertexShader>(L"Shaders/Shader.fx", "VS", "vs_5_0");
    if (FAILED(game->GetRenderer()->AddVertexShader(L"VS", vertexShader)))
        return 0;
    std::shared_ptr<PixelShader> pixelShader = std::make_shared<PixelShader>(L"Shaders/Shader.fx", "PS", "ps_5_0");
    if (FAILED(game->GetRenderer()->AddPixelShader(L"PS", pixelShader)))
        return 0;
    std::shared_ptr<PixelShader> pixelSolidShader = std::make_shared<PixelShader>(L"Shaders/Shader.fx", "PSSolid", "ps_5_0");
    if (FAILED(game->GetRenderer()->AddPixelShader(L"PSSolid", pixelSolidShader)))
        return 0;

    // Rotating Point Light
    XMFLOAT4 color;
    XMStoreFloat4(&color, Colors::White);
    std::shared_ptr<RotatingPointLight> rotatingPointLight = std::make_shared<RotatingPointLight>(
        XMFLOAT4(0.0f, 0.0f, -15.0f, 1.0f), color);
    if (FAILED(game->GetRenderer()->AddPointLight(0u, rotatingPointLight)))
        return 0;

    // Point Light
    XMStoreFloat4(&color, Colors::AntiqueWhite);
    std::shared_ptr<PointLight> pointLight = std::make_shared<PointLight>(
        XMFLOAT4(-5.77f, 5.77f, -5.77f, 1.0f), color);
    if (FAILED(game->GetRenderer()->AddPointLight(1u, pointLight)))
        return 0;

    std::shared_ptr<Cube> cube = std::make_shared<Cube>(color);
    if (FAILED(game->GetRenderer()->AddRenderable(L"Cube", cube))) {
        return 0;
    }
    if (FAILED(game->GetRenderer()->SetVertexShaderOfRenderable(L"Cube", L"VS"))) {
        return 0;
    }
    if (FAILED(game->GetRenderer()->SetPixelShaderOfRenderable(L"Cube", L"PS"))) {
        return 0;
    }

    std::shared_ptr<Material> cubeMaterial = std::make_shared<Material>(L"CubeMaterial");
    cubeMaterial->pDiffuse = std::make_shared<Texture>(L"../../Data/diffuse.png");
    cubeMaterial->pNormal = std::make_shared<Texture>(L"../../Data/normal.png");

    cube->AddMaterial(cubeMaterial);
    cube->RotateY(45);

    if (FAILED(game->Initialize(hInstance, nCmdShow)))
        return 0;

    return game->Run();
}