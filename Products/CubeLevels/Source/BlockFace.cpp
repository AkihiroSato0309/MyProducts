#include "BlockFace.h"
#include "SModelMgr.h"
#include "STextureMgr.h"

using namespace DirectX;
using namespace DirectX::SimpleMath;

VertexPositionColorTexture vertexes[4] = {
	VertexPositionColorTexture(Vector3( -BLOCK_SIZE_HALF,  0.0f,  -BLOCK_SIZE_HALF), Vector4( 0.8f, 0.8f, 0.8f, 0.7f ), Vector2( 0.0f, 0.0f )),	// 0
	VertexPositionColorTexture(Vector3(  BLOCK_SIZE_HALF,  0.0f,  -BLOCK_SIZE_HALF), Vector4( 0.8f, 0.8f, 0.8f, 0.7f ), Vector2( 1.0f, 0.0f )),	// 1
	VertexPositionColorTexture(Vector3( -BLOCK_SIZE_HALF,  0.0f,   BLOCK_SIZE_HALF), Vector4( 0.8f, 0.8f, 0.8f, 0.7f ), Vector2( 0.0f, 1.0f )),	// 2
	VertexPositionColorTexture(Vector3(  BLOCK_SIZE_HALF,  0.0f,   BLOCK_SIZE_HALF), Vector4( 0.8f, 0.8f, 0.8f, 0.7f ), Vector2( 1.0f, 1.0f )),	// 3
};

//*********************************************************************
// コンストラクタ
//*********************************************************************
BlockFace::BlockFace(int hit, int faceDirection): 
	_hit(hit),
	_faceDirection(faceDirection)
{
	g_FXFactory.reset( new EffectFactory(g_pd3dDevice));
	if(hit == 1)
	{
		STextureMgr::GetInstance()->Load( _pTexture, L"Image/BlockTexture.png" );
	}else if(hit == GOAL){
		STextureMgr::GetInstance()->Load( _pTexture, L"Image/GoalBlockTexture.png" );
	}else{
		STextureMgr::GetInstance()->Load( _pTexture, L"Image/NoBlockTexture.png" );
	}
}


//*********************************************************************
// デストラクタ
//*********************************************************************
BlockFace::~BlockFace()
{

}


//*********************************************************************
// 更新処理
//*********************************************************************
void BlockFace::Update()
{

}


//*********************************************************************
// 描画処理
//*********************************************************************
void BlockFace::Draw(Matrix* matrix)
{
	g_pImmediateContext->RSSetState(g_States->CullNone());
	g_pImmediateContext->OMSetBlendState(g_States->AlphaBlend(), NULL, 0xFFFFFF);

	// 作成した行列をエフェクトに設定
	g_BatchEffect->SetWorld( *matrix );
	g_BatchEffect->SetView( g_Camera->GetView() );
	g_BatchEffect->SetProjection( g_Camera->GetProjection() );

	g_BatchEffect->SetVertexColorEnabled(TRUE);

	// テクスチャの設定
	g_BatchEffect->SetTexture( _pTexture );

	// エフェクトの設定（各行列やテクスチャなどを設定している）
	g_BatchEffect->Apply( g_pImmediateContext );


	// 深度ステンシル ステートを設定する
	g_pImmediateContext->OMSetDepthStencilState( g_States->DepthDefault(), 0 );

	// ブレンディング ステートを設定する
//	g_pImmediateContext->OMSetBlendState( g_pBlendState, nullptr, 0xFFFFFFFF ); 

	// サンプラーステートを設定する
	auto samplerState = g_States->PointWrap();
	g_pImmediateContext->PSSetSamplers(0, 1, &samplerState);

	// 入力レイアウトを設定する
	g_pImmediateContext->IASetInputLayout( g_pBatchInputLayout );


	g_Batch->Begin();

	// 頂点データ
	uint16_t indexes[4] = { 0,  1,  2,  3 };

	// 三角形プリミティブの描画
	g_Batch->DrawIndexed(D3D_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP, indexes, 4, vertexes, 4);

	g_Batch->End();
	
}