// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerChar.h"

// Sets default values
APlayerChar::APlayerChar()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	PlayerCamComp = CreateDefaultSubobject<UCameraComponent>(TEXT("First Person Cam"));

	// Attaching camera to head of PlayerChar mesh
	PlayerCamComp->SetupAttachment(GetMesh(), "head");

	PlayerCamComp->bUsePawnControlRotation = true;

	// Setting up Resource arrays
	ResourcesArray.SetNum(3);
	ResourcesNameArray.Add(TEXT("Wood"));
	ResourcesNameArray.Add(TEXT("Stone"));
	ResourcesNameArray.Add(TEXT("Berry"));

	// Setting up Building array
	BuildingArray.SetNum(3);

}

// Called when the game starts or when spawned
void APlayerChar::BeginPlay()
{
	Super::BeginPlay();
	
	// Setting up Stats timer and connecting DecreaseStats function
	FTimerHandle StatsTimerHandle;
	GetWorld()->GetTimerManager().SetTimer(StatsTimerHandle, this, &APlayerChar::DecreaseStats, StatsUpdateInterval, true);

	// If Object Widget is valid
	if (objWidget)
	{
		// Setting intial values
		objWidget->UpdatebuildObj(0.0f);
		objWidget->UpdatematOBJ(0.0f);
	}

}

// Called every frame
void APlayerChar::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	// Updating stats every tick - Made values percentages
	PlayerUI->UpdateBars(Health, Hunger, Stamina);


	// Setting up building part placement
	if (isBuilding)
	{
		if (spawnedPart)
		{
			FVector StartLocation = PlayerCamComp->GetComponentLocation();
			FVector Direction = PlayerCamComp->GetForwardVector() * 400.0f;
			FVector EndLocation = StartLocation + Direction;
			spawnedPart->SetActorLocation(EndLocation);
		}
	}
}

// Called to bind functionality to input
void APlayerChar::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	// Attaching movement functions to PlayerChar
	PlayerInputComponent->BindAxis("MoveForward", this, &APlayerChar::MoveForward);
	PlayerInputComponent->BindAxis("MoveRight", this, &APlayerChar::MoveRight);
	PlayerInputComponent->BindAxis("LookUp", this, &APlayerChar::AddControllerPitchInput);
	PlayerInputComponent->BindAxis("Turn", this, &APlayerChar::AddControllerYawInput);

	// Attaching jump functions to PlayerChar
	PlayerInputComponent->BindAction("JumpEvent", IE_Pressed, this, &APlayerChar::StartJump);
	PlayerInputComponent->BindAction("JumpEvent", IE_Released, this, &APlayerChar::StopJump);

	// Attaching object interaction
	PlayerInputComponent->BindAction("Interact", IE_Pressed, this, &APlayerChar::FindObject);

	// Attaching part rotation
	PlayerInputComponent->BindAction("RotPart", IE_Pressed, this, &APlayerChar::RotateBuilding);
}

// Definining movement functions
void APlayerChar::MoveForward(float axisValue)
{
	FVector Direction = FRotationMatrix(Controller->GetControlRotation()).GetScaledAxis(EAxis::X);
	AddMovementInput(Direction, axisValue);
}

void APlayerChar::MoveRight(float axisValue)
{
	FVector Direction = FRotationMatrix(Controller->GetControlRotation()).GetScaledAxis(EAxis::Y);
	AddMovementInput(Direction, axisValue);
}

// Defining jump functions
void APlayerChar::StartJump()
{
	bPressedJump = true;
}

void APlayerChar::StopJump()
{
	bPressedJump = false;
}

