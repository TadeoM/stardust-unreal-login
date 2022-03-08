// Fill out your copyright notice in the Description page of Project Settings.


#include "StardustGameInstance.h"
#include "Json.h"
#include "JsonUtilities.h"
#include "TextReaderComponent.h"

UStardustGameInstance::UStardustGameInstance() {
	UTextReaderComponent* TextReader = CreateDefaultSubobject<UTextReaderComponent>(TEXT("TextReaderComp"));

	HttpModule = &FHttpModule::Get();
}

void UStardustGameInstance::Shutdown() {
	Super::Shutdown();

	Logout();
}

void UStardustGameInstance::SetCognitoTokens(FString NewAccessToken, FString NewIdToken, FString NewRefreshToken) {
	AccessToken = NewAccessToken;
	IdToken = NewIdToken;
	RefreshToken = NewRefreshToken;

	UE_LOG(LogTemp, Warning, TEXT("access token: %s"), *AccessToken);
	UE_LOG(LogTemp, Warning, TEXT("Id token: %s"), *IdToken);
	UE_LOG(LogTemp, Warning, TEXT("refresh token: %s"), *RefreshToken);


	GetSub(IdToken);

	GetWorld()->GetTimerManager().SetTimer(RetrieveNewTokensHandle, this, &UStardustGameInstance::RetrieveNewTokens, 1.0f, false, 3300.0f);
}

void UStardustGameInstance::Logout()
{
	if (AccessToken.Len() > 0) {
		TSharedRef<IHttpRequest, ESPMode::ThreadSafe> InvalidateTokensRequest = HttpModule->CreateRequest();
		InvalidateTokensRequest->SetURL(ApiUrl + "/invalidate-tokens");
		InvalidateTokensRequest->SetVerb("GET");
		InvalidateTokensRequest->SetHeader("Content-Type", "application/json");
		InvalidateTokensRequest->SetHeader("Authorization", AccessToken);
		InvalidateTokensRequest->ProcessRequest();
	}
}

void UStardustGameInstance::RetrieveNewTokens() {
	UE_LOG(LogTemp, Warning, TEXT("refresh token: %s"), *RefreshToken);
	if (AccessToken.Len() > 0 && RefreshToken.Len() > 0) {
		TSharedPtr<FJsonObject> RequestObj = MakeShareable(new FJsonObject);
		RequestObj->SetStringField("refreshToken", RefreshToken);
		//RequestObj->SetStringField("custom:gameId", GameId);

		FString RequestBody;
		TSharedRef<TJsonWriter<>> Writer = TJsonWriterFactory<>::Create(&RequestBody);

		if (FJsonSerializer::Serialize(RequestObj.ToSharedRef(), Writer)) {
			TSharedRef<IHttpRequest, ESPMode::ThreadSafe> RetrieveNewTokensRequest = HttpModule->CreateRequest();
			RetrieveNewTokensRequest->OnProcessRequestComplete().BindUObject(this, &UStardustGameInstance::OnRetrieveNewTokensResponseReceived);
			RetrieveNewTokensRequest->SetURL(ApiUrl + "/retrieve-new-tokens");
			RetrieveNewTokensRequest->SetVerb("POST");
			RetrieveNewTokensRequest->SetHeader("Content-Type", "application/json");
			RetrieveNewTokensRequest->SetHeader("Authorization", AccessToken);
			RetrieveNewTokensRequest->SetContentAsString(RequestBody);
			RetrieveNewTokensRequest->ProcessRequest();
		}
		else {
			GetWorld()->GetTimerManager().SetTimer(RetrieveNewTokensHandle, this, &UStardustGameInstance::RetrieveNewTokens, 1.0f, false, 30.0f);
		}
	}
}

void UStardustGameInstance::GetSub(FString idToken)
{
	TArray<FString> parts;
	idToken.ParseIntoArray(parts, TEXT("."));
	FString payload = parts[1];

	payload = payload.RightPad(payload.Len() + (4 - payload.Len() % 4) % 4);

	TArray<uint8> ByteArray;
	FString Dest;
	bool Success = FBase64::Decode(payload, ByteArray);

	FUTF8ToTCHAR StringSrc = FUTF8ToTCHAR((const ANSICHAR*)ByteArray.GetData(), ByteArray.Num());
	Dest.AppendChars(StringSrc.Get(), StringSrc.Length() + 1);

	UE_LOG(LogTemp, Warning, TEXT("json: %s"), *Dest);

	/*
	TSharedPtr<FJsonObject> JsonObject;
	TSharedRef<TJsonReader<>> Reader = TJsonReaderFactory<>::Create(Dest);

	if (FJsonSerializer::Deserialize(Reader, JsonObject)) {

			UE_LOG(LogTemp, Warning, TEXT("Holi"));

		if (JsonObject.IsValid())
		{
			UE_LOG(LogTemp, Warning, TEXT("Value I'm looking for %s"), *JsonObject->GetObjectField("sub"));
		}
	}
	*/
}

void UStardustGameInstance::OnRetrieveNewTokensResponseReceived(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bWasSuccessful) {
	if (bWasSuccessful) {
		TSharedPtr<FJsonObject> JsonObject;
		TSharedRef<TJsonReader<>> Reader = TJsonReaderFactory<>::Create(Response->GetContentAsString());

		if (FJsonSerializer::Deserialize(Reader, JsonObject)) {
			if (!JsonObject->HasField("error")) {
				SetCognitoTokens(JsonObject->GetStringField("accessToken"), JsonObject->GetStringField("idToken"), RefreshToken);
			}
		}
		else {
			GetWorld()->GetTimerManager().SetTimer(RetrieveNewTokensHandle, this, &UStardustGameInstance::RetrieveNewTokens, 1.0f, false, 30.0f);
		}
	}
	else {
		GetWorld()->GetTimerManager().SetTimer(RetrieveNewTokensHandle, this, &UStardustGameInstance::RetrieveNewTokens, 1.0f, false, 30.0f);
	}
}
