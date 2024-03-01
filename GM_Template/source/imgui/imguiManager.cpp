#include  "../main.h"
#include "../renderer.h"
#include "../input.h"
#include "imgui_impl_dx11.h"
#include "imgui_impl_win32.h"
#include "imguiManager.h"
#include "../manager.h"
#include "../scene.h"
#include "../player.h"
#include "../title.h"
#include "../camera.h"

char ImguiManager::m_Buffer[1024];
bool ImguiManager::flg;
bool ImguiManager::randomSeed = true;
float ImguiManager::vec4a[3] = {0.10f, 0.20f, 0.30f};
float ImguiManager::m_Rotation[3] = {0.0f, 0.0f, 0.0f};
float ImguiManager::m_Scale[3] = {1.0f, 1.0f, 1.0f};
float ImguiManager::m_Bump = 1.0f;
float ImguiManager::m_MaxHeight = 1.0f;

void ImguiManager::Init()
{
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO(); (void)io;
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls
    io.DisplaySize.x = SCREEN_WIDTH;
    io.DisplaySize.y = SCREEN_HEIGHT;

    ImGui_ImplWin32_Init(GetWindow());
	ImGui_ImplDX11_Init(Renderer::GetDevice(), Renderer::GetDeviceContext());
}

void ImguiManager::Uninit()
{
    ImGui_ImplWin32_Shutdown();
    ImGui_ImplDX11_Shutdown();
    ImGui::DestroyContext();
}

void ImguiManager::Begin()
{
    Scene* scene = Manager::GetScene();

    switch (scene->GetSceneName())
    {
    case SCENE_NAME::TITLE:
        TitleScene();
        break;
    case SCENE_NAME::STAGE:
        StageScene();
        break;
    }
}

void ImguiManager::End()
{
    ImGui::End();
    ImGui::Render();
    ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());
}


void ImguiManager::TitleScene()
{
    Scene* scene = Manager::GetScene();
    Title* title = scene->GetGameObject<Title>();

    ImGui_ImplDX11_NewFrame();
    ImGui::NewFrame();
    ImGui::Begin("Test");

    m_Bump = title->GetBump();
    ImGui::DragFloat("Bump", &m_Bump, 1.0f, 0.0f, 100.0f, "%3.1f");
    title->SetBump(m_Bump);

    m_MaxHeight = title->GetMaxHeight();
    ImGui::DragFloat("MaxHeight", &m_MaxHeight, 1.0f, 0.0f, 100.0f, "%3.1f");
    title->SetMaxHeight(m_MaxHeight);
}

void ImguiManager::StageScene()
{
    Scene* scene = Manager::GetScene();
    Player* player = scene->GetGameObject<Player>();
    Camera* camera = scene->GetGameObject<Camera>();

    //static float f;

    ImGui_ImplDX11_NewFrame();
    ImGui::NewFrame();
    ImGui::Begin("Test");

    //ImGui::Text("%.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);

    /*static float value[180];
    for (int i = 0; i < 179; i++)
    {
        value[i] = value[i + 1];
    }

    value[179] = 1ImGui::GetIO().DeltaTime * 1000.0f;

    ImGui::PlotLines("", value, sizeof(value) / sizeof(float), 0, NULL, 0.0f, 100.0f, ImVec2(0, 50));*/

    vec4a[0] = player->GetPosition().x;
    vec4a[1] = player->GetPosition().y;
    vec4a[2] = player->GetPosition().z;

    if (ImGui::CollapsingHeader("Transform", ImGuiTreeNodeFlags_DefaultOpen))
    {
        ImGui::Text("Position");
        ImGui::SameLine();
        ImGui::DragFloat3("##Position", vec4a, 1.0, 0.0, 0.0, "%3.1f");

        player->SetPosition(vec4a);

        ImGui::Text("Rotation");
        ImGui::SameLine();
        ImGui::DragFloat3("##Rotation", m_Rotation, 1.0, 0.0, 0.0, "%3.1f");

        ImGui::Text("Scale   ");
        ImGui::SameLine();
        ImGui::DragFloat3("##Scale", m_Scale, 1.0, 0.0, 0.0, "%3.1f");
    };

    ImGui::Checkbox("Hidden", &flg);

    if (flg)
    {
        ImGui::Text("TestText");
        player->SetEnable(false);
        ImGui::Begin("new");
        ImGui::End();
    }
    else
    {
        player->SetEnable(true);
    }
}