#pragma once

#include "CoreMinimal.h"
#include "GenericPlatform/GenericPlatformMisc.h"
#include "Kismet/BlueprintAsyncActionBase.h"
#include "Engine/TextureRenderTarget2D.h"
#include "Materials/MaterialRenderProxy.h"

#include "CalculateDirtnessShader.generated.h"

struct DIRTANALYSIS_API FCalculateDirtnessShaderDispatchParams
{
	int X;
	int Y;
	int Z;

	FRenderTarget* LiquidRenderTarget;
	FRenderTarget* DustRenderTarget;

	int Output;

	FCalculateDirtnessShaderDispatchParams(int x, int y, int z)
		: X(x)
		, Y(y)
		, Z(z)
	{
	}
};

// This is a public interface that we define so outside code can invoke our compute shader.
class DIRTANALYSIS_API FCalculateDirtnessShaderInterface 
{
public:
	// Executes this shader on the render thread
	static void DispatchRenderThread(
		FRHICommandListImmediate& RHICmdList,
		FCalculateDirtnessShaderDispatchParams Params,
		TFunction<void(int OutputVal)> AsyncCallback
	);

	// Executes this shader on the render thread from the game thread via EnqueueRenderThreadCommand
	static void DispatchGameThread(
		FCalculateDirtnessShaderDispatchParams Params,
		TFunction<void(int OutputVal)> AsyncCallback)
	{
		ENQUEUE_RENDER_COMMAND(SceneDrawCompletion)(
		[Params, AsyncCallback](FRHICommandListImmediate& RHICmdList)
		{
			DispatchRenderThread(RHICmdList, Params, AsyncCallback);
		});
	}

	// Dispatches this shader. Can be called from any thread
	static void Dispatch(
		FCalculateDirtnessShaderDispatchParams Params,
		TFunction<void(int OutputVal)> AsyncCallback
	)
	{
		if (IsInRenderingThread()) {
			DispatchRenderThread(GetImmediateCommandList_ForRenderCommand(), Params, AsyncCallback);
		}else{
			DispatchGameThread(Params, AsyncCallback);
		}
	}
};

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FCalculateDirtnessShaderLibrary_AsyncExecutionCompleted, const int, Value);


// This is a static blueprint library that can be used to invoke our compute shader from blueprints.
UCLASS()
class DIRTANALYSIS_API UCalculateDirtnessShaderLibrary : public UBlueprintAsyncActionBase
{
	GENERATED_BODY()
	
public:

	virtual void Activate() override 
	{
		FCalculateDirtnessShaderDispatchParams Params(LiquidRT->SizeX, LiquidRT->SizeY, 1);
		Params.LiquidRenderTarget = LiquidRT->GameThread_GetRenderTargetResource();
		Params.DustRenderTarget = DustRT->GameThread_GetRenderTargetResource();
		FCalculateDirtnessShaderInterface::Dispatch(Params,
			[this](int OutputVal)
			{
				const int TotalPixels = LiquidRT->SizeX * LiquidRT->SizeY;
				const float DirtinessPercentage = (TotalPixels > 0) ? (static_cast<float>(OutputVal) / static_cast<float>(TotalPixels)) * 100.0f : 0.0f;
				this->Completed.Broadcast(DirtinessPercentage);
			});
	}

	UFUNCTION(BlueprintCallable, meta = (BlueprintInternalUseOnly = "true", Category = "ComputeShader", WorldContext = "WorldContextObject"))
	static UCalculateDirtnessShaderLibrary* ExecuteRTComputeShader(UObject* WorldContextObject, UTextureRenderTarget2D* LiquidRT, UTextureRenderTarget2D* DustRT)
	{
		if (!LiquidRT)
		{
			return nullptr;
		}
		UCalculateDirtnessShaderLibrary* Action = NewObject<UCalculateDirtnessShaderLibrary>();
		Action->LiquidRT = LiquidRT;
		Action->DustRT = DustRT;
		Action->RegisterWithGameInstance(WorldContextObject);
		return Action;
	}

	UPROPERTY(BlueprintAssignable)
	FCalculateDirtnessShaderLibrary_AsyncExecutionCompleted Completed;

	UTextureRenderTarget2D* LiquidRT;
	UTextureRenderTarget2D* DustRT;
	
};
