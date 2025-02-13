// Fill out your copyright notice in the Description page of Project Settings.


#include "SpartaPlayerController.h"
#include "EnhancedInputSubsystems.h"
#include "Blueprint/UserWidget.h"
#include "SpartaGameState.h"
#include "SpartaGameInstance.h"
#include "Kismet/GameplayStatics.h"
#include "Components/TextBlock.h"

ASpartaPlayerController::ASpartaPlayerController()
	:	InputMappingContext(nullptr),
		MoveAction(nullptr),
		JumpAction(nullptr),
		LookAction(nullptr),
		SprintAction(nullptr),
		HUDWidgetClass(nullptr),
		HUDWidgetInstance(nullptr),
		MainMenuWidgetClass(nullptr),
		MainMenuWidgetInstance(nullptr)
{
}

UUserWidget* ASpartaPlayerController::GetHUDWidget() const
{
	return HUDWidgetInstance;
}

void ASpartaPlayerController::ShowGameHUD()
{
	//HUD�� ���� �ִٸ� �ݱ�
	if (HUDWidgetInstance)
	{
		HUDWidgetInstance->RemoveFromParent();
		HUDWidgetInstance = nullptr;
	}

	//�̹� �޴��� �������� ����
	if (MainMenuWidgetInstance)
	{
		MainMenuWidgetInstance->RemoveFromParent();
		MainMenuWidgetInstance = nullptr;
	}

	if (HUDWidgetClass)
	{
		HUDWidgetInstance = CreateWidget<UUserWidget>(this, HUDWidgetClass);
		if (HUDWidgetInstance)
		{
			HUDWidgetInstance->AddToViewport();

			bShowMouseCursor = false;
			SetInputMode(FInputModeGameOnly());

			ASpartaGameState* SpartaGameState = GetWorld() ? GetWorld()->GetGameState<ASpartaGameState>():nullptr;
			if (SpartaGameState)
			{
				SpartaGameState->UpdateHUD();
			}
		}
	}
}

void ASpartaPlayerController::ShowMainMenu()
{
	//���� ������ MenuLevel�� �ƴ϶�� GameOverMenu���� MenuLevel�� �̵�(GameOverMenu�� ���� �޴� ��ư Ŭ�� �� �ش� �Լ� ȣ��)
	FString CurrentMapName = GetWorld()->GetMapName();
	if (!CurrentMapName.Contains("MenuLevel"))
	{
		UGameplayStatics::OpenLevel(GetWorld(), FName("MenuLevel"));
	}
	//HUD�� ���� �ִٸ� �ݱ�
	if (HUDWidgetInstance)
	{
		HUDWidgetInstance->RemoveFromParent();
		HUDWidgetInstance = nullptr;
	}

	//�̹� �޴��� �������� ����
	if (MainMenuWidgetInstance)
	{
		MainMenuWidgetInstance->RemoveFromParent();
		MainMenuWidgetInstance = nullptr;
	}

	//�̹� ���� ���� �޴��� �������� ����
	if (GameOverMenuWidgetInstance)
	{
		GameOverMenuWidgetInstance->RemoveFromParent();
		GameOverMenuWidgetInstance = nullptr;
	}

	//�޴� UI ����
	if (MainMenuWidgetClass)
	{
		MainMenuWidgetInstance = CreateWidget<UUserWidget>(this, MainMenuWidgetClass);
		if (MainMenuWidgetInstance)
		{
			MainMenuWidgetInstance->AddToViewport();
			bShowMouseCursor = true;
			SetInputMode(FInputModeUIOnly());
			SetPause(true);
		}

	}
}
void ASpartaPlayerController::ShowGameOverMenu()
{
	//HUD�� ���� �ִٸ� �ݱ�
	if (HUDWidgetInstance)
	{
		HUDWidgetInstance->RemoveFromParent();
		HUDWidgetInstance = nullptr;
	}

	//�̹� �޴��� �������� ����
	if (MainMenuWidgetInstance)
	{
		MainMenuWidgetInstance->RemoveFromParent();
		MainMenuWidgetInstance = nullptr;
	}

	//�̹� ���� ���� �޴��� �������� ����
	if (GameOverMenuWidgetInstance)
	{
		GameOverMenuWidgetInstance->RemoveFromParent();
		GameOverMenuWidgetInstance = nullptr;
	}

	//���� ���� �޴� UI ����
	if (GameOverMenuWidgetClass)
	{
		GameOverMenuWidgetInstance = CreateWidget<UUserWidget>(this, GameOverMenuWidgetClass);
		if (GameOverMenuWidgetInstance)
		{
			GameOverMenuWidgetInstance->AddToViewport();

			bShowMouseCursor = true;
			SetInputMode(FInputModeUIOnly());
		}
		UFunction* PlayAnimFunc = GameOverMenuWidgetInstance->FindFunction(FName("PlayGameOverAnim"));
		if (PlayAnimFunc)
		{
			GameOverMenuWidgetInstance->ProcessEvent(PlayAnimFunc, nullptr);
		}
		if (UTextBlock* TotalScoreText = Cast<UTextBlock>(GameOverMenuWidgetInstance->GetWidgetFromName("TotalScoreText")))
		{
			if (USpartaGameInstance* SpartaGameInstance = Cast<USpartaGameInstance>(UGameplayStatics::GetGameInstance(this)))
			{
				TotalScoreText->SetText(FText::FromString(FString::Printf(TEXT("Total Score: %d"), SpartaGameInstance->TotalScore)));
			}
		}
	}
}

//���� ���� - BasicLevel ����, GameInstance ����
void ASpartaPlayerController::StartGame()
{
	if (USpartaGameInstance* SpartaGameInstance = Cast<USpartaGameInstance>(UGameplayStatics::GetGameInstance(this)))
	{
		/*SpartaGameInstance->CurrentLevelIndex = 0;*/
		SpartaGameInstance->TotalScore = 0;
	}

	UGameplayStatics::OpenLevel(GetWorld(), FName("BasicLevel"));
	SetPause(false);
	
}

void ASpartaPlayerController::BeginPlay()
{
	Super::BeginPlay();

	//���� PlayerController�� ����� Local Player ��ü ������
	if (ULocalPlayer* LocalPlayer = GetLocalPlayer())
	{
		//Local Player���� EnhancedInputLocalPlayerSubsystem ȹ��
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem = LocalPlayer->GetSubsystem<UEnhancedInputLocalPlayerSubsystem>())
		{
			if (InputMappingContext)
			{
				//Subsystem���� �츮�� �Ҵ��� IMC Ȱ��ȭ
				//�켱������ 0���� ���� ����
				Subsystem->AddMappingContext(InputMappingContext, 0);
			}
		}
	}

	//���� ���� �� �޴� �������� �޴� UI ���� ǥ��
	FString CurrentMapName = GetWorld()->GetMapName();
	if (CurrentMapName.Contains("MenuLevel"))
	{
		ShowMainMenu();
	}

	////HUD ���� ���� �� ǥ��
	//if (HUDWidgetClass)
	//{
	//	HUDWidgetInstance = CreateWidget<UUserWidget>(this, HUDWidgetClass);
	//	if (HUDWidgetInstance)
	//	{
	//		HUDWidgetInstance->AddToViewport();
	//	}
	//}

	//ASpartaGameState* SpartaGameState = GetWorld() ? GetWorld()->GetGameState<ASpartaGameState>() : nullptr;
	//if (SpartaGameState)
	//{
	//	SpartaGameState->UpdateHUD();
	//}
}
