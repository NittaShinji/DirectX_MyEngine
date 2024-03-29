#pragma once

/// <summary>
/// ゲームに必要なリソースをまとめて読み込む
/// </summary>
class ResourceLoader
{
public:
	
	//一度だけ起動時に一度だけ読み込み
	static void Load();

	//音データ読み込み
	static void SoundLoad();

	//画像データ読み込み
	static void ImageLoad();
};

