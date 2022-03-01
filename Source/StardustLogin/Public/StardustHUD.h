// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "StardustHUD.generated.h"

/**
 * 
 */
UCLASS()
class STARDUSTLOGIN_API AStardustHUD : public AHUD
{
	GENERATED_BODY()
	
public:
	AStardustHUD();

protected:
	virtual void BeginPlay()override;

private:
	UPROPERTY()
		TSubclassOf<UUserWidget> StardustWidgetClass;
};
