#version 330 core
out vec4 FragColor;
 
struct Light {
    vec3 position;  
  
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
    vec3 color;
	
    float constant;
    float linear;
    float quadratic;

};

uniform Mat{
	vec4 aAmbient;
	vec4 aDiffuse;
	vec4 aSpecular;
};

in vec3 FragPos;  
in vec3 Normal;  
in vec2 TexCoords;

uniform vec3 viewPos;
uniform Light light;
 
uniform sampler2D texture1;
 
void main()
{    
    
	// ambient
    vec3 ambient = light.ambient * aAmbient.rgb;
  	
    // diffuse 
    vec3 norm = normalize(Normal);
    vec3 lightDir = normalize(light.position - FragPos);
    float diff = max(dot(norm, lightDir), 0.0);
    vec3 diffuse =light.diffuse * diff * aDiffuse.rgb;  
      
    // attenuation
    float distance    = length(light.position - FragPos);
    float attenuation = 1.0 / (light.constant + light.linear * distance + light.quadratic * (distance * distance));    
	// specular
    vec3 viewDir = normalize(viewPos - FragPos);
    vec3 reflectDir = reflect(-lightDir, norm);  
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), 64);
    vec3 specular = light.specular * spec * aSpecular.rgb;  
	
    //ambient  *= attenuation;  
    diffuse   *= attenuation; 
    specular *= attenuation; 
	  
    vec3 result = ambient + diffuse +specular;
    FragColor = vec4(result ,1.0) * texture(texture1,TexCoords) * vec4(light.color,1.0f);
}

