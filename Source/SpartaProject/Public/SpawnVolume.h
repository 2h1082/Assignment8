// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ItemSpawnRow.h"
#include "SpawnVolume.generated.h"

//�ڽ� ������Ʈ ���漱��
class UBoxComponent;

UCLASS()
class SPARTAPROJECT_API ASpawnVolume : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ASpawnVolume();

	UFUNCTION(BlueprintCallable, Category = "Spawning")
	AActor* SpawnRandomItem();


protected:
	UPROPERTY(VisibleAnywhere,BlueprintReadOnly,Category="Spawning")
	USceneComponent* Scene;
	//���� ����(�ڽ� ������Ʈ)
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Spawning")
	UBoxComponent* SpawningBox;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Spawning")
	UDataTable* ItemDataTable;

	//���� ���� ������ ������ ��ǥ ������ �Լ�
	UFUNCTION(BlueprintCallable,Category="Spawning")
	FVector GetRandomPointVolume() const;

	FItemSpawnRow* GetRandomItem() const;

	//Ư�� ������ Ŭ������ �����ϴ� �Լ�
	AActor* SpawnItem(TSubclassOf<AActor> ItemClass);

};
