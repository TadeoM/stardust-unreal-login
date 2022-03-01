// Fill out your copyright notice in the Description page of Project Settings.


#include "StardustGameMode.h"
#include "StardustHUD.h"

AStardustGameMode::AStardustGameMode() {
	HUDClass = AStardustHUD::StaticClass();
}
