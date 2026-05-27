// Fill out your copyright notice in the Description page of Project Settings.


#include "BaseGameMode.h"
#include "DorothysJob/Controller/BasePlayerController.h"

ABaseGameMode::ABaseGameMode()
{
	// Set default pawn class to our blueprinted player
	static ConstructorHelpers::FClassFinder<APawn> oPlayerPawn(TEXT("/Game/Characters/Player/BP_Player"));
	if (oPlayerPawn.Class != nullptr)
	{
		DefaultPawnClass = oPlayerPawn.Class;
	}

	static ConstructorHelpers::FClassFinder<ABasePlayerController> oPlayerController(TEXT("/Game/Blueprints/Controller/BP_PlayerController"));
	if (oPlayerController.Class != nullptr)
	{
		PlayerControllerClass = oPlayerController.Class;
	}


}