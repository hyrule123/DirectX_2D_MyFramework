#include "pch.h"
#include "CComponent.h"

CComponent::CComponent(eCOMPONENT_TYPE _Type)
	: m_pOwner(nullptr)
	, m_Type(_Type)	
{
}

CComponent::~CComponent()
{
}
