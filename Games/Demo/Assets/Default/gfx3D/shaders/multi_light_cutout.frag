precision mediump float;

struct Light{
	vec3 position;
	vec3 direction;
	vec4 color;
	
	float intensity;
	float cut_off;
	float outer_cut_off;
};

uniform sampler2D uDiffuseTexture;

#ifdef USE_SPECULAR_MAP
uniform sampler2D uSpecularMap;
#else
uniform float uSpecular;
#endif

uniform float uShininess;

uniform Light uDirectionalLights[DIRECTIONAL_LIGHT_COUNT + 1];
uniform Light uPointLights[POINT_LIGHT_COUNT + 1];
uniform Light uSpotLights[SPOT_LIGHT_COUNT + 1];

uniform vec4 uAmbientLight;

varying vec2 vTexCoords;
varying vec3 vNormal;
varying vec3 vFragPosition;
varying vec3 vViewDirection;

vec4 CalcPointLight(Light light, vec4 diffuseColor, vec4 specularColor){
	vec3 lightDirection = normalize(light.position - vFragPosition);
	//attenaution
	float dist = length(light.position - vFragPosition);
	float attenaution = 1.0 / (1.0 + light.intensity * dist + light.intensity * dist * dist);
	//diffuse
	float diffEffect = max(dot(vNormal, lightDirection), 0.0);
	vec4 diffuse = light.color * diffEffect * diffuseColor;
	//specular
	vec3 reflectDirection = reflect(-lightDirection, vNormal);
	float specEffect = pow(max(dot(vViewDirection, reflectDirection), 0.0), uShininess);
	vec4 specular = light.color * specEffect * specularColor;
	
	return (diffuse + specular) * attenaution;
}

vec4 CalcDirectionalLight(Light light, vec4 diffuseColor, vec4 specularColor){
	//diffuse
	vec3 lightDirection = normalize(-light.direction);
	float diffEffect = max(dot(vNormal, lightDirection), 0.0);
	vec4 diffuse = light.color * diffEffect * diffuseColor;
	//specular
	vec3 reflectDirection = reflect(-lightDirection, vNormal);
	float specEffect = pow(max(dot(vViewDirection, reflectDirection), 0.0), uShininess);
	vec4 specular = light.color * specEffect * specularColor;
	return (diffuse + specular);
}

vec4 CalcSpotLight(Light light, vec4 diffuseColor, vec4 specularColor){
	vec3 lightDirection = normalize(light.position - vFragPosition);
	
	float theta = dot(lightDirection, normalize(-light.direction));
	float epsilon = light.cut_off - light.outer_cut_off;
	float intensity = clamp((theta - light.outer_cut_off) / epsilon, 0.0, 1.0);

	float dist = length(light.position - vFragPosition);
	float attenaution = 1.0 / (1.0 + light.intensity * dist + light.intensity * dist * dist);
	//diffuse
	float diffEffect = max(dot(vNormal, lightDirection), 0.0);
	vec4 diffuse = light.color * diffEffect * diffuseColor;
	//specular
	vec3 reflectDirection = reflect(-lightDirection, vNormal);
	float specEffect = pow(max(dot(vViewDirection, reflectDirection), 0.0), uShininess);
	vec4 specular = light.color * specEffect * specularColor;
	
	return (diffuse + specular) * attenaution * intensity;
}

void main(){
	vec4 diffuseColor = texture2D(uDiffuseTexture, vTexCoords);
	
	if(diffuseColor.a < 0.1){
		discard;
	}
#ifdef USE_SPECULAR_MAP
	vec4 specularColor = texture2D(uSpecularMap, vTexCoords);
#else
	vec4 specularColor = vec4(uSpecular);
#endif
	
	vec4 result = diffuseColor * uAmbientLight;
	
	for(int i = 0; i < DIRECTIONAL_LIGHT_COUNT; i++){
		result += CalcDirectionalLight(uDirectionalLights[i], diffuseColor, specularColor);
	}
	
	for(int i = 0; i < POINT_LIGHT_COUNT; i++){
		result += CalcPointLight(uPointLights[i], diffuseColor, specularColor);
	}
	
	for(int i = 0; i < SPOT_LIGHT_COUNT; i++){
		result += CalcSpotLight(uSpotLights[i], diffuseColor, specularColor);
	}
	
	gl_FragColor = result;
}