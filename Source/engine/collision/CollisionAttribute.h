#pragma once

/// <summary>
/// 衝突判定属性
/// </summary>

const unsigned short COLLISION_ATTR_LANDSHAPE = 0b1 << 0; //地形
const unsigned short COLLISION_ATTR_ALLIES = 0b1 << 1; //味方陣営
const unsigned short COLLISION_ATTR_ENEMYS = 0b1 << 2; //敵陣営
const unsigned short COLLISION_ATTR_YELLOW = 0b1 << 3; //黄色属性
const unsigned short COLLISION_ATTR_PINK = 0b1 << 4; //ピンク属性
const unsigned short COLLISION_ATTR_BLACK = 0b1 << 5; //黒属性
const unsigned short COLLISION_ATTR_GOAL = 0b1 << 6; //ゴール