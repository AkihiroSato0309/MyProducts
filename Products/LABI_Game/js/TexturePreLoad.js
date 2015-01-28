//==========================================================================
// 
// 画像管理
//
//==========================================================================

//--------------------------------------------------------------------------
// タイトルシーンで使用する画像
//--------------------------------------------------------------------------
var TEX_TITLE_BACK = "texture/title_back_graph.png";			// タイトルの背景画像
var TEX_START_BUTTON = "texture/start_button.png";				// スタートボタン画像
var TEX_HELP_BUTTON = 'texture/help_button.png';				// ヘルプボタン画像

//--------------------------------------------------------------------------
// リザルトシーンで使用する画像
//--------------------------------------------------------------------------
var TEX_HELP_1 = 'texture/help_1.png';
var TEX_HELP_2 = 'texture/help_2.png';
var TEX_HELP_3 = 'texture/help_3.png';

//--------------------------------------------------------------------------
// ゲームシーンで使用する画像
//--------------------------------------------------------------------------
var TEX_GAME_BACK = "texture/game_back_graph.png";				// ゲーム画像
var TEX_TUKUMO_TAN = "texture/tukumo_tan_graph.png";			// つくもたん画像
var TEX_LABI_TAN = "texture/labi_tan_graph.png";				// らびたん画像
var TEX_TUKUMO_ICON = "texture/tukumo_icon.png";				// つくもたんアイコン画像
var TEX_JUNK_ICON = "texture/junk_icon.png";					// じゃんくたんアイコン画像
var TEX_GAME_RIGHT_SPACE = "texture/game_right_space.png";		// ゲーム右側のがぞう
var TEX_ITEM_GRAPHS = "texture/item_graphs.png";				// 商品の画像
var TEX_GET = "texture/get_texture.png";						// ゲット文字画像
var TEX_IN = "texture/in_texture.png";							// イン文字画像
var TEX_TIME_BACK = 'texture/box_texture.png';					// 商品回収ボックスの画像
var TEX_WEIGHT_BAR = 'texture/weight_bar.png';					// 重さゲージの画像（緑）
var TEX_WEIGHT_BAR_RED = 'texture/weight_bar_red.png';			// 重さゲージの画像（赤）
var TEX_BLACK_BACK = 'texture/texture_black.png';				// 黒い画像
var TEX_TUKUMO_CUTIN = 'texture/tukumo_cutin.png';				// つくもたんのカットイン	
var TEX_JUNK_CUTIN = 'texture/junk_cutin.png';					// じゃんくたんのカットイン

//--------------------------------------------------------------------------
// リザルトシーンで使用する画像
//--------------------------------------------------------------------------
var TEX_RESULT_BACK = "texture/result_back.png";				// リザルト画面の背景画像
var TEX_SCORE_SHOW_BACK = "texture/score_show_back.png";		// スコア表示の背景画像

//--------------------------------------------------------------------------
// 共通で使用する画像
//--------------------------------------------------------------------------
var TEX_NUMBER = "texture/number_graph_s.png";					// 数字画像


//==========================================================================
// 読み込み配列
//==========================================================================
var TEXTURE_LOAD = [
	TEX_TITLE_BACK,
	TEX_GAME_RIGHT_SPACE,
	TEX_ITEM_GRAPHS,
	TEX_RESULT_BACK,
	TEX_SCORE_SHOW_BACK,
	TEX_NUMBER,
	TEX_START_BUTTON,
	TEX_TUKUMO_TAN,
	TEX_LABI_TAN,
	TEX_TUKUMO_ICON,
	TEX_JUNK_ICON,
	TEX_GET,
	TEX_GAME_BACK,
	TEX_TIME_BACK,
	TEX_IN,
	TEX_WEIGHT_BAR,
	TEX_WEIGHT_BAR_RED,
	TEX_BLACK_BACK,
	TEX_TUKUMO_CUTIN,
	TEX_JUNK_CUTIN,
	TEX_HELP_BUTTON,
	TEX_HELP_1,
	TEX_HELP_2,
	TEX_HELP_3
];


//==========================================================================
// 画像を読み込む関数
//==========================================================================
function TexturePreLoad()
{
	// 画像プリロード
	g_game.preload(TEXTURE_LOAD);
}
