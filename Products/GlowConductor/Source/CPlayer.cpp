#include "CPlayer.h"
#include "CalclateMcr.h"
#include "CCScriptSupport.h"
#include "SCameraTargetMgr.h"
#include "CustomParticle.h"
#include "SGraphMgr.h"
#include "SimpleAudioEngine.h"
#include "SCameraTargetMgr.h"

using namespace CocosDenshion;
USING_NS_CC;

static const int SCORE_DANGER_ACTION_TAG = 50;

//**********************************************************************
// コンストラクタ
//**********************************************************************
CPlayer::CPlayer()
{
	// パラメータの設定
	_turnFlag = false;
	_speed = 7.0f;
	_moveRadian = 0.0f;
	_turnRadianMax = 0.02f;
	_radius = 64.0f;
	_remainTap = TOUCH_REMAIN_INIT;
	_hitDeleteFlag = false;
	_killFlag = false;

	Size winSize = Director::getInstance()->getVisibleSize();

	// スコアの設定
	_score = 0;
	_pScoreLabel = Label::create("a", "fonts/HuiFontP29.ttf", 100);
	_pScoreLabel->setAnchorPoint(Point(0.0f, 0.0f));
	_pScoreLabel->setPosition(
		winSize.width * 0.35f,
		0);

	// 残りタップ回数の背景設定
	_pTapRemainBack_1 = Sprite::createWithTexture(SGraphMgr::Instance()->GetBatchTexture("touchWindow"));
	_pTapRemainBack_1->setPosition(Point(
		_pTapRemainBack_1->getContentSize().width / 2,
		winSize.height - _pTapRemainBack_1->getContentSize().height / 2));
	_pTapRemainBack_1->runAction(RepeatForever::create(
		EaseInOut::create(RotateBy::create(2.0f, 500.0f), 2.0f)));

	_pTapRemainBack_2 = Sprite::createWithTexture(SGraphMgr::Instance()->GetBatchTexture("touchWindow"));
	_pTapRemainBack_2->setPosition(_pTapRemainBack_1->getPosition());
	_pTapRemainBack_2->setScale(1.0f);
	_pTapRemainBack_2->setBlendFunc(BlendFunc::ADDITIVE);
	_pTapRemainBack_2->runAction(RepeatForever::create(
		EaseInOut::create(RotateBy::create(1.7f, -288.0f), 2.0f)));

	// 残りタップ回数設定
	String* pTapRemain = String::createWithFormat("%d", _remainTap);
	_pTapRemainLabel = Label::create(pTapRemain->getCString(), "fonts/HuiFontP29.ttf", 130);
	_pTapRemainLabel->setPosition(_pTapRemainBack_1->getPosition());
	_pTapRemainLabel->setColor(Color3B(0, 0, 255));

	// 大きさを合わせる
	this->setScale(_radius / BALL_RADIUS);

	// フラグを初期化
	_removeFlag = false;

	// 遷移カウントを初期化
	_removeCount = REMOVE_COUNT;

}



//**********************************************************************
// デストラクタ
//**********************************************************************
CPlayer::~CPlayer()
{

}



//**********************************************************************
// 親ノードを設定する
//**********************************************************************
void CPlayer::SetParent(Node* parentNode)
{
	_pParentNode = parentNode;
}



//**********************************************************************
// 進む処理
//**********************************************************************
void CPlayer::Move()
{
	// キルフラグが立っていたら通らない
	if (_killFlag == true) return;

	// 曲がる必要がある場合、曲げる
	if (_turnFlag == true) Turn();

	// X,Y方向の移動距離を計算
	float moveX = _speed * cosf(_moveRadian);
	float moveY = _speed * sinf(_moveRadian);

	// 現在の位置に、移動分を加算する
	this->setPosition(Point(this->getPosition().x + moveX, this->getPosition().y + moveY));

	// プレイヤーが画面外に出た場合、キルフラグを立てる
	if ((TargetPointer->getPositionX() - Director::getInstance()->getWinSize().width / 2 + this->GetRadius() > this->getPosition().x ||
		this->getPosition().y < this->GetRadius() ||
		this->getPosition().y > Director::getInstance()->getWinSize().height - this->GetRadius()) &&
		_killFlag == false)
	{
		this->KillPlayer();
		this->CollisionFunction();
	}
}



