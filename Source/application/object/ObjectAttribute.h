#pragma once

/// <summary>
/// ゲーム内オブジェクトの属性関連の型定義
/// </summary>

enum Attribute
{
	pink,		//ピンク
	yellow,		//黄色
	black,		//プレイヤーが乗れない色
	Goal,		//ゴール
	NormalObj,   //当たり判定のない普通のオブジェクト
	NReverseObj, //当たり判定のないオブジェクトを反転したもの

};