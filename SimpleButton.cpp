#include "SimpleButton.h"

SimpleButton::SimpleButton(const RectF& rect, const String& text, const ButtonStyle style, const bool& isEnable)
	:rect(rect)
	, text(text)
	, style(style)
	, isEnable(isEnable)
{

}

// 左クリックしたときTrueを返す
bool SimpleButton::isLeftClicked()
{
	bool clicked = false;

	Point stretched = style.stretched;
	double rounded = style.rounded;
	// ボタンが有効ならTrueを返す
	if (isEnable)
	{
		if (rect.stretched(stretched).rounded(rounded).leftClicked())
		{
			clicked = true;
		}
	}

	return clicked;
}

void SimpleButton::update()
{
	
}

// ボタンを描画する
void SimpleButton::draw() const
{
	const RectF body{ rect.pos, rect.w, rect.h };
	Point stretched = style.stretched;
	double rounded = style.rounded;
	String font = style.font;
	ColorF shapeColor = style.shapeColor;
	ColorF textColor = style.textColor;

	// ボタンが有効なときは指定したスタイルを反映
	if (isEnable)
	{
		// マウスオーバー時
		if (body.stretched(stretched).rounded(rounded).mouseOver())
		{
			Cursor::RequestStyle(CursorStyle::Hand);
			rect.stretched(stretched).rounded(rounded).draw(shapeColor);
		}
		else
		{
			rect.stretched(stretched).rounded(rounded).drawFrame(3.0, shapeColor);
		}
		FontAsset(font)(text).drawAt((body.h * 0.5), body.center(), textColor);
	}
	else
	{
		rect.stretched(stretched).rounded(rounded).draw(Palette::Dimgray);
		FontAsset(font)(text).drawAt((body.h * 0.5), body.center(), Palette::Lightgray);
	}

}
