// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "TextReaderComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class STARDUSTLOGIN_API UTextReaderComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	UTextReaderComponent();

	UFUNCTION()
		FString ReadFile(FString FilePath);

	UFUNCTION()
		FString WriteFile(FString FileName, FString Content);

	//Text File Read-Write
	UFUNCTION(BlueprintCallable, Category = "Test")
		void WriteText(FString path, FString data);
	UFUNCTION(BlueprintCallable, Category = "Test")
		FString ReadText(FString path);
};
