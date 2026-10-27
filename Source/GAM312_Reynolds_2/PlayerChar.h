// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Camera/CameraComponent.h"
#include "Resource_M.h"
#include "Kismet/GameplayStatics.h"
#include "BuildingPart.h"
#include "PlayerWidget.h"
#include "ObjectiveWidget.h"
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

	// Assigning Objective Widget Variables
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		UObjectiveWidget* objWidget;

	UPROPERTY()
		float objectsBuilt;

	UPROPERTY()
		float matsCollected;

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
	// Opening gameplay variables to Unreal Engine
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Interaction", meta = (ClampMin = "0.0"))
		float ResourceStaminaCost = 5.0f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Interaction", meta = (ClampMin = "0.0"))
		float InteractionDistance = 800.0f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Building", meta = (ClampMin = "0.0"))
		float BuildingPlacementDistance = 400.0f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Building", meta = (ClampMin = "0.0"))
		float BuildingRotationAngle = 90.0f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Player Stats", meta = (ClampMin = "0.0"))
		float MaximumHealthValue = 100.0f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Player Stats", meta = (ClampMin = "0.0"))
		float MaximumHungerValue = 100.0f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Player Stats", meta = (ClampMin = "0.0"))
		float MaximumStaminaValue = 100.0f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Player Stats", meta = (ClampMin = "0.0"))
		float StatsUpdateInterval = 2.0f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Player Stats", meta = (ClampMin = "0.0"))
		float HungerDecayAmount = 1.0f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Player Stats", meta = (ClampMin = "0.0"))
		float StaminaRegenerationAmount = 10.0f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Player Stats", meta = (ClampMin = "0.0"))
		float StarvationDamage = 3.0f;

	// Opening decal variables to Unreal Engine
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Interaction")
		FVector HitDecalSize = FVector(10.0f, 10.0f, 10.0f);

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Resource Collection|Decal")
		FRotator HitDecalRotation = FRotator(-90.0f, 0.0f, 0.0f);

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Interaction", meta = (ClampMin = "0.0"))
		float HitDecalLifetime = 2.0f;

};
