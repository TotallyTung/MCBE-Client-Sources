#pragma once
#include <variant>

class ItemStackNetIdVariant {
private:
	class DmmMojang {
		uint64_t a;
		uint64_t b;
	};
	std::variant<DmmMojang> id;
};