// fPhong.glsl
// Fragmet shader for per-pixel Phong shading.

varying vec3 N;
//varying vec3 L;
varying vec3 p;
uniform sampler2D texMap;
uniform int decalMode;

void main()
{
    
    vec4 texColor = texture2D(texMap, gl_TexCoord[0].st);
    
    vec3 L = normalize(gl_LightSource[0].position.xyz - p);
    vec3 V = normalize(-p); // we are in Eye Coordinates, so EyePos is (0,0,0)
    vec3 R = normalize(-reflect(L,N));
    
    //calculate Ambient Term:
    vec4 Iamb = gl_FrontLightProduct[0].ambient;
    
    //calculate Diffuse Term:
    vec4 Idiff = gl_FrontLightProduct[0].diffuse * max(dot(N,L), 0.0);
    
    // calculate Specular Term:
    vec4 Ispec = gl_FrontLightProduct[0].specular
    * pow(max(dot(R,V),0.0),0.3*gl_FrontMaterial.shininess);
    
    // write Total Color:
    // gl_FrontLightModelProduct.sceneColor = emissive material property for front facing surfaces
    
    //float fogFactor = 1.0-(length(p)/50.0);
    
    gl_FragColor = texColor * (gl_FrontLightModelProduct.sceneColor + Iamb + Idiff);
   // gl_FragColor.g *= clamp(fogFactor*1.3, 0.0, 1.0);
    //gl_FragColor.b *= clamp(fogFactor, 0.0, 1.0);
    //gl_FragColor.a *= clamp(fogFactor, 0.0, 1.0);
    
    float dist = sqrt(pow(p.x,2.0) + pow(p.y,2.0));
    float maxDist = (1.0-1.0/abs(p.z));
    if(dist < maxDist)
    {
        gl_FragColor.r = clamp(gl_FragColor.r + 0.2, 0.0, 1.0);
        gl_FragColor.g = clamp(gl_FragColor.g + 0.2, 0.0, 1.0);
        gl_FragColor.b = clamp(gl_FragColor.b + 0.12, 0.0, 1.0);
    }
    
    if(decalMode == 1)
    {
        gl_FragColor = texColor;
    }
}
