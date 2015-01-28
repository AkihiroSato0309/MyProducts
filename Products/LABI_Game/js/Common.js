
//=============================================================================
// グローバル変数
//=============================================================================

var g_game = null;					// ゲーム本体
var g_totalScore = 0;				// 獲得した総スコア
var g_numberGraph = null;			// 数字の画像
var g_backBGM = null;				// BGM


//=============================================================================
// 定数
//=============================================================================

//-----------------------------------------------------------------------------
// 画像に関する定数
//-----------------------------------------------------------------------------
const ITEM_SIZE = 64;										// アイテムのサイズ
const NUMBER_WIDTH = 32;									// 数字画像の横幅
const NUMBER_HEIGHT = 64;									// 数字画像の縦幅
const SUPPORT_CION_SIZE = 46;								// サポートアイコンのサイズ
const WEIGHT_BAR_WIDTH = 64;								// 重さバーの画像の横幅
const BOX_WIDTH = 64;										// ボックス画像の横幅
const BOX_HEIGHT = 128;										// ボックス画像の縦幅

//-----------------------------------------------------------------------------
// ゲーム全般に関する定数
//-----------------------------------------------------------------------------
const GAME_FPS = 30;										// フレームレートを指定する
const WIN_SIZE_WIDTH = 480;									// 画面の横幅
const WIN_SIZE_HEIGHT = 320;								// 画面の縦幅
const SCENE_FADE_TIME = 60;									// シーン遷移の時間

//-----------------------------------------------------------------------------
// ゲームシーンに関する定数
//-----------------------------------------------------------------------------
const GAME_SIZE_WIDTH = 380;								//　ゲームの主な更新部分
const GAME_RIGHT_SPACE = WIN_SIZE_WIDTH - GAME_SIZE_WIDTH;	// 画面右のスペース
const GAME_FLOOR_Y = WIN_SIZE_HEIGHT - 12;					// ゲームの地面のy座標
const ITEM_BREAK_Y = GAME_FLOOR_Y - ITEM_SIZE;				// アイテムが消えるY座標
const GAME_LIMIT_TIME = 60;									// ゲームのタイムリミット
const SCORE_MAX = 8;										// スコアの最大桁数
const TIME_MAX = 2;											// タイムの最大桁数
const ITEM_CREATE_FRAME = 50;								// アイテムを生成するフレーム

//-----------------------------------------------------------------------------
// キャラクターのパラメータに関する定数
//-----------------------------------------------------------------------------
const TUKUMO_MOVE_SPEED = 1.5;								// つくもたんの移動速度
const LABI_MOVE_SPEED = 4.1;								// らびたんの通常移動速度
const LABI_MOVEDOWN_SPEED = 3.1;							// らびたんの速度低下量の最大
const TUKUMO_SUPPORT_TIME = 15;								// つくもたんのサポートタイム
const JUNK_SUPPORT_TIME = 15;								// じゃんくたんのサポートタイム
const JUNK_UPSPEED = 2.0;									// つくもたんのスピードアップ量
const CARRY_WEIGHT_MAX = 100;								// 運べる重量数の上限
const GET_ANIMATION = 3;									// ゲットした時の画像
const GET_STOP_FRAME = 3;									// ゲットした時の硬直時間
const TUKUMO_APPEAR_TIME = 6;								// つくもたんの登場時のアクション時間

//-----------------------------------------------------------------------------
// 商品に関する定数
//-----------------------------------------------------------------------------
const ITEM_VAL = 6;											// 商品の種類

// 商品のナンバー
const ITEM_DVD = 0;											// DVD
const ITEM_DRIER = 1;										// ドライヤー
const ITEM_HEADPHONE = 2;									// ヘッドホン
const ITEM_TABLET = 3;										// タブレット
const ITEM_DISICAMERA = 4;									// デジカメ
const ITEM_3DPRINTER = 5;									// 3Dプリンター

// 商品の値段
const ITEM_PRICE_DVD = 1500;						
const ITEM_PRICE_DRIER = 10000;
const ITEM_PRICE_HEADPHONE = 20000;
const ITEM_PRICE_TABLET = 30000;
const ITEM_PRICE_DISICAMERA = 50000;
const ITEM_PRICE_3DPRINTER = 150000;


		
			



