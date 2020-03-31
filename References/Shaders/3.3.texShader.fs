#version 330 core

uniform vec3 lightPosition;
uniform vec3 viewPosition;
uniform sampler2D textureSampler;

in vec3 vertexColor;
in vec2 vertexUV;
in vec3 Normal;
in vec3 FragPos;

out vec4 FragColor;
		
void main() {
	vec4 textureColor = texture(textureSampler, vertexUV);
		
// Add Ambient light, part 1/3 for Phong Point Lighting
	vec3 lightColor = vec3(1.0f, 1.0f, 1.0f);
	float ambientFactor = 0.24;
	vec3 ambientLight = ambientFactor * lightColor;
		
// Add Diffuse lighting, part 2/3 for Phong Point Lighting
	vec3 norm = normalize(Normal); // We want direction only so normalize
	vec3 lightDirection = normalize(lightPosition - FragPos); 
	float diff = max(dot(norm, lightDirection), 0.0); // Non negative, so max
	vec3 diffusedLight = diff * lightColor; // Diffuse impact
		
// Add Specular lighting, part 3/3 for Phong Point Lighting
	float specularStrength = 0.5;
	vec3 viewDirection = normalize(viewPosition - FragPos);
	vec3 reflectDirection = reflect(-lightDirection, norm);
	float spec = pow(max(dot(viewDirection, reflectDirection), 0.0), 24);
	vec3 specularLight = specularStrength * spec * lightColor;
		
// Result of Phong Point Lighting: 
	vec3 result = (ambientLight + diffusedLight + specularLight) * vec3(textureColor.r, textureColor.g, textureColor.b);
	FragColor = vec4(result, textureColor.g);
};