###VERTEX

#version 130

// Inputs
in vec2 inGenericTexturePosition;
in vec2 inGenericTextureCoordinates;
in float inGenericTextureAmbientLightSensitivity;

// Outputs
out vec2 vertexTextureCoords;
out float vertexAmbientLightSensitivity;

// Params
uniform mat4 paramOrthoMatrix;

void main()
{
    vertexTextureCoords = inGenericTextureCoordinates; 
    vertexAmbientLightSensitivity = inGenericTextureAmbientLightSensitivity;
    gl_Position = paramOrthoMatrix * vec4(inGenericTexturePosition.xy, -1.0, 1.0);
}

###FRAGMENT

#version 130

// Inputs from previous shader
in vec2 vertexTextureCoords;
in float vertexAmbientLightSensitivity;

// The texture
uniform sampler2D inputTexture;

// Parameters        
uniform float paramAmbientLightIntensity;

void main()
{
    vec4 textureColor = texture2D(inputTexture, vertexTextureCoords);

    float ambientLightIntensity = 
	(1.0 - vertexAmbientLightSensitivity)
	+ vertexAmbientLightSensitivity * paramAmbientLightIntensity;

    gl_FragColor = vec4(
	textureColor.xyz * ambientLightIntensity,
	textureColor.w);
} 
