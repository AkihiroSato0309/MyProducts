//--------------------------------------------------------------------------------------
// File: Global.h
// Author:Imase Hideyasu
//
//--------------------------------------------------------------------------------------
#pragma once

#include "Common.h"

#ifdef GLOBAL_CPP
#define EXTERN
#else
#define EXTERN extern
#endif

EXTERN HWND						g_hWnd;
EXTERN ID3D11Device*			g_pd3dDevice;
EXTERN ID3D11DeviceContext*		g_pImmediateContext;

EXTERN ID3D11InputLayout*		g_pBatchInputLayout;
EXTERN ID3D11RasterizerState*	g_pRasterizerState;

EXTERN std::unique_ptr<DirectX::CommonStates>	g_States;
EXTERN std::unique_ptr<DirectX::SpriteBatch>	g_Sprites;
EXTERN std::unique_ptr<DirectX::SpriteFont>		g_SpriteFont;
EXTERN std::unique_ptr<DirectX::EffectFactory>	g_FXFactory;
EXTERN std::unique_ptr<Camera>					g_Camera;		// ÉJÉÅÉâ
EXTERN std::unique_ptr<InputManager>			g_InputManager;

EXTERN std::unique_ptr<DirectX::BasicEffect>	g_BatchEffect;
EXTERN std::unique_ptr<DirectX::PrimitiveBatch<DirectX::VertexPositionColorTexture>>	g_Batch;

EXTERN ID3D11BlendState* g_pBlendState;

