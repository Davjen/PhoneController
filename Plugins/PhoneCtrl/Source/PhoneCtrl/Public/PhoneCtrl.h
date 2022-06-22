// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Common/UdpSocketBuilder.h"
#include "ILiveLinkSource.h"
#include "ILiveLinkClient.h"

#include "Roles/LiveLinkBasicRole.h"
#include "Roles/LiveLinkLightTypes.h"
#include "Roles/LiveLinkLightRole.h"
#include "SocketSubsystem.h"
#include "Modules/ModuleManager.h"

typedef class FInputReader;
class PHONECTRL_API FPhoneCtrlLiveLink : public ILiveLinkSource, public FTickableGameObject
{
public:
	
	FPhoneCtrlLiveLink();
	
	bool IsSourceStillValid() const override;
    bool RequestSourceShutdown() override;
	bool IsTickableInEditor()const override;
	
	void ReceiveClient(ILiveLinkClient* InClient, FGuid InSourceGuid) override;
	void Tick(float DeltaTime) override;
	
	TStatId GetStatId()const override;

	FText GetSourceType() const override;
	FText GetSourceMachineName() const override;
	FText GetSourceStatus() const override;

	FLiveLinkSubjectKey Input;
	ILiveLinkClient* LiveLinkClient;

	FSocket* Socket;
	FInputReader* InputReader;
};
class FPhoneCtrlModule : public IModuleInterface
{
public:

	/** IModuleInterface implementation */
	virtual void StartupModule() override;
	virtual void ShutdownModule() override;

};
