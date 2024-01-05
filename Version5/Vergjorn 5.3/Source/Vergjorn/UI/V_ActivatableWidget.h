// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CommonActivatableWidget.h"
#include "V_ActivatableWidget.generated.h"

/**
 * 
 */


UENUM(BlueprintType)
enum EVergjorn_WidgetType {
	SCREEN = 0,
	PANEL = 1,
	OTHER = 2
};


UCLASS(Blueprintable, BlueprintType)
class VERGJORN_API UV_ActivatableWidget : public UCommonActivatableWidget
{
	GENERATED_BODY()
	
public:

	UFUNCTION(BlueprintCallable)
	void Widget_Setup(TEnumAsByte<EVergjorn_WidgetType> type, bool activate);

	UFUNCTION(BlueprintCallable)
	bool Is_Active();

	UFUNCTION(BlueprintCallable)
	TEnumAsByte<EVergjorn_WidgetType> Get_Type();
private:
	TEnumAsByte<EVergjorn_WidgetType> m_Type = SCREEN;

	int32 m_WidgetID = 0;
	int32 m_ParentID = 0;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta = (AllowPrivateAccess))
	bool m_CanBePaused = true;

};
