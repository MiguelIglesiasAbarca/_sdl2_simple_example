#include <GL/glew.h>
#include <chrono>
#include <thread>
#include <exception>
#include <glm/glm.hpp>
#include "MyWindow.h"
#include "imgui.h"
#include "imgui_impl_sdl2.h"
#include "imgui_impl_opengl3.h"
using namespace std;

using hrclock = chrono::high_resolution_clock;
using u8vec4 = glm::u8vec4;
using ivec2 = glm::ivec2;
using vec3 = glm::dvec3;

static const ivec2 WINDOW_SIZE(512, 512);
static const unsigned int FPS = 60;
static const auto FRAME_DT = 1.0s / FPS;

static void init_openGL() {
	glewInit();
	if (!GLEW_VERSION_3_0) throw exception("OpenGL 3.0 API is not available.");
	glEnable(GL_DEPTH_TEST);
	glClearColor(0.5, 0.5, 0.5, 1.0);
}

static void draw_triangle(const u8vec4& color, const vec3& center, double size) {
	glColor4ub(color.r, color.g, color.b, color.a);
	glBegin(GL_TRIANGLES);
	glVertex3d(center.x, center.y + size, center.z);
	glVertex3d(center.x - size, center.y - size, center.z);
	glVertex3d(center.x + size, center.y - size, center.z);
	glEnd();
}

static void draw_cube(const u8vec4& color) {
	glColor4ub(color.r, color.g, color.b, color.a);
	glLineWidth(2.0f);
	glBegin(GL_TRIANGLES);  // draw a cube with 12 triangles

	glRotatef(0.1f, 1.0f, 1.0f, 0.0f);

	float v0[3] = { -0.5f, -0.5f, -0.5f }; // vértice 0
	float v1[3] = { 0.5f, -0.5f, -0.5f }; // vértice 1
	float v2[3] = { 0.5f,  0.5f, -0.5f }; // vértice 2
	float v3[3] = { -0.5f,  0.5f, -0.5f }; // vértice 3
	float v4[3] = { -0.5f, -0.5f,  0.5f }; // vértice 4
	float v5[3] = { 0.5f, -0.5f,  0.5f }; // vértice 5
	float v6[3] = { 0.5f,  0.5f,  0.5f }; // vértice 6
	float v7[3] = { -0.5f,  0.5f,  0.5f }; // vértice 7

	// front face =================
	glVertex3fv(v0);    // v0-v1-v2
	glVertex3fv(v1);
	glVertex3fv(v2);

	glVertex3fv(v2);    // v2-v3-v0
	glVertex3fv(v3);
	glVertex3fv(v0);

	// right face =================
	glVertex3fv(v0);    // v0-v3-v4
	glVertex3fv(v3);
	glVertex3fv(v4);

	glVertex3fv(v4);    // v4-v5-v0
	glVertex3fv(v5);
	glVertex3fv(v0);

	// top face ===================
	glVertex3fv(v0);    // v0-v5-v6
	glVertex3fv(v5);
	glVertex3fv(v6);

	glVertex3fv(v6);    // v6-v1-v0
	glVertex3fv(v1);
	glVertex3fv(v0);




		glEnd();
}

static void display_func() {
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	ImGui_ImplOpenGL3_NewFrame();
	ImGui_ImplSDL2_NewFrame();
	ImGui::NewFrame();
	if (ImGui::BeginMainMenuBar()) {
		if (ImGui::BeginMenu("Menu")) {
			if (ImGui::MenuItem("adeu")) {
				// ... codi activació
			}
			ImGui::EndMenu();
		}
		ImGui::EndMainMenuBar();
	}
	draw_cube(u8vec4(255, 0, 0, 235));
	ImGui::Render();
	ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}

int main(int argc, char** argv) {
	MyWindow window("CHIMICHANGA ENGINE", WINDOW_SIZE.x, WINDOW_SIZE.y);

	init_openGL();

	while(window.processEvents() && window.isOpen()) {
		const auto t0 = hrclock::now();
		display_func();
		window.swapBuffers();
		const auto t1 = hrclock::now();
		const auto dt = t1 - t0;
		if(dt<FRAME_DT) this_thread::sleep_for(FRAME_DT - dt);
	}

	return 0;
}