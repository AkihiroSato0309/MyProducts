ItemMgr = Class.create(EventTarget,{
	// コンストラクタ
	initialize:function(addNode)
	{
		EventTarget.call(this);
		
		this.m_items = [];			// アイテムの配列
		this.m_addNode = addNode;	// アイテムを追加するノード
		
		//===============================================================
		// 役割		: 配列にアイテムを追加する
		// 第一引数	: 追加するアイテム
		// 戻り値		: なし
		//===============================================================
		this.AddItem = function(item)
		{
			this.m_items.push(item);
		}
		
		//===============================================================
		// 更新処理
		//===============================================================
		this.onenterframe = function()
		{
			for(var i = 0; i < this.m_items.length; i++)
			{
				// 地面に落ちたら、当たり判定配列からは消える
				if(this.m_items[i].m_removeFlag == true )
				{
					this.m_items.splice(i, 1);
				}
			}
		}
		
		//===============================================================
		// 役割		: あたり判定をとる
		// 第一引数	:　商品とのあたり判定をとるオブジェクト
		// 戻り値		: 当たっていたらtrue, そうでなければfalse
		//===============================================================
		this.Hit = function(player)
		{
			var hitFlag = false;
			for(var i = 0; i < this.m_items.length; i++)
			{
				// プレイヤーと当たり判定をとる
				if(player.intersect(this.m_items[i]))
				{
					// 商品総計を増やす
					g_totalScore += this.m_items[i].m_price;
					
					this.m_items[i].parentNode.removeChild(this.m_items[i]);
					this.m_items.splice(i, 1);
			
					//-----------------------------------------------------------
					// ゲットラベルを表示させる
					//-----------------------------------------------------------
					var getSprite = new Sprite(64, 32);
					getSprite.image = g_game.assets[TEX_GET];
					getSprite.x = player.x + player.width * 0.5 - getSprite.width * 0.5;
					getSprite.y = player.y - 20;
					this.m_addNode.parentNode.addChild(getSprite);
					
					// 移動して消える処理
					getSprite.tl.moveBy(0, -20, 40).then(function(){DeleteThis(this);});					
					
					hitFlag = true;
				}
			}
			
			return hitFlag;
		}
		
	}
})
