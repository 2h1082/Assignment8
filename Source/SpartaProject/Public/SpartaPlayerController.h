// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "SpartaPlayerController.generated.h"

/**
 * 
 */
class UInputMappingContext; //IMC ���� ����
class UInputAction; //IA ���� ���� ����

UCLASS()
class SPARTAPROJECT_API ASpartaPlayerController : public APlayerController
{
	GENERATED_BODY()

public:
	ASpartaPlayerController();

	//�����Ϳ��� ������ IMC
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Input");
	UInputMappingContext* InputMappingContext;
	//IA_Move ���� ����
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Input");
	UInputAction* MoveAction;
	//IA_Jump�� ������ ����
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Input");
	UInputAction* JumpAction;
	//IA_Sprint�� ������ ����
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Input");
	UInputAction* SprintAction;
	//IA_Look�� ������ ����
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Input");
	UInputAction* LookAction;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "HUD")
	TSubclassOf<UUserWidget> HUDWidgetClass;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "HUD")
	UUserWidget* HUDWidgetInstance;

	//�޴� UI
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Menu")
	TSubclassOf<UUserWidget> MainMenuWidgetClass;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Menu")
	UUserWidget* MainMenuWidgetInstance;

	//���� ���� �޴� UI
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Menu")
	TSubclassOf<UUserWidget> GameOverMenuWidgetClass;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Menu")
	UUserWidget* GameOverMenuWidgetInstance;

	UFUNCTION(BlueprintPure, Category = "HUD")
	UUserWidget* GetHUDWidget() const;
	//HUD ǥ��
	UFUNCTION(BlueprintCallable, Category = "Menu")
	void ShowGameHUD();
	//���� �޴� ǥ��
	UFUNCTION(BlueprintCallable, Category = "Menu")
	void ShowMainMenu();
	//���� ���� �޴�
	UFUNCTION(BlueprintCallable, Category = "Menu")
	void ShowGameOverMenu();

	//���� ����
	UFUNCTION(BlueprintCallable, Category = "Menu")
	void StartGame();

	virtual void BeginPlay() override;
};
