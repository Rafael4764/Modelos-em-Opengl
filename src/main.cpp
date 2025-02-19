#include <iostream>
#include <GLFW/glfw3.h>
#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#define STB_IMAGE_IMPLEMENTATION
#define CHUNK_SIZE 10
#define RENDER_DISTANCE 70
#include "../vendor/stb_image.h"
#include "../vendor/FastNoise.h"
#include "Shader.h"
#include <vector>
int SCR_WIDTH = 500;
int SCR_HEIGHT = 400;
glm::vec3 camPos = glm::vec3(0.0, 0.0, 10.0);
glm::vec3 camFront = glm::vec3(0.0, 0.0, -1.0);
typedef glm::mat4 mat4;
typedef glm::vec3 vec3;
float lastX = 0;
float lastY = 0;
float yaw = -90.0f;
float pitch = 0;
bool firstMove = true;
glm::vec3 camUp = glm::vec3(0.0, 1.0, 0.0);
FastNoise noise;
Shader* c_shader;
unsigned int VAO, VBO, shader, texture;
void u3f(const char* path, glm::vec3 vec)
{
    glUniform3f(glGetUniformLocation(shader, path), vec.x, vec.y, vec.z);
}
void u1f(const char* path, float f)
{
    glUniform1f(glGetUniformLocation(shader, path), f);
}
void u1i(const char* path, int i)
{
    glUniform1i(glGetUniformLocation(shader, path), i);
}
float vertices[] = {
    // positions              // normals              // texture coords
    -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f, 0.0f, // Bottom-left
     0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f, 0.0f, // bottom-right    
     0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f, 1.0f, // top-right              
     0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f, 1.0f, // top-right
    -0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f, 1.0f, // top-left
    -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f, 0.0f, // bottom-left                
    // Front face
    -0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  0.0f, 0.0f, // bottom-left
     0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  1.0f, 1.0f, // top-right
     0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  1.0f, 0.0f, // bottom-right        
     0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  1.0f, 1.0f, // top-right
    -0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  0.0f, 0.0f, // bottom-left
    -0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  0.0f, 1.0f, // top-left        
    // Left face
    -0.5f,  0.5f,  0.5f,  -1.0f,  0.0f,  0.0f,  1.0f, 0.0f, // top-right
    -0.5f, -0.5f, -0.5f,  -1.0f,  0.0f,  0.0f,  0.0f, 1.0f, // bottom-left
    -0.5f,  0.5f, -0.5f,  -1.0f,  0.0f,  0.0f,  1.0f, 1.0f, // top-left       
    -0.5f, -0.5f, -0.5f,  -1.0f,  0.0f,  0.0f,  0.0f, 1.0f, // bottom-left
    -0.5f,  0.5f,  0.5f,  -1.0f,  0.0f,  0.0f,  1.0f, 0.0f, // top-right
    -0.5f, -0.5f,  0.5f,  -1.0f,  0.0f,  0.0f,  0.0f, 0.0f, // bottom-right
    // Right face
     0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  1.0f, 0.0f, // top-left
     0.5f,  0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  1.0f, 1.0f, // top-right      
     0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  0.0f, 1.0f, // bottom-right          
     0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  0.0f, 1.0f, // bottom-right
     0.5f, -0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  0.0f, 0.0f, // bottom-left
     0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  1.0f, 0.0f, // top-left
     // Bottom face          
    -0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  0.0f, 1.0f, // top-right
     0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  1.0f, 0.0f, // bottom-left
     0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  1.0f, 1.0f, // top-left        
     0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  1.0f, 0.0f, // bottom-left
    -0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  0.0f, 1.0f, // top-right
    -0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  0.0f, 0.0f, // bottom-right
    // Top face
   -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  0.0f, 1.0f, // top-left
    0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  1.0f, 1.0f, // top-right
    0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  1.0f, 0.0f, // bottom-right                 
    0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  1.0f, 0.0f, // bottom-right
   -0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  0.0f, 0.0f, // bottom-left  
   -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  0.0f, 1.0f
};


