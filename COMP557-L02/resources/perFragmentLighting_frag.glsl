//Rudolf C. Kischer
//260956107
#version 410 core
uniform vec3 col;
uniform vec3 ks;
uniform vec3 lightPos;
uniform float shininess;
uniform vec3 lightColor;
uniform bool enableLighting;

uniform sampler2D depthMap;
uniform float sigma;

in vec3 positionForFP;
in vec3 normalForFP;
in vec4 positionLightCVV;

out vec4 fragColor;



mat4 windowingTransform(){
    mat4 windowTransform = mat4(
        0.5, 0.0, 0.0, 0.5,  // 1. column
        0.0, 0.5, 0.0, 0.5,  // 2. column
        0.0, 0.0, 0.5, 0.5,  // 3. column
        0.0, 0.0, 0.0, 1.0); // 4. column
    
    return transpose(windowTransform);
}

void main(void) {
	vec3 kd = vec3( col.x/2.0, col.y/2.0, col.z/2.0 );
	float shininess = 32.0;
	vec3 rgb = kd;

	if ( enableLighting ) {
		vec3 lightDirection = normalize( lightPos - positionForFP );
		float diffuse = max( dot( normalForFP, lightDirection), 0 );
		vec3 viewDirection = normalize( - positionForFP );
		vec3 halfVector = normalize( lightDirection + viewDirection );
		float specular = max( 0, dot( normalForFP, halfVector ) );
		if (diffuse == 0.0) {
		    specular = 0.0;
		} else {
	   		specular = pow( specular, shininess );
		}
		vec3 scatteredLight = kd * lightColor * diffuse;
		vec3 reflectedLight = ks * lightColor * specular;
		vec3 ambientLight = kd * vec3( 0.1, 0.1, 0.1 );
        
        
        // TODO: compute shadows
        
        
                          
        mat4 posToMap = windowingTransform();
        vec4 positionLightCVV = positionLightCVV.xyzw / positionLightCVV.w;
        vec3 pos = vec3(posToMap * positionLightCVV);
        
	    float shadow = 1.0;
        
        float distanceFromLight = texture( depthMap, pos.xy ).r * 2.0 - 1.0;
        
        if ( distanceFromLight < positionLightCVV.z - sigma ){
            shadow = 0.0 ;
        }
        
	    rgb = min( shadow * (scatteredLight + reflectedLight) + ambientLight, vec3(1,1,1) );
 	} 
	fragColor = vec4( rgb ,1 );
}
