#include "pch.h"
#include "CMaterial.h"

#include "CDevice.h"
#include "CConstBuffer.h"

CMaterial::CMaterial()
	: CRes(eRES_TYPE::MATERIAL)
	, m_Const{}
	, m_arrTex{}
{	

}

CMaterial::~CMaterial()
{
}


void CMaterial::UpdateData()
{
	if (nullptr == m_pShader)
		return;

	m_pShader->UpdateData();


	// Texture Update
	for (UINT i = 0; i < eTEX_END; ++i)
	{
		if (nullptr == m_arrTex[i])
		{
			m_Const.arrbTex[i] = 0;
		}

		else
		{
			m_Const.arrbTex[i] = 1;
			m_arrTex[i]->UpdateData(i, eSHADER_PIPELINE_STAGE_FLAG::eSHADER_PIPELINE_FLAG_PIXEL);
		}
	}

	// Constant Update
	CConstBuffer* pMtrlBuffer = CDevice::GetInst()->GetConstBuffer(eCB_TYPE::MATERIAL);
	pMtrlBuffer->SetData(&m_Const);
	pMtrlBuffer->UpdateData();
}


void CMaterial::SetScalarParam(eSCALAR_PARAM _Param, void* _Src)
{
	switch (_Param)
	{
	case INT_0:
	case INT_1:
	case INT_2:
	case INT_3:		
		m_Const.arrInt[_Param] = *((int*)_Src);
		break;
	case FLOAT_0:
	case FLOAT_1:
	case FLOAT_2:
	case FLOAT_3:
		m_Const.arrFloat[_Param - FLOAT_0] = *((float*)_Src);
		break;

	case VEC2_0:
	case VEC2_1:
	case VEC2_2:
	case VEC2_3:
		m_Const.arrV2[_Param - VEC2_0] = *((Vec2*)_Src);
		break;

	case VEC4_0:
	case VEC4_1:
	case VEC4_2:
	case VEC4_3:
		m_Const.arrV4[_Param - VEC4_0] = *((Vec4*)_Src);
		break;

	case MAT_0:
	case MAT_1:
	case MAT_2:
	case MAT_3:
		m_Const.arrMat[_Param - MAT_0] = *((Matrix*)_Src);
		break;	


	}
}


void CMaterial::SetTexParam(eTEX_PARAM _Param, Ptr<CTexture> _Tex)
{
	m_arrTex[_Param] = _Tex;
}