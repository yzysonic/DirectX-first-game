#pragma once

#include "Common.h"
#include "Layer.h"
#include "RendererType.h"
#include "Color.h"

//*****************************************************************************
// �\���̒�`
//*****************************************************************************
class ObjectBase;

class Drawable
{
public:
	ObjectBase *object;			// ���L����I�u�W�F�N�g�ւ̎Q��
	RendererType rendType;	// �`����@�w��
	int list_index;			// ���ʔԍ�

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
	Layer layer;	// �`��̃��C���[
	Color color;	// �F
	std::string render_space;
	int render_space_index;

	friend class RenderSpace;

private:
	bool visible;
};
