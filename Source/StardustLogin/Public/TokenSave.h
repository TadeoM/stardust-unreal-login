// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/SaveGame.h"
#include "TokenSave.generated.h"

/**
 * 
 */
UCLASS()
class STARDUSTLOGIN_API UTokenSave : public USaveGame
{
	GENERATED_BODY()

public:

		UTokenSave();

		UPROPERTY(EditAnywhere, Category = "StardustProperties")
		FString AToken;
		UPROPERTY(EditAnywhere, Category = "StardustProperties")
		FString RToken;
};
