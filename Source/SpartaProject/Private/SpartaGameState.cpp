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

	//레벨 시작 시 코인 개수 초기화
	SpawnedCoinCount = 0;
	CollectedCoinCount = 0;

	//현재 맵에 배치된 모든 SpawnVolume 찾아 Wave에 맞는 아이템 개수만큼 스폰
	TArray<AActor*> FoundVolumes;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), ASpawnVolume::StaticClass(), FoundVolumes);

	//Wave에 따라 아이템 스폰 개수 증가
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
				//스폰된 액터 코인타입이면 개수 증가
				if (SpawnedActor && SpawnedActor->IsA(ACoinItem::StaticClass()))
				{
					SpawnedCoinCount++;
				}
			}
		}
	}

	//30초 후에 OnWaveTimeUp()이 호출되도록 타이머 설정
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

	//현재 레벨에 스폰된 코인을 전부 주우면 즉시 웨이브 종료
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
	//타이머 해제
	GetWorldTimerManager().ClearTimer(WaveTimerHandle);

	if (UGameInstance* GameInstance = GetGameInstance())
	{
		USpartaGameInstance* SpartaGameInstance = Cast<USpartaGameInstance>(GameInstance);
		if (SpartaGameInstance)
		{
			AddScore(Score);
			//다음 Wave 인덱스
			CurrentWaveIndex++;
		/*	SpartaGameInstance->CurrentLevelIndex=CurrentWaveIndex;*/
		}
	}

	//모든 Wave 다 돌았으면 게임 오버
	if (CurrentWaveIndex >= MaxWaves)
	{
		OnGameOver();
		return;
	}
	else
	{
		//아직 Wave가 남았으니 새로운 Wave 시작
		StartWave();
	}
	////레벨 맵 이름이 있다면 해당 맵 불러오기
	//if (LevelMapNames.IsValidIndex(CurrentWaveIndex))
	//{
	//	UGameplayStatics::OpenLevel(GetWorld(), LevelMapNames[CurrentWaveIndex]);
	//}
	//else
	//{
	//	//맵 이름 없으면 게임 오버
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
				//시간 상태
				if (UTextBlock* TimeText = Cast<UTextBlock>(HUDWidget->GetWidgetFromName(TEXT("TIme"))))
				{
					float RemainingTime = GetWorldTimerManager().GetTimerRemaining(WaveTimerHandle);
					TimeText->SetText(FText::FromString(FString::Printf(TEXT("Time: %.1f"), RemainingTime)));
				}
				//점수 상태
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
				//Wave 상태
				if (UTextBlock* WaveIndexText = Cast<UTextBlock>(HUDWidget->GetWidgetFromName(TEXT("Wave"))))
				{
					WaveIndexText->SetText(FText::FromString(FString::Printf(TEXT("Wave: %d"), CurrentWaveIndex + 1)));
				}
				//HP 상태 
				if (UTextBlock* HPText = Cast<UTextBlock>(HUDWidget->GetWidgetFromName(TEXT("HP"))))
				{
					ASpartaCharacter* SpartaCharacter = Cast<ASpartaCharacter>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));
					if (SpartaCharacter)
					{
						HPText->SetText(FText::FromString(FString::Printf(TEXT("%.1f / %.1f"),SpartaCharacter->GetHealth(),SpartaCharacter->GetMaxHealth())));
					}
				}
				//디버프 관련 상태(속도 감소)
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
				//디버프 관련 상태(속도 감소)
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

				//디버프 관련 상태(반전)
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
				//디버프 관련 상태(반전)
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
	//Wave 타이머 해제
	GetWorldTimerManager().ClearTimer(WaveTimerHandle);
	//HUDupdate 타이머 해제
	GetWorldTimerManager().ClearTimer(HUDUpdateTimerHandle);
}
