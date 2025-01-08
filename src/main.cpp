//     Universidade Federal do Rio Grande do Sul
//             Instituto de Informática
//       Departamento de Informática Aplicada
//
//    INF01047 Fundamentos de Computação Gráfica
//               Prof. Eduardo Gastal
//
//                   LABORATÓRIO 4
//

// Arquivos "headers" padrões de C podem ser incluídos em um
// programa C++, sendo necessário somente adicionar o caractere
// "c" antes de seu nome, e remover o sufixo ".h". Exemplo:
//    #include <stdio.h> // Em C
//  vira
//    #include <cstdio> // Em C++
//
#include <cstdio>
#include <cstdlib>

// Headers abaixo são específicos de C++
#include <map>
#include <stack>
#include <string>
#include <vector>
#include <limits>
#include <fstream>
#include <sstream>
#include <stdexcept>
#include <algorithm>
#include <iostream>

// Headers das bibliotecas OpenGL
#include <glad/glad.h>   // Criação de contexto OpenGL 3.3
#include <GLFW/glfw3.h>  // Criação de janelas do sistema operacional

// Headers da biblioteca GLM: criação de matrizes e vetores.
#include <glm/mat4x4.hpp>
#include <glm/vec4.hpp>
#include <glm/gtc/type_ptr.hpp>

// Headers da biblioteca para carregar modelos obj
#include <tiny_obj_loader.h>

// Headers locais, definidos na pasta "include/"
#include "utils.h"
#include "matrices.h"
#include "object.h"
#include "load_shader.h"
#include "gpuFunctions.h"
#include "free_camera.h"
#include "scene.h"
#include "gameObject.h"
#include "staticGameObject.h"
#include "texture.h"
#include "input_handler.h"
#include "player.h"
#include "collisions.h"
#include "game_logic.h"
#include "door.h"
#define M_PI 3.14159265358979323846

// Declaração de funções utilizadas para pilha de matrizes de modelagem.
void PushMatrix(glm::mat4 M);
void PopMatrix(glm::mat4& M);

// Declaração de várias funções utilizadas em main().  Essas estão definidas
// logo após a definição de main() neste arquivo.
void DrawVirtualObject(const char* object_name); // Desenha um objeto armazenado em g_VirtualScene
GLuint LoadShader_Vertex(const char* filename);   // Carrega um vertex shader
GLuint LoadShader_Fragment(const char* filename); // Carrega um fragment shader
void LoadShader(const char* filename, GLuint shader_id); // Função utilizada pelas duas acima
GLuint CreateGpuProgram(GLuint vertex_shader_id, GLuint fragment_shader_id); // Cria um programa de GPU
//void PrintObjModelInfo(ObjModel*); // Função para debugging

// Declaração de funções auxiliares para renderizar texto dentro da janela
// OpenGL. Estas funções estão definidas no arquivo "textrendering.cpp".
void TextRendering_Init();
float TextRendering_LineHeight(GLFWwindow* window);
float TextRendering_CharWidth(GLFWwindow* window);
void TextRendering_PrintString(GLFWwindow* window, const std::string &str, float x, float y, float scale = 1.0f);
void TextRendering_PrintMatrix(GLFWwindow* window, glm::mat4 M, float x, float y, float scale = 1.0f);
void TextRendering_PrintVector(GLFWwindow* window, glm::vec4 v, float x, float y, float scale = 1.0f);
void TextRendering_PrintMatrixVectorProduct(GLFWwindow* window, glm::mat4 M, glm::vec4 v, float x, float y, float scale = 1.0f);
void TextRendering_PrintMatrixVectorProductMoreDigits(GLFWwindow* window, glm::mat4 M, glm::vec4 v, float x, float y, float scale = 1.0f);
void TextRendering_PrintMatrixVectorProductDivW(GLFWwindow* window, glm::mat4 M, glm::vec4 v, float x, float y, float scale = 1.0f);

// Funções abaixo renderizam como texto na janela OpenGL algumas matrizes e
// outras informações do programa. Definidas após main().
void TextRendering_ShowModelViewProjection(GLFWwindow* window, glm::mat4 projection, glm::mat4 view, glm::mat4 model, glm::vec4 p_model);
void TextRendering_ShowEulerAngles(GLFWwindow* window);
void TextRendering_ShowProjection(GLFWwindow* window);
void TextRendering_ShowFramesPerSecond(GLFWwindow* window);

// Funções callback para comunicação com o sistema operacional e interação do
// usuário. Veja mais comentários nas definições das mesmas, abaixo.
void FramebufferSizeCallback(GLFWwindow* window, int width, int height);
void ErrorCallback(int error, const char* description);
//void KeyCallback(GLFWwindow* window, int key, int scancode, int action, int mode);
//void MouseButtonCallback(GLFWwindow* window, int button, int action, int mods);


// Definimos uma estrutura que armazenará dados necessários para renderizar
// cada objeto da cena virtual.
struct SceneObject
{
    std::string  name;        // Nome do objeto
    size_t       first_index; // Índice do primeiro vértice dentro do vetor indices[] definido em BuildTrianglesAndAddToVirtualScene()
    size_t       num_indices; // Número de índices do objeto dentro do vetor indices[] definido em BuildTrianglesAndAddToVirtualScene()
    GLenum       rendering_mode; // Modo de rasterização (GL_TRIANGLES, GL_TRIANGLE_STRIP, etc.)
    GLuint       vertex_array_object_id; // ID do VAO onde estão armazenados os atributos do modelo
};

// Abaixo definimos variáveis globais utilizadas em várias funções do código.

// A cena virtual é uma lista de objetos nomeados, guardados em um dicionário
// (map).  Veja dentro da função BuildTrianglesAndAddToVirtualScene() como que são incluídos
// objetos dentro da variável g_VirtualScene, e veja na função main() como
// estes são acessados.
std::map<std::string, SceneObject> g_VirtualScene;

