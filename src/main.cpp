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
    InputHandler input_handler;

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
    TextRendering_Init();

    Object sphere("../../data/sphere.obj");
    Object bunny("../../data/bunny.obj");
    Object plane("../../data/plane.obj");
    Object chair("../../data/uploads_files_2101984_Skin_chair.obj");
    Object cube("../../data/cube.obj");
    Object window_object("../../data/uploads_files_3158526_Small_Square_Window-Final.obj");
    Object white_board("../../data/uploads_files_3685891_whiteBoard.obj");


    StaticGameObject white_board_object(&gpu_functions, &white_board, TextureType::OBJ_FILE, texture.GetTexture("white_board"), LightType::NO);
    white_board_object.transform.SetPosition(0.0f, -1.0f, 0.0f);
    white_board_object.transform.SetScale(10.0f, 10.0f, 10.0f);
    white_board_object.UpdateModel();

    StaticGameObject chair1(&gpu_functions, &chair, TextureType::OBJ_FILE ,texture.GetTexture("wood"), LightType::NO);
    chair1.transform.SetPosition(2.0f, -10.0f, -10.0f);
    chair1.transform.SetScale(0.1f, 0.1f, 0.1f);
    chair1.UpdateModel();

    StaticGameObject chair2(&gpu_functions, &chair, TextureType::OBJ_FILE ,texture.GetTexture("wood"), LightType::NO);
    chair2.transform.SetPosition(12.0f, -10.0f, -10.0f);
    chair2.transform.SetScale(0.1f, 0.1f, 0.1f);
    chair2.UpdateModel();
   
    StaticGameObject chair3(&gpu_functions, &chair, TextureType::OBJ_FILE ,texture.GetTexture("wood"), LightType::NO);
    chair3.transform.SetPosition(22.0f, -10.0f, -10.0f);
    chair3.transform.SetScale(0.1f, 0.1f, 0.1f);
    chair3.UpdateModel();

    StaticGameObject chair4(&gpu_functions, &chair, TextureType::OBJ_FILE ,texture.GetTexture("wood"), LightType::NO);
    chair4.transform.SetPosition(-8.0f, -10.0f, -10.0f);
    chair4.transform.SetScale(0.1f, 0.1f, 0.1f);
    chair4.UpdateModel();

    StaticGameObject chair5(&gpu_functions, &chair, TextureType::OBJ_FILE ,texture.GetTexture("wood"), LightType::NO);
    chair5.transform.SetPosition(-18.0f, -10.0f, -10.0f);
    chair5.transform.SetScale(0.1f, 0.1f, 0.1f);
    chair5.UpdateModel();

    StaticGameObject chair6(&gpu_functions, &chair, TextureType::OBJ_FILE ,texture.GetTexture("wood"), LightType::NO);
    chair6.transform.SetPosition(-28.0f, -10.0f, -10.0f);
    chair6.transform.SetScale(0.1f, 0.1f, 0.1f);
    chair6.UpdateModel();

     StaticGameObject chair7(&gpu_functions, &chair, TextureType::OBJ_FILE ,texture.GetTexture("wood"), LightType::NO);
    chair7.transform.SetPosition(2.0f, -10.0f, -20.0f);
    chair7.transform.SetScale(0.1f, 0.1f, 0.1f);
    chair7.UpdateModel();

    StaticGameObject chair8(&gpu_functions, &chair, TextureType::OBJ_FILE ,texture.GetTexture("wood"), LightType::NO);
    chair8.transform.SetPosition(12.0f, -10.0f, -20.0f);
    chair8.transform.SetScale(0.1f, 0.1f, 0.1f);
    chair8.UpdateModel();
   
    StaticGameObject chair9(&gpu_functions, &chair, TextureType::OBJ_FILE ,texture.GetTexture("wood"), LightType::NO);
    chair9.transform.SetPosition(22.0f, -10.0f, -20.0f);
    chair9.transform.SetScale(0.1f, 0.1f, 0.1f);
    chair9.UpdateModel();

    StaticGameObject chair10(&gpu_functions, &chair, TextureType::OBJ_FILE ,texture.GetTexture("wood"), LightType::NO);
    chair10.transform.SetPosition(-8.0f, -10.0f, -20.0f);
    chair10.transform.SetScale(0.1f, 0.1f, 0.1f);
    chair10.UpdateModel();

    StaticGameObject chair11(&gpu_functions, &chair, TextureType::OBJ_FILE ,texture.GetTexture("wood"), LightType::NO);
    chair11.transform.SetPosition(-18.0f, -10.0f, -20.0f);
    chair11.transform.SetScale(0.1f, 0.1f, 0.1f);
    chair11.UpdateModel();

    StaticGameObject chair12(&gpu_functions, &chair, TextureType::OBJ_FILE ,texture.GetTexture("wood"), LightType::NO);
    chair12.transform.SetPosition(-28.0f, -10.0f, -20.0f);
    chair12.transform.SetScale(0.1f, 0.1f, 0.1f);
    chair12.UpdateModel();


    // Cria objetos de jogo e os adiciona à cena
    StaticGameObject floor_object(&gpu_functions, &plane, TextureType::PLANE, texture.GetTexture("green_floor"), LightType::NO); 
    floor_object.transform.SetPosition(0.0f, -10.0f, 0.0f);
    floor_object.transform.SetScale(40.0, 1.0, 40.0); 
    floor_object.UpdateModel();

    // Parede de trás
    StaticGameObject back_wall(&gpu_functions, &cube, TextureType::OBJ_FILE ,texture.GetTexture("bege_wall"), LightType::NO); 
    back_wall.transform.SetPosition(0.0f, 0.0f, 40.0f); 
    back_wall.transform.SetScale(40.0f, 20.0f, 1.0f); 
    back_wall.UpdateModel();

    // Parede da direita
    StaticGameObject right_wall(&gpu_functions, &cube, TextureType::OBJ_FILE, texture.GetTexture("bege_wall"), LightType::NO);
    right_wall.transform.SetPosition(40.0f, 0.0f, 0.0f); 
    right_wall.transform.SetScale(1.0f, 20.0f, 40.0f);
    right_wall.UpdateModel();

    // Parede da esquerda
    StaticGameObject left_wall(&gpu_functions, &cube,  TextureType::OBJ_FILE,  texture.GetTexture("bege_wall"), LightType::NO);
    left_wall.transform.SetPosition(-40.0f, 0.0f, 0.0f); 
    left_wall.transform.SetScale(1.0f, 20.0f, 40.0f);
    left_wall.UpdateModel();

    // Parede da frente
    StaticGameObject front_wall(&gpu_functions, &cube,  TextureType::OBJ_FILE, texture.GetTexture("bege_wall"), LightType::NO);
    front_wall.transform.SetPosition(0.0f, 0.0f, -40.0f); 
    front_wall.transform.SetScale(40.0f, 20.0f, 1.0f);
    front_wall.UpdateModel();

    // (Opcional) Teto
    StaticGameObject ceiling_object(&gpu_functions, &cube, TextureType::OBJ_FILE,  texture.GetTexture("bege_wall"), LightType::NO);
    ceiling_object.transform.SetPosition(0.0f, 20.0f, 0.0f); 
    ceiling_object.transform.SetScale(40.0f, 1.0f, 40.0f); 
    ceiling_object.UpdateModel();



    //scene.AddGameObject(&sphere_object);
    //scene.AddGameObject(&bunny_object);
    scene.AddGameObject(&floor_object);
    scene.AddGameObject(&back_wall);
    scene.AddGameObject(&right_wall);
    scene.AddGameObject(&left_wall);
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
    //scene.AddGameObject(&window1);
    scene.AddGameObject(&white_board_object);


    // Habilitamos o Z-buffer. Veja slides 104-116 do documento Aula_09_Projecoes.pdf.
    glEnable(GL_DEPTH_TEST);

    // Habilitamos o Backface Culling. Veja slides 8-13 do documento Aula_02_Fundamentos_Matematicos.pdf, slides 23-34 do documento Aula_13_Clipping_and_Culling.pdf e slides 112-123 do documento Aula_14_Laboratorio_3_Revisao.pdf.
    glEnable(GL_CULL_FACE);
    glCullFace(GL_BACK);
    glFrontFace(GL_CCW);


    float old_time = glfwGetTime();
    float current_time = 0;
    float delta_time = 0;
    //FreeCamera camera_test;

    Player player;

    // Ficamos em um loop infinito, renderizando, até que o usuário feche a janela
    while (!glfwWindowShouldClose(window))
    {
        // Aqui executamos as operações de renderização

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

        
        current_time = (float)glfwGetTime();
        delta_time = current_time - old_time;
        old_time = current_time;

        InputState current_state = input_handler.getInputState();

        player.update(current_state, delta_time);
        //camera_test.update(current_state, delta_time);

        glm::mat4 view = Matrices::Matrix_Camera_View(player.getCamera().getCameraPosition(), player.getCamera().getCameraViewVector(), player.getCamera().getCameraUpVector());
        glm::mat4 projection;

        float nearplane = -0.1f;  // Posição do "near plane"
        float farplane  = -100.0f; // Posição do "far plane"

        
        
        // Projeção Perspectiva.
        // Para definição do field of view (FOV), veja slides 205-215 do documento Aula_09_Projecoes.pdf.
        float field_of_view = 3.141592 / 3.0f;
        projection = Matrices::Matrix_Perspective(field_of_view, g_ScreenRatio, nearplane, farplane);
        

        glm::mat4 model = Matrices::Matrix_Identity(); // Transformação identidade de modelagem

        // Enviamos as matrizes "view" e "projection" para a placa de vídeo
        // (GPU). Veja o arquivo "shader_vertex.glsl", onde estas são
        // efetivamente aplicadas em todos os pontos.
        gpu_functions.updateCameraMatrices(view, projection);


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

