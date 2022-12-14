#pragma once
#include "CEntity.h"


class CComponent;
class CTransform;
class CMeshRender;
class CCamera;
class CRenderComponent;
class CScript;

class CGameObject :
    public CEntity
{
private:

    //Components
    CComponent*         m_arrCom[(UINT)eCOMPONENT_TYPE::END];
    CRenderComponent*   m_RenderCom;

    //Hierarchy
    CGameObject*            m_Parent;
    vector<CGameObject*>    m_vecChild;

    int                     m_iLayerIdx;
    bool                    m_bFixLayer;    //레이어 번호를 고정. 부모 레이어를 옮겨도 자신은 옮겨지지 않음.

    //초기화 되어 현재 Level 안에서 작동중인지 여부를 저장.
    //작동 이후 컴포넌트가 추가될 시 바로 init 호출.
    bool                m_initalized;

public:
    //Inline Setter
    void                SetLayerIdx(int _iLayerIdx) { m_iLayerIdx = _iLayerIdx; }
    void                SetLayerFixed(bool _bFix) { m_bFixLayer = _bFix; }

    //Inline Getter
    CTransform*         Transform() const { return (CTransform*)m_arrCom[(UINT)eCOMPONENT_TYPE::TRANSFORM]; }
    CMeshRender*        MeshRender() const { return (CMeshRender*)m_arrCom[(UINT)eCOMPONENT_TYPE::MESHRENDER]; }
    CCamera*            Camera() const { return (CCamera*)m_arrCom[(UINT)eCOMPONENT_TYPE::CAMERA]; }
    CRenderComponent*   GetRenderComponent() const { return m_RenderCom; }
    CGameObject*        GetParent() const { return m_Parent; }
    
    //Master GameObject만 LevelMgr에서 tick()를 호출한다.
    bool                IsMaster()  const { return (nullptr == m_Parent); }

    //Add
    void AddComponent(CComponent* _Component);
    void AddScript(CScript* _Script);
    void AddChild(CGameObject* _Object);

    //Recursive
    void AddAllHierarchyObjects(__in int _iLayerIdx, __out list<CGameObject*>& _listObj);

    //기타
    bool                GetParentWorldMatrix(__out Matrix& _mat);

private:
    void RemoveChild(CGameObject* _Object);

public:
    void init();
    void tick();
    void finaltick();
    void render();


    CLONE(CGameObject)    
public:
    CGameObject();
    ~CGameObject();

    //재귀 private 내부 호출을 위해 GameObject끼리는 friend 설정
    friend class CGameObject;
};