void setupShaders()
{
    const char* vss = R"(
    #version 330 core
    layout(location = 0) in vec3 pos;
    layout(location = 1) in vec3 aNormal;
    layout(location = 2) in vec2 aTexCoord;
    uniform mat4 model;
    uniform mat4 view;
    uniform mat4 proj;
    out vec3 normal;
    out vec3 FragPos;
    out vec2 TexCoord;
    void main()
{
    FragPos = vec3(model * vec4(pos, 1.0));
    gl_Position = proj * view * model * vec4(pos, 1.0);
    normal = mat3(transpose(inverse(model))) * aNormal;
    TexCoord = aTexCoord;
}
)";
    const char* fss = R"(
    #version 330 core
    layout(location = 0) out vec4 color;
    in vec2 TexCoord;
    uniform sampler2D texture1;
    in vec3 normal;
    in vec3 FragPos;
    struct Material
{
    sampler2D diffuse;
    vec3 specular;
    float shininess;
};
    struct Light
{
    vec3 ambient;
    vec3 position;
    vec3 diffuse;
    vec3 specular;
};
    uniform Material material;
    uniform Light light;
    uniform vec3 viewPos;
    void main()
{
    vec3 ambient = light.ambient * texture(material.diffuse, TexCoord).rgb;

    vec3 norm = normalize(normal);
    vec3 lightDir = normalize(light.position - FragPos);
    float diff = max(dot(norm, lightDir), 0.0);
    vec3 diffuse = light.diffuse * (diff * texture(material.diffuse, TexCoord).rgb);

    vec3 viewDir = normalize(viewPos - FragPos);
    vec3 reflectDir = reflect(-lightDir, norm);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
    vec3 specular = light.specular * spec *  texture(material.diffuse, TexCoord).rgb;
    color = vec4(ambient + diffuse + specular, 1.0);
}
)";
    unsigned int vertex = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertex, 1, &vss, nullptr);
    glCompileShader(vertex);
    unsigned int fragment = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragment, 1, &fss, nullptr);
    glCompileShader(fragment);
    shader = glCreateProgram();
    glAttachShader(shader, vertex);
    glAttachShader(shader, fragment);
    glLinkProgram(shader);
    glValidateProgram(shader);
    glUseProgram(shader);
}
void initialize()
{
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), 0);
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(2);
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
}
void um4(const char* path, glm::mat4 mat)
{
    glUniformMatrix4fv(glGetUniformLocation(shader, path), 1, GL_FALSE, &mat[0][0]);
}
void KeyboardInput(GLFWwindow* window)
{
    float camSpeed = 0.1f;
    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
    {
        camPos += camFront * camSpeed;
    }
    else if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
    {
        camPos -= camFront * camSpeed;
    }
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
    {
        camPos += glm::normalize(glm::cross(camFront, camUp)) * camSpeed;
    }
    else if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
    {
        camPos -= glm::normalize(glm::cross(camFront, camUp)) * camSpeed;
    }
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
    {
        exit(0);
    }
    if (glfwGetKey(window, GLFW_KEY_LEFT_CONTROL) == GLFW_PRESS)
    {
        camPos -= camUp * camSpeed;
    }
    else if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS)
    {
        camPos += camUp * camSpeed;
    }
}

void mouse_callback(GLFWwindow* window, double xPos, double yPos)
{
    if (firstMove)
    {
        lastX = xPos;
        lastY = yPos;
        firstMove = false;
    }
    float xOffset = xPos - lastX;
    float yOffset = lastY - yPos;
    lastX = xPos;
    lastY = yPos;
    float sens = 0.1f;
    xOffset *= sens;
    yOffset *= sens;
    yaw += xOffset;
    pitch += yOffset;
    glm::vec3 front;
    front.x = glm::cos(glm::radians(yaw)) * glm::cos(glm::radians(pitch));
    front.y = glm::sin(glm::radians(pitch));
    front.z = glm::sin(glm::radians(yaw)) * glm::cos(glm::radians(pitch));
    camFront = glm::normalize(front);
}

