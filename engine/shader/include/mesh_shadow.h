
highp float directional_light_shadow_attenuation(highp vec2 uv, highp float current_depth, highp float bias)
{
    highp float closest_depth = texture(directional_light_shadow, uv).r + bias;
    return closest_depth >= current_depth ? 1.0f : 0.0f;
}

highp float directional_light_shadow_attenuation_pcf3x3(highp vec2 uv, highp float current_depth, highp float bias)
{
    highp vec2 offsets[9];
    offsets[0] = vec2(-1.0,-1.0f);
    offsets[1] = vec2(-1.0, 0.0f);
    offsets[2] = vec2(-1.0, 1.0f);
    offsets[3] = vec2(0.0, -1.0f);
    offsets[4] = vec2(0.0,  0.0f);
    offsets[5] = vec2(0.0,  1.0f);
    offsets[6] = vec2(1.0, -1.0f);
    offsets[7] = vec2(1.0,  0.0f);
    offsets[8] = vec2(1.0,  1.0f);

    highp float att = 0.0f;
    highp ivec2 shadow_tex_size = textureSize(directional_light_shadow, 0);
    for(int i = 0; i < 9; ++i)
    {
        highp vec2 tex_size = vec2(float(shadow_tex_size.x), float(shadow_tex_size.y));
        att += directional_light_shadow_attenuation(uv + offsets[i] / tex_size, current_depth, bias);
    }

    return att / 9.0f;
}

highp float directional_light_shadow_attenuation_pcf_odd(highp vec2 uv, highp float current_depth, highp float bias, int tileSize)
{
    highp vec2 offsets[81];
    for(int j = 0; j < tileSize; ++j)
    {
        for(int i = 0; i < tileSize; ++i)
        {
            int index = j*tileSize + i;
            highp float x = float(i - (tileSize>>1) );
            highp float y = float(j - (tileSize>>1) );
            offsets[index] = vec2(x,y);
        }
    }

    highp float att = 0.0f;
    highp ivec2 shadow_tex_size = textureSize(directional_light_shadow, 0);
    for(int i = 0; i < tileSize*tileSize; ++i)
    {
        highp vec2 tex_size = vec2(float(shadow_tex_size.x), float(shadow_tex_size.y));
        att += directional_light_shadow_attenuation(uv + offsets[i] / tex_size, current_depth, bias);
    }

    return att / float(tileSize*tileSize);
}

highp float directional_light_shadow_attenuation_pcf_even(highp vec2 uv, highp float current_depth, highp float bias, int tileSize)
{
    highp vec2 offsets[64];
    for(int j = 0; j < tileSize; ++j)
    {
        for(int i = 0; i < tileSize; ++i)
        {
            int index = j*tileSize + i;
            highp float x = float(i - (tileSize>>1) );
            highp float y = float(j - (tileSize>>1) );
            offsets[index] = vec2(x + 0.5f, y + 0.5f);
        }
    }

    highp float att = 0.0f;
    highp ivec2 shadow_tex_size = textureSize(directional_light_shadow, 0);
    for(int i = 0; i < tileSize*tileSize; ++i)
    {
        highp vec2 tex_size = vec2(float(shadow_tex_size.x), float(shadow_tex_size.y));
        att += directional_light_shadow_attenuation(uv + offsets[i] / tex_size, current_depth, bias);
    }

    return att / float(tileSize*tileSize);
}

highp float directional_light_shadow_attenuation_pcf(highp vec2 uv, highp float current_depth, highp float bias, int tileSize)
{
    highp float shadow;
    if (tileSize%2 == 1)
    {
        shadow = directional_light_shadow_attenuation_pcf_odd(uv, current_depth, bias, tileSize);
    }
    else
    {
        shadow = directional_light_shadow_attenuation_pcf_even(uv, current_depth, bias, tileSize);
    }
    
    return shadow;
}
