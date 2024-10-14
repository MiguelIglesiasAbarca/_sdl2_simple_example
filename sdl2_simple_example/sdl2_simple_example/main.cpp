#include <GL/glew.h>
#include <chrono>
#include <thread>
#include <exception>
#include <glm/glm.hpp>
#include <assimp/cimport.h>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
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
    
    glRotatef(1.5f, 1.0f, 1.0f, 0.0f);
    glBegin(GL_TRIANGLES);

    // Frontal
    glColor4ub(204, 255, 51, color.a);
    glVertex3f(-0.5f, -0.5f, -0.5f);
    glVertex3f(0.5f, -0.5f, -0.5f);
    glVertex3f(0.5f, 0.5f, -0.5f);

    glVertex3f(-0.5f, -0.5f, -0.5f);
    glVertex3f(0.5f, 0.5f, -0.5f);
    glVertex3f(-0.5f, 0.5f, -0.5f);

    // Trasero
    glColor4ub(0, 255, 51, color.a);
    glVertex3f(-0.5f, -0.5f, 0.5f);
    glVertex3f(0.5f, -0.5f, 0.5f);
    glVertex3f(0.5f, 0.5f, 0.5f);

    glVertex3f(-0.5f, -0.5f, 0.5f);
    glVertex3f(0.5f, 0.5f, 0.5f);
    glVertex3f(-0.5f, 0.5f, 0.5f);

    // Izquierda
    glColor4ub(0, 102, 255, color.a);
    glVertex3f(-0.5f, -0.5f, -0.5f);
    glVertex3f(-0.5f, 0.5f, -0.5f);
    glVertex3f(-0.5f, 0.5f, 0.5f);

    glVertex3f(-0.5f, -0.5f, -0.5f);
    glVertex3f(-0.5f, 0.5f, 0.5f);
    glVertex3f(-0.5f, -0.5f, 0.5f);

    // Derecha
    glColor4ub(255, 0, 102, color.a);
    glVertex3f(0.5f, -0.5f, -0.5f);
    glVertex3f(0.5f, 0.5f, -0.5f);
    glVertex3f(0.5f, 0.5f, 0.5f);

    glVertex3f(0.5f, -0.5f, -0.5f);
    glVertex3f(0.5f, 0.5f, 0.5f);
    glVertex3f(0.5f, -0.5f, 0.5f);

    // Arriba
    glColor4ub(255, 0, 102, color.a);
    glVertex3f(-0.5f, 0.5f, -0.5f);
    glVertex3f(0.5f, 0.5f, -0.5f);
    glVertex3f(0.5f, 0.5f, 0.5f);

    glVertex3f(-0.5f, 0.5f, -0.5f);
    glVertex3f(0.5f, 0.5f, 0.5f);
    glVertex3f(-0.5f, 0.5f, 0.5f);

    // Abajo
    glColor4ub(0, 51, 102, color.a);
    glVertex3f(-0.5f, -0.5f, -0.5f);
    glVertex3f(0.5f, -0.5f, -0.5f);
    glVertex3f(0.5f, -0.5f, 0.5f);

    glVertex3f(-0.5f, -0.5f, -0.5f);
    glVertex3f(0.5f, -0.5f, 0.5f);
    glVertex3f(-0.5f, -0.5f, 0.5f);


    glEnd();
}

void fbx() {
    const char* file = "cube.fbx"; // Ruta del fitxer a carregar
    const struct aiScene* scene = aiImportFile(file, aiProcess_Triangulate);
    if (!scene) {
        fprintf(stderr, "Error en carregar el fitxer: %s\n", aiGetErrorString());
    }
    printf("Numero de malles: %u\n", scene->mNumMeshes);
    for (unsigned int i = 0; i < scene->mNumMeshes; i++) {
        aiMesh* mesh = scene->mMeshes[i];
        printf("\nMalla %u:\n", i);
        printf(" Numero de vertexs: %u\n", mesh->mNumVertices);
        printf(" Numero de triangles: %u\n", mesh->mNumFaces);
        // Vèrtexs
        glBegin(GL_TRIANGLES);
        for (unsigned int v = 0; v < mesh->mNumVertices; v++) {
            aiVector3D vertex = mesh->mVertices[v];
            printf(" Vertex %u: (%f, %f, %f)\n", v, vertex.x, vertex.y, vertex.z);
            glVertex3f(vertex.x, vertex.y, vertex.z);
      

                    
        }
        glEnd();
        // Índexs de triangles (3 per triangle)
        for (unsigned int f = 0; f < mesh->mNumFaces; f++) {

            aiFace face = mesh->mFaces[f];
            printf(" Indexs triangle %u: ", f);

            for (unsigned int j = 0; j < face.mNumIndices; j++) {
                printf("%u ", face.mIndices[j]);

            }

            printf("\n");
        }
    }
    aiReleaseImport(scene);
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
	/*draw_cube(u8vec4(255, 0, 0, 235));*/
	ImGui::Render();
	ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}

int main(int argc, char** argv) {
	MyWindow window("CHIMICHANGA ENGINE", WINDOW_SIZE.x, WINDOW_SIZE.y);

	init_openGL();
    fbx();

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