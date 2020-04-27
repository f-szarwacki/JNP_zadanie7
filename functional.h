//
// Created by frk on 26.01.2020.
//

#ifndef ZADANIE7_FUNCTIONAL_H
#define ZADANIE7_FUNCTIONAL_H

#include <functional>

template <class... Fs>
auto compose(Fs... funcs);

template<typename H, typename... Fs>
auto lift(H h, Fs... fs);

namespace Detail {
	template<class F, class G>
	auto operator>>(F &&f, G &&g) {
		return [f, g] (auto a) { return g(f(a)); };
	}

	template <class... Fs>
	auto compose_detail(Fs... fs) {
		return (... >> fs);
	}

	template <class... Fs>
	auto compose_detail() {
		return [] (auto x) {return x;};
	}
}

template <class... Fs>
auto compose(Fs... fs) {
	return Detail::compose_detail(fs...);
}

template<typename H, typename... Fs>
auto lift(H h, Fs... fs) {
	return [h, fs...] (auto x) {
		return h(fs(x)...);
	};
}

#endif //ZADANIE7_FUNCTIONAL_H
