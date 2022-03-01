// Fill out your copyright notice in the Description page of Project Settings.


#include "TextReaderComponent.h"
#include "Runtime/Core/Public/Misc/Paths.h"
#include "Runtime/Core/Public/HAL/PlatformFilemanager.h"
#include "Runtime/Core/Public/Misc/FileHelper.h"

UTextReaderComponent::UTextReaderComponent()
{

}

FString UTextReaderComponent::ReadFile(FString FilePath) {
	FString DirectoryPath = FPaths::ProjectContentDir();
	FString FullPath = DirectoryPath + "/" + FilePath;
	FString Result;
	IPlatformFile& File = FPlatformFileManager::Get().GetPlatformFile();

	if (File.FileExists(*FullPath)) {
		FFileHelper::LoadFileToString(Result, *FullPath);
	}

	return Result;
}

FString UTextReaderComponent::WriteFile(FString FileName, FString Content) {
	FString DirectoryPath = FPaths::ProjectContentDir();
	FString Result;

	IPlatformFile& file = FPlatformFileManager::Get().GetPlatformFile();

	if(file.CreateDirectory(*DirectoryPath)) {
		FString MyFile = DirectoryPath + "/" + FileName;
		FFileHelper::SaveStringToFile(Content, *MyFile);
	}

	return FileName;
}

void UTextReaderComponent::WriteText(FString path, FString data) {
	FString ProjectDir = FPaths::ProjectContentDir();
	ProjectDir += path;
	if (!ProjectDir.Contains(".txt")) {
		ProjectDir += ".txt";
	}
	FFileHelper::SaveStringToFile(data, *ProjectDir);
}

FString UTextReaderComponent::ReadText(FString path) {
	FString data;
	FString ProjectDir = FPaths::ProjectContentDir();
	ProjectDir += path;
	if (!ProjectDir.Contains(".txt")) {
		ProjectDir += ".txt";
	}
	if (!FPlatformFileManager::Get().GetPlatformFile().FileExists(*ProjectDir)) {
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("Couldn't find file"));
		return FString();
	}
	else {
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("Could find file"));
		FFileHelper::LoadFileToString(data, *ProjectDir);
		return data;
	}
}
