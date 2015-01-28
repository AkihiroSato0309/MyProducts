//--------------------------------------------------------------------------------------
// File: Main.cpp
// Author:Imase Hideyasu
//
//--------------------------------------------------------------------------------------
#include "Common.h"
#include "Global.h"
#include "Stage.h"
#include "Stage2D.h"
#include "Player.h"
#include "FpsTimer.h"

using namespace DirectX;
using namespace DirectX::SimpleMath;

// �E�C���h�E�̃^�C�g��
static const TCHAR WINDOW_TITLE[] = _T("CubeLevels");

//--------------------------------------------------------------------------------------
// �O���[�o���ϐ�
//--------------------------------------------------------------------------------------
static HINSTANCE					s_hInst = NULL;
static D3D_DRIVER_TYPE				s_driverType = D3D_DRIVER_TYPE_NULL;
static D3D_FEATURE_LEVEL			s_featureLevel = D3D_FEATURE_LEVEL_11_0;
static IDXGISwapChain*				s_pSwapChain = NULL;
static ID3D11RenderTargetView*		s_pRenderTargetView = NULL;
static ID3D11Texture2D*				s_pDepthStencil = NULL;
static ID3D11DepthStencilView*		s_pDepthStencilView = NULL;
static IDXGIDevice1*				s_pDXGIDevice = NULL;
static IDXGIAdapter*				s_pDXGIAdapter = NULL;
static IDXGIFactory*				s_pDXGIFactory = NULL;

static Stage* stage;
static std::unique_ptr<Player> player;

//�󕈂ƃ��C�A�E�g�ւ̃|�C���^	<--�ǉ�
static ID3D11InputLayout*			s_pBatchInputLayout = nullptr;

// DirectXTK���g�����߂̊e�I�u�W�F�N�g	<--�ǉ�
static std::unique_ptr<Model> s_Model;	//���f���`�掞�ɒǉ�

// �G���h�t���O
static bool s_endFlag;


//--------------------------------------------------------------------------------------
// �֐��̃v���g�^�C�v�錾
//--------------------------------------------------------------------------------------
HRESULT InitWindow( HINSTANCE hInstance, int nCmdShow );
HRESULT InitDevice();
void CleanupDevice();
LRESULT CALLBACK    WndProc( HWND, UINT, WPARAM, LPARAM );
void Update();
void Render();
void InitDirectXTK();

//--------------------------------------------------------------------------------------
// ���C�� 
//--------------------------------------------------------------------------------------
int WINAPI wWinMain( HINSTANCE hInstance, HINSTANCE hPrevInstance, LPWSTR lpCmdLine, int nCmdShow )
{
	srand((unsigned)time(NULL));

    UNREFERENCED_PARAMETER( hPrevInstance );
    UNREFERENCED_PARAMETER( lpCmdLine );

#ifdef _DEBUG
	// �������[���[�N���o�̂��߂̐ݒ�
	::_CrtSetDbgFlag(_CRTDBG_LEAK_CHECK_DF | _CRTDBG_ALLOC_MEM_DF);
#endif

	// �E�C���h�E�̍쐬
    if( FAILED( InitWindow( hInstance, nCmdShow ) ) )
        return 0;

	// �f�o�C�X�̏�����
    if( FAILED( InitDevice() ) )
    {
        CleanupDevice();
        return 0;
    }

	// �G���h�t���O��|��
	s_endFlag = false;

	// �}�b�v�`�b�v�ǂݍ���
	std::ifstream ifs( "Map/TestMap_6.txt" );
	std::string str;

	int map[1350];

	ifs >> str;

	for(int i = 0; i < 1350; i++)
	{
		map[i] = atoi(&str[i * 2]);
	}

	map[225 * eFront + 5 * 2 + 15 * 1 + 2] = GOAL;

	// �ϐ�����������
	stage = new Stage(map);
	player.reset( new Player() );
	stage->Initialize();

	// �}�g���N�X���`�F���W���邽�߂̂���
	player->GetMatrixChanger(stage);

	// DirectXTK�̏�����
	InitDirectXTK();
	
	DWORD startTime = 0, endTime = 0;	

	// ���C�����[�v
    MSG msg = {0};
	while( WM_QUIT != msg.message && s_endFlag == false )
    {
        if( PeekMessage( &msg, NULL, 0, 0, PM_REMOVE ) )
        {
            TranslateMessage( &msg );
            DispatchMessage( &msg );
        }
        else
        {

//#ifdef _RELEASE
//			WCHAR updateFlame[8], renderFlame[8];
//			float updateFlameWork, renderFlameWork;
//
//			startTime = timeGetTime();
//#endif

			// �X�V
		Update();

//#ifdef _RELEASE
//			endTime = timeGetTime();
//			updateFlameWork = 1000.0f / (endTime - startTime);
//			startTime = timeGetTime();
//#endif
			// �`��
		Render();

//#ifdef _RELEASE
//			endTime = timeGetTime();
//
//			renderFlameWork = 1000.0f / (endTime - startTime);
//
//			_itow_s(updateFlameWork * 1000, updateFlame, 10);
//			_itow_s(renderFlameWork * 1000, renderFlame, 10);
//
//			g_Sprites->Begin();
//			g_SpriteFont->DrawString(g_Sprites.get(), updateFlame, Vector2(0, 0), Vector4(1.0f, 0.0f, 0.0f, 1.0f));
//			g_SpriteFont->DrawString(g_Sprites.get(), renderFlame, Vector2(0, 50), Vector4(1.0f, 0.0f, 0.0f, 1.0f));
//			g_Sprites->End();
//#endif

			// �o�b�N�o�b�t�@�ƃt�����g�o�b�t�@�����ւ���i��ʂ̃��t���b�V�����[�g�ɍ��킹��j
			s_pSwapChain->Present( 1, 0 );
        }
    }

    CleanupDevice();

    return ( int )msg.wParam;
}

