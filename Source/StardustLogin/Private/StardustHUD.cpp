// Fill out your copyright notice in the Description page of Project Settings.


#include "StardustHUD.h"
#include "Blueprint/UserWidget.h"

AStardustHUD::AStardustHUD() {
	static ConstructorHelpers::FClassFinder<UUserWidget> StardustObj(TEXT("/Game/StardustWidget/Blueprints/Widgets/WBP_Stardust"));
	StardustWidgetClass = StardustObj.Class;
}

void AStardustHUD::BeginPlay() {
	Super::BeginPlay();

	APlayerController* PlayerController = GetWorld()->GetFirstPlayerController();
	if (PlayerController != nullptr) {
		PlayerController->bShowMouseCursor = true;
	}

	if (StardustWidgetClass != nullptr) {
		UUserWidget* StardustWidget = CreateWidget<UUserWidget>(GetWorld(), StardustWidgetClass);

		if (StardustWidget != nullptr) {
			StardustWidget->AddToViewport();
			StardustWidget->SetFocus();
		}
	}
}