// Definining line trace for object interaction
void APlayerChar::FindObject()
{
	// Complete building placement before checking resources
	if (isBuilding)
	{
		isBuilding = false;

		objectsBuilt += 1.0f;

		if (IsValid(objWidget))
		{
			objWidget->UpdatebuildObj(objectsBuilt);
		}

		return;
	}

	// Do not trace if the player lacks the required stamina
	if (Stamina <= ResourceStaminaCost)
	{
		return;
	}

	// Defining how far away a player can collect resources
	const FVector StartLocation = PlayerCamComp->GetComponentLocation();
	const FVector EndLocation = StartLocation + PlayerCamComp->GetForwardVector() * InteractionDistance;

	// Create collision settings
	FCollisionQueryParams QueryParams;

	// Ignore the player
	QueryParams.AddIgnoredActor(this);

	// Use complex collision to provide more precise hit detection
	QueryParams.bTraceComplex = true;

	FHitResult HitResult;

	// Check whether the player is looking at a resource
	if (!GetWorld()->LineTraceSingleByChannel(HitResult, StartLocation, EndLocation, ECC_Visibility, QueryParams))
	{
		return;
	}

	AResource_M* HitResource = Cast<AResource_M>(HitResult.GetActor());

	if (!IsValid(HitResource))
	{
		return;
	}

	const FString ResourceName = HitResource->resourceName;
	const int32 ResourceValue = HitResource->resourceAmount;

	HitResource->totalResources -= ResourceValue;

	// Remove the resource actor when depleted
	if (HitResource->totalResources <= ResourceValue)
	{
		HitResource->Destroy();
		return;
	}

	// Add the collected resource to the player's inventory
	GiveResource(ResourceValue, ResourceName);

	matsCollected += ResourceValue;

	if (IsValid(objWidget))
	{
		objWidget->UpdatematOBJ(matsCollected);
	}

	// Spawn the resource hit decal using configurable settings.
	if (hitDecal)
	{
		UGameplayStatics::SpawnDecalAtLocation(GetWorld(), hitDecal, HitDecalSize, HitResult.Location, HitDecalRotation, HitDecalLifetime);
	}

	// Apply the configurable stamina cost
	SetStamina(-ResourceStaminaCost);
}

// Defining stat functions
void APlayerChar::SetHealth(float amount)
{
	// Allows player to heal up to, but not past max health
	Health = FMath::Clamp(Health + amount, 0.0f, MaximumHealthValue);
}

void APlayerChar::SetHunger(float amount)
{
	// Allows player to recover hunger up to, but not past max hunger
	Hunger = FMath::Clamp(Hunger + amount, 0.0f, MaximumHungerValue);
}

void APlayerChar::SetStamina(float amount)
{
	// Allows player to recover stamina up to, but not past max stamina
	Stamina = FMath::Clamp(Stamina + amount, 0.0f, MaximumStaminaValue);
}

void APlayerChar::DecreaseStats()
{
	if (Hunger > 0.0f)
	{
		SetHunger(-HungerDecayAmount);
	}

	SetStamina(StaminaRegenerationAmount);

	if (Hunger <= 0.0f)
	{
		SetHealth(-StarvationDamage);
	}
}

// Defining resource collection
void APlayerChar::GiveResource(float amount, FString resourceType)
{
	if (resourceType == "Wood")
	{
		ResourcesArray[0] = ResourcesArray[0] + amount;
	}

	if (resourceType == "Stone")
	{
		ResourcesArray[1] = ResourcesArray[1] + amount;
	}

	if (resourceType == "Berry")
	{
		ResourcesArray[2] = ResourcesArray[2] + amount;
	}
}

// Defining building related functions
void APlayerChar::UpdateResources(float woodAmount, float stoneAmount, FString buildingObject)
{
	// Subtracting resources to build objects
	if (woodAmount <= ResourcesArray[0])
	{
		if (stoneAmount <= ResourcesArray[1])
		{
			ResourcesArray[0] = ResourcesArray[0] - woodAmount;
			ResourcesArray[1] = ResourcesArray[1] - stoneAmount;

			if (buildingObject == "Wall")
			{
				BuildingArray[0] = BuildingArray[0] + 1;
			}
			if (buildingObject == "Floor")
			{
				BuildingArray[1] = BuildingArray[1] + 1;
			}
			if (buildingObject == "Ceiling")
			{
				BuildingArray[2] = BuildingArray[2] + 1;
			}
		}
	}
}

void APlayerChar::SpawnBuilding(int buildingID, bool& isSuccess)
{
	if (!isBuilding)
	{
		if (BuildingArray[buildingID] >= 1)
		{
			// Starting to build, spawns object
			isBuilding = true;

			FActorSpawnParameters SpawnParams;

			FVector StartLocation = PlayerCamComp->GetComponentLocation();

			const FVector Direction = PlayerCamComp->GetForwardVector() * BuildingPlacementDistance;

			FVector EndLocation = StartLocation + Direction;

			FRotator myRot(0, 0, 0);

			BuildingArray[buildingID] = BuildingArray[buildingID] - 1;

			spawnedPart = GetWorld()->SpawnActor<ABuildingPart>(BuildPartClass, EndLocation, myRot, SpawnParams);

			isSuccess = true;
		}

		isSuccess = false;
	}
}

// Rotates building placement
void APlayerChar::RotateBuilding()
{
	// Only rotate the building while it is being placed
	if (isBuilding && IsValid(spawnedPart))
	{
		spawnedPart->AddActorWorldRotation(FRotator(0.0f, BuildingRotationAngle, 0.0f));
	}
}