//--------------------------------------------------------------------------------------
// �E�C���h�E�̍쐬
//--------------------------------------------------------------------------------------
HRESULT InitWindow( HINSTANCE hInstance, int nCmdShow )
{
    // �E�C���h�E�N���X�̓o�^
    WNDCLASSEX wcex;
    wcex.cbSize = sizeof( WNDCLASSEX );
    wcex.style = CS_HREDRAW | CS_VREDRAW;
    wcex.lpfnWndProc = WndProc;
    wcex.cbClsExtra = 0;
    wcex.cbWndExtra = 0;
    wcex.hInstance = hInstance;
    wcex.hIcon = LoadIcon( hInstance, ( LPCTSTR )NULL );
    wcex.hCursor = LoadCursor( NULL, IDC_ARROW );
    wcex.hbrBackground = ( HBRUSH )( COLOR_WINDOW + 1 );
    wcex.lpszMenuName = NULL;
    wcex.lpszClassName = _T("TutorialWindowClass");
    wcex.hIconSm = LoadIcon( wcex.hInstance, ( LPCTSTR )NULL );
    if( !RegisterClassEx( &wcex ) )
        return E_FAIL;

    // �E�C���h�E�̍쐬
    s_hInst = hInstance;
    RECT rc = { 0, 0, WINDOW_W, WINDOW_H };
    AdjustWindowRect( &rc, WS_OVERLAPPEDWINDOW, FALSE );
    g_hWnd = CreateWindow( _T("TutorialWindowClass"), WINDOW_TITLE, WS_OVERLAPPEDWINDOW,
                           CW_USEDEFAULT, CW_USEDEFAULT, rc.right - rc.left, rc.bottom - rc.top, NULL, NULL, hInstance,
                           NULL );
    
	if( !g_hWnd )
		return E_FAIL;

    ShowWindow( g_hWnd, nCmdShow );

    return S_OK;
}


//--------------------------------------------------------------------------------------
// �E�C���h�E�v���V�[�W��
//--------------------------------------------------------------------------------------
LRESULT CALLBACK WndProc( HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam )
{
    PAINTSTRUCT ps;
    HDC hdc;

    switch( message )
    {
        case WM_PAINT:
            hdc = BeginPaint( hWnd, &ps );
            EndPaint( hWnd, &ps );
            break;

        case WM_DESTROY:
            PostQuitMessage( 0 );
            break;

        default:
            return DefWindowProc( hWnd, message, wParam, lParam );
    }

    return 0;
}


