// Copyright Epic Games, Inc. All Rights Reserved.

#include "PhoneCtrl.h"
#include "InputReader.h"
#include "Engine/GameViewportClient.h"
#define LOCTEXT_NAMESPACE "FPhoneCtrlModule"

#define HANDSHAKING (1)
#define PING (2)
#define DISCONNECT (3)
#define MOVEMENT (4)
#define BUTTONS (5)

void FPhoneCtrlModule::StartupModule()
{
	ILiveLinkClient& LiveLinkClient = IModularFeatures::Get().GetModularFeature<ILiveLinkClient>(ILiveLinkClient::ModularFeatureName);
	LiveLinkClient.AddSource(MakeShared< FPhoneCtrlLiveLink>());
	// This code will execute after your module is loaded into memory; the exact timing is specified in the .uplugin file per-module
}

void FPhoneCtrlModule::ShutdownModule()
{
	// This function may be called during shutdown to clean up your module.  For modules that support dynamic reloading,
	// we call this function before unloading the module.
}

#undef LOCTEXT_NAMESPACE
	
IMPLEMENT_MODULE(FPhoneCtrlModule, PhoneCtrl)

FPhoneCtrlLiveLink::FPhoneCtrlLiveLink()
{
	InputReader = new FInputReader();
	LiveLinkClient = nullptr;
	Socket = nullptr;
}

bool FPhoneCtrlLiveLink::IsSourceStillValid() const
{
	return true;
}

bool FPhoneCtrlLiveLink::RequestSourceShutdown()
{
	LiveLinkClient = nullptr;
	return true;
}

bool FPhoneCtrlLiveLink::IsTickableInEditor() const 
{
	return true;
}

void FPhoneCtrlLiveLink::ReceiveClient(ILiveLinkClient* InClient, FGuid InSourceGuid)
{
	if (Socket)
	{
		Socket->Close();
	}

	ISocketSubsystem* SocketSubsystem = ISocketSubsystem::Get(PLATFORM_SOCKETSUBSYSTEM);

	TSharedPtr<FInternetAddr> EndpointInternetAddr = SocketSubsystem->CreateInternetAddr();
	bool bIsValid = false;
	EndpointInternetAddr->SetIp(TEXT("192.168.1.142"), bIsValid);
	EndpointInternetAddr->SetPort(12345);

	FIPv4Endpoint Endpoint = FIPv4Endpoint(EndpointInternetAddr);

	Socket = FUdpSocketBuilder(TEXT("Phone Server Server"))
		.AsNonBlocking()
		.AsReusable()
		.BoundToEndpoint(Endpoint);
	LiveLinkClient = InClient;

	Input.SubjectName = FLiveLinkSubjectName(TEXT("ButtonPressed"));
	Input.Source = InSourceGuid;


	FLiveLinkStaticDataStruct InputStaticData;
	InputStaticData.InitializeWith(FLiveLinkBaseStaticData::StaticStruct(), nullptr);
	FLiveLinkBaseStaticData* InputStaticDataPtr = InputStaticData.Cast<FLiveLinkBaseStaticData>();
	InputStaticDataPtr->PropertyNames.Add("Forward");
	InputStaticDataPtr->PropertyNames.Add("Right");
	InputStaticDataPtr->PropertyNames.Add("JumpInput");
	InputStaticDataPtr->PropertyNames.Add("Yaw");
	InputStaticDataPtr->PropertyNames.Add("Pitch");



	InClient->PushSubjectStaticData_AnyThread(Input, ULiveLinkBasicRole::StaticClass(), MoveTemp(InputStaticData));
}

void FPhoneCtrlLiveLink::Tick(float DeltaTime)
{
	if (LiveLinkClient)
	{
		if (Socket)
		{
			uint32 RequiredDataSize;
			if (!Socket->HasPendingData(RequiredDataSize))
			{
				LiveLinkClient->ClearAllSubjectsFrames_AnyThread();
				return;
			}
			TArray<uint8> Data;
			Data.AddUninitialized(RequiredDataSize);
			int32 ReceivedDataSize;
			if (!Socket->Recv(Data.GetData(), Data.Num(), ReceivedDataSize))
			{
				return;
			}

			float* packet = reinterpret_cast<float*>(Data.GetData());

			/*InputReader->PackageUnpacker(MoveTemp(packet));

			UE_LOG(LogTemp, Warning, TEXT("PIPPO"));*/

			if (packet[0] == MOVEMENT)
			{

			}
			
			/*float* Floats = reinterpret_cast<float*>(Data.GetData());*/

			FLiveLinkFrameDataStruct FrameDataStruct = FLiveLinkFrameDataStruct(FLiveLinkBaseFrameData::StaticStruct());
			FLiveLinkBaseFrameData* FrameDataPtr = FrameDataStruct.Cast<FLiveLinkBaseFrameData>();
			FrameDataPtr->WorldTime = FPlatformTime::Seconds();
			FrameDataPtr->PropertyValues.Add(packet[0]);//HERE NEED TO SEND THE PACKET FROM PHONE WITH THE BUTTON PRESSED
			FrameDataPtr->PropertyValues.Add(packet[1]);//HERE NEED TO SEND THE PACKET FROM PHONE WITH THE BUTTON PRESSED
			FrameDataPtr->PropertyValues.Add(packet[2]);//HERE NEED TO SEND THE PACKET FROM PHONE WITH THE BUTTON PRESSED
			FrameDataPtr->PropertyValues.Add(packet[3]);//HERE NEED TO SEND THE PACKET FROM PHONE WITH THE BUTTON PRESSED
			FrameDataPtr->PropertyValues.Add(packet[4]);//HERE NEED TO SEND THE PACKET FROM PHONE WITH THE BUTTON PRESSED
			LiveLinkClient->PushSubjectFrameData_AnyThread(Input, MoveTemp(FrameDataStruct));
			
		}
	}
}

TStatId FPhoneCtrlLiveLink::GetStatId() const
{
	return TStatId();
}

FText FPhoneCtrlLiveLink::GetSourceType() const
{
	return FText::FromString("Phone Source");
}

FText FPhoneCtrlLiveLink::GetSourceMachineName() const
{
	return FText::FromString(FPlatformProcess::ComputerName());
}

FText FPhoneCtrlLiveLink::GetSourceStatus() const
{
	return FText::FromString("Active");
}
