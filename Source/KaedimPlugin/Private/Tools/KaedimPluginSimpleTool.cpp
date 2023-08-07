// Copyright Epic Games, Inc. All Rights Reserved.

#include "KaedimPluginSimpleTool.h"
#include "InteractiveToolManager.h"
#include "ToolBuilderUtil.h"
#include "CollisionQueryParams.h"
#include "HttpModule.h"
#include "Interfaces/IHttpRequest.h"
#include "Interfaces/IHttpResponse.h"
#include "Serialization/JsonSerializer.h"
#include "Serialization/JsonWriter.h"
#include "JsonUtilities.h"
#include "Dom/JsonObject.h"
#include "Engine/StaticMesh.h"
#include "Engine/StaticMeshActor.h"
#include "Misc/FileHelper.h"
#include "Misc/Paths.h"
// localization namespace
#define LOCTEXT_NAMESPACE "KaedimPluginSimpleTool"

/*
 * ToolBuilder implementation
 */

UInteractiveTool* UKaedimPluginSimpleToolBuilder::BuildTool(const FToolBuilderState& SceneState) const
{
	UKaedimPluginSimpleTool* NewTool = NewObject<UKaedimPluginSimpleTool>(SceneState.ToolManager);
	NewTool->SetWorld(SceneState.World);
	return NewTool;
}



/*
 * ToolProperties implementation
 */

UKaedimPluginSimpleToolProperties::UKaedimPluginSimpleToolProperties()
{
	DevID = FText::FromString(TEXT(""));
	APIKey = FText::FromString(TEXT(""));
}


/*
 * Tool implementation
 */

UKaedimPluginSimpleTool::UKaedimPluginSimpleTool()
	: TargetWorld(nullptr)
{
}


void UKaedimPluginSimpleTool::SetWorld(UWorld* World)
{
	this->TargetWorld = World;
}


void UKaedimPluginSimpleTool::Setup()
{
	USingleClickTool::Setup();

	Properties = NewObject<UKaedimPluginSimpleToolProperties>(this);
	AddToolPropertySource(Properties);
	Properties->DevID = FText::FromString(TEXT(""));
	Properties->APIKey = FText::FromString(TEXT(""));
	Properties->jwt = "";
}


void UKaedimPluginSimpleTool::OnClicked(const FInputDeviceRay& ClickPos)
{
	return;
}

void UKaedimPluginSimpleToolProperties::ImportAsset(const FString& name, const FString& ObjUrl)
{
	// Create the destination path
	FString DestinationPath = FPaths::ProjectContentDir() + "Kaedim/" + name;

	// Create the HTTP request
	TSharedRef<IHttpRequest> HttpRequest = FHttpModule::Get().CreateRequest();
	HttpRequest->SetURL(ObjUrl);
	HttpRequest->SetVerb("GET");

	// Set the completion callback as a lambda function
	HttpRequest->OnProcessRequestComplete().BindLambda([=](FHttpRequestPtr Request, FHttpResponsePtr Response, bool bWasSuccessful)
		{
			if (bWasSuccessful && Response.IsValid())
			{
				// Save the response data to a file
				FFileHelper::SaveArrayToFile(Response->GetContent(), *DestinationPath);

				UE_LOG(LogTemp, Display, TEXT("File downloaded successfully: %s"), *DestinationPath);
			}
			else
			{
				UE_LOG(LogTemp, Error, TEXT("Failed to download file: %s"), *Request->GetURL());
			}
		});

	// Send the request
	HttpRequest->ProcessRequest();
}

void UKaedimPluginSimpleToolProperties::ProcessAssets(const FString& JsonData)
{
	TSharedPtr<FJsonObject> JsonObject;
	TSharedRef<TJsonReader<>> JsonReader = TJsonReaderFactory<>::Create(JsonData);
	if (FJsonSerializer::Deserialize(JsonReader, JsonObject))
	{
		TArray<TSharedPtr<FJsonValue>> AssetsArray = JsonObject->GetArrayField("assets");
		for (const TSharedPtr<FJsonValue>& AssetValue : AssetsArray)
		{
			TSharedPtr<FJsonObject> AssetObject = AssetValue->AsObject();
			TArray<TSharedPtr<FJsonValue>> IterationsArray = AssetObject->GetArrayField("iterations");
			for (const TSharedPtr<FJsonValue>& IterationValue : IterationsArray)
			{
				TSharedPtr<FJsonObject> IterationObject = IterationValue->AsObject();
				TSharedPtr<FJsonObject> ResultsObject = IterationObject->GetObjectField("results");
				FString ObjUrl = ResultsObject->GetStringField("obj");

				// Process the Obj file URL here
				UE_LOG(LogTemp, Warning, TEXT("Obj URL: %s"), *ObjUrl);
				ImportAsset(TEXT("sample name"), ObjUrl);
			}
		}
	}
}


