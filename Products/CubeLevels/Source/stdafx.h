#include <windows.h>
#include <iostream>
#include <tchar.h>
#include <ctime>
#include <cstdlib>
#include <timeapi.h>
#include <fstream>
#include <string>

#include <d3d11.h>

// DirectXTKのヘッダ
#include <CommonStates.h>
#include <SimpleMath.h>
#include <WICTextureLoader.h>
#include <SpriteBatch.h>
#include <SpriteFont.h>

//DirectXTK	<--追加
#include <Effects.h>
#include <Model.h>	//モデル描画時に追加

#include <PrimitiveBatch.h>
#include <VertexTypes.h>

#pragma comment(lib,"winmm.lib")
#pragma comment(lib,"d3d11.lib")
#pragma comment(lib,"DirectXTK.lib")
#pragma comment(lib,"Xinput9_1_0.lib")
