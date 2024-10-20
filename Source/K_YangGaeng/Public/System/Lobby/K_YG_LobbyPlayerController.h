// © 2024 Team 'K_YangGaeng'. All rights reserved.

#pragma once

#include "../K_YangGaeng.h"
#include "GameFramework/PlayerController.h"
#include "K_YG_LobbyPlayerController.generated.h"

class UUserWidget;
class USoundBase;
class UAudioComponent;

/**
 * 
 */
UCLASS()
class K_YANGGAENG_API AK_YG_LobbyPlayerController : public APlayerController
{
	GENERATED_BODY()

protected:

    virtual void BeginPlay() override;

public:
    AK_YG_LobbyPlayerController();

    UFUNCTION(BlueprintCallable, Category = "Sound")
    void SetSoundVolume(float Volume);

    UFUNCTION(BlueprintCallable, Category = "Menu")
    void OpenMenuWidget();

    UFUNCTION(Client, Reliable, WithValidation, BlueprintCallable, Category = "UI")
    void Client_SwitchToMain();

    UFUNCTION(Client, Reliable, WithValidation, BlueprintCallable, Category = "UI")
    void Client_SwitchToLogin();

    UFUNCTION(Client, Reliable, WithValidation, BlueprintCallable, Category = "UI")
    void Client_SwitchToCredit();

    UFUNCTION(Client, Reliable, WithValidation, BlueprintCallable, Category = "UI")
    void Client_SwitchToStart();

    UFUNCTION(Client, Reliable, WithValidation, BlueprintCallable, Category = "UI")
    void Client_SwitchToSetting();

    ////////////////////////////////////////////////////////////////////////////////////

    UFUNCTION(Server, Reliable, WithValidation)
    void ServerNameChecked(const FString& NickName);

    UFUNCTION(Client, Reliable, WithValidation)
    void ClientNamePassed(const FString& NickName);

    UFUNCTION(Client, Reliable, WithValidation)
    void ClientNameUnPassed(const FString& NickName);

protected:

    // Pointers to hold references to the widgets
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UI")
    TSubclassOf<class UUserWidget> MainWidgetClass;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UI")
    TSubclassOf<class UUserWidget> LoginWidgetClass;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UI")
    TSubclassOf<class UUserWidget> CreditWidgetClass;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UI")
    TSubclassOf<class UUserWidget> SettingWidgetClass;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UI")
    TSubclassOf<class UUserWidget> MenuWidgetClass;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UI")
    TSubclassOf<class UUserWidget> ScoreBoardWidgetClass;

    virtual void SetupInputComponent() override;

    void ServerNameChecked_Implementation(const FString& NickName);
    bool ServerNameChecked_Validate(const FString& NickName);

    void ClientNamePassed_Implementation(const FString& NickName);
    bool ClientNamePassed_Validate(const FString& NickName);

    void ClientNameUnPassed_Implementation(const FString& NickName);
    bool ClientNameUnPassed_Validate(const FString& NickName);

    ////////////////////////////////////////////////////////////////////////////

    void Client_SwitchToMain_Implementation();
    bool Client_SwitchToMain_Validate();

    void Client_SwitchToLogin_Implementation();
    bool Client_SwitchToLogin_Validate();

    void Client_SwitchToCredit_Implementation();
    bool Client_SwitchToCredit_Validate();

    void Client_SwitchToStart_Implementation();
    bool Client_SwitchToStart_Validate();

    void Client_SwitchToSetting_Implementation();
    bool Client_SwitchToSetting_Validate();

    bool bIsOpenMenu;

private:

    UPROPERTY()
    class UUserWidget* CurrentWidget;

protected:

    //#. asset에서 사운드가 나오게 해주는 클래스라고 하네요
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Sound")
    USoundBase* MySoundCue;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Sound")
    UAudioComponent* MyAudioComponent;
};

