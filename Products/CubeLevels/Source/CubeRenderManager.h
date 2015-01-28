#pragma once

class CubeRenderManager
{
public:
	CubeRenderManager(){}
	~CubeRenderManager(){}

private:
	// ステージの各頂点
	DirectX::SimpleMath::Vector3	_position[8];
	
};