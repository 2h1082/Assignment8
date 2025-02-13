// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameState.h"
#include "SpartaGameState.generated.h"

/**
 * 
 */
UCLASS()
class SPARTAPROJECT_API ASpartaGameState : public AGameState
{
	GENERATED_BODY()
	
public:
	ASpartaGameState();

	virtual void BeginPlay() override;
	//전역 점수 저장할 변수
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Score")
	int32 Score;
	//현재 스폰된 코인 개수
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Coin")
	int32 SpawnedCoinCount;
	//플레이어가 수집한 코인 개수
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Coin")
	int32 CollectedCoinCount;
	//각 레벨이 유지되는 시간(초)
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Wave")
	float WaveDuration;
	//현재 진행 중인 레벨 인덱스
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Wave")
	int32 CurrentWaveIndex;
	//전체 레벨의 개수
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Wave")
	int32 MaxWaves;
	//실제 레벨 맵 이름 배열, 여기 있는 인덱스를 차례대로 연동
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Wave")
	TArray<FName> LevelMapNames;

	//매 레벨이 끝나기 전까지 시간이 흐르도록 관리하는 타이머
	FTimerHandle WaveTimerHandle;
	FTimerHandle HUDUpdateTimerHandle;

	//점수를 읽는 함수
	UFUNCTION(BlueprintPure, Category = "Score")
	int32 GetScore() const;
	//점수를 추가하는 함수
	UFUNCTION(BlueprintCallable, Category = "Score")
	void AddScore(int32 Amount);
	//게임이 완전히 끝났을 때 실행되는 함수
	UFUNCTION(BlueprintCallable, Category = "Level")
	void OnGameOver();

	//레벨 시작할 때, 아이템 스폰 및 타이머 설정
	void StartWave();
	//레벨 제한 시간 만료 시 호출
	void OnWaveTimeUp();
	//코인 주웠을 때 호출
	void OnCoinCollected();
	//레벨을 강제 종료 후 다음 레벨 이동
	void EndWave();

	void UpdateHUD();
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;
};
