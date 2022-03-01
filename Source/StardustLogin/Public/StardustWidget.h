// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Runtime/Online/HTTP/Public/Http.h"
#include "StardustData.h"
#include "StardustWidget.generated.h"

class UWebBrowser;
DECLARE_DYNAMIC_DELEGATE_ThreeParams(FSnackbarDelegate, FText, FeedBackTittle, FText, FeedBackText, bool, isSuccess);
/**
 * 
 */
UCLASS()
class STARDUSTLOGIN_API UStardustWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:
	UStardustWidget(const FObjectInitializer& ObjectInitializer);

	UPROPERTY()
		FSnackbarDelegate SnackbarDelegateVar;
	
	UFUNCTION(BlueprintCallable, Category = "Stardust")
		void UpdateSnackBar(FSnackbarDelegate SnackbarDelegate);

	UPROPERTY(BlueprintReadOnly, Category = "Stardust Properties")
		FString LogoUrl;
	UPROPERTY(BlueprintReadOnly, Category = "Stardust Properties")
		FString TermsOfServicesUrl;
	UPROPERTY(BlueprintReadOnly, Category = "Stardust Properties")
		FString PrivacyPolicyURL;
	UPROPERTY(BlueprintReadOnly, Category = "Stardust Properties")
		FString ApiUrl;
	UPROPERTY(BlueprintReadOnly, Category = "Stardust Properties")
		FString Domain;
	UPROPERTY(BlueprintReadOnly, Category = "Stardust Properties")
		FString Region;
	UPROPERTY(BlueprintReadOnly, Category = "Stardust Properties")
		FString ClientId;
	UPROPERTY(BlueprintReadOnly, Category = "Stardust Properties")
		FString CallbackUrl;
	UPROPERTY(BlueprintReadOnly, Category = "Stardust Properties")
		FString GameId;

protected:
	virtual void NativeConstruct() override;

private:

	FHttpModule* HttpModule;

	UPROPERTY()
		UWebBrowser* WebBrowser;	

	UPROPERTY()
	FStardustData StardustData;

	UPROPERTY()
		FString LoginUrl;


	UFUNCTION()
		void HandleLoginUrlChange();

	
	UFUNCTION(BlueprintCallable, Category = "Stardust")
		void CustomProvider(FString Provider);
	UFUNCTION(BlueprintCallable, Category = "Stardust")
		void HostedUIProvider();


	void OnExchangeCodeForTokensResponseReceived(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bWasSuccessful);
};
