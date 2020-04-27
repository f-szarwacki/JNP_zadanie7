//
// Created by frk on 26.01.2020.
//

#include "images.h"
#include "coordinate.h"
#include "color.h"

Vector point_difference(const Point p, const Point q) {
	return Vector{p.first - q.first, p.second - q.second};
}

Image cond(Region region, Image this_way, Image that_way) {
	return lift([=] (const Point p, bool b) {return b ? this_way(p) : that_way(p);}, compose(), region);
}

Image lerp(Blend blend, Image this_way, Image that_way) {
	return lift(std::bind(&Color::weighted_mean, _1, _2, _3), this_way, that_way, blend);
}

Image darken(Image image, Blend blend) {
	return lerp(blend, image, constant(Colors::black));
}
Image lighten(Image image, Blend blend) {
	return lerp(blend, image, constant(Colors::white));
}