//--------------------------------------------------------------------------------------
// �f�o�C�X�̏�����
//--------------------------------------------------------------------------------------
HRESULT InitDevice()
{
    HRESULT hr = S_OK;

    RECT rc;
    GetClientRect( g_hWnd, &rc );
    UINT width = rc.right - rc.left;
    UINT height = rc.bottom - rc.top;

    UINT createDeviceFlags = 0;
#ifdef _DEBUG
    createDeviceFlags |= D3D11_CREATE_DEVICE_DEBUG;
#endif

    D3D_DRIVER_TYPE driverTypes[] =
    {
        D3D_DRIVER_TYPE_HARDWARE,
        D3D_DRIVER_TYPE_WARP,
        D3D_DRIVER_TYPE_REFERENCE,
    };
    UINT numDriverTypes = ARRAYSIZE( driverTypes );

    D3D_FEATURE_LEVEL featureLevels[] =
    {
        D3D_FEATURE_LEVEL_11_0,
        D3D_FEATURE_LEVEL_10_1,
        D3D_FEATURE_LEVEL_10_0,
    };
	UINT numFeatureLevels = ARRAYSIZE( featureLevels );

	// DirectX11�̃f�o�C�X���쐬
	for( UINT driverTypeIndex = 0; driverTypeIndex < numDriverTypes; driverTypeIndex++ )
    {
        s_driverType = driverTypes[driverTypeIndex];
        hr = D3D11CreateDevice( NULL, s_driverType, NULL, createDeviceFlags, featureLevels, numFeatureLevels,
                                            D3D11_SDK_VERSION, &g_pd3dDevice, &s_featureLevel, &g_pImmediateContext );
        if( SUCCEEDED( hr ) )
            break;
    }

	// �g�p�\�ȃ}���`�T���v�����擾
    DXGI_SAMPLE_DESC SampleDesc;
    for (UINT i = 1; i <= 2; i ++) {
        UINT Quality;
        if (SUCCEEDED(g_pd3dDevice->CheckMultisampleQualityLevels(DXGI_FORMAT_R8G8B8A8_UNORM, i, &Quality))) {
            if (Quality > 0) {
                DXGI_SAMPLE_DESC Desc = { i, Quality - 1 };
                SampleDesc = Desc;
            }
        }
    }

	// �C���^�[�t�F�C�X�擾
	hr = g_pd3dDevice->QueryInterface( __uuidof( IDXGIDevice1 ), ( LPVOID* )&s_pDXGIDevice );
    if (FAILED(hr)) {
		return hr;
	}

	// �A�_�v�^�[�擾
    hr = s_pDXGIDevice->GetAdapter(&s_pDXGIAdapter);
    if (FAILED(hr)) {
		return hr;
	}

	// �t�@�N�g���[�擾
	hr = s_pDXGIAdapter->GetParent(__uuidof(IDXGIFactory), (LPVOID*)&s_pDXGIFactory);
    if (FAILED(hr)) {
		return hr;
	}

	// �X���b�v�`�F�C���̍쐬
    DXGI_SWAP_CHAIN_DESC sd;
    ZeroMemory( &sd, sizeof( sd ) );
    sd.BufferCount = 1;
    sd.BufferDesc.Width = width;
    sd.BufferDesc.Height = height;
    sd.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
    sd.BufferDesc.RefreshRate.Numerator = 60;
    sd.BufferDesc.RefreshRate.Denominator = 1;
    sd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
    sd.OutputWindow = g_hWnd;
    sd.SampleDesc = SampleDesc;
    sd.Windowed = TRUE;
	sd.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;
	sd.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH;	// <-- �t���X�N���[������ʉ𑜓x��؂�ւ���

	hr = s_pDXGIFactory->CreateSwapChain(g_pd3dDevice, &sd, &s_pSwapChain);
    if (FAILED(hr)) {
        return hr;
    }

	// �����_�[�^�[�Q�b�g�r���[�̍쐬
    ID3D11Texture2D* pBackBuffer = NULL;
    hr = s_pSwapChain->GetBuffer( 0, __uuidof( ID3D11Texture2D ), ( LPVOID* )&pBackBuffer );
    if( FAILED( hr ) )
        return hr;


    hr = g_pd3dDevice->CreateRenderTargetView( pBackBuffer, NULL, &s_pRenderTargetView );
    pBackBuffer->Release();
    if( FAILED( hr ) )
        return hr;


    // �[�x�X�e���V���p�e�N�X�`���̍쐬
    D3D11_TEXTURE2D_DESC descDepth;
    ZeroMemory( &descDepth, sizeof(descDepth) );
    descDepth.Width = width;
    descDepth.Height = height;
    descDepth.MipLevels = 1;
    descDepth.ArraySize = 1;
    descDepth.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
    descDepth.SampleDesc = SampleDesc;
    descDepth.Usage = D3D11_USAGE_DEFAULT;
    descDepth.BindFlags = D3D11_BIND_DEPTH_STENCIL;
    descDepth.CPUAccessFlags = 0;
    descDepth.MiscFlags = 0;
    hr = g_pd3dDevice->CreateTexture2D( &descDepth, NULL, &s_pDepthStencil );
    if( FAILED( hr ) )
        return hr;


	// �u�����f�B���O �X�e�[�g�̍쐬
	D3D11_BLEND_DESC blendDesc;
	ZeroMemory( &blendDesc, sizeof( blendDesc ) );
	blendDesc.AlphaToCoverageEnable = FALSE;
	blendDesc.IndependentBlendEnable = FALSE;
	blendDesc.RenderTarget[0].BlendEnable = TRUE;
	blendDesc.RenderTarget[0].SrcBlend = D3D11_BLEND_SRC_ALPHA;
	blendDesc.RenderTarget[0].DestBlend = D3D11_BLEND_INV_SRC_ALPHA;
	blendDesc.RenderTarget[0].BlendOp = D3D11_BLEND_OP_ADD;
	blendDesc.RenderTarget[0].SrcBlendAlpha = D3D11_BLEND_ONE;
	blendDesc.RenderTarget[0].DestBlendAlpha = D3D11_BLEND_ZERO;
	blendDesc.RenderTarget[0].BlendOpAlpha = D3D11_BLEND_OP_ADD;
	blendDesc.RenderTarget[0].RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;
	g_pd3dDevice->CreateBlendState( &blendDesc, &g_pBlendState );


    // �[�x�X�e���V���r���[�̍쐬
    D3D11_DEPTH_STENCIL_VIEW_DESC descDSV;
    ZeroMemory( &descDSV, sizeof(descDSV) );
    descDSV.Format = descDepth.Format;
    descDSV.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2DMS;
    descDSV.Texture2D.MipSlice = 0;
    hr = g_pd3dDevice->CreateDepthStencilView( s_pDepthStencil, &descDSV, &s_pDepthStencilView );
    if( FAILED( hr ) )
        return hr;

    g_pImmediateContext->OMSetRenderTargets( 1, &s_pRenderTargetView, s_pDepthStencilView );

    // �r���[�|�[�g�̐ݒ�
    D3D11_VIEWPORT vp;
    vp.Width = (FLOAT)width;
    vp.Height = (FLOAT)height;
    vp.MinDepth = 0.0f;
    vp.MaxDepth = 1.0f;
    vp.TopLeftX = 0;
    vp.TopLeftY = 0;
    g_pImmediateContext->RSSetViewports( 1, &vp );

	return S_OK;	
}


