#include "CalculateDirtnessShader.h"
#include "DirtAnalysis/Public/CalculateDirtnessShader/CalculateDirtnessShader.h"
#include "PixelShaderUtils.h"
#include "MeshPassProcessor.inl"
#include "StaticMeshResources.h"
#include "DynamicMeshBuilder.h"
#include "RenderGraphResources.h"
#include "GlobalShader.h"
#include "UnifiedBuffer.h"
#include "CanvasTypes.h"
#include "MeshDrawShaderBindings.h"
#include "RHIGPUReadback.h"
#include "MeshPassUtils.h"
#include "MaterialShader.h"

DECLARE_STATS_GROUP(TEXT("CalculateDirtnessShader"), STATGROUP_CalculateDirtnessShader, STATCAT_Advanced);
DECLARE_CYCLE_STAT(TEXT("CalculateDirtnessShader Execute"), STAT_CalculateDirtnessShader_Execute, STATGROUP_CalculateDirtnessShader);

// This class carries our parameter declarations and acts as the bridge between cpp and HLSL.
class DIRTANALYSIS_API FCalculateDirtnessShader : public FGlobalShader
{
public:

  DECLARE_GLOBAL_SHADER(FCalculateDirtnessShader);
  SHADER_USE_PARAMETER_STRUCT(FCalculateDirtnessShader, FGlobalShader);


  class FCalculateDirtnessShader_Perm_TEST : SHADER_PERMUTATION_INT("TEST", 1);
  using FPermutationDomain = TShaderPermutationDomain<
    FCalculateDirtnessShader_Perm_TEST
  >;

  BEGIN_SHADER_PARAMETER_STRUCT(FParameters, )
    SHADER_PARAMETER_RDG_TEXTURE_SRV(Texture2D<float4>, LiquidRenderTarget)
    SHADER_PARAMETER_RDG_TEXTURE_SRV(Texture2D<float4>, DustRenderTarget)
    SHADER_PARAMETER_RDG_BUFFER_UAV(RWBuffer<int>, Output)
  END_SHADER_PARAMETER_STRUCT()

public:
  static bool ShouldCompilePermutation(const FGlobalShaderPermutationParameters& Parameters)
  {
    const FPermutationDomain PermutationVector(Parameters.PermutationId);

    return true;
  }

  static void ModifyCompilationEnvironment(const FGlobalShaderPermutationParameters& Parameters, FShaderCompilerEnvironment& OutEnvironment)
  {
    FGlobalShader::ModifyCompilationEnvironment(Parameters, OutEnvironment);

    const FPermutationDomain PermutationVector(Parameters.PermutationId);

    /*
    * These defines are used in the thread count section of our shader
    */
    OutEnvironment.SetDefine(TEXT("THREADS_X"), NUM_THREADS_CalculateDirtnessShader_X);
    OutEnvironment.SetDefine(TEXT("THREADS_Y"), NUM_THREADS_CalculateDirtnessShader_Y);
    OutEnvironment.SetDefine(TEXT("THREADS_Z"), NUM_THREADS_CalculateDirtnessShader_Z);

  }
private:
};

// This will tell the engine to create the shader and where the shader entry point is.
//                            ShaderType                            ShaderPath                     Shader function name    Type
IMPLEMENT_GLOBAL_SHADER(FCalculateDirtnessShader, "/DirtAnalysisShaders/CalculateDirtnessShader/CalculateDirtnessShader.usf", "CalculateDirtnessShader", SF_Compute);

