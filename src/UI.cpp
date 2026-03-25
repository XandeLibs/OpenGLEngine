#include "UI.hpp"

#include "imgui/backends/imgui_impl_glfw.h"
#include "imgui/backends/imgui_impl_opengl3.h"
#include "imgui/imgui.h"

#include "scene.hpp"

namespace UI {

void showShaderWindow();

void setup(GLFWwindow *window) {

  IMGUI_CHECKVERSION();
  ImGui::CreateContext();
  ImGuiIO &io = ImGui::GetIO();

  // io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
  io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;

  ImGui_ImplGlfw_InitForOpenGL(
      window, true); // Second param install_callback=true will install
                     // GLFW callbacks and chain to existing ones.
  ImGui_ImplOpenGL3_Init();
}

void startLoop() {
  ImGui_ImplOpenGL3_NewFrame();
  ImGui_ImplGlfw_NewFrame();
  ImGui::NewFrame();
  showShaderWindow();
  float main_scale =
      ImGui_ImplGlfw_GetContentScaleForMonitor(glfwGetPrimaryMonitor());
  ImGuiStyle &style = ImGui::GetStyle();
  style.ScaleAllSizes(main_scale);
}

void endLoop() {
  ImGui::Render();
  ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}

void shutdown() {
  ImGui_ImplOpenGL3_Shutdown();
  ImGui_ImplGlfw_Shutdown();
  ImGui::DestroyContext();
}

void showShaderWindow() {
  ImGui::Begin("Shaders");

  const char *types[] = {"normal", "depth", "border"};

  assert(IM_COUNTOF(types) == Scene::RENDER_COUNT);

  if (ImGui::BeginCombo("Render Type", types[scene.renderType])) {
    for (int n = 0; n < scene.RENDER_COUNT; n++) {
      const bool is_selected = (scene.renderType == n);
      if (ImGui::Selectable(types[n], is_selected))
        scene.renderType = (Scene::RenderType)n;

      // Set the initial focus when opening the combo (scrolling + keyboard
      // navigation focus)
      if (is_selected)
        ImGui::SetItemDefaultFocus();
    }
    ImGui::EndCombo();
  }

  ImGui::End();
}
} // namespace UI