//--------------------------------------------------------------------------------------
// �X�V
//--------------------------------------------------------------------------------------
void Update()
{
	// �L�[�̍X�V
	g_InputManager->Update();

	// �v���C���[���ړ����Ă���Ƃ��͓������Ȃ�
	if(player->GetMovingFlag() != true)
	{
		stage->Rotation(player->GetExistFace(), player->GetUpDirection());
	}

	stage->Update();
	
	// ��]���̓v���C���[�𑀍삷�邱�Ƃ��ł��Ȃ�
	if(stage->GetMovingFlag() == false)
	{
		if(g_InputManager->IsButtonDown(XINPUT_GAMEPAD_DPAD_RIGHT) )
		{
			player->MoveRight();
		}
		if(g_InputManager->IsButtonDown(XINPUT_GAMEPAD_DPAD_LEFT) )
		{
			player->MoveLeft();
		}
		if(g_InputManager->IsButtonPressed(XINPUT_GAMEPAD_DPAD_UP) )
		{
			player->Jump();
		}
		//if(g_InputManager->IsButtonDown(XINPUT_GAMEPAD_DPAD_UP) )
		//{
		//	player->MoveUp();
		//}
		if(g_InputManager->IsButtonDown(XINPUT_GAMEPAD_DPAD_DOWN) )
		{
			player->MoveDown();
		}

#ifdef _DEBUG

		if( g_InputManager->IsKeyDown('D') )
		{
			player->MoveRight();
		}
		if( g_InputManager->IsKeyDown('A') )
		{
			player->MoveLeft();
		}
		if( g_InputManager->IsKeyPressed(VK_SPACE) )
		{
			player->Jump();
		}

#endif
	}

	// �S�[�������Ƃ��̏���
	if( player->Update(stage->GetStage2D()->GetMapData(player->GetExistFace()), stage->GetMovingFlag()) == true )
	{
		//s_endFlag = true;
	}

	// �X�e�[�W����]���Ă����ꍇ�A�v���C���[�ɂ��e��������
	if(stage->GetRotationInfoFlag() == true)
	{
		player->PlayerRotationWithCube(stage->GetStageRotationInfo());
	}

	// �J�����̍X�V
	g_Camera->Update(player->GetExistFace(), player->GetUpDirection());
}


