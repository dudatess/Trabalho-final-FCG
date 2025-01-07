#version 330 core

// Atributos de fragmentos recebidos como entrada ("in") pelo Fragment Shader.
// Neste exemplo, este atributo foi gerado pelo rasterizador como a
// interpolação da posição global e a normal de cada vértice, definidas em
// "shader_vertex.glsl" e "main.cpp".
in vec4 position_world;
in vec4 normal;

in vec4 position_model;
float px = position_model.x;
float py = position_model.y;
float pz = position_model.z;

in vec2 texture_coords;
in vec3 gouraud_shading_term;

// Matrizes computadas no código C++ e enviadas para a GPU
uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

uniform vec4 bbox_min;
uniform vec4 bbox_max;

// Identificador que define qual objeto está sendo desenhado no momento
//#define SPHERE 0
//#define BUNNY  1
//#define PLANE  2
//uniform int object_id;

#define LIGHT_TYPE_GOURAUD 0
#define LIGHT_TYPE_PHONG   1
#define LIGHT_TYPE_NO      2
uniform int light_type;

#define TEXTURE_SPHERE 0
#define TEXTURE_PLANE 1
#define TEXTURE_OBJ_FILE 2
uniform int texture_type;
uniform sampler2D texture_id;

// O valor de saída ("out") de um Fragment Shader é a cor final do fragmento.
out vec4 color;

#define M_PI   3.14159265358979323846
#define M_PI_2 1.57079632679489661923

//Lab 5
vec2 SphereProjection(vec4 bbox_min, vec4 bbox_max, vec4 position_model)
{
    vec4 bbox_center = (bbox_min + bbox_max) / 2.0;

    float theta = atan(position_model.x, position_model.z);
    float phi = asin(position_model.y/length(position_model - bbox_center));

    float u = (theta + M_PI)/(2*M_PI);
    float v = (phi + M_PI_2)/M_PI;

    return vec2(u, v);
}

vec2 PlaneProjection(vec4 bbox_min, vec4 bbox_max, vec4 position_model)
{
    
    float u = 0.0;
    float v = 0.0;

    u = px;
    v = pz;

    return vec2(u, v);
}

void main()
{
    // Obtemos a posição da câmera utilizando a inversa da matriz que define o
    // sistema de coordenadas da câmera.
    vec4 origin = vec4(0.0, 0.0, 0.0, 1.0);
    vec4 camera_position = inverse(view) * origin;

    // O fragmento atual é coberto por um ponto que percente à superfície de um
    // dos objetos virtuais da cena. Este ponto, p, possui uma posição no
    // sistema de coordenadas global (World coordinates). Esta posição é obtida
    // através da interpolação, feita pelo rasterizador, da posição de cada
    // vértice.
    vec4 p = position_world;

    // Normal do fragmento atual, interpolada pelo rasterizador a partir das
    // normais de cada vértice.
    vec4 n = normalize(normal);

    // Vetor que define o sentido da câmera em relação ao ponto atual.
    vec4 v = normalize(camera_position - p);

    // Vetor que define o sentido da fonte de luz em relação ao ponto atual.
    // vec4 l = normalize(vec4(1.0,1.0,0.5,0.0));
    vec4 l = v;

    // Vetor que define o sentido da reflexão especular ideal.
    vec4 r = -l + 2*n*(dot(n,l)); // PREENCHA AQUI o vetor de reflexão especular ideal

    vec2 definitive_texture = vec2(0.0, 0.0);
    //Implementar outras texturas também!
    if(texture_type == TEXTURE_PLANE)
    {
        definitive_texture = PlaneProjection(bbox_min, bbox_max, position_model);
    }
    else if(texture_type == TEXTURE_SPHERE)
    {
        definitive_texture = SphereProjection(bbox_min, bbox_max, position_model);
    }
    else if(texture_type == TEXTURE_OBJ_FILE)
    {
        definitive_texture = texture_coords;
    }

    vec3 Kd = texture(texture_id, definitive_texture).rgb;
    vec3 Ks = vec3(0.8,0.8,0.8);
    float q = 32.0;
    vec3 I = vec3(1.0, 1.0, 1.0);  // Light intensity (white light)
   
    //Termo difuso utilizando a lei dos cossenos de Lambert (Diffuse light)
    vec3 lambert_diffuse_term = Kd * (max(0.1, dot(n, l)) + 0.01);
    //vec3 lambert_diffuse_term = Kd * I * max(dot(n,l),0.05);

    // Termo ambiente
    //vec3 ambient_term = Ka * Ia; 

    // Termo especular utilizando o modelo de iluminação de Phong
    vec4 half_vector = normalize(v + l);
    vec3 phong_specular_term  = Ks * pow(max(0, dot(n, half_vector)), q); // PREENCH AQUI o termo especular de Phong


    // Alpha component
    color.a = 1;

    if(light_type == LIGHT_TYPE_GOURAUD)
    {
        color.rgb = lambert_diffuse_term + gouraud_shading_term;
    }
    else if ( light_type == LIGHT_TYPE_PHONG)
    {
        color.rgb = lambert_diffuse_term + phong_specular_term;
    }
    else 
    {
        color.rgb = lambert_diffuse_term;
    }


     // NOTE: Se você quiser fazer o rendering de objetos transparentes, é
    // necessário:
    // 1) Habilitar a operação de "blending" de OpenGL logo antes de realizar o
    //    desenho dos objetos transparentes, com os comandos abaixo no código C++:
    //      glEnable(GL_BLEND);
    //      glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    // 2) Realizar o desenho de todos objetos transparentes *após* ter desenhado
    //    todos os objetos opacos; e
    // 3) Realizar o desenho de objetos transparentes ordenados de acordo com
    //    suas distâncias para a câmera (desenhando primeiro objetos
    //    transparentes que estão mais longe da câmera).
    // Alpha default = 1 = 100% opaco = 0% transparente
    color.a = 1;

    // Cor final do fragmento calculada com uma combinação dos termos difuso,
    // especular, e ambiente. Veja slide 129 do documento Aula_17_e_18_Modelos_de_Iluminacao.pdf.

    // Gamma correction (sRGB monitor)
    color.rgb = pow(color.rgb, vec3(1.0,1.0,1.0)/2.2);

} 
