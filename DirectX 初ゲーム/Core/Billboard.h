#pragma once
#include "Polygon.h"

class Billboard : public RectPolygon
{
public:
	Billboard(
		std::string texture_name = "none", 
		Layer layer = Layer::DEFAULT, 
		std::string render_space_name = "default"
	) :RectPolygon(texture_name, layer, render_space_name) {};

	void Draw(void) override;

};