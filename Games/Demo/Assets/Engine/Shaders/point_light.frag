precision mediump float;

struct PointLight{
	vec3 position;
	
	vec3 ambient;
	vec3 diffuse;
	vec3 specular;
	
	float constant;
	float linear;
	float quadratic;
};

uniform sampler2D uDiffuseMap0;
uniform sampler2D uSpecularMap0;
uniform float uMaterialShininess;

uniform PointLight uLight;

uniform vec3 uCameraPosition;

varying vec2 vTexCoords;
varying vec3 vNormal;
varying vec3 vFragPosition;

void main() {
	//attenaution
	float dist = length(uLight.position - vFragPosition);
	float attenaution = 1.0 / (uLight.constant + uLight.linear * dist + uLight.quadratic * dist * dist);
	//ambient
	vec3 ambient = uLight.ambient * vec3(texture2D(uDiffuseMap0, vTexCoords));
	ambient *= attenaution;
	//diffuse
	vec3 normal = normalize(vNormal);
	vec3 lightDirection = normalize(uLight.position - vFragPosition);
	float diffEffect = max(dot(normal, lightDirection), 0.0);
	vec3 diffuse = uLight.diffuse * diffEffect * vec3(texture2D(uDiffuseMap0, vTexCoords));
	diffuse *= attenaution;
	//specular
	vec3 viewDirection = normalize(uCameraPosition - vFragPosition);
	vec3 reflectDirection = reflect(-lightDirection, normal);
	float specEffect = pow(max(dot(viewDirection, reflectDirection), 0.0), uMaterialShininess);
	vec3 specular = uLight.specular * specEffect * vec3(texture2D(uSpecularMap0, vTexCoords));
	specular *= attenaution;
	
	vec3 result = ambient + diffuse + specular;
	gl_FragColor = vec4(result, 1.0);
} 