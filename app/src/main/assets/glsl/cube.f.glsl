#version 300 es
precision mediump float;
out vec4 FragColor;

struct Material {
    sampler2D diffuse;
    vec3 specular;     // 镜面光分量
    float shininess;
};

struct Light {
    vec3 position;

    vec3 ambient;
    vec3 diffuse;
    vec3 specular;

    float constant;     // 常数项
    float linear;       // 一次项
    float quadratic;    // 二次项
};

// in vec3 vertexColor;
in vec2 TexCoords;
in vec3 Normal;
in vec3 FragPos;

uniform vec3 viewPos;
uniform Material material;
uniform Light light;
uniform bool isLight;

void main()
{
    // ambient 环境光
    // vec3 ambient = light.ambient * texture(material.diffuse, TexCoords).rgb;
    vec3 ambient  = light.ambient  * vec3(texture(material.diffuse, TexCoords));

    // diffuse 漫反射
    vec3 norm = normalize(Normal);      // ambient light
    vec3 lightDir = normalize(light.position - FragPos);    // light direction
    float diff = max(dot(norm, lightDir), 0.0);     // avoid light value negative
    vec3 diffuse = light.diffuse * diff * vec3(texture(material.diffuse, TexCoords));

    // specular 物体反射
    vec3 viewDir = normalize(viewPos - FragPos);
    vec3 reflectDir = reflect(-lightDir, norm);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
    vec3 material_specular = vec3(0.2, 0.2, 0.2);
    vec3 specular = light.specular * (spec * material_specular);

    float distance = length(light.position - FragPos);
    float attenuation = 2.0 / (light.constant + light.linear * distance +
                light.quadratic * (distance * distance));

    ambient *= attenuation;
    diffuse *= attenuation;
    specular *= attenuation;
    // 环境光+物体的反光+边框的反光
    if (!isLight)
        FragColor = vec4(ambient + diffuse + specular, 1.0);
    else
        FragColor = vec4(texture(material.diffuse, TexCoords));
}