void FCalculateDirtnessShaderInterface::DispatchRenderThread(FRHICommandListImmediate& RHICmdList, FCalculateDirtnessShaderDispatchParams Params, TFunction<void(int OutputVal)> AsyncCallback)
{
  FRDGBuilder GraphBuilder(RHICmdList);

  {
    SCOPE_CYCLE_COUNTER(STAT_CalculateDirtnessShader_Execute);
    DECLARE_GPU_STAT(CalculateDirtnessShader)
    RDG_EVENT_SCOPE(GraphBuilder, "CalculateDirtnessShader");
    RDG_GPU_STAT_SCOPE(GraphBuilder, CalculateDirtnessShader);

    typename FCalculateDirtnessShader::FPermutationDomain PermutationVector;

    // Add any static permutation options here
    // PermutationVector.Set<FCalculateDirtnessShader::FMyPermutationName>(12345);

    TShaderMapRef<FCalculateDirtnessShader> ComputeShader(GetGlobalShaderMap(GMaxRHIFeatureLevel), PermutationVector);

    if (ComputeShader.IsValid())
    {
      FCalculateDirtnessShader::FParameters* PassParameters = GraphBuilder.AllocParameters<FCalculateDirtnessShader::FParameters>();

      // INPUT
      FRDGTextureDesc LiquidDesc(FRDGTextureDesc::Create2D(Params.LiquidRenderTarget->GetSizeXY(), PF_B8G8R8A8, FClearValueBinding::White, TexCreate_RenderTargetable | TexCreate_ShaderResource | TexCreate_UAV));
      FRDGTextureRef LiquidTargetTexture = RegisterExternalTexture(GraphBuilder, Params.LiquidRenderTarget->GetRenderTargetTexture(), TEXT("CalculateDirtnessShader_RT"));
      PassParameters->LiquidRenderTarget = GraphBuilder.CreateSRV(LiquidTargetTexture);

      FRDGTextureDesc DustDesc(FRDGTextureDesc::Create2D(Params.DustRenderTarget->GetSizeXY(), PF_B8G8R8A8, FClearValueBinding::White, TexCreate_RenderTargetable | TexCreate_ShaderResource | TexCreate_UAV));
      FRDGTextureRef DustTargetTexture = RegisterExternalTexture(GraphBuilder, Params.DustRenderTarget->GetRenderTargetTexture(), TEXT("CalculateDirtnessShader_RT"));
      PassParameters->DustRenderTarget = GraphBuilder.CreateSRV(DustTargetTexture);

      // OUTPUT
      FRDGBufferRef OutputBuffer = GraphBuilder.CreateBuffer(
        FRDGBufferDesc::CreateBufferDesc(sizeof(int), 1),
        TEXT("OutputBuffer"));

      PassParameters->Output = GraphBuilder.CreateUAV(FRDGBufferUAVDesc(OutputBuffer, PF_R32_SINT));
      AddClearUAVPass(GraphBuilder, PassParameters->Output, 0); // Clears the buffer to 0

      auto GroupCount = FComputeShaderUtils::GetGroupCount(FIntVector(Params.X, Params.Y, Params.Z), FComputeShaderUtils::kGolden2DGroupSize);
      GraphBuilder.AddPass(
        RDG_EVENT_NAME("ExecuteCalculateDirtnessComputeShader"),
        PassParameters,
        ERDGPassFlags::AsyncCompute,
        [&PassParameters, ComputeShader, GroupCount](FRHIComputeCommandList& RHICmdList)
        {
          FComputeShaderUtils::Dispatch(RHICmdList, ComputeShader, *PassParameters, GroupCount);
        });

      FRHIGPUBufferReadback* GPUBufferReadback = new FRHIGPUBufferReadback(TEXT("ExecuteCalculateDirtnessComputeShaderOutput"));
      AddEnqueueCopyPass(GraphBuilder, GPUBufferReadback, OutputBuffer, 0u);

      auto RunnerFunc = [GPUBufferReadback, AsyncCallback](auto&& RunnerFunc) -> void {
        if (GPUBufferReadback->IsReady()) {

          int* Buffer = (int*)GPUBufferReadback->Lock(1);
          int OutVal = Buffer[0];

          GPUBufferReadback->Unlock();

          AsyncTask(ENamedThreads::GameThread, [AsyncCallback, OutVal]() {
            AsyncCallback(OutVal);
            });

          delete GPUBufferReadback;
        }
        else {
          AsyncTask(ENamedThreads::ActualRenderingThread, [RunnerFunc]() {
            RunnerFunc(RunnerFunc);
            });
        }
        };

      AsyncTask(ENamedThreads::ActualRenderingThread, [RunnerFunc]() {
        RunnerFunc(RunnerFunc);
        });
    }
    else
    {
#if WITH_EDITOR
      GEngine->AddOnScreenDebugMessage((uint64)42145125184, 6.f, FColor::Red, FString(TEXT("The compute shader has a problem.")));
#endif
    }
  }
  GraphBuilder.Execute();
}