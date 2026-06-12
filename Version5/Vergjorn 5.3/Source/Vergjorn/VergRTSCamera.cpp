#include "VergRTSCamera.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "GameFramework/FloatingPawnMovement.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"

AVergRTSCamera::AVergRTSCamera()
{
	PrimaryActorTick.bCanEverTick = true;

	CameraRoot = CreateDefaultSubobject<USceneComponent>(TEXT("CameraRoot"));
	SetRootComponent(CameraRoot);

	SpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArm"));
	SpringArm->SetupAttachment(CameraRoot);
	SpringArm->TargetArmLength = TargetArmLength;
	SpringArm->SetRelativeRotation(FRotator(-55.f, 0.f, 0.f));
	SpringArm->bDoCollisionTest = false;
	SpringArm->bInheritPitch    = false;
	SpringArm->bInheritRoll     = false;

	Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	Camera->SetupAttachment(SpringArm, USpringArmComponent::SocketName);

	Movement = CreateDefaultSubobject<UFloatingPawnMovement>(TEXT("Movement"));
	Movement->MaxSpeed    = PanSpeed;
	Movement->Acceleration = PanSpeed * 4.f;
	Movement->Deceleration = PanSpeed * 8.f;
}

void AVergRTSCamera::BeginPlay()
{
	Super::BeginPlay();
	TargetArmLength = SpringArm->TargetArmLength;

	if (APlayerController* PC = Cast<APlayerController>(GetController()))
	{
		if (UEnhancedInputLocalPlayerSubsystem* Sub =
			ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PC->GetLocalPlayer()))
		{
			if (IMC_Camera) Sub->AddMappingContext(IMC_Camera, 0);
		}
	}
}

void AVergRTSCamera::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	ApplyEdgeScroll(DeltaTime);
	ApplyZoomLerp(DeltaTime);
}

void AVergRTSCamera::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	if (UEnhancedInputComponent* EIC = Cast<UEnhancedInputComponent>(PlayerInputComponent))
	{
		if (IA_Pan)    EIC->BindAction(IA_Pan,    ETriggerEvent::Triggered, this, &AVergRTSCamera::HandlePan);
		if (IA_Zoom)   EIC->BindAction(IA_Zoom,   ETriggerEvent::Triggered, this, &AVergRTSCamera::HandleZoom);
		if (IA_Rotate) EIC->BindAction(IA_Rotate, ETriggerEvent::Triggered, this, &AVergRTSCamera::HandleRotate);
	}
}

void AVergRTSCamera::HandlePan(const FInputActionValue& Value)
{
	FVector2D Input = Value.Get<FVector2D>();
	if (Input.IsNearlyZero()) return;

	FVector Right   = GetActorRightVector()   * Input.X;
	FVector Forward = GetActorForwardVector() * Input.Y;
	Right.Z = Forward.Z = 0.f;
	AddMovementInput(Right + Forward, 1.f);
}

void AVergRTSCamera::HandleZoom(const FInputActionValue& Value)
{
	TargetArmLength = FMath::Clamp(
		TargetArmLength - Value.Get<float>() * ZoomSpeed,
		MinArmLength, MaxArmLength);
}

void AVergRTSCamera::HandleRotate(const FInputActionValue& Value)
{
	AddActorLocalRotation(FRotator(0.f, Value.Get<float>() * RotateSpeed * GetWorld()->GetDeltaSeconds(), 0.f));
}

void AVergRTSCamera::ApplyEdgeScroll(float DeltaTime)
{
	APlayerController* PC = Cast<APlayerController>(GetController());
	if (!PC) return;

	int32 SizeX, SizeY;
	PC->GetViewportSize(SizeX, SizeY);

	float MouseX, MouseY;
	if (!PC->GetMousePosition(MouseX, MouseY)) return;

	FVector2D Dir = FVector2D::ZeroVector;
	if (MouseX <= EdgeScrollMargin)                Dir.X = -1.f;
	else if (MouseX >= SizeX - EdgeScrollMargin)   Dir.X =  1.f;
	if (MouseY <= EdgeScrollMargin)                Dir.Y =  1.f;
	else if (MouseY >= SizeY - EdgeScrollMargin)   Dir.Y = -1.f;

	if (!Dir.IsNearlyZero())
	{
		FVector Right   = GetActorRightVector()   * Dir.X;
		FVector Forward = GetActorForwardVector() * Dir.Y;
		Right.Z = Forward.Z = 0.f;
		AddMovementInput((Right + Forward).GetSafeNormal(), EdgeScrollSpeed * DeltaTime);
	}
}

void AVergRTSCamera::ApplyZoomLerp(float DeltaTime)
{
	SpringArm->TargetArmLength = FMath::FInterpTo(
		SpringArm->TargetArmLength, TargetArmLength, DeltaTime, 8.f);
}
