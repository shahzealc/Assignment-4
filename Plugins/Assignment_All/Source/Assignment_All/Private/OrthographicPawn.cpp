#include "OrthographicPawn.h"


void AOrthographicPawn::KeyMap(UInputMappingContext* InputMappingContext, UInputAction* InputAction, FKey Key,
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

AOrthographicPawn::AOrthographicPawn()
{

	PrimaryActorTick.bCanEverTick = true;
	USceneComponent* Scene = CreateDefaultSubobject<USceneComponent>(TEXT("Scene"));
	SetRootComponent(Scene);
	SpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("Spring Arm"));
	SpringArm->SetRelativeRotation(FRotator(-90.0f, 0.0f, 0.0f));
	SpringArm->SetRelativeLocation(FVector(0.0f, 0.0f, 300.0f));
	SpringArm->TargetArmLength = 1000.0f;
	SpringArm->SetupAttachment(Scene);
	SpringArm->bDoCollisionTest = false;

	Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	Camera->SetupAttachment(SpringArm, USpringArmComponent::SocketName);
	Camera->SetProjectionMode(ECameraProjectionMode::Orthographic);

	Movement = CreateDefaultSubobject<UFloatingPawnMovement>(TEXT("Movement"));
	MoveScale = 10.0f;
	ZoomScale = 100.0f;

	Camera->OrthoWidth = 800;
}

void AOrthographicPawn::BeginPlay()
{
	Super::BeginPlay();

}

void AOrthographicPawn::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AOrthographicPawn::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	PawnMappingContext = NewObject<UInputMappingContext>(this);
	MoveAction = NewObject<UInputAction>(this);
	MoveAction->ValueType = EInputActionValueType::Axis3D;
	KeyMap(PawnMappingContext, MoveAction, EKeys::W);
	KeyMap(PawnMappingContext, MoveAction, EKeys::S, true);
	KeyMap(PawnMappingContext, MoveAction, EKeys::A, true, true);
	KeyMap(PawnMappingContext, MoveAction, EKeys::D, false, true);
	KeyMap(PawnMappingContext, MoveAction, EKeys::SpaceBar, false, true, EInputAxisSwizzle::ZYX);
	KeyMap(PawnMappingContext, MoveAction, EKeys::LeftShift, true, true, EInputAxisSwizzle::ZYX);

	ZoomAction = NewObject<UInputAction>(this);
	ZoomAction->ValueType = EInputActionValueType::Axis1D;
	KeyMap(PawnMappingContext, ZoomAction, EKeys::MouseWheelAxis);


	UEnhancedInputComponent* EIC = Cast<UEnhancedInputComponent>(PlayerInputComponent);
	APlayerController* FPC = Cast<APlayerController>(Controller);
	check(EIC);
	EIC->BindAction(MoveAction, ETriggerEvent::Triggered, this, &AOrthographicPawn::Move);
	EIC->BindAction(ZoomAction, ETriggerEvent::Triggered, this, &AOrthographicPawn::Zoom);
	ULocalPlayer* LocalPlayer = FPC->GetLocalPlayer();
	check(LocalPlayer);
	UEnhancedInputLocalPlayerSubsystem* Subsystem = LocalPlayer->GetSubsystem<UEnhancedInputLocalPlayerSubsystem>();
	check(Subsystem);
	Subsystem->ClearAllMappings();
	Subsystem->AddMappingContext(PawnMappingContext, 0);
}

void AOrthographicPawn::Move(const FInputActionValue& ActionValue)
{
	FVector MovementValue = ActionValue.Get<FVector>();
	FVector DeltaLocation = FVector(MovementValue.X, MovementValue.Y, 0.0f) * MoveScale;
	AddActorLocalOffset(DeltaLocation, true);
}

void AOrthographicPawn::Zoom(const FInputActionValue& ActionValue)
{
	float ZoomValue = ActionValue.Get<float>();
	float NewOrthoWidth = Camera->OrthoWidth - (ZoomValue * ZoomScale);
	Camera->OrthoWidth = FMath::Clamp(NewOrthoWidth, 512.0f, 4096.0f);
}