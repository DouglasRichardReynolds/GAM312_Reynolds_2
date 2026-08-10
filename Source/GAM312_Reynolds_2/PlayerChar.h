// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Camera/CameraComponent.h"
#include "Resource_M.h"
#include "Kismet/GameplayStatics.h"
#include "BuildingPart.h"
#include "PlayerWidget.h"
#include "PlayerChar.generated.h"

UCLASS()
class GAM312_REYNOLDS_2_API APlayerChar : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	APlayerChar();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;


	// Assigning PlayerChar Functions for Unreal to use
	UFUNCTION()
		void MoveForward(float axisValue);

	UFUNCTION()
		void MoveRight(float axisValue);

	UFUNCTION()
		void StartJump();

	UFUNCTION()
		void StopJump();

	UFUNCTION()
		void FindObject();

	UPROPERTY(VisibleAnywhere)
		UCameraComponent* PlayerCamComp;

	// Assigning PlayerChar Stats
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Player Stats")
		float Health = 100.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Player Stats")
		float Hunger = 100.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Player Stats")
		float Stamina = 100.0f;
		
	// Assigning Resources
	UPROPERTY(EditAnywhere, Category = "Resources")
		int Wood;

	UPROPERTY(EditAnywhere, Category = "Resources")
		int Stone;

	UPROPERTY(EditAnywhere, Category = "Resources")
		int Berry;

	// Assigning Resources Amount array
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Resources")
		TArray<int> ResourcesArray;

	// Assigning Resource Name array
	UPROPERTY(EditAnywhere, Category = "Resources")
		TArray<FString> ResourcesNameArray;

	// Assigning Hit Marker
	UPROPERTY(EditAnywhere, Category = "HitMarker")
		UMaterialInterface* hitDecal;

	// Assinging array of building supplies
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Building Supplies")
		TArray<int> BuildingArray;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
		TSubclassOf<ABuildingPart> BuildPartClass;

	UPROPERTY()
		bool isBuilding;

	// Assigning variable to spawned part
	UPROPERTY()
		ABuildingPart* spawnedPart;

	// Assigning Player Widget
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		UPlayerWidget* PlayerUI;

	// Assigning Functions that interact with PlayerChar stats
	UFUNCTION(BlueprintCallable)
		void SetHealth(float amount);

	UFUNCTION(BlueprintCallable)
		void SetHunger(float amount);

	UFUNCTION(BlueprintCallable)
		void SetStamina(float amount);

	UFUNCTION(BlueprintCallable)
		void DecreaseStats();

	// Assigning resource collection function
	UFUNCTION()
		void GiveResource(float amount, FString resourceType);

	// Assinging resource update function
	UFUNCTION(BlueprintCallable)
		void UpdateResources(float woodAmount, float stoneAmount, FString buildingObject);

	// Assinging functions to handle buildings
	UFUNCTION(BlueprintCallable)
		void SpawnBuilding(int buildingID, bool& isSuccess);

	UFUNCTION()
		void RotateBuilding();
};