// Pilha que guardará as matrizes de modelagem.
std::stack<glm::mat4>  g_MatrixStack;

// Razão de proporção da janela (largura/altura). Veja função FramebufferSizeCallback().
float g_ScreenRatio = 1.0f;

// Ângulos de Euler que controlam a rotação de um dos cubos da cena virtual
float g_AngleX = 0.0f;
float g_AngleY = 0.0f;
float g_AngleZ = 0.0f;

// "g_LeftMouseButtonPressed = true" se o usuário está com o botão esquerdo do mouse
// pressionado no momento atual. Veja função MouseButtonCallback().
bool g_LeftMouseButtonPressed = false;
bool g_RightMouseButtonPressed = false; // Análogo para botão direito do mouse
bool g_MiddleMouseButtonPressed = false; // Análogo para botão do meio do mouse




// Variável que controla o tipo de projeção utilizada: perspectiva ou ortográfica.
bool g_UsePerspectiveProjection = true;

// Variável que controla se o texto informativo será mostrado na tela.
bool g_ShowInfoText = true;

// Variáveis que definem um programa de GPU (shaders). Veja função LoadShadersFromFiles().
//GLuint g_GpuProgramID = 0;
//GLint g_model_uniform;
//GLint g_view_uniform;
//GLint g_projection_uniform;
//GLint g_object_id_uniform;

int main(int argc, char* argv[])
{
    // Inicializamos a biblioteca GLFW, utilizada para criar uma janela do
    // sistema operacional, onde poderemos renderizar com OpenGL.
    int success = glfwInit();
    if (!success)
    {
        fprintf(stderr, "ERROR: glfwInit() failed.\n");
        std::exit(EXIT_FAILURE);
    }

    // Definimos o callback para impressão de erros da GLFW no terminal
    glfwSetErrorCallback(ErrorCallback);

    // Pedimos para utilizar OpenGL versão 3.3 (ou superior)
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);

    #ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
    #endif

    // Pedimos para utilizar o perfil "core", isto é, utilizaremos somente as
    // funções modernas de OpenGL.
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    // Criamos uma janela do sistema operacional, com 800 colunas e 600 linhas
    // de pixels, e com título "INF01047 ...".
    GLFWwindow* window;
    window = glfwCreateWindow(800, 600, "INF01047 - Grêmio", NULL, NULL);
    if (!window)
    {
        glfwTerminate();
        fprintf(stderr, "ERROR: glfwCreateWindow() failed.\n");
        std::exit(EXIT_FAILURE);
    }
    GameLogic game_logic;
    InputHandler input_handler;
    Collisions collisions;
    Player player;

    // Definimos a função de callback que será chamada sempre que o usuário
    // pressionar alguma tecla do teclado ...
    // Set the user pointer to associate the input_handler with the window
    glfwSetWindowUserPointer(window, &input_handler);

    // Set the key callback
    glfwSetKeyCallback(window, InputHandler::keyCallback);
    // ... ou clicar os botões do mouse ...
    //glfwSetMouseButtonCallback(window, MouseButtonCallback);
    // ... ou movimentar o cursor do mouse em cima da janela ...
    glfwSetCursorPosCallback(window, InputHandler::cursorPosCallback);
    // ... ou rolar a "rodinha" do mouse.
    //glfwSetScrollCallback(window, ScrollCallback);




    // Make the window's context current
    glfwMakeContextCurrent(window);
    glfwSwapInterval(1);

    // Hide and capture the cursor
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED); // Disable cursor
    //glfwSetCursorPos(window, 400, 300);  // Set cursor to the center (optional)

    // Indicamos que as chamadas OpenGL deverão renderizar nesta janela
    glfwMakeContextCurrent(window);

    // Carregamento de todas funções definidas por OpenGL 3.3, utilizando a
    // biblioteca GLAD.
    gladLoadGLLoader((GLADloadproc) glfwGetProcAddress);

    // Definimos a função de callback que será chamada sempre que a janela for
    // redimensionada, por consequência alterando o tamanho do "framebuffer"
    // (região de memória onde são armazenados os pixels da imagem).
    glfwSetFramebufferSizeCallback(window, FramebufferSizeCallback);
    FramebufferSizeCallback(window, 800, 600); // Forçamos a chamada do callback acima, para definir g_ScreenRatio.

    // Imprimimos no terminal informações sobre a GPU do sistema
    const GLubyte *vendor      = glGetString(GL_VENDOR);
    const GLubyte *renderer    = glGetString(GL_RENDERER);
    const GLubyte *glversion   = glGetString(GL_VERSION);
    const GLubyte *glslversion = glGetString(GL_SHADING_LANGUAGE_VERSION);

    printf("GPU: %s, %s, OpenGL %s, GLSL %s\n", vendor, renderer, glversion, glslversion);

    GLuint g_GpuProgramID = LoadShadersFromFiles();
    Scene scene;

    // Carregamos os shaders de vértices e de fragmentos que serão utilizados
    // para renderização. Veja slides 180-200 do documento Aula_03_Rendering_Pipeline_Grafico.pdf.
    //

    // Construímos a representação de objetos geométricos através de malhas de triângulos


    //LoadShadersFromFiles();
    GpuFunctions gpu_functions(g_GpuProgramID);
    Texture texture;

    texture.LoadTextureImage("../../data/wood.jpg", "wood");
    texture.LoadTextureImage("../../data/green_floor.jpg", "green_floor");
    texture.LoadTextureImage("../../data/bege_wall.jpg", "bege_wall");
    texture.LoadTextureImage("../../data/white_board.jpg", "white_board");
    texture.LoadTextureImage("../../data/bezier_board.jpg", "bezier_board");
    texture.LoadTextureImage("../../data/10057_wooden_door_v1_diffuse.jpg", "door");
    texture.LoadTextureImage("../../data/wood_desk.jpg", "wood_desk");
    texture.LoadTextureImage("../../data/paper.jpg", "paper");
    TextRendering_Init();

    Object sphere("../../data/sphere.obj");
    Object bunny("../../data/bunny.obj");
    Object plane("../../data/plane.obj");
    Object chair("../../data/uploads_files_2101984_Skin_chair.obj");
    Object cube("../../data/cube.obj");
    Object window_object("../../data/uploads_files_3158526_Small_Square_Window-Final.obj");
    Object white_board("../../data/uploads_files_3685891_whiteBoard.obj");
    Object door_object("../../data/10057_wooden_door_v3_iterations-2.obj");
    Object cow_object("../../data/cow.obj");
    Object desk("../../data/MetalDesk.obj");
    Object lamp("../../data/wooden_lamp.obj", "lamp");


