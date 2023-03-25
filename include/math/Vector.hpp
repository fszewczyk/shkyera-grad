#include <stdlib.h>

#pragma once

namespace st {

template <typename T, size_t size> class Vector {};

using Vector2i = Vector<double, 2>;
using Vector2f = Vector<float, 2>;
using Vector2d = Vector<int, 2>;

using Vector3i = Vector<double, 3>;
using Vector3f = Vector<float, 3>;
using Vector3d = Vector<int, 3>;

using Vector4i = Vector<double, 4>;
using Vector4f = Vector<float, 4>;
using Vector4d = Vector<int, 4>;

} // namespace st