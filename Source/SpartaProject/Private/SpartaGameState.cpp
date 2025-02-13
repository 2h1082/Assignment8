// Fill out your copyright notice in the Description page of Project Settings.


#include "SpartaGameState.h"
#include "Kismet/GameplayStatics.h"
#include "SpawnVolume.h"
#include "SpartaGameInstance.h"
#include "CoinItem.h"
#include "SpartaPlayerController.h"
#include "Components/TextBlock.h"
#include "Blueprint/UserWidget.h"
#include "SpartaCharacter.h"
#include "Components/Image.h"

ASpartaGameState::ASpartaGameState()
{
	Score = 0;
	SpawnedCoinCount = 0;
	CollectedCoinCount = 0;
	WaveDuration = 30.0f;
	CurrentWaveIndex = 0;
	MaxWaves = 3;
}

void ASpartaGameState::BeginPlay()
{
	Super::BeginPlay();

	//UpdateHUD();
	StartWave();

	GetWorldTimerManager().SetTimer(
		HUDUpdateTimerHandle,
		this,
		&ASpartaGameState::UpdateHUD,
		0.1f,
		true
	);
}

int32 ASpartaGameState::GetScore() const
{
	return Score;
}

void ASpartaGameState::AddScore(int32 Amount)
{
	if (UGameInstance* GameInstance = GetGameInstance())
	{
		USpartaGameInstance* SpartaGameInstance = Cast<USpartaGameInstance>(GameInstance);
		if (SpartaGameInstance)
		{
			SpartaGameInstance->AddToScore(Amount);
		}
	}
	Score += Amount;
	UE_LOG(LogTemp, Warning, TEXT("Score: %d"), Score);
}

void ASpartaGameState::StartWave()
{
	GetWorldTimerManager().ClearTimer(WaveTimerHandle);
	if (APlayerController* PlayerController = GetWorld()->GetFirstPlayerController())
	{
		if (ASpartaPlayerController* SpartaPlayerController = Cast<ASpartaPlayerController>(PlayerController))
		{
			SpartaPlayerController->ShowGameHUD();
		}
	}

	/*if (UGameInstance* GameInstance = GetGameInstance())
	{
		USpartaGameInstance* SpartaGameInstance = Cast<USpartaGameInstance>(GameInstance);
		if (SpartaGameInstance)
		{
			CurrentWaveIndex = SpartaGameInstance->CurrentLevelIndex;
		}
	}*/

	//���� ���� �� ���� ���� �ʱ�ȭ
	SpawnedCoinCount = 0;
	CollectedCoinCount = 0;

	//���� �ʿ� ��ġ�� ��� SpawnVolume ã�� Wave�� �´� ������ ������ŭ ����
	TArray<AActor*> FoundVolumes;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), ASpawnVolume::StaticClass(), FoundVolumes);

	//Wave�� ���� ������ ���� ���� ����
	const int32 ItemToSpawn = 40+5*(CurrentWaveIndex+1);
	UE_LOG(LogTemp, Warning, TEXT("Spawned Item: %d"), ItemToSpawn);
	for (int32 i = 0; i < ItemToSpawn; ++i)
	{
		if (FoundVolumes.Num() > 0)
		{
			ASpawnVolume* SpawnVolume = Cast<ASpawnVolume>(FoundVolumes[0]);
			if (SpawnVolume)
			{
				AActor* SpawnedActor = SpawnVolume->SpawnRandomItem();
				//������ ���� ����Ÿ���̸� ���� ����
				if (SpawnedActor && SpawnedActor->IsA(ACoinItem::StaticClass()))
				{
					SpawnedCoinCount++;
				}
			}
		}
	}

	//30�� �Ŀ� OnWaveTimeUp()�� ȣ��ǵ��� Ÿ�̸� ����
	GetWorldTimerManager().SetTimer(
		WaveTimerHandle,
		this,
		&ASpartaGameState::OnWaveTimeUp,
		WaveDuration,
		false
	);
	FString CurrentMapName = GetWorld()->GetMapName();
	if (CurrentMapName.Contains("BasicLevel"))
	{
		UE_LOG(LogTemp, Warning, TEXT("Wave %d Start!!!"),CurrentWaveIndex+1);
	}
	//UpdateHUD();
}