//------------------------------------SALA 1------------------------------------------------------------------

    StaticGameObject white_board_object(
        &gpu_functions, &white_board, TextureType::OBJ_FILE, texture.GetTexture("white_board"), 
        LightType::NO, "WHITE_BOARD_OBJECT");
    white_board_object.transform.SetPosition(0.0f, -1.0f, 0.0f);
    white_board_object.transform.SetScale(10.0f, 10.0f, 10.0f);
    white_board_object.UpdateModel();

    StaticGameObject chair1(
        &gpu_functions, &chair, TextureType::OBJ_FILE ,texture.GetTexture("wood"), 
        LightType::PHONG, "CHAIR1");
    chair1.transform.SetPosition(2.0f, -10.0f, -10.0f);
    chair1.transform.SetScale(0.1f, 0.1f, 0.1f);
    chair1.UpdateModel();

    StaticGameObject chair2(
        &gpu_functions, &chair, TextureType::OBJ_FILE ,texture.GetTexture("wood"), 
        LightType::GOURAUD, "CHAIR2");
    chair2.transform.SetPosition(12.0f, -10.0f, -10.0f);
    chair2.transform.SetScale(0.1f, 0.1f, 0.1f);
    chair2.UpdateModel();
   
    StaticGameObject chair3(
        &gpu_functions, &chair, TextureType::OBJ_FILE ,texture.GetTexture("wood"), 
        LightType::PHONG, "CHAIR3");
    chair3.transform.SetPosition(22.0f, -10.0f, -10.0f);
    chair3.transform.SetScale(0.1f, 0.1f, 0.1f);
    chair3.UpdateModel();

    StaticGameObject chair4(
        &gpu_functions, &chair, TextureType::OBJ_FILE ,texture.GetTexture("wood"), 
        LightType::GOURAUD, "CHAIR4");
    chair4.transform.SetPosition(-8.0f, -10.0f, -10.0f);
    chair4.transform.SetScale(0.1f, 0.1f, 0.1f);
    chair4.UpdateModel();

    StaticGameObject chair5(
        &gpu_functions, &chair, TextureType::OBJ_FILE ,texture.GetTexture("wood"), 
        LightType::PHONG, "CHAIR5");
    chair5.transform.SetPosition(-18.0f, -10.0f, -10.0f);
    chair5.transform.SetScale(0.1f, 0.1f, 0.1f);
    chair5.UpdateModel();

    StaticGameObject chair6(
        &gpu_functions, &chair, TextureType::OBJ_FILE ,texture.GetTexture("wood"), 
        LightType::GOURAUD, "CHAIR6");
    chair6.transform.SetPosition(-28.0f, -10.0f, -10.0f);
    chair6.transform.SetScale(0.1f, 0.1f, 0.1f);
    chair6.UpdateModel();

     StaticGameObject chair7(
        &gpu_functions, &chair, TextureType::OBJ_FILE ,texture.GetTexture("wood"), 
        LightType::PHONG, "CHAIR7");
    chair7.transform.SetPosition(2.0f, -10.0f, -20.0f);
    chair7.transform.SetScale(0.1f, 0.1f, 0.1f);
    chair7.UpdateModel();

    StaticGameObject chair8(
        &gpu_functions, &chair, TextureType::OBJ_FILE ,texture.GetTexture("wood"), 
        LightType::GOURAUD, "CHAIR8");
    chair8.transform.SetPosition(12.0f, -10.0f, -20.0f);
    chair8.transform.SetScale(0.1f, 0.1f, 0.1f);
    chair8.UpdateModel();
   
    StaticGameObject chair9(
        &gpu_functions, &chair, TextureType::OBJ_FILE ,texture.GetTexture("wood"), 
        LightType::PHONG, "CHAIR9");
    chair9.transform.SetPosition(22.0f, -10.0f, -20.0f);
    chair9.transform.SetScale(0.1f, 0.1f, 0.1f);
    chair9.UpdateModel();

    StaticGameObject chair10(
        &gpu_functions, &chair, TextureType::OBJ_FILE ,texture.GetTexture("wood"), 
        LightType::GOURAUD, "CHAIR10");
    chair10.transform.SetPosition(-8.0f, -10.0f, -20.0f);
    chair10.transform.SetScale(0.1f, 0.1f, 0.1f);
    chair10.UpdateModel();

    StaticGameObject chair11(
        &gpu_functions, &chair, TextureType::OBJ_FILE ,texture.GetTexture("wood"), 
        LightType::PHONG, "CHAIR11");
    chair11.transform.SetPosition(-18.0f, -10.0f, -20.0f);
    chair11.transform.SetScale(0.1f, 0.1f, 0.1f);
    chair11.UpdateModel();

    StaticGameObject chair12(
        &gpu_functions, &chair, TextureType::OBJ_FILE ,texture.GetTexture("wood"), 
        LightType::GOURAUD, "CHAIR12");
    chair12.transform.SetPosition(-28.0f, -10.0f, -20.0f);
    chair12.transform.SetScale(0.1f, 0.1f, 0.1f);
    chair12.UpdateModel();


    // Cria objetos de jogo e os adiciona à cena
    StaticGameObject floor_object(
        &gpu_functions, &plane, TextureType::PLANE, texture.GetTexture("green_floor"), 
        LightType::NO, "FLOOR_OBJECT"); 
    floor_object.transform.SetPosition(0.0f, -10.0f, 0.0f);
    floor_object.transform.SetScale(40.0, 1.0, 40.0); 
    floor_object.UpdateModel();

    // Parede de trás
    StaticGameObject back_wall(
        &gpu_functions, &cube, TextureType::OBJ_FILE ,texture.GetTexture("bege_wall"), 
        LightType::NO, "BACK_WALL"); 
    back_wall.transform.SetPosition(0.0f, 0.0f, -40.0f); 
    back_wall.transform.SetScale(40.0f, 20.0f, 1.0f); 
    back_wall.UpdateModel();
    back_wall.setHitbox(glm::vec4(-40.0f, 0.0f, -42.0f, 1.0f), glm::vec4(40.0f, 20.0f, -38.0f, 1.0f));
    collisions.addHitbox(back_wall);

    // Parede da direita
    StaticGameObject right_wall(
        &gpu_functions, &cube, TextureType::OBJ_FILE, texture.GetTexture("bege_wall"), 
        LightType::NO, "RIGHT_WALL");
    right_wall.transform.SetPosition(-40.0f, 0.0f, 0.0f); 
    right_wall.transform.SetScale(1.0f, 20.0f, 40.0f);
    right_wall.UpdateModel();
    right_wall.setHitbox(glm::vec4(-42.0f, 0.0f, -40.0f, 1.0f), glm::vec4(-38.0f, 20.0f, 40.0f, 1.0f));
    collisions.addHitbox(right_wall);

    // Parede da esquerda
    StaticGameObject left_wall1(
        &gpu_functions, &cube,  TextureType::OBJ_FILE,  texture.GetTexture("bege_wall"), 
        LightType::NO, "LEFT_WALL1");
    left_wall1.transform.SetPosition(40.0f, 0.0f, -30.0f); 
    left_wall1.transform.SetScale(1.0f, 20.0f, 40.0f);
    left_wall1.UpdateModel();
    left_wall1.setHitbox(glm::vec4(38.0f, 0.0f, -40.0f, 1.0f), glm::vec4(40.0f, 20.0f, 10.0f, 1.0f));
    collisions.addHitbox(left_wall1);

    
    // Parede da esquerda
    StaticGameObject left_wall2(
        &gpu_functions, &cube,  TextureType::OBJ_FILE,  texture.GetTexture("bege_wall"), 
        LightType::NO, "LEFT_WALL2");
    left_wall2.transform.SetPosition(40.0f, 0.0f, 56.0f); 
    left_wall2.transform.SetScale(1.0f, 20.0f, 40.0f);
    left_wall2.UpdateModel();
    left_wall2.setHitbox(glm::vec4(37.0f, 0.0f, 16.0f, 1.0f), glm::vec4(44.0f, 0.0f, 44.0f, 1.0f));
    collisions.addHitbox(left_wall2);

    
    StaticGameObject left_wall3(
        &gpu_functions, &cube,  TextureType::OBJ_FILE,  texture.GetTexture("bege_wall"), 
        LightType::NO, "LEFT_WALL3");
    left_wall3.transform.SetPosition(40.0f, 16.0f, 10.0f); 
    left_wall3.transform.SetScale(1.0f, 10.0f, 10.0f);
    left_wall3.UpdateModel();


    // Parede da frente
    StaticGameObject front_wall(
        &gpu_functions, &cube,  TextureType::OBJ_FILE, texture.GetTexture("bege_wall"), 
        LightType::NO, "FRONT_WALL");
    front_wall.transform.SetPosition(0.0f, 0.0f, 40.0f);
    front_wall.transform.SetScale(40.0f, 20.0f, 1.0f);
    front_wall.UpdateModel();
    front_wall.setHitbox(glm::vec4(-40.0f, -10.0f, 38.0f, 1.0f), glm::vec4(40.0f, 10.0f, 42.0f, 1.0f));
    collisions.addHitbox(front_wall);

    StaticGameObject board(&gpu_functions, &plane, TextureType::PLANE, texture.GetTexture("bezier_board"), LightType::NO);
    board.transform.SetPosition(0.0f, 5.0f, 38.5f);
    board.transform.SetRotation(-3.14/2, 0.0f, 0.0f);
    board.transform.SetScale(15.0f, 10.0f, 10.0f);
    board.UpdateModel();

    /*StaticGameObject board(&gpu_functions, &cube, TextureType::OBJ_FILE ,texture.GetTexture("bege_wall"), LightType::NO); 
    board.transform.SetPosition(0.0f, 0.0f, -40.0f); 
    board.transform.SetScale(10.0f, 7.0f, 0.3f); 
    board.UpdateModel();
    board.setHitbox(glm::vec4(-40.0f, 0.0f, -42.0f, 1.0f), glm::vec4(40.0f, 20.0f, -38.0f, 1.0f));
    collisions.addHitbox(back_wall);*/


    // (Opcional) Teto
    StaticGameObject ceiling_object(
        &gpu_functions, &cube, TextureType::OBJ_FILE,  texture.GetTexture("bege_wall"), 
        LightType::NO, "CEILING_OBJECT");
    ceiling_object.transform.SetPosition(0.0f, 20.0f, 0.0f); 
    ceiling_object.transform.SetScale(40.0f, 1.0f, 40.0f); 
    ceiling_object.UpdateModel();

    StaticGameObject sphere_object(
        &gpu_functions, &sphere, TextureType::OBJ_FILE, texture.GetTexture("wood"), 
        LightType::PHONG, "SPHERE_OBJECT");
    glm::vec4 sphere_position = glm::vec4(20.0f, -4.0f, 10.0f, 1.0f);
    float sphere_radius = 5.0f;
    sphere_object.transform.SetPosition(sphere_position.x, sphere_position.y, sphere_position.z);
    sphere_object.transform.SetScale(sphere_radius, sphere_radius, sphere_radius);
    sphere_object.UpdateModel();
    sphere_object.setHitsphere(sphere_position, sphere_radius+2);
    collisions.addHitsphere(sphere_object);

    // Cubo que o jogador deve interagir para inserir a senha
    StaticGameObject password_cube(
        &gpu_functions, &cube, TextureType::OBJ_FILE, texture.GetTexture("white_board"), 
        LightType::NO, "PASSWORD_CUBE");
    password_cube.transform.SetPosition(-30.0f,-5.0f,20.0f);
    password_cube.transform.SetScale(2.0f, 2.0f, 2.0f);
    password_cube.UpdateModel();
    password_cube.setHitbox(glm::vec4(-33.0f, -100.0f, 16.0f, 1.0f), glm::vec4(-27.0f, 0.0f, 23.0f, 1.0f));
    collisions.addHitbox(password_cube);
    collisions.addClickableHitbox(password_cube);

    // Cubo que o jogador deve interagir para checkar a senha
    StaticGameObject check_cube(
        &gpu_functions, &cube, TextureType::OBJ_FILE, texture.GetTexture("wood"), 
        LightType::NO, "CHECK_CUBE");
    check_cube.transform.SetPosition(-30.0f,-5.0f,30.0f);
    check_cube.transform.SetScale(2.0f, 2.0f, 2.0f);
    check_cube.UpdateModel();
    check_cube.setHitbox(glm::vec4(-33.0f, -100.0f, 26.0f, 1.0f), glm::vec4(-27.0f, 0.0f, 33.0f, 1.0f));
    collisions.addHitbox(check_cube);
    collisions.addClickableHitbox(check_cube);

    // Door
    Door door(
        &gpu_functions, &door_object, TextureType::OBJ_FILE, texture.GetTexture("door"), 
        LightType::PHONG, "DOOR");
    door.transform.SetPosition(39.0f, 6.0f, 13.0f);
    door.transform.SetScale(0.08f, 0.08f, 0.08f);
    door.transform.SetRotation(3.14/2, 0.0f, -3.14/2);
    door.UpdateModel();
    //door.setHitbox(glm::vec4(38.0f, 0.0f, -40.0f, 1.0f), glm::vec4(42.0f, 20.0f, 40.0f, 1.0f));
    //collisions.addHitbox(door);

    StaticGameObject cow(
        &gpu_functions, &cow_object, TextureType::PLANE, texture.GetTexture("bege_wall"), 
        LightType::GOURAUD, "COW");
    cow.transform.SetPosition(-20.0f, -3.0f, 0.0f);
    cow.transform.SetScale(10.0f, 10.0f, 10.0f);
    cow.UpdateModel();  

    StaticGameObject desk_object(
        &gpu_functions, &desk, TextureType::OBJ_FILE, texture.GetTexture("wood_desk"), 
        LightType::NO, "DESK");
    desk_object.transform.SetPosition(18.0f, -8.0f, 25.0f);
    desk_object.transform.SetScale(6.0f, 6.0f, 6.0f);
    desk_object.UpdateModel();

    StaticGameObject paper(
        &gpu_functions, &plane, TextureType::PLANE, texture.GetTexture("paper"), 
        LightType::NO, "PAPER"
    );
    paper.transform.SetPosition(18.0f, -3.0f, 25.0f);
    paper.transform.SetScale(1.0f, 1.0f, 2.0f);
    paper.UpdateModel();

    StaticGameObject lamp_object(
        &gpu_functions, &lamp, TextureType::OBJ_FILE, texture.GetTexture("wood"), 
        LightType::NO, "LAMP"
    );
    lamp_object.transform.SetPosition(0.0f, 10.0f, 0.0f);
    lamp_object.transform.SetScale(5.0f, 5.0f, 5.0f);
    lamp_object.UpdateModel();

