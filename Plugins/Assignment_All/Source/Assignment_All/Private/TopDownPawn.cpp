// Fill out your copyright notice in the Description page of Project Settings.


#include "TopDownPawn.h"
#include "Components/CapsuleComponent.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "InputAction.h"
#include "InputMappingContext.h"
#include "InputModifiers.h"
#include "GameFramework/FloatingPawnMovement.h"
#include "GameFramework/SpringArmComponent.h"

ATopDownPawn::ATopDownPawn()
{

	CameraSpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraSpringArm"));
	CameraSpringArm->SetupAttachment(RootComponent);
	CameraSpringArm->SetUsingAbsoluteRotation(true);
	CameraSpringArm->TargetArmLength = -211.599960;
	CameraSpringArm->TargetOffset = FVector(-520, 0, 540);
	CameraSpringArm->bDoCollisionTest = true;

	float Angle = FMath::RadiansToDegrees(atan(CameraSpringArm->TargetOffset.Z / CameraSpringArm->TargetArmLength));

	TopDownCameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("TopDownCamera"));
	TopDownCameraComponent->SetupAttachment(CameraSpringArm, USpringArmComponent::SocketName);
	TopDownCameraComponent->SetRelativeRotation(FRotator(-Angle, 0, 0));

	PrimaryActorTick.bCanEverTick = true;
	PrimaryActorTick.bStartWithTickEnabled = true;

	Movement = CreateDefaultSubobject<UFloatingPawnMovement>(TEXT("Movement"));

	MoveScale = 2.0f;
}

static void KeyMap(UInputMappingContext* InputMappingContext, UInputAction* InputAction, FKey Key,
	bool bNegate = false,
	bool bSwizzle = false, EInputAxisSwizzle SwizzleOrder = EInputAxisSwizzle::YXZ)
{
	FEnhancedActionKeyMapping& Mapping = InputMappingContext->MapKey(InputAction, Key);
	UObject* Outer = InputMappingContext->GetOuter();

	if (bNegate)
	{
		UInputModifierNegate* Negate = NewObject<UInputModifierNegate>(Outer);
		Mapping.Modifiers.Add(Negate);
	}

	if (bSwizzle)
	{
		UInputModifierSwizzleAxis* Swizzle = NewObject<UInputModifierSwizzleAxis>(Outer);
		Swizzle->Order = SwizzleOrder;
		Mapping.Modifiers.Add(Swizzle);
	}
}

void ATopDownPawn::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	TopDownPawnMappingContext = NewObject<UInputMappingContext>(this);

	MoveAction = NewObject<UInputAction>(this);
	MoveAction->ValueType = EInputActionValueType::Axis2D;
	KeyMap(TopDownPawnMappingContext, MoveAction, EKeys::A, true, true);
	KeyMap(TopDownPawnMappingContext, MoveAction, EKeys::D, false, true);
	KeyMap(TopDownPawnMappingContext, MoveAction, EKeys::S, true);
	KeyMap(TopDownPawnMappingContext, MoveAction, EKeys::W);


	ZoomInOutAction = NewObject<UInputAction>(this);
	ZoomInOutAction->ValueType = EInputActionValueType::Axis1D;

	KeyMap(TopDownPawnMappingContext, ZoomInOutAction, EKeys::MouseWheelAxis);

	UEnhancedInputComponent* EIC = Cast<UEnhancedInputComponent>(PlayerInputComponent);

	APlayerController* FPC = Cast<APlayerController>(Controller);

	check(EIC && FPC);

	EIC->BindAction(MoveAction, ETriggerEvent::Triggered, this, &ATopDownPawn::Move);
	EIC->BindAction(ZoomInOutAction, ETriggerEvent::Triggered, this, &ATopDownPawn::ZoomInOut);

	ULocalPlayer* LocalPlayer = FPC->GetLocalPlayer();
	check(LocalPlayer);

	UEnhancedInputLocalPlayerSubsystem* Subsystem = LocalPlayer->GetSubsystem<UEnhancedInputLocalPlayerSubsystem>();
	check(Subsystem);

	Subsystem->AddMappingContext(TopDownPawnMappingContext, 0);

}


void ATopDownPawn::Move(const FInputActionValue& ActionValue)
{
	FVector ActionVal = ActionValue.Get<FInputActionValue::Axis3D>();
	ActionVal.Z = 0;
	AddMovementInput(ActionVal, MoveScale);
}

void ATopDownPawn::ZoomInOut(const FInputActionValue& ActionValue)
{

	float Value = ActionValue.Get<float>();

	float TargetArmLength = CameraSpringArm->TargetArmLength;
	float TargetOffsetZ = CameraSpringArm->TargetOffset.Z;

	if (TargetArmLength >= -460) {
		CameraSpringArm->TargetOffset.Z -= Value * 20;
	}
	if (TargetOffsetZ <= 1000) {
		CameraSpringArm->TargetArmLength -= Value * 9.2;
	}

	CameraSpringArm->TargetArmLength = FMath::Clamp(CameraSpringArm->TargetArmLength, -500, -0);
	CameraSpringArm->TargetOffset.Z = FMath::Clamp(CameraSpringArm->TargetOffset.Z, 0, 4000);

	float Angle;
	if (CameraSpringArm->TargetArmLength == 0) {
		Angle = -90;
	}
	else {
		Angle = FMath::RadiansToDegrees(atan(CameraSpringArm->TargetOffset.Z / CameraSpringArm->TargetArmLength));
		Angle = FMath::Clamp(Angle, -90, 0);
	}

	if (Angle < -90) {
		Angle = -90;
	}
	if (Angle > 0) {
		Angle = 0;
	}

	TopDownCameraComponent->SetRelativeRotation(FRotator(Angle, 0, 0));

}

void ATopDownPawn::BeginPlay()

{
	Super::BeginPlay();
	
}

void ATopDownPawn::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}


