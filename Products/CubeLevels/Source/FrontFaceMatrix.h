#pragma once

static const DirectX::SimpleMath::Matrix FRONT_RIGHT	= DirectX::SimpleMath::Matrix::CreateFromAxisAngle(DirectX::SimpleMath::Vector3(0.0f, 1.0f, 0.0f), DirectX::XMConvertToRadians(-90.0f));
static const DirectX::SimpleMath::Matrix FRONT_LEFT		= DirectX::SimpleMath::Matrix::CreateFromAxisAngle(DirectX::SimpleMath::Vector3(0.0f, 1.0f, 0.0f), DirectX::XMConvertToRadians(90.0f));
static const DirectX::SimpleMath::Matrix FRONT_TOP		= DirectX::SimpleMath::Matrix::CreateFromAxisAngle(DirectX::SimpleMath::Vector3(1.0f, 0.0f, 0.0f), DirectX::XMConvertToRadians(90.0f));
static const DirectX::SimpleMath::Matrix FRONT_BOTTOM	= DirectX::SimpleMath::Matrix::CreateFromAxisAngle(DirectX::SimpleMath::Vector3(1.0f, 0.0f, 0.0f), DirectX::XMConvertToRadians(-90.0f));
static const DirectX::SimpleMath::Matrix FRONT_FRONT	= DirectX::SimpleMath::Matrix::Identity();
static const DirectX::SimpleMath::Matrix FRONT_BACK		= DirectX::SimpleMath::Matrix::CreateFromAxisAngle(DirectX::SimpleMath::Vector3(0.0f, 1.0f, 0.0f), DirectX::XMConvertToRadians(180.0f));