//-----------------------------------------SALA 2 --------------------------------------------------------------    
    
     StaticGameObject right_wall2(
        &gpu_functions, &cube,  TextureType::OBJ_FILE,  texture.GetTexture("bege_wall"), 
        LightType::NO, "RIGHT_WALL2");
    right_wall2.transform.SetPosition(42.0f, 0.0f, -30.0f); 
    right_wall2.transform.SetScale(1.0f, 20.0f, 40.0f);
    right_wall2.UpdateModel();
    right_wall2.setHitbox(glm::vec4(42.0f, 20.0f, 10.0f, 1.0f), glm::vec4(42.0f, 0.0f, -38.0f, 1.0f));
    collisions.addHitbox(right_wall2);

    
    StaticGameObject right_wall3(
        &gpu_functions, &cube,  TextureType::OBJ_FILE,  texture.GetTexture("green_floor"), 
        LightType::NO, "RIGHT_WALL3");
    right_wall3.transform.SetPosition(42.0f, 0.0f, 56.0f); 
    right_wall3.transform.SetScale(1.0f, 20.0f, 40.0f);
    right_wall3.UpdateModel();
    right_wall3.setHitbox(glm::vec4(42.0f, 0.0f, -38.0f, 1.0f), glm::vec4(42.0f, 20.0f, 10.0f, 1.0f));
    collisions.addHitbox(right_wall3);

    StaticGameObject floor_object2(
        &gpu_functions, &plane, TextureType::PLANE, texture.GetTexture("wood"), 
        LightType::NO, "FLOOR_OBJECT2"); 
    floor_object2.transform.SetPosition(80.0f, -10.0f, 0.0f);
    floor_object2.transform.SetScale(40.0, 1.0, 40.0); 
    floor_object2.UpdateModel();

     StaticGameObject back_wall2(
        &gpu_functions, &cube, TextureType::OBJ_FILE ,texture.GetTexture("green_floor"), 
        LightType::NO, "BACK_WALL2"); 
    back_wall2.transform.SetPosition(80.0f, 0.0f, -40.0f); 
    back_wall2.transform.SetScale(40.0f, 20.0f, 1.0f); 
    back_wall2.UpdateModel();
    back_wall2.setHitbox(glm::vec4(-40.0f, 0.0f, -42.0f, 1.0f), glm::vec4(40.0f, 20.0f, -38.0f, 1.0f));
    collisions.addHitbox(back_wall2);

     StaticGameObject front_wall2(
        &gpu_functions, &cube,  TextureType::OBJ_FILE, texture.GetTexture("green_floor"), 
        LightType::NO, "FRONT_WALL2");
    front_wall2.transform.SetPosition(80.0f, 0.0f, 40.0f);
    front_wall2.transform.SetScale(40.0f, 20.0f, 1.0f);
    front_wall2.UpdateModel();
    front_wall2.setHitbox(glm::vec4(-40.0f, -10.0f, 38.0f, 1.0f), glm::vec4(40.0f, 10.0f, 42.0f, 1.0f));
    collisions.addHitbox(front_wall2);

     // Parede da esquerda
    StaticGameObject left_wall4(
        &gpu_functions, &cube,  TextureType::OBJ_FILE,  texture.GetTexture("bege_wall"), 
        LightType::NO, "LEFT_WALL4");
    left_wall4.transform.SetPosition(100.0f, 0.0f, -30.0f); 
    left_wall4.transform.SetScale(1.0f, 20.0f, 40.0f);
    left_wall4.UpdateModel();
    left_wall4.setHitbox(glm::vec4(38.0f, 0.0f, -40.0f, 1.0f), glm::vec4(40.0f, 20.0f, 10.0f, 1.0f));
    collisions.addHitbox(left_wall4);

    
    // Parede da esquerda
    StaticGameObject left_wall5(
        &gpu_functions, &cube,  TextureType::OBJ_FILE,  texture.GetTexture("bege_wall"), 
        LightType::NO, "LEFT_WALL5");
    left_wall5.transform.SetPosition(100.0f, 0.0f, 56.0f); 
    left_wall5.transform.SetScale(1.0f, 20.0f, 40.0f);
    left_wall5.UpdateModel();
    left_wall5.setHitbox(glm::vec4(37.0f, 0.0f, 16.0f, 1.0f), glm::vec4(44.0f, 0.0f, 44.0f, 1.0f));
    collisions.addHitbox(left_wall5);

    
    StaticGameObject left_wall6(
        &gpu_functions, &cube,  TextureType::OBJ_FILE,  texture.GetTexture("bege_wall"), 
        LightType::NO, "LEFT_WALL6");
    left_wall6.transform.SetPosition(100.0f, 16.0f, 10.0f); 
    left_wall6.transform.SetScale(1.0f, 10.0f, 10.0f);
    left_wall6.UpdateModel();

    StaticGameObject ceiling_object2(
        &gpu_functions, &cube, TextureType::OBJ_FILE,  texture.GetTexture("bege_wall"), 
        LightType::NO, "CEILING_OBJECT2");
    ceiling_object2.transform.SetPosition(80.0f, 20.0f, 0.0f); 
    ceiling_object2.transform.SetScale(40.0f, 1.0f, 40.0f); 
    ceiling_object2.UpdateModel();

    Door door2(
        &gpu_functions, &door_object, TextureType::OBJ_FILE, texture.GetTexture("door"), 
        LightType::PHONG, "DOOR2");
    door2.transform.SetPosition(100.0f, 6.0f, 13.0f);
    door2.transform.SetScale(0.08f, 0.08f, 0.08f);
    door2.transform.SetRotation(3.14/2, 0.0f, -3.14/2);
    door2.UpdateModel();


    //SALA 1
    //scene.AddGameObject(&sphere_object);
    //scene.AddGameObject(&bunny_object);
    scene.AddGameObject(&floor_object);
    scene.AddGameObject(&back_wall);
    scene.AddGameObject(&right_wall);
    scene.AddGameObject(&left_wall1);
    scene.AddGameObject(&left_wall2);
    scene.AddGameObject(&left_wall3);
    scene.AddGameObject(&front_wall);
    scene.AddGameObject(&ceiling_object);
    scene.AddGameObject(&chair1);
    scene.AddGameObject(&chair2);
    scene.AddGameObject(&chair3);
    scene.AddGameObject(&chair4);
    scene.AddGameObject(&chair5);
    scene.AddGameObject(&chair6);
    scene.AddGameObject(&chair7);
    scene.AddGameObject(&chair8);
    scene.AddGameObject(&chair9);
    scene.AddGameObject(&chair10);
    scene.AddGameObject(&chair11);
    scene.AddGameObject(&chair12);
    scene.AddGameObject(&board);
    //scene.AddGameObject(&window1);
    scene.AddGameObject(&white_board_object);
    scene.AddGameObject(&sphere_object);
    scene.AddGameObject(&password_cube);
    scene.AddGameObject(&check_cube);
    scene.AddGameObject(&door);
    scene.AddGameObject(&cow);
    scene.AddGameObject(&desk_object);
    scene.AddGameObject(&paper);
    scene.AddGameObject(&lamp_object);

    //SALA 2
    scene.AddGameObject(&right_wall2);
    scene.AddGameObject(&right_wall3);
    scene.AddGameObject(&floor_object2);
    scene.AddGameObject(&back_wall2);
    scene.AddGameObject(&front_wall2);
    scene.AddGameObject(&left_wall4);
    scene.AddGameObject(&left_wall5);
    scene.AddGameObject(&left_wall6);
    scene.AddGameObject(&ceiling_object2);
    scene.AddGameObject(&door2);
    
   


    // Habilitamos o Z-buffer. Veja slides 104-116 do documento Aula_09_Projecoes.pdf.
    glEnable(GL_DEPTH_TEST);

    // Habilitamos o Backface Culling. Veja slides 8-13 do documento Aula_02_Fundamentos_Matematicos.pdf, slides 23-34 do documento Aula_13_Clipping_and_Culling.pdf e slides 112-123 do documento Aula_14_Laboratorio_3_Revisao.pdf.
    glEnable(GL_CULL_FACE);
    glCullFace(GL_BACK);
    glFrontFace(GL_CCW);


    float old_time = glfwGetTime();
    float current_time = 0;
    float delta_time = 0;

    bool isOpening = true; //Flag para controlar abertura do jogo
    bool isFading = false;        // Flag para controle do escurecimento
    bool isFreeExploration = false; // Flag para movimentação livre
    bool testingLookAt = true;

    // Ficamos em um loop infinito, renderizando, até que o usuário feche a janela
    while (!glfwWindowShouldClose(window))
    { // Aqui executamos as operações de renderização

        // Definimos a cor do "fundo" do framebuffer como branco.  Tal cor é
        // definida como coeficientes RGBA: Red, Green, Blue, Alpha; isto é:
        // Vermelho, Verde, Azul, Alpha (valor de transparência).
        // Conversaremos sobre sistemas de cores nas aulas de Modelos de Iluminação.
        //
        //           R     G     B     A
        glClearColor(1.0f, 1.0f, 1.0f, 1.0f);

        // "Pintamos" todos os pixels do framebuffer com a cor definida acima,
        // e também resetamos todos os pixels do Z-buffer (depth buffer).
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // Pedimos para a GPU utilizar o programa de GPU criado acima (contendo
        // os shaders de vértice e fragmentos).
        glUseProgram(g_GpuProgramID);

        InputState current_state = input_handler.getInputState();
        if (current_state.is_lookat)
        {
        //Teste do look at camera
            LookAtCamera camera;

            // Define uma câmera fixa ao fundo
            camera.setStaticCamera(
                glm::vec4(37.0f, 0.0f, -37.0f, 1.0f), // Posição fixa
                glm::vec4(0.0f, 0.0f, 0.0f, 1.0f)    // Olhando para o centro da sala
            );

            gpu_functions.updateLookAtCameraMatrices(camera);

        }
        else
        {
    
        
            current_time = (float)glfwGetTime();
            delta_time = current_time - old_time;
            old_time = current_time;

        
            //player.updateBezier(delta_time);

            // Movimento que o usuário deseja fazer
            
            player.updateVelocity(current_state, delta_time);

            // Verificamos se o player colidiu com algum objeto e atualizamos a velocidade
            glm::vec4 new_player_velocity = collisions.checkPlayerCollision(player);
            player.setVelocity(new_player_velocity);

            // Atualizamos a posição do player
            player.update(current_state, delta_time);
            
            
            auto clickable_objects_collision = collisions.checkClickableCollision(player);
            game_logic.checkInteraction(clickable_objects_collision, current_state.is_interacting);
            game_logic.updateLastInteraction(delta_time);


            gpu_functions.updateFreeCameraMatrices(player.getFreeCamera());

            player.printPlayerPosition();


            if (game_logic.isPasswordCorrect())
            {
                door.OpeningDoor(delta_time);
                door.UpdateModel();
            }
        }
        

        scene.Render();

        // Imprimimos na tela informação sobre o número de quadros renderizados
        // por segundo (frames per second).
        TextRendering_ShowFramesPerSecond(window);

        // O framebuffer onde OpenGL executa as operações de renderização não
        // é o mesmo que está sendo mostrado para o usuário, caso contrário
        // seria possível ver artefatos conhecidos como "screen tearing". A
        // chamada abaixo faz a troca dos buffers, mostrando para o usuário
        // tudo que foi renderizado pelas funções acima.
        // Veja o link: https://en.wikipedia.org/w/index.php?title=Multiple_buffering&oldid=793452829#Double_buffering_in_computer_graphics
        glfwSwapBuffers(window);

        // Verificamos com o sistema operacional se houve alguma interação do
        // usuário (teclado, mouse, ...). Caso positivo, as funções de callback
        // definidas anteriormente usando glfwSet*Callback() serão chamadas
        // pela biblioteca GLFW.
        glfwPollEvents();
    }

    // Finalizamos o uso dos recursos do sistema operacional
    glfwTerminate();

    // Fim do programa
    return 0;
}

