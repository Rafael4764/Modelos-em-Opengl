#shader vertex
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
#shader fragment
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