void ASpartaGameState::OnCoinCollected()
{
	CollectedCoinCount++;
	UE_LOG(LogTemp, Warning, TEXT("Coin Collected: %d / %d"), CollectedCoinCount, SpawnedCoinCount);

	//���� ������ ������ ������ ���� �ֿ�� ��� ���̺� ����
	if (SpawnedCoinCount > 0 && CollectedCoinCount >= SpawnedCoinCount)
	{
		EndWave();
	}
}

void ASpartaGameState::OnWaveTimeUp()
{
	EndWave();
}


void ASpartaGameState::EndWave()
{
	//Ÿ�̸� ����
	GetWorldTimerManager().ClearTimer(WaveTimerHandle);

	if (UGameInstance* GameInstance = GetGameInstance())
	{
		USpartaGameInstance* SpartaGameInstance = Cast<USpartaGameInstance>(GameInstance);
		if (SpartaGameInstance)
		{
			AddScore(Score);
			//���� Wave �ε���
			CurrentWaveIndex++;
		/*	SpartaGameInstance->CurrentLevelIndex=CurrentWaveIndex;*/
		}
	}

	//��� Wave �� �������� ���� ����
	if (CurrentWaveIndex >= MaxWaves)
	{
		OnGameOver();
		return;
	}
	else
	{
		//���� Wave�� �������� ���ο� Wave ����
		StartWave();
	}
	////���� �� �̸��� �ִٸ� �ش� �� �ҷ�����
	//if (LevelMapNames.IsValidIndex(CurrentWaveIndex))
	//{
	//	UGameplayStatics::OpenLevel(GetWorld(), LevelMapNames[CurrentWaveIndex]);
	//}
	//else
	//{
	//	//�� �̸� ������ ���� ����
	//	OnGameOver();
	//}
}


void ASpartaGameState::OnGameOver()
{
	if (APlayerController* PlayerController = GetWorld()->GetFirstPlayerController())
	{
		if (ASpartaPlayerController* SpartaPlayerController = Cast<ASpartaPlayerController>(PlayerController))
		{
			SpartaPlayerController->SetPause(true);
			SpartaPlayerController->ShowGameOverMenu();
		}
	}
}

