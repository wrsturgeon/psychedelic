#ifndef CONV_HPP_
#define CONV_HPP_

#include <unsupported/Eigen/CXX11/TensorSymmetry>

#include "util.hpp"
#include "bitshift.hpp"

// Stupid fucking sqrt & erfc aren't constexpr >:(((
#include "gauss_kernel.hpp"

namespace conv {



template <typename index_t, index_t h, index_t w, index_t c>
EIGEN_DEVICE_FUNC INLINE
Eigen::TensorFixedSize<uint8_t, Eigen::Sizes<((h - 2) >> 1), ((w - 2) >> 1), c>, Eigen::ColMajor, index_t> gauss_half(
      Eigen::TensorFixedSize<uint8_t, Eigen::Sizes<h, w, c>, Eigen::ColMajor, index_t>& src) {
  // Would love to use ldexp but not constexpr :_(
  static constexpr uint8_t _w_p = gauss::kPeriph * 256 + 0.5;
  // static constexpr uint8_t _w_c = gauss::kCenter * 256 + 0.5;
  static constexpr uint8_t _w_c = 256 - (_w_p << 1); // Necessarily adds up to e.g. 256

  typedef Eigen::TensorFixedSize<uint8_t, Eigen::Sizes<3>, Eigen::ColMajor, index_t> kernel_t;
  static const kernel_t kernel{kernel_t().setValues({_w_p, _w_c, _w_p})};
  static constexpr Eigen::array<index_t, 3> patch_y{3, 1, 1};
  // static constexpr Eigen::array<index_t, 3> patch_x{1, 3, 1};
  static constexpr Eigen::array<index_t, 2> shape_y1{3, (h - 2) * w * c};
  static constexpr Eigen::array<index_t, 3> shape_y2{h - 2, w, c};
  static constexpr Eigen::array<index_t, 3> stride_y{2, 1, 1};
  // static constexpr Eigen::array<index_t, 3> stride_x{1, 2, 1};
  static constexpr Eigen::array<Eigen::IndexPair<index_t>, 1> trans{{Eigen::IndexPair<index_t>(0, 0)}};
  Eigen::TensorFixedSize<uint8_t, Eigen::Sizes<h - 2, w, c>, Eigen::ColMajor, index_t> vertical{(src
    .extract_patches(patch_y)   // 3, 1, 1, (h-2)wc
    .reshape(shape_y1)          // 3,       (h-2)wc
    .template cast<uint16_t>()
    .contract(kernel, trans)    // 1,       (h-2)wc
    >> 8)
    .template cast<uint8_t>()
    .reshape(shape_y2)          //  h-2   , w, c
    .stride(stride_y)           // (h-2)/2, w, c
  };
  // Eigen::TensorFixedSize<uint8_t, Eigen::Sizes<h - 2, w, c>, Eigen::ColMajor, index_t> squeeze_v{src.convolve(kernel, vertical)};
  // Eigen::TensorFixedSize<uint8_t, Eigen::Sizes<3, 1, 1,  (h - 2)        * w * c>, Eigen::ColMajor, index_t> _b{src.extract_patches(patch_y)}; // <3, 1, 1, (h-2)wc>
  // Eigen::TensorFixedSize<uint8_t, Eigen::Sizes<3,        (h - 2)        * w * c>, Eigen::ColMajor, index_t> _c{_b.reshape(shape_y1)};         // <3, (h-2)wc>
  // Eigen::TensorFixedSize<uint8_t, Eigen::Sizes<1,        (h - 2)        * w * c>, Eigen::ColMajor, index_t> _d{kernel.contract(_c, mmul)};    // <1, (h-2)wc>
  // Eigen::TensorFixedSize<uint8_t, Eigen::Sizes<           h - 2,          w,  c>, Eigen::ColMajor, index_t> _e{_d.reshape(shape_y2)};         // <h-2, w, c>
  // Eigen::TensorFixedSize<uint8_t, Eigen::Sizes<         ((h - 2) >> 1),   w,  c>, Eigen::ColMajor, index_t> _f{_e.stride(stride_y)};          // <hh, w, c>
  // Eigen::TensorFixedSize<uint8_t, Eigen::Sizes<         ((h - 2) >> 1),   w,  c>, Eigen::ColMajor, index_t> vertical{_f};
  util::write(vertical, "vertical.png");
  return {};
}



} // namespace conv

#endif // CONV_HPP_
