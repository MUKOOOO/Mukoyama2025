#include  "../main.h"
#include "../renderer.h"
#include "../input.h"
#include "imgui_impl_dx11.h"
#include "imgui_impl_win32.h"
#include "imguiManager.h"
#include "../manager.h"
#include "../scene.h"
#include "../player.h"

char ImguiManager::m_Buffer[1024];
bool ImguiManager::flg;
float ImguiManager::vec4a[3] = {0.10f, 0.20f, 0.30f};
float ImguiManager::m_Rotation[3] = {0.0f, 0.0f, 0.0f};
float ImguiManager::m_Scale[3] = {1.0f, 1.0f, 1.0f};

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

    if (scene->GetSceneNumber() == 1)
    {
        ImGui_ImplDX11_NewFrame();
        ImGui::NewFrame();
        ImGui::Begin("Test");
    }
    

    if (scene->GetSceneNumber() == 2)
    {
        Player* player = scene->GetGameObject<Player>();

        static float f;

        ImGui_ImplDX11_NewFrame();
        ImGui::NewFrame();
        ImGui::Begin("Test");

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
}


void ImguiManager::End()
{
    ImGui::End();
    ImGui::Render();
    ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());
}