struct Chunk {
    int x, z;
    bool isVisible;
};
struct Block {
    int x, y, z;
};
std::vector<Block> getBlocks(Chunk& chunk)
{
    std::vector<Block> blocks;
    for (int x = 0; x < CHUNK_SIZE; x++)
    {
        for (int z = 0; z < CHUNK_SIZE; z++)
        {
            float worldX = chunk.x * CHUNK_SIZE + x;
            float worldZ = chunk.z * CHUNK_SIZE + z;
            float baseHeight = noise.GetNoise(worldX * 0.01, worldZ * 0.01) * 5.0f;
            baseHeight = std::round(baseHeight);
            blocks.push_back(Block{ (int)worldX, (int)baseHeight, (int)worldZ });
        }
    }
    return blocks;
}
void applyCollision(Chunk& chunk)
{
    std::vector<Block> blocks = getBlocks(chunk);
    int maxHeight = 0;
    for(Block block : blocks)
    {
        if(block.x == camPos.x && block.z == camPos.z)
        {
            if(block.y + 1 > camPos.y - 1)
            {
                maxHeight = block.y + 1;
            }
        }
    }
    if(maxHeight != 0)
    {
        camPos.y = maxHeight;
    }
}
void generateChunksAroundCamera(std::vector<Chunk>& chunks, const glm::vec3& camPos) {
    // Calculando a área ao redor da câmera
    int startX = (int)(camPos.x - RENDER_DISTANCE) / CHUNK_SIZE;
    int startZ = (int)(camPos.z - RENDER_DISTANCE) / CHUNK_SIZE;
    int endX = (int)(camPos.x + RENDER_DISTANCE) / CHUNK_SIZE;
    int endZ = (int)(camPos.z + RENDER_DISTANCE) / CHUNK_SIZE;

    // Limpa o vetor de chunks antes de preencher novamente
    chunks.clear();

    // Preenche os chunks ao redor da posição da câmera
    for (int x = startX; x <= endX; ++x) {
        for (int z = startZ; z <= endZ; ++z) {
            Chunk chunk;
            chunk.x = x;
            chunk.z = z;
            chunk.isVisible = false;
            chunks.push_back(chunk);
        }
    }
}
bool isChunkVisible(const Chunk& chunk) {
    // Calcula o centro do chunk
    float chunkCenterX = chunk.x * CHUNK_SIZE + CHUNK_SIZE / 2.0f;
    float chunkCenterZ = chunk.z * CHUNK_SIZE + CHUNK_SIZE / 2.0f;

    // Distância da câmera ao centro do chunk (considerando apenas x e z)
    float distance = glm::distance(glm::vec2(camPos.x, camPos.z), glm::vec2(chunkCenterX, chunkCenterZ));

    return distance < RENDER_DISTANCE;
}
void renderTerrain(int chunkX, int chunkZ) {
    // Loop para renderizar os blocos dentro de um chunk
    for (int xi = 0; xi < CHUNK_SIZE; ++xi) {
        for (int zi = 0; zi < CHUNK_SIZE; ++zi) {
            // Calculando as posições locais do terreno dentro do chunk
            float worldX = chunkX * CHUNK_SIZE + xi;
            float worldZ = chunkZ * CHUNK_SIZE + zi;

            // Gera a altura do terreno com base no ruído
            float height = noise.GetNoise(worldX * 0.01f, worldZ * 0.01f) * 5.0f; // Ajuste a escala conforme necessário
            height = std::round(height);

            // Define a posição do bloco no mundo
            mat4 model = glm::translate(mat4(1.0f), glm::vec3(worldX, height, worldZ));
            c_shader->um4("model", model);

            // Renderiza o bloco
            glBindVertexArray(VAO);
            glDrawArrays(GL_TRIANGLES, 0, 36); // Assumindo que você está desenhando blocos com 36 vértices
        }
    }
}
void renderChunk(std::vector<Chunk>& chunks)
{
    for (Chunk& chunk : chunks)
    {
        if (isChunkVisible(chunk))
        {
            chunk.isVisible = true;
            renderTerrain(chunk.x, chunk.z);
            applyCollision(chunk);
        }
        else
        {
            chunk.isVisible = false;
        }
    }
}
void loadTexture()
{
    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_2D, texture);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    int width, height, BPP;
    const void* data = stbi_load("C:/Users/User/Downloads/terra.png", &width, &height, &BPP, 4);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
}
int main()
{
    glfwInit();
    glfwWindowHint(GLFW_DECORATED, GL_FALSE);
    GLFWmonitor* monitor = glfwGetPrimaryMonitor();
    const GLFWvidmode* mode = glfwGetVideoMode(monitor);
    GLFWwindow* window = glfwCreateWindow(1, 1, "teste", nullptr, nullptr);
    glfwMakeContextCurrent(window);
    gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);
    glfwSetWindowPos(window, (mode->width - SCR_WIDTH) / 2, (mode->height - SCR_HEIGHT) / 2);
    glfwSetCursorPosCallback(window, mouse_callback);
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    glfwSetWindowSize(window, SCR_WIDTH, SCR_HEIGHT);
    glfwSetWindowAttrib(window, GLFW_DECORATED, GL_TRUE);
    glEnable(GL_DEPTH_TEST);
    initialize();
    loadTexture();
    c_shader = new Shader("C:/Users/User/Desktop/Projetos/algoritmo mine/Project48/Project48/Program/src/basic.shader");
    c_shader->Bind();
    glm::mat4 model = glm::mat4(1.0f), view, proj;
    c_shader->um4("model", model);
    vec3 lightColor = vec3(1.0, 1.0, 1.0);
    vec3 lightPos = vec3(3.0, 100.0, 0.0);
    c_shader->u3f("light.ambient", lightColor * 0.3f);
    c_shader->u3f("light.position", lightPos);
    c_shader->u3f("light.diffuse", lightColor * 0.5f);
    c_shader->u3f("viewPos", camPos);
    c_shader->u3f("light.specular", lightColor * 0.7f);
    c_shader->u3f("material.specular", lightColor * 0.8f);
    c_shader->u1f("material.shininess", 32);
    noise.SetNoiseType(FastNoise::Perlin);
    noise.SetFrequency(1.4);
    std::vector<Chunk> chunk;
    while (!glfwWindowShouldClose(window))
    {
        glfwGetWindowSize(window, &SCR_WIDTH, &SCR_HEIGHT);
        glViewport(0, 0, SCR_WIDTH, SCR_HEIGHT);
        glClearColor(0.0, 0.4, 1.0, 1.0);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        KeyboardInput(window);
        generateChunksAroundCamera(chunk, camPos);
        view = glm::lookAt(camPos, camPos + camFront, camUp);
        proj = glm::perspective(glm::radians(90.0f), (float)(SCR_WIDTH / SCR_HEIGHT), 0.1f, 100.0f);
        c_shader->um4("view", view);
        c_shader->um4("proj", proj);
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, texture);
        glEnable(GL_CULL_FACE);
        glCullFace(GL_BACK);
        glFrontFace(GL_CW);
        c_shader->u1i("material.diffuse", 0);
        glDrawArrays(GL_TRIANGLES, 0, 36);
        renderChunk(chunk);
        glfwSwapBuffers(window);
        glfwPollEvents();
    }
    return 0;
}