// Função que desenha um objeto armazenado em g_VirtualScene. Veja definição
// dos objetos na função BuildTrianglesAndAddToVirtualScene().
/*void DrawVirtualObject(const char* object_name)
{
    // "Ligamos" o VAO. Informamos que queremos utilizar os atributos de
    // vértices apontados pelo VAO criado pela função BuildTrianglesAndAddToVirtualScene(). Veja
    // comentários detalhados dentro da definição de BuildTrianglesAndAddToVirtualScene().
    glBindVertexArray(g_VirtualScene[object_name].vertex_array_object_id);

    // Pedimos para a GPU rasterizar os vértices dos eixos XYZ
    // apontados pelo VAO como linhas. Veja a definição de
    // g_VirtualScene[""] dentro da função BuildTrianglesAndAddToVirtualScene(), e veja
    // a documentação da função glDrawElements() em
    // http://docs.gl/gl3/glDrawElements.
    glDrawElements(
        g_VirtualScene[object_name].rendering_mode,
        g_VirtualScene[object_name].num_indices,
        GL_UNSIGNED_INT,
        (void*)(g_VirtualScene[object_name].first_index * sizeof(GLuint))
    );

    // "Desligamos" o VAO, evitando assim que operações posteriores venham a
    // alterar o mesmo. Isso evita bugs.
    glBindVertexArray(0);
}*/


