

#ifndef OPTION_H_
#define OPTION_H_

#include "optional.hpp"

template <typename T>
using Option = std::experimental::optional<T>;

template<typename T>
Option<T> No()
{
	return Option<T>();
}

#endif