//**********************************************************************
// 曲がる処理
//**********************************************************************
void CPlayer::Turn()
{
	// タッチポイントと現在地との角度を求める
	float distanceX = _touchedPoint.x - this->getPosition().x;
	float distanceY = _touchedPoint.y - this->getPosition().y;

	float radian = atan2f(distanceY, distanceX);

	// 差によって、右回り、左回りを決める
	float defRadian = radian - _moveRadian;

	if (defRadian > 0.0f)
	{
		if (defRadian < PI)
		{
			// 角度が、最大曲がり角度より小さい場合、その差分回転をさせ、フラグを切る
			if (defRadian < _turnRadianMax)
			{
				_moveRadian -= defRadian;
				_turnFlag = false;
			}
			else	_moveRadian += _turnRadianMax;
		}
		if (defRadian > PI){
			if ((defRadian - PI * 2) > _turnRadianMax)
			{
				_moveRadian += (defRadian - PI * 2);
				_turnFlag = false;
			}
			else	_moveRadian -= _turnRadianMax;
		}
	}
	else if (defRadian < 0.0f)
	{
		if (defRadian > -PI)
		{
			// 角度が、最大曲がり角度より小さい場合、その差分回転をさせ、フラグを切る
			if (-defRadian < _turnRadianMax)
			{
				_moveRadian += defRadian;
				_turnFlag = false;
			}
			else	_moveRadian -= _turnRadianMax;
		}
		if (defRadian < -PI)
		{
			if ((defRadian + PI * 2) < _turnRadianMax)
			{
				_moveRadian += (defRadian + PI * 2);
				_turnFlag = false;
			}
			else	_moveRadian += _turnRadianMax;
		}
	}

	// 進行角度の補正を行う
	if (_moveRadian > PI * 2)	_moveRadian = _moveRadian - 2 * PI;
	if (_moveRadian < -PI * 2)	_moveRadian = _moveRadian + 2 * PI;

}



//**********************************************************************
// タッチされた座標を取得する
//**********************************************************************
void CPlayer::SetTouchPoint(const Point& point)
{
	_touchedPoint = point;
	_turnFlag = true;
}



//**********************************************************************
// プレイヤーパーティクルを発生させる関数
//**********************************************************************
void CPlayer::ParticleCreater(float delta)
{
	auto pParticle = ParticleExplosion::createWithTotalParticles(1);
	pParticle->setStartSize(100);
	pParticle->setLife(0.5f);
	pParticle->setDuration(0.1f);
	pParticle->setSpeed(50.0f);
	pParticle->setStartSpin(600.0f);
	pParticle->setPositionType(ParticleSystem::PositionType::RELATIVE);
	pParticle->setPosition(this->getPosition());
	pParticle->setBlendAdditive(true);
	pParticle->setStartColor(Color4F(1.0f, 1.0f, 0.2f, 0.5f));
	pParticle->setAutoRemoveOnFinish(true);
	this->getParent()->addChild(pParticle);
}



//**************************************************************************************************
// クリエイト関数
//**************************************************************************************************
CPlayer* CPlayer::createWithTexture(Texture2D *texture)
{
	CPlayer *sprite = new CPlayer();
	if (sprite && sprite->initWithTexture(texture))
	{
		sprite->autorelease();
		sprite->schedule(schedule_selector(CPlayer::ParticleCreater));
		
		// 点滅するアクションをつける
		auto changeColor_1 = TintTo::create(0.2f, 255.0f, 255.0f, 0.0f);
		auto changeColor_2 = TintTo::create(0.2f, 255.0f, 255.0f, 255.0f);

		auto changeColor = Sequence::createWithTwoActions(changeColor_1, changeColor_2);

		sprite->runAction(RepeatForever::create(changeColor));

		return sprite;
	}
	CC_SAFE_DELETE(sprite);
	return nullptr;
}



