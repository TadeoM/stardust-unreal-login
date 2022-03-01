// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "Runtime/Online/HTTP/Public/Http.h"
#include "StardustGameInstance.generated.h"

/**
 * 
 */
UCLASS()
class STARDUSTLOGIN_API UStardustGameInstance : public UGameInstance
{
	GENERATED_BODY()
	
public:
	UStardustGameInstance();

	virtual void Shutdown() override;


	UPROPERTY()
		FString AccessToken;

	UPROPERTY()
		FString IdToken;

	UPROPERTY()
		FString RefreshToken;

	UPROPERTY()
		FTimerHandle RetrieveNewTokensHandle;

	UFUNCTION()
		void SetCognitoTokens(FString NewAccessToken, FString NewIdToken, FString NewRefreshToken);

	UFUNCTION(BlueprintCallable, Category = "Stardust")
		void Logout();

	UFUNCTION(BlueprintCallable, Category = "Stardust")
		void RetrieveNewTokens();

	UPROPERTY(BlueprintReadWrite, Category = "Stardust Properties")
		FString LogoUrl = "https://1000marcas.net/wp-content/uploads/2020/02/logo-Google.png";
	UPROPERTY(BlueprintReadWrite, Category = "Stardust Properties")
		FString TermsOfServicesUrl = "https://www.stardust.gg/";
	UPROPERTY(BlueprintReadWrite, Category = "Stardust Properties")
		FString PrivacyPolicyURL = "https://www.stardust.gg/";

	UPROPERTY(BlueprintReadWrite, Category = "Stardust Properties")
		FString ApiUrl = "https://bddtm60cbd.execute-api.us-east-1.amazonaws.com/v1/oauth2";
	UPROPERTY(BlueprintReadWrite, Category = "Stardust Properties")
		FString Domain = "stardustplayers-dev";
	UPROPERTY(BlueprintReadWrite, Category = "Stardust Properties")
		FString Region = "us-east-1";
	UPROPERTY(BlueprintReadWrite, Category = "Stardust Properties")
		FString ClientId = "4rlsk57pb34chl7819oh7tm7f7";
	UPROPERTY(BlueprintReadWrite, Category = "Stardust Properties")
		FString CallbackUrl = "https://dev.d3twdqo7cb5otm.amplifyapp.com/";
	UPROPERTY(BlueprintReadWrite, Category = "Stardust Properties")
		FString GameId = "1";

	void SaveToken(FString AToken, FString RToken);
	void LoadToken();

private:
	FHttpModule* HttpModule;

	void OnRetrieveNewTokensResponseReceived(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bWasSuccessful);
};
