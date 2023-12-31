#include "Object3d.hlsli"

float32_t4 main(VertexShaderOutput input) : SV_TARGET {
    // インデックスを抽出
    uint32_t m = gIndex[input.id].material;
    uint32_t t = gIndex[input.id].tex2d;


    // 最終的な結果
    float32_t4 output;

    if (gMaterial[m].enableLighting != 0) { // Lightingの計算を行う
        // -- テクスチャ -- //
        // uvTransform
        float4 transformUV = mul(float32_t4(input.texcoord, 0.0f, 1.0f), gMaterial[m].uvTransform);
        // テクスチャからサンプリング
        float4 texColor = gTexture[t].Sample(gSampler, transformUV.xy);


        // 必要な変数を宣言
        float3 lighting = { 0.0f,0.0f,0.0f }; // 最終的なライティングの値

        {   // -- 平行光源 -- //
            // ライティング
            float NdotL = dot(normalize(input.normal), -gDirectionalLight.direction);
            float cos = pow(NdotL * 0.5f + 0.5f, 2.0f);
            float3 light = gDirectionalLight.color.rgb * cos * gDirectionalLight.intensity;
            // シャドウマッピング
            float depth = gDirectionShadowMap[0].Sample(gDirectionShadowMapSampler, input.posSM.xy);
            float sma = (input.posSM.z - 0.00000075f < depth) ? 1.0f : kShadowDensity;
            // 結果を加算
            lighting += light * sma;
        }

        // -- 点光源 -- //

        const float far = 100.0f;  // パースペクティブのfar
        const float near = 0.01f;  // パースペクティブのnear
        for (uint n = 0; n < gStructCount.pointLight; n++) {
            // ライティング
            float3 dir = input.WorldPos - gPointLight[n].position;
            float NdotL = dot(normalize(input.normal), -normalize(dir));
            float cos = NdotL <= 0 ? 0.0f : 1.0f;
            //float cos = pow(NdotL * 0.5f + 0.5f, 2.0f);
            float distance = length(gPointLight[n].position - input.WorldPos);   // ポイントライトへの距離
            float factor = pow(saturate(-distance / gPointLight[n].radius + 1.0), gPointLight[n].decay);    // 逆二乗則による減衰係数
            float3 light = gPointLight[n].color.rgb * cos * gPointLight[n].intensity * factor;
            // シャドウマッピング
            float depth = gPointShadowMap[n].Sample(gPointShadowMapSampler, normalize(dir)).x;
            float3 absVec = abs(dir);
            float z = max(absVec.x, max(absVec.y, absVec.z));
            float normZComp = far / (far - near) - (far * near) / (far - near) / z;
            float sma = (normZComp - 0.00000075f < depth)  ? 1.0f : kShadowDensity;
            // 結果を加算
            lighting += light * sma;
        }
        

        output.rgb = input.color.rgb * texColor.rgb * lighting;
        output.w = input.color.a * texColor.a;   // 透明度を保持
    }
    else { // Lightingの計算を行わない
        float4 transformUV = mul(float32_t4(input.texcoord,0.0f,1.0f), gMaterial[m].uvTransform);
        output = input.color * gTexture[t].Sample(gSampler, transformUV.xy);
    }
    return output;
}