void UKaedimPluginSimpleToolProperties::downloadAssets()
{
	// Handle button click event here
	UE_LOG(LogTemp, Display, TEXT("loading assets"));
	UE_LOG(LogTemp, Display, TEXT("registering keys"));
	UE_LOG(LogTemp, Display, TEXT("DevID: %s"), *DevID.ToString());
	UE_LOG(LogTemp, Display, TEXT("APIKey: %s"), *DevID.ToString());
	UE_LOG(LogTemp, Display, TEXT("JWT: %s"), *FString(jwt.c_str()));

	// Create the HTTP request object
	TSharedRef<IHttpRequest, ESPMode::ThreadSafe> HttpRequest = FHttpModule::Get().CreateRequest();
	HttpRequest->SetURL("https://api.kaedim3d.com/api/v1/fetchAll");
	HttpRequest->SetVerb("GET");

	HttpRequest->SetHeader(TEXT("X-API-Key"), APIKey.ToString());
	HttpRequest->SetHeader(TEXT("Authorization-Type"), FString(jwt.c_str()));

	TSharedPtr<FJsonObject> JsonObject = MakeShared<FJsonObject>();
	JsonObject->SetStringField(TEXT("devID"), DevID.ToString());

	FString JsonString;
	TSharedRef<TJsonWriter<>> JsonWriter = TJsonWriterFactory<>::Create(&JsonString);
	FJsonSerializer::Serialize(JsonObject.ToSharedRef(), JsonWriter);
	HttpRequest->SetContentAsString(JsonString);
	HttpRequest->OnProcessRequestComplete().BindLambda([this](FHttpRequestPtr Request, FHttpResponsePtr Response, bool bSuccess)
		{
			if (bSuccess && Response.IsValid())
			{
				FString ResponseBody = Response->GetContentAsString();
				UE_LOG(LogTemp, Warning, TEXT("API response: %s"), *ResponseBody);
				if (Response->GetResponseCode() == 200) {
					this->ProcessAssets(ResponseBody);
				}
				else {
					UE_LOG(LogTemp, Warning, TEXT("API response: %s"), *ResponseBody);
				}
			}
			else
			{
				UE_LOG(LogTemp, Error, TEXT("API request failed, please check your keys"));
			}
		});
	HttpRequest->ProcessRequest();
}

void LoadObjectIntoWorld(const FString& FilePath, UWorld* World)
{
	UStaticMesh* StaticMesh = Cast<UStaticMesh>(StaticLoadObject(UStaticMesh::StaticClass(), nullptr, *FilePath));

	if (StaticMesh)
	{
		FActorSpawnParameters SpawnParams;
		SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

		AStaticMeshActor* SpawnedActor = World->SpawnActor<AStaticMeshActor>(AStaticMeshActor::StaticClass(), FVector::ZeroVector, FRotator::ZeroRotator, SpawnParams);
		if (SpawnedActor)
		{
			SpawnedActor->GetStaticMeshComponent()->SetStaticMesh(StaticMesh);
		}
	}
}


void UKaedimPluginSimpleToolProperties::registerKeys()
{


	// Handle button click event here
	UE_LOG(LogTemp, Display, TEXT("registering keys"));
	UE_LOG(LogTemp, Display, TEXT("DevID: %s"), *DevID.ToString());
	UE_LOG(LogTemp, Display, TEXT("APIKey: %s"), *APIKey.ToString());
	UE_LOG(LogTemp, Display, TEXT("JWT: %s"), UTF8_TO_TCHAR(jwt.c_str()));



    // Create the HTTP request object
	TSharedRef<IHttpRequest, ESPMode::ThreadSafe> HttpRequest = FHttpModule::Get().CreateRequest();
	HttpRequest->SetURL("https://api.kaedim3d.com/api/v1/registerHook");
	HttpRequest->SetVerb("POST");

	HttpRequest->SetHeader(TEXT("Content-Type"), TEXT("application/json"));
	HttpRequest->SetHeader(TEXT("X-API-Key"), APIKey.ToString());

	TSharedPtr<FJsonObject> JsonObject = MakeShared<FJsonObject>();
	JsonObject->SetStringField(TEXT("devID"), DevID.ToString());
	JsonObject->SetStringField(TEXT("destination"), FString("www.example.com/test"));

	FString JsonString;
	TSharedRef<TJsonWriter<>> JsonWriter = TJsonWriterFactory<>::Create(&JsonString);
	FJsonSerializer::Serialize(JsonObject.ToSharedRef(), JsonWriter);
	HttpRequest->SetContentAsString(JsonString);

	HttpRequest->OnProcessRequestComplete().BindLambda([this](FHttpRequestPtr Request, FHttpResponsePtr Response, bool bSuccess)
	{
			if (bSuccess && Response.IsValid())
			{
				FString ResponseBody = Response->GetContentAsString();
				UE_LOG(LogTemp, Warning, TEXT("API response: %s"), *ResponseBody);
				if (Response->GetResponseCode() != 201) {
					UE_LOG(LogTemp, Error, TEXT("API request failed, please check your keys"));
				}
				else {
					TSharedPtr<FJsonObject> ResponseJsonObject;
					TSharedRef<TJsonReader<>> JsonReader = TJsonReaderFactory<>::Create(ResponseBody);
					if (FJsonSerializer::Deserialize(JsonReader, ResponseJsonObject))
					{
						FString Value;
						if (ResponseJsonObject->TryGetStringField(TEXT("jwt"), Value))
						{
							const TCHAR* TCHARArray = *Value;
							this->jwt = TCHAR_TO_UTF8(TCHARArray);
							UE_LOG(LogTemp, Display, TEXT("Setting 'jwt': %s"), *Value);
						}
						else
						{
							UE_LOG(LogTemp, Error, TEXT("Failed to retrieve value of 'jwt'"));
						}
					}
					else
					{
						UE_LOG(LogTemp, Error, TEXT("Failed to parse JSON response"));
					}
				}
			}
			else
			{
				UE_LOG(LogTemp, Error, TEXT("API request failed, please check your keys"));
			}
		});
	HttpRequest->ProcessRequest();
}



#undef LOCTEXT_NAMESPACE
