#pragma once

class SimpleButton
{
public:
	struct ButtonStyle
	{
		ColorF shapeColor;	// 表示するボタンの色
		ColorF textColor;	// 表示する文字の色
		String font;		// フォント
		Point stretched;
		double rounded;
	};

	SimpleButton(const RectF& rect, const String& text,  const ButtonStyle style, const bool& isEnable);

	bool isLeftClicked();
	void update();
	void draw() const;

private:
	RectF rect;
	String text;
	ButtonStyle style;
	bool isEnable;
};
