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
	//���� ���� ������ ����
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Score")
	int32 Score;
	//���� ������ ���� ����
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Coin")
	int32 SpawnedCoinCount;
	//�÷��̾ ������ ���� ����
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Coin")
	int32 CollectedCoinCount;
	//�� ������ �����Ǵ� �ð�(��)
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Wave")
	float WaveDuration;
	//���� ���� ���� ���� �ε���
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Wave")
	int32 CurrentWaveIndex;
	//��ü ������ ����
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Wave")
	int32 MaxWaves;
	//���� ���� �� �̸� �迭, ���� �ִ� �ε����� ���ʴ�� ����
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Wave")
	TArray<FName> LevelMapNames;

	//�� ������ ������ ������ �ð��� �帣���� �����ϴ� Ÿ�̸�
	FTimerHandle WaveTimerHandle;
	FTimerHandle HUDUpdateTimerHandle;

	//������ �д� �Լ�
	UFUNCTION(BlueprintPure, Category = "Score")
	int32 GetScore() const;
	//������ �߰��ϴ� �Լ�
	UFUNCTION(BlueprintCallable, Category = "Score")
	void AddScore(int32 Amount);
	//������ ������ ������ �� ����Ǵ� �Լ�
	UFUNCTION(BlueprintCallable, Category = "Level")
	void OnGameOver();

	//���� ������ ��, ������ ���� �� Ÿ�̸� ����
	void StartWave();
	//���� ���� �ð� ���� �� ȣ��
	void OnWaveTimeUp();
	//���� �ֿ��� �� ȣ��
	void OnCoinCollected();
	//������ ���� ���� �� ���� ���� �̵�
	void EndWave();

	void UpdateHUD();
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;
};