// Função que pega a matriz M e guarda a mesma no topo da pilha
void PushMatrix(glm::mat4 M)
{
    g_MatrixStack.push(M);
}

// Função que remove a matriz atualmente no topo da pilha e armazena a mesma na variável M
void PopMatrix(glm::mat4& M)
{
    if ( g_MatrixStack.empty() )
    {
        M = Matrices::Matrix_Identity();
    }
    else
    {
        M = g_MatrixStack.top();
        g_MatrixStack.pop();
    }
}

// Definição da função que será chamada sempre que a janela do sistema
// operacional for redimensionada, por consequência alterando o tamanho do
// "framebuffer" (região de memória onde são armazenados os pixels da imagem).
void FramebufferSizeCallback(GLFWwindow* window, int width, int height)
{
    // Indicamos que queremos renderizar em toda região do framebuffer. A
    // função "glViewport" define o mapeamento das "normalized device
    // coordinates" (NDC) para "pixel coordinates".  Essa é a operação de
    // "Screen Mapping" ou "Viewport Mapping" vista em aula ({+ViewportMapping2+}).
    glViewport(0, 0, width, height);

    // Atualizamos também a razão que define a proporção da janela (largura /
    // altura), a qual será utilizada na definição das matrizes de projeção,
    // tal que não ocorra distorções durante o processo de "Screen Mapping"
    // acima, quando NDC é mapeado para coordenadas de pixels. Veja slides 205-215 do documento Aula_09_Projecoes.pdf.
    //
    // O cast para float é necessário pois números inteiros são arredondados ao
    // serem divididos!
    g_ScreenRatio = (float)width / height;
}

