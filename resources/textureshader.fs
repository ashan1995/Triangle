#version 330 core

//in vec3 ourColor;
in vec2 TexCoord;
in vec3 Normal;  
in vec3 FragPos;  

out vec4 FragColor;

uniform sampler2D ourTexture;
uniform vec3 lightColor;
uniform vec3 lightPos;  
uniform vec3 viewPos;


void main()
{
    float ambientStrength = 0.1;
    float specularStrength = 1.0;

    vec3 ambient = ambientStrength * lightColor;

    vec4 textureColor = texture(ourTexture, TexCoord);


    vec3 norm = normalize(Normal);
    vec3 lightDir = normalize(lightPos - FragPos); 

    float diff = max(dot(norm, lightDir), 0.0);
    vec3 diffuse = diff * lightColor;

    vec3 viewDir = normalize(viewPos - FragPos);
    vec3 reflectDir = reflect(-lightDir, norm);  

    float spec = pow(max(dot(viewDir, reflectDir), 0.0), 256);
    vec3 specular = specularStrength * spec * lightColor;  

    vec3 result = (ambient + diffuse + specular) * textureColor.rgb;
    FragColor = vec4(result, 1.0);

//    FragColor =mix(texture(ourTexture, TexCoord),vec4(ourColor,1),0.5);
    FragColor =vec4(result, 1.0);
}