void ASpartaGameState::UpdateHUD()
{
	if (APlayerController* PlayerController = GetWorld()->GetFirstPlayerController())
	{
		if (ASpartaPlayerController* SpartaPlayerController = Cast<ASpartaPlayerController>(PlayerController))
		{
			if (UUserWidget* HUDWidget = SpartaPlayerController->GetHUDWidget())
			{
				//�ð� ����
				if (UTextBlock* TimeText = Cast<UTextBlock>(HUDWidget->GetWidgetFromName(TEXT("TIme"))))
				{
					float RemainingTime = GetWorldTimerManager().GetTimerRemaining(WaveTimerHandle);
					TimeText->SetText(FText::FromString(FString::Printf(TEXT("Time: %.1f"), RemainingTime)));
				}
				//���� ����
				if (UTextBlock* ScoreText = Cast<UTextBlock>(HUDWidget->GetWidgetFromName(TEXT("Score"))))
				{
					if (UGameInstance* GameInstance = GetGameInstance())
					{
						USpartaGameInstance* SpartaGameInstance = Cast<USpartaGameInstance>(GameInstance);
						if (SpartaGameInstance)
						{
							ScoreText->SetText(FText::FromString(FString::Printf(TEXT("Score: %d"), SpartaGameInstance->TotalScore)));
						}
					}
				}
				//Wave ����
				if (UTextBlock* WaveIndexText = Cast<UTextBlock>(HUDWidget->GetWidgetFromName(TEXT("Wave"))))
				{
					WaveIndexText->SetText(FText::FromString(FString::Printf(TEXT("Wave: %d"), CurrentWaveIndex + 1)));
				}
				//HP ���� 
				if (UTextBlock* HPText = Cast<UTextBlock>(HUDWidget->GetWidgetFromName(TEXT("HP"))))
				{
					ASpartaCharacter* SpartaCharacter = Cast<ASpartaCharacter>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));
					if (SpartaCharacter)
					{
						HPText->SetText(FText::FromString(FString::Printf(TEXT("%.1f / %.1f"),SpartaCharacter->GetHealth(),SpartaCharacter->GetMaxHealth())));
					}
				}
				//����� ���� ����(�ӵ� ����)
				if (UImage* SlowImage = Cast<UImage>(HUDWidget->GetWidgetFromName(TEXT("SlowImage"))))
				{
					ASpartaCharacter* SpartaCharacter = Cast<ASpartaCharacter>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));
					if (SpartaCharacter)
					{
						if (SpartaCharacter->bIsSlow())
						{
							SlowImage->SetVisibility(ESlateVisibility::Visible);
						}
						else
						{
							SlowImage->SetVisibility(ESlateVisibility::Hidden);
						}
					}
				}
				//����� ���� ����(�ӵ� ����)
				if (UTextBlock* SlowText = Cast<UTextBlock>(HUDWidget->GetWidgetFromName(TEXT("SlowText"))))
				{
					ASpartaCharacter* SpartaCharacter = Cast<ASpartaCharacter>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));
					if (SpartaCharacter)
					{
						if (SpartaCharacter->bIsSlow())
						{
							SlowText->SetVisibility(ESlateVisibility::Visible);
						}
						else
						{
							SlowText->SetVisibility(ESlateVisibility::Hidden);
						}
						float Remaining=SpartaCharacter->GetWorldTimerManager().GetTimerRemaining(SpartaCharacter->SlowTimerHandle);
						SlowText->SetText(FText::FromString(FString::Printf(TEXT("%.1f"), Remaining)));
					}
				}

				//����� ���� ����(����)
				if (UImage* InversionImage = Cast<UImage>(HUDWidget->GetWidgetFromName(TEXT("InversionImage"))))
				{
					ASpartaCharacter* SpartaCharacter = Cast<ASpartaCharacter>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));
					if (SpartaCharacter)
					{
						if (SpartaCharacter->bIsInversion())
						{
							InversionImage->SetVisibility(ESlateVisibility::Visible);
						}
						else
						{
							InversionImage->SetVisibility(ESlateVisibility::Hidden);
						}
					}
				}
				//����� ���� ����(����)
				if (UTextBlock* InversionText = Cast<UTextBlock>(HUDWidget->GetWidgetFromName(TEXT("InversionText"))))
				{
					ASpartaCharacter* SpartaCharacter = Cast<ASpartaCharacter>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));
					if (SpartaCharacter)
					{
						if (SpartaCharacter->bIsInversion())
						{
							InversionText->SetVisibility(ESlateVisibility::Visible);
						}
						else
						{
							InversionText->SetVisibility(ESlateVisibility::Hidden);
						}
						float Remaining = SpartaCharacter->GetWorldTimerManager().GetTimerRemaining(SpartaCharacter->InversionTimerHandle);
						InversionText->SetText(FText::FromString(FString::Printf(TEXT("%.1f"), Remaining)));
					}
				}
			}
		}
	}
}

void ASpartaGameState::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	//Wave Ÿ�̸� ����
	GetWorldTimerManager().ClearTimer(WaveTimerHandle);
	//HUDupdate Ÿ�̸� ����
	GetWorldTimerManager().ClearTimer(HUDUpdateTimerHandle);
}