//--------------------------------------------------------------------------------------
// �`��
//--------------------------------------------------------------------------------------
void Render()
{
	// ��ʂ̃N���A
    float ClearColor[4] = { 0.1f, 0.1f, 0.2f, 1.0f }; // red, green, blue, alpha
    g_pImmediateContext->ClearRenderTargetView( s_pRenderTargetView, ClearColor );
	// �[�x�X�e���V���o�b�t�@�̃N���A
	g_pImmediateContext->ClearDepthStencilView( s_pDepthStencilView, D3D11_CLEAR_DEPTH, 1.0f, 0 );

	//---- �����ɕ`�揈�������� ----//
	player->Render();
	stage->Render(player->GetExistFace());	
}


//--------------------------------------------------------------------------------------
// �N���[���A�b�v
//--------------------------------------------------------------------------------------
void CleanupDevice()
{
    if( g_pImmediateContext ) g_pImmediateContext->ClearState();

	if( s_pDXGIFactory ) s_pDXGIFactory->Release();
	if( s_pDXGIAdapter ) s_pDXGIAdapter->Release();
	if( s_pDXGIDevice ) s_pDXGIDevice->Release();
	if( s_pDepthStencil ) s_pDepthStencil->Release();
	if( s_pDepthStencilView ) s_pDepthStencilView->Release();
    if( s_pRenderTargetView ) s_pRenderTargetView->Release();
    if( s_pSwapChain ) s_pSwapChain->Release();
    if( g_pImmediateContext ) g_pImmediateContext->Release();
    if( g_pd3dDevice ) g_pd3dDevice->Release();
	if( g_pRasterizerState ) g_pRasterizerState->Release();
	if( g_pBatchInputLayout ) g_pBatchInputLayout->Release();
	if( g_pBlendState ) g_pBlendState->Release();

	if( stage ) delete stage;
}


//--------------------------------------------------------------------------------------
// DirectXTK�̏�����
//--------------------------------------------------------------------------------------
void InitDirectXTK()
{
	g_States.reset( new CommonStates( g_pd3dDevice ) );
	g_Sprites.reset( new SpriteBatch( g_pImmediateContext ) );
	g_SpriteFont.reset( new SpriteFont(g_pd3dDevice, L"italic.spritefont") );
	g_FXFactory.reset( new EffectFactory(g_pd3dDevice));			//3D���f���`�掞

	g_BatchEffect.reset( new BasicEffect( g_pd3dDevice ) );
	g_BatchEffect->SetVertexColorEnabled(true);
	g_BatchEffect->SetTextureEnabled(true);

	g_Batch.reset( new PrimitiveBatch<VertexPositionColorTexture>( g_pImmediateContext ) );

	void const* shaderByteCode;
	size_t byteCodeLength;

	// �V�F�[�_�[�̎擾
	g_BatchEffect->GetVertexShaderBytecode( &shaderByteCode, &byteCodeLength );

	// ���̓��C�A�E�g�̍쐬
	g_pd3dDevice->CreateInputLayout( VertexPositionColorTexture::InputElements,
									 VertexPositionColorTexture::InputElementCount,
									 shaderByteCode, byteCodeLength,
									 &g_pBatchInputLayout );

	// �J�����̏�����
	g_Camera.reset( new Camera(	Vector3(0.0f, 0.0f, DISTANCE_FROM_STAGE),
								Vector3(0.0f, 0.0f, 0.0f),
								Vector3(0.0f, 1.0f, 0.0f)));

	// ���̓}�l�[�W���̏�����
	g_InputManager.reset( new InputManager() );

	g_States->CullNone();
}


