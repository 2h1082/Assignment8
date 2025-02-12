// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ItemSpawnRow.h"
#include "SpawnVolume.generated.h"

//박스 컴포넌트 전방선언
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
	//스폰 영역(박스 컴포넌트)
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Spawning")
	UBoxComponent* SpawningBox;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Spawning")
	UDataTable* ItemDataTable;

	//스폰 볼륨 내에서 무작위 좌표 얻어오는 함수
	UFUNCTION(BlueprintCallable,Category="Spawning")
	FVector GetRandomPointVolume() const;

	FItemSpawnRow* GetRandomItem() const;

	//특정 아이템 클래스를 스폰하는 함수
	AActor* SpawnItem(TSubclassOf<AActor> ItemClass);

};
