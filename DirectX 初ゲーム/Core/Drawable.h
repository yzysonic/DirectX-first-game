#pragma once

#include "Common.h"
#include "Layer.h"
#include "RendererType.h"
#include "Color.h"

//*****************************************************************************
// 構造体定義
//*****************************************************************************
class ObjectBase;

class Drawable
{
public:
	ObjectBase *object;			// 所有するオブジェクトへの参照
	RendererType rendType;	// 描画方法指定
	int list_index;			// 識別番号

	Drawable(Layer layer, std::string render_space = "default");
	virtual ~Drawable(void);

	virtual void draw(void) = 0;
	Layer getLayer(void);
	Color getColor(void);
	virtual void setColor(Color color);
	virtual void setColor(unsigned char r, unsigned char g, unsigned char b, unsigned char a);
	float getOpacity(void);
	void setOpacity(float opacity);
	void setVisibility(bool visible);

protected:
	Layer layer;	// 描画のレイヤー
	Color color;	// 色
	std::string render_space;
	int render_space_index;

	friend class RenderSpace;

private:
	bool visible;
};
