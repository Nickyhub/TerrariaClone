#version 400 core

out vec2 TexCoord;

void main()
{
    vec2 positions[6] = vec2[6](
        vec2(-1.0, -1.0), // bottom left
        vec2( 1.0, -1.0), // bottom right
        vec2( 1.0,  1.0), // top right

        vec2(-1.0, -1.0), // bottom left
        vec2( 1.0,  1.0), // top right
        vec2(-1.0,  1.0)  // top left
    );

    gl_Position = vec4(positions[gl_VertexID], 0.0, 1.0);
    TexCoord = positions[gl_VertexID] * 0.5 + 0.5;
}