// Variáveis globais que armazenam a última posição do cursor do mouse, para
// que possamos calcular quanto que o mouse se movimentou entre dois instantes
// de tempo. Utilizadas no callback CursorPosCallback() abaixo.
double g_LastCursorPosX, g_LastCursorPosY;

// Função callback chamada sempre que o usuário aperta algum dos botões do mouse
void MouseButtonCallback(GLFWwindow* window, int button, int action, int mods)
{
    if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS)
    {
        // Se o usuário pressionou o botão esquerdo do mouse, guardamos a
        // posição atual do cursor nas variáveis g_LastCursorPosX e
        // g_LastCursorPosY.  Também, setamos a variável
        // g_LeftMouseButtonPressed como true, para saber que o usuário está
        // com o botão esquerdo pressionado.
        glfwGetCursorPos(window, &g_LastCursorPosX, &g_LastCursorPosY);
        g_LeftMouseButtonPressed = true;
    }
    if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_RELEASE)
    {
        // Quando o usuário soltar o botão esquerdo do mouse, atualizamos a
        // variável abaixo para false.
        g_LeftMouseButtonPressed = false;
    }
    if (button == GLFW_MOUSE_BUTTON_RIGHT && action == GLFW_PRESS)
    {
        // Se o usuário pressionou o botão esquerdo do mouse, guardamos a
        // posição atual do cursor nas variáveis g_LastCursorPosX e
        // g_LastCursorPosY.  Também, setamos a variável
        // g_RightMouseButtonPressed como true, para saber que o usuário está
        // com o botão esquerdo pressionado.
        glfwGetCursorPos(window, &g_LastCursorPosX, &g_LastCursorPosY);
        g_RightMouseButtonPressed = true;
    }
    if (button == GLFW_MOUSE_BUTTON_RIGHT && action == GLFW_RELEASE)
    {
        // Quando o usuário soltar o botão esquerdo do mouse, atualizamos a
        // variável abaixo para false.
        g_RightMouseButtonPressed = false;
    }
    if (button == GLFW_MOUSE_BUTTON_MIDDLE && action == GLFW_PRESS)
    {
        // Se o usuário pressionou o botão esquerdo do mouse, guardamos a
        // posição atual do cursor nas variáveis g_LastCursorPosX e
        // g_LastCursorPosY.  Também, setamos a variável
        // g_MiddleMouseButtonPressed como true, para saber que o usuário está
        // com o botão esquerdo pressionado.
        glfwGetCursorPos(window, &g_LastCursorPosX, &g_LastCursorPosY);
        g_MiddleMouseButtonPressed = true;
    }
    if (button == GLFW_MOUSE_BUTTON_MIDDLE && action == GLFW_RELEASE)
    {
        // Quando o usuário soltar o botão esquerdo do mouse, atualizamos a
        // variável abaixo para false.
        g_MiddleMouseButtonPressed = false;
    }
}

