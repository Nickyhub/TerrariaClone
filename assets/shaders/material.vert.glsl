#version 450 core
#extension GL_EXT_scalar_block_layout : enable

out vec2 TexCoord;

struct Transform {
    vec2 pos;
    vec2 atlasPos;
    vec2 tileSize;

    float scale;
};

layout(std430, binding = 0) buffer TransformSBO
{
    Transform transforms[100];
};

uniform mat4 ortho;

void main()
{
    Transform t = transforms[gl_InstanceID];

    vec2 positions[6] = vec2[6](
        vec2(t.pos.x,  t.pos.y + t.tileSize.y * t.scale), // bottom left
        vec2(t.pos.x + t.tileSize.x* t.scale,  t.pos.y + t.tileSize.y* t.scale), // bottom right
        vec2(t.pos.x + t.tileSize.x* t.scale,  t.pos.y), // top right

        vec2(t.pos.x,  t.pos.y + t.tileSize.y* t.scale), // bottom left
        vec2(t.pos.x + t.tileSize.x* t.scale,  t.pos.y), // top right
        vec2(t.pos.x,  t.pos.y)  // top left
    );

    vec2 texCoords[6] = vec2[6](
        vec2(t.atlasPos.x, t.atlasPos.y + t.tileSize.y), // bottom left
        vec2(t.atlasPos.x + t.tileSize.x, t.atlasPos.y + t.tileSize.y), // bottom right
        vec2(t.atlasPos.x + t.tileSize.x, t.atlasPos.y), // top right

        vec2(t.atlasPos.x, t.atlasPos.y + t.tileSize.y), // bottom left
        vec2(t.atlasPos.x + t.tileSize.x, t.atlasPos.y),
        vec2(t.atlasPos.x, t.atlasPos.y)  // top left
    );

    vec2 pos = positions[gl_VertexID];

    gl_Position = ortho * vec4(pos, 0.0, 1.0);

    TexCoord = texCoords[gl_VertexID];
}