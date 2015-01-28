//=================================================================
// 内容		: 指定したスプライトの透過度を指定した値に変える
// 戻り値		: なし
// 第一引数	: 指定スプライト
// 第二引数	: 指定透過度
//=================================================================
var SetSpriteOpacity = function(sprite, opacity)
{
	sprite.opacity = opacity;
}


//=================================================================
// 内容		: 指定したノードを親ノードから削除する
// 戻り値		: なし
// 第一引数	: 指定スプライト
//=================================================================
var DeleteThis = function(sprite)
{
	sprite.parentNode.removeChild(sprite);
}