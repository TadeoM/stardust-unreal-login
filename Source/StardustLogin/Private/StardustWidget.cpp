// Fill out your copyright notice in the Description page of Project Settings.

#include "StardustWidget.h"
#include "TextReaderComponent.h"
#include "WebBrowser.h"
#include "WebBrowserModule.h"
#include "IWebBrowserSingleton.h"
#include "IWebBrowserCookieManager.h"
#include "Json.h"
#include "JsonUtilities.h"
#include "StardustGameInstance.h"
#include "Serialization/JsonSerializer.h"

UStardustWidget::UStardustWidget(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer) {

	UTextReaderComponent* TextReader = CreateDefaultSubobject<UTextReaderComponent>(TEXT("TextReaderComp"));

}
void UStardustWidget::NativeConstruct() {
	Super::NativeConstruct();
	bIsFocusable = true;

	UGameInstance* GameInstance = GetGameInstance();
	if (GameInstance != nullptr) {
		UStardustGameInstance* StardustGameInstance = Cast<UStardustGameInstance>(GameInstance);
		if (StardustGameInstance != nullptr) {
			LogoUrl = StardustGameInstance->LogoUrl;
			TermsOfServicesUrl = StardustGameInstance->TermsOfServicesUrl;
			PrivacyPolicyURL = StardustGameInstance->PrivacyPolicyURL;
			ApiUrl = StardustGameInstance->ApiUrl;
			Domain = StardustGameInstance->Domain;
			Region = StardustGameInstance->Region;
			ClientId = StardustGameInstance->ClientId;
			CallbackUrl = StardustGameInstance->CallbackUrl;
			GameId = StardustGameInstance->GameId;
		}
	}

	WebBrowser = (UWebBrowser*)GetWidgetFromName(TEXT("WebBrowser_Login"));
}

void UStardustWidget::CustomProvider(FString Provider) {

	IWebBrowserSingleton* WebBrowserSingleton = IWebBrowserModule::Get().GetSingleton();
	if (WebBrowserSingleton != nullptr) {
		TOptional<FString> DefaultContext;
		TSharedPtr<IWebBrowserCookieManager> CookieManager = WebBrowserSingleton->GetCookieManager(DefaultContext);
		if (CookieManager.IsValid()) {
			CookieManager->DeleteCookies();
		}
	}

	FString URL = "https://" + Domain + ".auth." + Region + ".amazoncognito.com/oauth2/authorize?identity_provider=" + Provider + "&redirect_uri=" + CallbackUrl + "&response_type=CODE&client_id=" + ClientId;

	WebBrowser->LoadURL(URL);
	FScriptDelegate LoginDelegate;
	LoginDelegate.BindUFunction(this, "HandleLoginUrlChange");
	WebBrowser->OnUrlChanged.Add(LoginDelegate);
}

void UStardustWidget::HostedUIProvider() {
	IWebBrowserSingleton* WebBrowserSingleton = IWebBrowserModule::Get().GetSingleton();
	if (WebBrowserSingleton != nullptr) {
		TOptional<FString> DefaultContext;
		TSharedPtr<IWebBrowserCookieManager> CookieManager = WebBrowserSingleton->GetCookieManager(DefaultContext);
		if (CookieManager.IsValid()) {
			CookieManager->DeleteCookies();
		}
	}

	LoginUrl = "https://stardustplayers-dev.auth.us-east-1.amazoncognito.com/login?client_id=60qs7psie9c3sp61f0jd7led4r&response_type=code&scope=email+openid+profile&redirect_uri=http://localhost:3000";

	WebBrowser->LoadURL(LoginUrl);
	FScriptDelegate LoginDelegate;
	LoginDelegate.BindUFunction(this, "HandleLoginUrlChange");
	WebBrowser->OnUrlChanged.Add(LoginDelegate);
}

void UStardustWidget::UpdateSnackBar(FSnackbarDelegate SnackbarDelegate) {
	SnackbarDelegateVar = SnackbarDelegate;
}