// Definimos o callback para impressão de erros da GLFW no terminal
void ErrorCallback(int error, const char* description)
{
    fprintf(stderr, "ERROR: GLFW: %s\n", description);
}

// Escrevemos na tela o número de quadros renderizados por segundo (frames per
// second).
void TextRendering_ShowFramesPerSecond(GLFWwindow* window)
{
    if ( !g_ShowInfoText )
        return;

    // Variáveis estáticas (static) mantém seus valores entre chamadas
    // subsequentes da função!
    static float old_seconds = (float)glfwGetTime();
    static int   ellapsed_frames = 0;
    static char  buffer[20] = "?? fps";
    static int   numchars = 7;

    ellapsed_frames += 1;

    // Recuperamos o número de segundos que passou desde a execução do programa
    float seconds = (float)glfwGetTime();

    // Número de segundos desde o último cálculo do fps
    float ellapsed_seconds = seconds - old_seconds;

    if ( ellapsed_seconds > 1.0f )
    {
        numchars = snprintf(buffer, 20, "%.2f fps", ellapsed_frames / ellapsed_seconds);
    
        old_seconds = seconds;
        ellapsed_frames = 0;
    }

    float lineheight = TextRendering_LineHeight(window);
    float charwidth = TextRendering_CharWidth(window);

    TextRendering_PrintString(window, buffer, 1.0f-(numchars + 1)*charwidth, 1.0f-lineheight, 1.0f);
}


// set makeprg=cd\ ..\ &&\ make\ run\ >/dev/null
// vim: set spell spelllang=pt_br :

