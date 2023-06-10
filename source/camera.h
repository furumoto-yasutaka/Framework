/*******************************************************************************
*
*	タイトル：	カメラコンポーネント
*	ファイル：	camera.h
*	作成者：	古本 泰隆
*
*******************************************************************************/
#pragma once
#include "main.h"
#include "cameraComponent.h"

class Camera : public CameraComponent
{
protected:
	static inline list<Camera*> m_CameraList;

	D3DXMATRIX m_ViewMatrix;	// カメラの姿勢
	D3DXMATRIX m_ProjectionMatrix;

public:
	Camera(GameObject* attachObject)
		: CameraComponent(attachObject, -1000)
	{}
	virtual ~Camera() {}

	virtual void Init(bool isActive);
	void Uninit() override;
	void Update() override {}
	void DrawView() override;

	bool CheckFrustumCulling(D3DXVECTOR3 position);

protected:
	virtual void SetStructuredBuffer();

public:
	/////////////////////////////
	//　↓↓　アクセサ　↓↓　//
	static inline list<Camera*> GetCameraList() { return m_CameraList; }

	virtual D3DXMATRIX GetViewMatrix() { return m_ViewMatrix; }
	/////////////////////////////

#ifdef _DEBUG
public:
	void DrawInspector() override;
	void DrawInspectorElem();
#endif
};