//**************************************************************************************************
// 衝突時
//**************************************************************************************************
void CPlayer::CollisionFunction(IPlayerStateChange* playerStateChanger)
{
	//--------------------------------------------------------------
	// キルフラグが立っていたら、消失エフェクトを追加する
	//--------------------------------------------------------------
	if (_killFlag == true && _hitDeleteFlag == false)
	{
		_hitDeleteFlag = true;
		ActionInterval* pActionBig = EaseOut::create(ScaleTo::create(0.8f, 1.5f), 2.0f);
		ActionInterval* pActionOpacity = FadeOut::create(0.8f);
		ActionInterval* pDeleteAction = Spawn::createWithTwoActions(pActionBig, pActionOpacity);
		this->runAction(pDeleteAction);

		this->schedule(schedule_selector(CPlayer::CountDown));

		//------------------------------------------------------------
		// 音を鳴らす
		//------------------------------------------------------------
		SimpleAudioEngine::getInstance()->playEffect("sound/disappear_se.mp3");

		//------------------------------------------------------------
		// 画面全体を揺らす
		//------------------------------------------------------------
		float actionTime = 0.6f;			// 画面揺れてる時間

		Node* pGameMainNode = this->getParent()->getParent();
		pGameMainNode->runAction(
			Sequence::create(
			RotateTo::create(actionTime / 7.0f, 20.0f),
			RotateTo::create(actionTime / 7.0f, -20.0f),
			RotateTo::create(actionTime / 7.0f, 10.0f),
			RotateTo::create(actionTime / 7.0f, -10.0f),
			RotateTo::create(actionTime / 7.0f, 5.0f),
			RotateTo::create(actionTime / 7.0f, -5.0f),
			RotateTo::create(actionTime / 7.0f, 0.0f),
			nullptr));

		pGameMainNode->runAction(
			Sequence::create(
			EaseOut::create(ScaleTo::create(actionTime / 2.0f, 1.6f), 2.0f),
			EaseIn::create(ScaleTo::create(actionTime / 2.0f, 1.0f), 2.0f),
			nullptr));

		//------------------------------------------------------------
		// 画面のスクロールを止める
		//------------------------------------------------------------
		SCameraTargetMgr::Instance()->GetCameraTarget()->unscheduleUpdate();
		SCameraTargetMgr::Instance()->GetCameraTarget()->InitMoveX();
	}
}



//**************************************************************************************************
// removeCountを減らす関数
//**************************************************************************************************
void CPlayer::CountDown(float delta)
{
	_removeCount--;
	if (_removeCount <= 0)
	{
		_removeFlag = true;
		this->unschedule(schedule_selector(CPlayer::CountDown));
	}
}



//**************************************************************************************************
// スコアの更新
//**************************************************************************************************
void CPlayer::UpdateScore()
{
	_score = (_score < this->getPosition().x * 0.1f ? this->getPosition().x * 0.1f : _score);
	String* scoreString = String::createWithFormat("Score : %d", (int)_score );
	_pScoreLabel->setString(scoreString->getCString());
}



//**********************************************************************
// タップ回数の更新
//**********************************************************************
void CPlayer::UpdateTapRemain()
{
	String* pTapRemain = String::createWithFormat("%d", _remainTap);
	_pTapRemainLabel->setString(pTapRemain->getCString());
	auto actMgr = Director::getInstance()->getActionManager();

	// タップ回数が5回以下の場合、アクションをつける
	if (_remainTap <= 5)
	{
		if (actMgr->getActionByTag(SCORE_DANGER_ACTION_TAG, _pTapRemainLabel) != nullptr)
			return;

		// スコアのアクションをあらかじめ登録しておく
		ActionInterval* bigAction = EaseInOut::create(ScaleTo::create(0.5f, 2), 2.0f);
		ActionInterval* smallAction = EaseInOut::create(ScaleTo::create(0.5f, 1), 2.0f);

		ActionInterval* pRunAction = nullptr;


		pRunAction = Sequence::create(smallAction, bigAction, nullptr);
		pRunAction = RepeatForever::create(pRunAction);
		pRunAction->setTag(SCORE_DANGER_ACTION_TAG);

		_pTapRemainLabel->runAction(pRunAction);
		_pTapRemainLabel->setColor(Color3B::RED);

	}
	else
	{
		_pTapRemainLabel->stopActionByTag(SCORE_DANGER_ACTION_TAG);
		_pTapRemainLabel->setColor(Color3B::BLACK);
	}


}



//**********************************************************************
//	スコアとタップ回数を表示
//**********************************************************************
void CPlayer::ShowState(Node* pUiLayer)
{
	pUiLayer->addChild(_pScoreLabel);
	pUiLayer->addChild(_pTapRemainBack_1);
	pUiLayer->addChild(_pTapRemainBack_2);
	pUiLayer->addChild(_pTapRemainLabel);
}



//**********************************************************************
// プレイヤーの状態変化をオーバーライド
//**********************************************************************
void CPlayer::AddTouch(int add)
{ 
	this->_remainTap += add;

	// タップ回数アップのアクションをつける
	ActionInterval* scaleUp = Sequence::createWithTwoActions(EaseOut::create(ScaleTo::create(0.1f, 1.4f), 1.0f), EaseOut::create(ScaleTo::create(0.5f, 1.0f), 1.f));

	_pTapRemainLabel->runAction(scaleUp);
}
void CPlayer::DownTouch(int down)
{ 
	_remainTap -= down;
	if (_remainTap < 0) _remainTap = 0;
}
void CPlayer::ScoreUp(int score)
{
	// スコアを挙げる
	_score += score;
}
void CPlayer::ScoreDown(int score){ _score -= score; }
void CPlayer::KillPlayer(){ _killFlag = true; }