###VERTEX

#version 120

#define in attribute
#define out varying

// Inputs
in vec2 inLand; // Position

// Parameters
uniform mat4 paramOrthoMatrix;

// Outputs
out vec2 texturePos;

void main()
{
    gl_Position = paramOrthoMatrix * vec4(inLand.xy, -1.0, 1.0);
    texturePos = inLand.xy;
}

###FRAGMENT

#version 120

#define in varying

// Inputs from previous shader
in vec2 texturePos;

// The texture
uniform sampler2D paramLandTexture;

// Parameters        
uniform float paramAmbientLightIntensity;
uniform vec2 paramTextureScaling;

void main()
{
    vec4 textureColor = texture2D(paramLandTexture, texturePos * paramTextureScaling);
    gl_FragColor = vec4(textureColor.xyz * paramAmbientLightIntensity, 1.0);
} 
