#include "glsl_validate.hpp"
#include <glslang/Public/ShaderLang.h>
#include <glslang/Public/ResourceLimits.h>
#include <memory>

namespace node_glslang {

const TBuiltInResource DefaultTBuiltInResource = {
    /* .MaxLights = */ 32,
    /* .MaxClipPlanes = */ 6,
    /* .MaxTextureUnits = */ 32,
    /* .MaxTextureCoords = */ 32,
    /* .MaxVertexAttribs = */ 64,
    /* .MaxVertexUniformComponents = */ 4096,
    /* .MaxVaryingFloats = */ 64,
    /* .MaxVertexTextureImageUnits = */ 32,
    /* .MaxCombinedTextureImageUnits = */ 80,
    /* .MaxTextureImageUnits = */ 32,
    /* .MaxFragmentUniformComponents = */ 4096,
    /* .MaxDrawBuffers = */ 32,
    /* .MaxVertexUniformVectors = */ 128,
    /* .MaxVaryingVectors = */ 8,
    /* .MaxFragmentUniformVectors = */ 16,
    /* .MaxVertexOutputVectors = */ 16,
    /* .MaxFragmentInputVectors = */ 15,
    /* .MinProgramTexelOffset = */ -8,
    /* .MaxProgramTexelOffset = */ 7,
    /* .MaxClipDistances = */ 8,
    /* .MaxComputeWorkGroupCountX = */ 65535,
    /* .MaxComputeWorkGroupCountY = */ 65535,
    /* .MaxComputeWorkGroupCountZ = */ 65535,
    /* .MaxComputeWorkGroupSizeX = */ 1024,
    /* .MaxComputeWorkGroupSizeY = */ 1024,
    /* .MaxComputeWorkGroupSizeZ = */ 64,
    /* .MaxComputeUniformComponents = */ 1024,
    /* .MaxComputeTextureImageUnits = */ 16,
    /* .MaxComputeImageUniforms = */ 8,
    /* .MaxComputeAtomicCounters = */ 8,
    /* .MaxComputeAtomicCounterBuffers = */ 1,
    /* .MaxVaryingComponents = */ 60,
    /* .MaxVertexOutputComponents = */ 64,
    /* .MaxGeometryInputComponents = */ 64,
    /* .MaxGeometryOutputComponents = */ 128,
    /* .MaxFragmentInputComponents = */ 128,
    /* .MaxImageUnits = */ 8,
    /* .MaxCombinedImageUnitsAndFragmentOutputs = */ 8,
    /* .MaxCombinedShaderOutputResources = */ 8,
    /* .MaxImageSamples = */ 0,
    /* .MaxVertexImageUniforms = */ 0,
    /* .MaxTessControlImageUniforms = */ 0,
    /* .MaxTessEvaluationImageUniforms = */ 0,
    /* .MaxGeometryImageUniforms = */ 0,
    /* .MaxFragmentImageUniforms = */ 8,
    /* .MaxCombinedImageUniforms = */ 8,
    /* .MaxGeometryTextureImageUnits = */ 16,
    /* .MaxGeometryOutputVertices = */ 256,
    /* .MaxGeometryTotalOutputComponents = */ 1024,
    /* .MaxGeometryUniformComponents = */ 1024,
    /* .MaxGeometryVaryingComponents = */ 64,
    /* .MaxTessControlInputComponents = */ 128,
    /* .MaxTessControlOutputComponents = */ 128,
    /* .MaxTessControlTextureImageUnits = */ 16,
    /* .MaxTessControlUniformComponents = */ 1024,
    /* .MaxTessControlTotalOutputComponents = */ 4096,
    /* .MaxTessEvaluationInputComponents = */ 128,
    /* .MaxTessEvaluationOutputComponents = */ 128,
    /* .MaxTessEvaluationTextureImageUnits = */ 16,
    /* .MaxTessEvaluationUniformComponents = */ 1024,
    /* .MaxTessPatchComponents = */ 120,
    /* .MaxPatchVertices = */ 32,
    /* .MaxTessGenLevel = */ 64,
    /* .MaxViewports = */ 16,
    /* .MaxVertexAtomicCounters = */ 0,
    /* .MaxTessControlAtomicCounters = */ 0,
    /* .MaxTessEvaluationAtomicCounters = */ 0,
    /* .MaxGeometryAtomicCounters = */ 0,
    /* .MaxFragmentAtomicCounters = */ 8,
    /* .MaxCombinedAtomicCounters = */ 8,
    /* .MaxAtomicCounterBindings = */ 1,
    /* .MaxVertexAtomicCounterBuffers = */ 0,
    /* .MaxTessControlAtomicCounterBuffers = */ 0,
    /* .MaxTessEvaluationAtomicCounterBuffers = */ 0,
    /* .MaxGeometryAtomicCounterBuffers = */ 0,
    /* .MaxFragmentAtomicCounterBuffers = */ 1,
    /* .MaxCombinedAtomicCounterBuffers = */ 1,
    /* .MaxAtomicCounterBufferSize = */ 16384,
    /* .MaxTransformFeedbackBuffers = */ 4,
    /* .MaxTransformFeedbackInterleavedComponents = */ 64,
    /* .MaxCullDistances = */ 8,
    /* .MaxCombinedClipAndCullDistances = */ 8,
    /* .MaxSamples = */ 4,
    /* .maxMeshOutputVerticesNV = */ 256,
    /* .maxMeshOutputPrimitivesNV = */ 512,
    /* .maxMeshWorkGroupSizeX_NV = */ 32,
    /* .maxMeshWorkGroupSizeY_NV = */ 1,
    /* .maxMeshWorkGroupSizeZ_NV = */ 1,
    /* .maxTaskWorkGroupSizeX_NV = */ 32,
    /* .maxTaskWorkGroupSizeY_NV = */ 1,
    /* .maxTaskWorkGroupSizeZ_NV = */ 1,
    /* .maxMeshViewCountNV = */ 4,
    /* .maxMeshOutputVerticesEXT = */ 256,
    /* .maxMeshOutputPrimitivesEXT = */ 256,
    /* .maxMeshWorkGroupSizeX_EXT = */ 128,
    /* .maxMeshWorkGroupSizeY_EXT = */ 128,
    /* .maxMeshWorkGroupSizeZ_EXT = */ 128,
    /* .maxTaskWorkGroupSizeX_EXT = */ 128,
    /* .maxTaskWorkGroupSizeY_EXT = */ 128,
    /* .maxTaskWorkGroupSizeZ_EXT = */ 128,
    /* .maxMeshViewCountEXT = */ 4,
    /* .maxDualSourceDrawBuffersEXT = */ 1,

    /* .limits = */ {
        /* .nonInductiveForLoops = */ 1,
        /* .whileLoops = */ 1,
        /* .doWhileLoops = */ 1,
        /* .generalUniformIndexing = */ 1,
        /* .generalAttributeMatrixVectorIndexing = */ 1,
        /* .generalVaryingIndexing = */ 1,
        /* .generalSamplerIndexing = */ 1,
        /* .generalVariableIndexing = */ 1,
        /* .generalConstantMatrixVectorIndexing = */ 1,
    }};

bool init() {
    return glslang::InitializeProcess();
}

Napi::Value compiles(Napi::CallbackInfo const& info) {
    Napi::Env env = info.Env();

    if (info.Length() != 3) {
        Napi::TypeError::New(env, "wrong number of arguments").ThrowAsJavaScriptException();
        return env.Undefined();
    }

    if (!info[0].IsString()) {
        Napi::TypeError::New(env, "invalid argument 1, expected 'string'").ThrowAsJavaScriptException();
        return env.Undefined();
    }

    if (!info[1].IsNumber()) {
        Napi::TypeError::New(env, "invalid argument 2, expected 'number'").ThrowAsJavaScriptException();
        return env.Undefined();
    }

    if (!info[2].IsString()) {
        Napi::TypeError::New(env, "invalid argument 3, expected 'string'").ThrowAsJavaScriptException();
        return env.Undefined();
    }

    const auto code = info[0].As<Napi::String>().Utf8Value();
    const auto version = info[1].As<Napi::Number>().Uint32Value();
    const auto stage = info[2].As<Napi::String>().Utf8Value();
    const auto lang = stage == "vert" ?
        EShLanguage::EShLangVertex : EShLanguage::EShLangFragment;

    const std::array<const char*, 1> strings = {code.c_str()};
    auto shader = std::make_unique<glslang::TShader>(lang);
    shader->setStrings(strings.cbegin(), 1);
    shader->setEnvInput(glslang::EShSourceGlsl, lang, glslang::EShClient::EShClientNone, 0);
    shader->setEnvTarget(glslang::EShTargetNone, static_cast<glslang::EShTargetLanguageVersion>(0));
    
    // These limits are set for WebGL 1.0 minimums, per spec:
    // https://www.khronos.org/files/opengles_shading_language.pdf
    /*
    const mediump int gl_MaxVertexAttribs = 8;
    const mediump int gl_MaxVertexUniformVectors = 128;
    const mediump int gl_MaxVaryingVectors = 8;
    const mediump int gl_MaxVertexTextureImageUnits = 0;
    const mediump int gl_MaxCombinedTextureImageUnits = 8;
    const mediump int gl_MaxTextureImageUnits = 8;
    const mediump int gl_MaxFragmentUniformVectors = 16;
    const mediump int gl_MaxDrawBuffers = 1;
    */

    TBuiltInResource builtInResources = DefaultTBuiltInResource;
    builtInResources.maxTextureImageUnits = 8;
    builtInResources.maxFragmentUniformVectors = 16;
    builtInResources.maxVaryingVectors = 8;
    builtInResources.maxCombinedTextureImageUnits = 8;
    builtInResources.maxVertexAttribs = 8;
    builtInResources.maxVertexTextureImageUnits = 0;
    builtInResources.maxVertexUniformVectors = 128;
    builtInResources.maxDrawBuffers = 1;

    Napi::Object obj = Napi::Object::New(env);
    obj.Set(Napi::String::New(env, "compiled"),
        shader->parse(&builtInResources, version, false, EShMessages::EShMsgDefault));
    obj.Set(Napi::String::New(env, "log"), shader->getInfoLog());

    return obj;
}

}