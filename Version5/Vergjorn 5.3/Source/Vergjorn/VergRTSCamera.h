#pragma once
#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "InputActionValue.h"
#include "VergRTSCamera.generated.h"

class UInputMappingContext;
class UInputAction;
class UCameraComponent;
class USpringArmComponent;
class UFloatingPawnMovement;

// Top-down RTS camera: pan, zoom, rotate, edge-scroll
UCLASS(BlueprintType, Blueprintable)
class VERGJORN_API AVergRTSCamera : public APawn
{
	GENERATED_BODY()
public:
	AVergRTSCamera();

	virtual void Tick(float DeltaTime) override;
	virtual void SetupPlayerInputComponent(UInputComponent* Input) override;

protected:
	virtual void BeginPlay() override;

private:
	UPROPERTY(VisibleDefaultsOnly) TObjectPtr<USceneComponent>       CameraRoot;
	UPROPERTY(VisibleDefaultsOnly) TObjectPtr<USpringArmComponent>   SpringArm;
	UPROPERTY(VisibleDefaultsOnly) TObjectPtr<UCameraComponent>      Camera;
	UPROPERTY(VisibleDefaultsOnly) TObjectPtr<UFloatingPawnMovement> Movement;

	// Input assets — set in BP or DefaultPawn
	UPROPERTY(EditDefaultsOnly, Category="Input") TObjectPtr<UInputMappingContext> IMC_Camera;
	UPROPERTY(EditDefaultsOnly, Category="Input") TObjectPtr<UInputAction> IA_Pan;
	UPROPERTY(EditDefaultsOnly, Category="Input") TObjectPtr<UInputAction> IA_Zoom;
	UPROPERTY(EditDefaultsOnly, Category="Input") TObjectPtr<UInputAction> IA_Rotate;

	// Tuning
	UPROPERTY(EditDefaultsOnly, Category="Camera") float PanSpeed        = 1200.f;
	UPROPERTY(EditDefaultsOnly, Category="Camera") float ZoomSpeed       = 500.f;
	UPROPERTY(EditDefaultsOnly, Category="Camera") float RotateSpeed     = 80.f;
	UPROPERTY(EditDefaultsOnly, Category="Camera") float MinArmLength    = 400.f;
	UPROPERTY(EditDefaultsOnly, Category="Camera") float MaxArmLength    = 5000.f;
	UPROPERTY(EditDefaultsOnly, Category="Camera") float EdgeScrollSpeed  = 800.f;
	UPROPERTY(EditDefaultsOnly, Category="Camera") float EdgeScrollMargin = 20.f;

	float TargetArmLength = 2000.f;

	void HandlePan(const FInputActionValue& Value);
	void HandleZoom(const FInputActionValue& Value);
	void HandleRotate(const FInputActionValue& Value);
	void ApplyEdgeScroll(float DeltaTime);
	void ApplyZoomLerp(float DeltaTime);
};
