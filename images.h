//
// Created by frk on 26.01.2020.
//

#ifndef ZADANIE7_IMAGES_H
#define ZADANIE7_IMAGES_H

#include "coordinate.h"
#include "color.h"
#include "functional.h"
#include <functional>
#include <cmath>
#include <type_traits>

using namespace std::placeholders;

using Fraction = double;

template<typename T>
using Base_image = std::function<T(const Point)>;

using Region = Base_image<bool>;
using Image = Base_image<Color>;
using Blend = Base_image<Fraction>;

template<typename T>
Base_image<T> constant(T t);

template<typename T>
Base_image<T> rotate(Base_image<T> image, double phi);

template<typename T>
Base_image<T> translate(Base_image<T> image, Vector v);

template<typename T>
Base_image<T> scale(Base_image<T> image, double s);

template<typename T>
Base_image<T> circle(Point q, double r, T inner, T outer);

template<typename T>
Base_image<T> checker(double d, T this_way, T that_way);

template<typename T>
Base_image<T> polar_checker(double d, int n, T this_way, T that_way);

template<typename T>
Base_image<T> rings(Point q, double d, T this_way, T that_way);

template<typename T>
Base_image<T> vertical_stripe(double d, T this_way, T that_way);

Image cond(Region region, Image this_way, Image that_way);
Image lerp(Blend blend, Image this_way, Image that_way);
Image darken(Image image, Blend blend);
Image lighten(Image image, Blend blend);
Vector point_difference(const Point p, const Point q);

template<typename T>
Base_image<T> constant(T t) {
	return [t] (const Point p __attribute__((unused))) {return t;};
}

template<typename T>
Base_image<T> circle(Point q, double r, T inner, T outer) {
	return [=] (const Point p) {
		return (distance(p, q) <= r) ? inner : outer;
	};
}

template<typename T>
Base_image<T> checker(double d, T this_way, T that_way) {
	return [=] (const Point p) {
		return ((int(floor(p.first / d)) + int(floor(p.second / d))) % 2 == 0) ? this_way : that_way;
	};
}

template<typename T>
Base_image<T> vertical_stripe(double d, T this_way, T that_way) {
	return [=] (const Point p) {
		return (p.first <= d/2 && p.first >= -d/2) ? this_way : that_way;
	};
}

template<typename T>
Base_image<T> rotate(Base_image<T> image, double phi) {
	return compose(to_polar, [phi] (const Point p) { return Point{p.first, p.second - phi, true}; }, from_polar, image);
}

template<typename T>
Base_image<T> translate(Base_image<T> image, Vector v) {
	return compose([v] (const Point p) { return Point{p.first - v.first, p.second - v.second}; } , image);
}

template<typename T>
Base_image<T> scale(Base_image<T> image, double s) {
	return compose([s] (const Point p) { return Point{p.first / s, p.second / s}; }, image);
}

template<typename T>
Base_image<T> polar_checker(double d, int n, T this_way, T that_way) {
	return std::invoke(scale<T>, compose(to_polar, checker(2. * M_PI / n, this_way, that_way)), d*n / (2. * M_PI));
}

template<typename T>
Base_image<T> rings(Point q, double d, T this_way, T that_way) {
	return std::invoke(translate<T>, polar_checker(d, 1, this_way, that_way), point_difference(q, {0, 0}));
}

#endif //ZADANIE7_IMAGES_H