void UStardustWidget::HandleLoginUrlChange() {
	FString BrowserUrl = WebBrowser->GetUrl();
	FString Url;
	FString QueryParameters;

	if (BrowserUrl.Split("?", &Url, &QueryParameters)) {
		if (Url.Equals(CallbackUrl)) {
			FString ParameterName;
			FString ParameterValue;

			if (QueryParameters.Split("=", &ParameterName, &ParameterValue)) {
				if (ParameterName.Equals("code")) {
					ParameterValue = ParameterValue.Replace(*FString("#"), *FString(""));

					TSharedPtr<FJsonObject> RequestObj = MakeShareable(new FJsonObject);
					RequestObj->SetStringField(ParameterName, ParameterValue);
					RequestObj->SetStringField("custom:gameId", GameId);

					FString RequestBody;
					TSharedRef<TJsonWriter<>> Writer = TJsonWriterFactory<>::Create(&RequestBody);

					//UE_LOG(LogTemp, Warning, TEXT("Request: %d"), RequestBody);

					if (FJsonSerializer::Serialize(RequestObj.ToSharedRef(), Writer)) {
						TSharedRef<IHttpRequest, ESPMode::ThreadSafe> ExchangeCodeForTokensRequest = (&FHttpModule::Get())->CreateRequest();
						ExchangeCodeForTokensRequest->OnProcessRequestComplete().BindUObject(this, &UStardustWidget::OnExchangeCodeForTokensResponseReceived);
						ExchangeCodeForTokensRequest->SetURL(ApiUrl + "/exchange-code-for-tokens");
						ExchangeCodeForTokensRequest->SetVerb("POST");
						ExchangeCodeForTokensRequest->SetHeader("Content-Type", "application/json");
						ExchangeCodeForTokensRequest->SetContentAsString(RequestBody);
						ExchangeCodeForTokensRequest->ProcessRequest();

					}
				}
			}
		}
	}
}

void UStardustWidget::OnExchangeCodeForTokensResponseReceived(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bWasSuccessful) {

	FString FBackTittle;
	FString FBackMsg;
	bool bSuccess = false;

	if (bWasSuccessful) {

		TSharedPtr<FJsonObject> JsonObject;
		TSharedRef<TJsonReader<>> Reader = TJsonReaderFactory<>::Create(Response->GetContentAsString());

		if (!Response.IsValid()) {
			FBackTittle = "Error Loggin in. Please try again.";
			FBackMsg = "Invalid Http Response!";
			GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, FString::Printf(TEXT("Invalid Http Response!")));
			UE_LOG(LogTemp, Warning, TEXT("Invalid Http Response!"));
		}
		else if (Response->GetResponseCode() == 200) {
			GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, FString::Printf(TEXT(" Valid Http Response.. %s"), *FBackMsg));
			UE_LOG(LogTemp, Warning, TEXT("%s"), *FBackMsg);
		}
		else {
			FBackTittle = "Error Loggin in. Please try again.";
			FBackMsg = "Error code " +("Error code %d", FString::FromInt(Response->GetResponseCode()));
			GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, FString::Printf(TEXT("Error code %d"), Response->GetResponseCode()));
			UE_LOG(LogTemp, Warning, TEXT("%d"), Response->GetResponseCode());
		}


		if (FJsonSerializer::Deserialize(Reader, JsonObject)) {
			if (!JsonObject->HasField("error")) {
				UGameInstance* GameInstance = GetGameInstance();
				if (GameInstance != nullptr) {
					UStardustGameInstance* StardustGameInstance = Cast<UStardustGameInstance>(GameInstance);
					if (StardustGameInstance != nullptr) {
						StardustGameInstance->SetCognitoTokens(JsonObject->GetStringField("access_token"), JsonObject->GetStringField("id_token"), JsonObject->GetStringField("refresh_token"));
						
						//UE_LOG(LogTemp, Warning, TEXT("Access token: %d"), *JsonObject->GetStringField("access_token"));
						
						//StardustGameInstance->RetrieveNewTokens();
						bSuccess = true;
						FBackTittle = "Succesfully Logged In";
						FBackMsg = "Succesfully Logged In";
					}
				}
			}
		}
	}
	else {
		FBackTittle = "Error Loggin in. Please try again.";
		FBackMsg = "Wasn't able to connect please try again";
	}
		SnackbarDelegateVar.ExecuteIfBound(FText::FromString(FBackTittle), FText::FromString(FBackMsg), bSuccess);
}
