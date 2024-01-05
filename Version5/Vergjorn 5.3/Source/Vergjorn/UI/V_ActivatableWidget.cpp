// Fill out your copyright notice in the Description page of Project Settings.


#include "V_ActivatableWidget.h"


void UV_ActivatableWidget::Widget_Setup(TEnumAsByte<EVergjorn_WidgetType> type, bool activate)
{
	m_Type = type;

	//Register at registry
	//m_WidgetID = 

	//Tell parent im active


	ActivateWidget();

}

bool UV_ActivatableWidget::Is_Active()
{
	return IsActivated();
}

TEnumAsByte<EVergjorn_WidgetType> UV_ActivatableWidget::Get_Type()
{
	return m_Type;
}
