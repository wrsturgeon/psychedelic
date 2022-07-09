#ifndef UTIL_HPP_
#define UTIL_HPP_

#include <stdint.h>
#include <unsupported/Eigen/CXX11/TensorSymmetry>
#include <opencv2/imgcodecs.hpp>

#define STR(x) STR_(x)
#define STR_(x) #x

typedef uint8_t kDefaultImageType;
typedef Eigen::DenseIndex kIndexType;

#define MOVE // std::move on/off

namespace util {



template <typename dtype, typename index_t, index_t h, index_t w, index_t c, int layout>
EIGEN_DEVICE_FUNC INLINE
const Eigen::TensorFixedSize<dtype, Eigen::Sizes<h, w, c>, !layout, index_t> memory_transpose(
      Eigen::TensorFixedSize<dtype, Eigen::Sizes<h, w, c>,  layout, index_t> const& src) {
  typedef Eigen::TensorFixedSize<dtype, Eigen::Sizes<c, w, h>, layout, index_t> Confused; // TODO @wrsturgeon: TensorMap %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
  static constexpr Eigen::array<index_t, 3> shuffler{2, 1, 0};
  return MOVE(Eigen::TensorLayoutSwapOp<Confused>(Confused(src.shuffle(shuffler))));
}

template <typename dtype, typename index_t, int layout>
EIGEN_DEVICE_FUNC INLINE
const Eigen::Tensor<dtype, 3, !layout, index_t> memory_transpose(
      Eigen::Tensor<dtype, 3,  layout, index_t> const& src) {
  typedef Eigen::Tensor<dtype, 3, layout, index_t> Confused; // TODO @wrsturgeon: TensorMap %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
  static constexpr Eigen::array<index_t, 3> shuffler{2, 1, 0};
  return MOVE(Eigen::TensorLayoutSwapOp<Confused>(Confused(src.shuffle(shuffler))));
}



template <typename dtype, typename index_t, index_t h, index_t w, index_t c, int layout>
EIGEN_DEVICE_FUNC INLINE
const Eigen::TensorFixedSize<dtype, Eigen::Sizes<h, w, c>, Eigen::RowMajor, index_t> row_major(
      Eigen::TensorFixedSize<dtype, Eigen::Sizes<h, w, c>, layout, index_t> const& src) {
  if constexpr (layout == Eigen::RowMajor) { return MOVE(src); }
  return MOVE(memory_transpose(src));
}

template <typename dtype, typename index_t, int layout>
EIGEN_DEVICE_FUNC INLINE
const Eigen::Tensor<dtype, 3, Eigen::RowMajor, index_t> row_major(
      Eigen::Tensor<dtype, 3, layout, index_t> const& src) {
  if constexpr (layout == Eigen::RowMajor) { return MOVE(src); }
  return MOVE(memory_transpose(src));
}



template <typename dtype, typename index_t, index_t h, index_t w, index_t c, int layout>
EIGEN_DEVICE_FUNC INLINE
const Eigen::TensorFixedSize<dtype, Eigen::Sizes<h, w, c>, Eigen::ColMajor, index_t> col_major(
      Eigen::TensorFixedSize<dtype, Eigen::Sizes<h, w, c>, layout, index_t> const& src) {
  if constexpr (layout == Eigen::ColMajor) { return MOVE(src); }
  return MOVE(memory_transpose(src));
}

template <typename dtype, typename index_t, int layout>
EIGEN_DEVICE_FUNC INLINE
const Eigen::Tensor<dtype, 3, Eigen::ColMajor, index_t> col_major(
      Eigen::Tensor<dtype, 3, layout, index_t> const& src) {
  if constexpr (layout == Eigen::ColMajor) { return MOVE(src); }
  return MOVE(memory_transpose(src));
}



template <typename dtype = kDefaultImageType>
EIGEN_DEVICE_FUNC INLINE
Eigen::TensorMap<Eigen::Tensor<dtype, 3, Eigen::RowMajor, kIndexType>> wrap(cv::Mat const& src) {
  CV_CheckTypeEQ(src.type(), CV_MAKETYPE(cv::traits::Type<dtype>::value, src.channels()), "util::wrap dtype != src dtype");
  return MOVE(Eigen::TensorMap<Eigen::Tensor<dtype, 3, Eigen::RowMajor, kIndexType>>(
        static_cast<dtype*>(src.data), src.rows, src.cols, src.channels()));
}

template <kIndexType h, kIndexType w, kIndexType c = 3, typename dtype = kDefaultImageType>
EIGEN_DEVICE_FUNC INLINE
Eigen::TensorMap<Eigen::TensorFixedSize<dtype, Eigen::Sizes<h, w, c>, Eigen::RowMajor, kIndexType>> wrap(cv::Mat const& src) {
  CV_CheckTypeEQ(src.type(), CV_MAKETYPE(cv::traits::Type<dtype>::value, src.channels()), "util::wrap dtype != src dtype");
  if (h != src.rows) { throw std::runtime_error("util::wrap passed inconsistent height"); }
  if (w != src.cols) { throw std::runtime_error("util::wrap passed inconsistent width"); }
  if (c != src.channels()) { throw std::runtime_error("util::wrap passed inconsistent channel count"); }
  return MOVE(Eigen::TensorMap<Eigen::TensorFixedSize<dtype, Eigen::Sizes<h, w, c>, Eigen::RowMajor, kIndexType>>(
        static_cast<dtype*>(src.data), src.rows, src.cols, src.channels()));
}



template <typename dtype, typename index_t, index_t h, index_t w, index_t c, int layout>
EIGEN_DEVICE_FUNC INLINE
void write(Eigen::TensorFixedSize<dtype, Eigen::Sizes<h, w, c>, layout, index_t> const& src, std::string const& path) {
  cv::imwrite(path, cv::Mat(static_cast<int>(h), static_cast<int>(w),
        static_cast<int>(CV_MAKETYPE(cv::DataType<dtype>::type, c)),
        const_cast<void*>(static_cast<void const*>(row_major(src).data())),
        static_cast<size_t>(0)));
}

template <typename dtype, typename index_t>
EIGEN_DEVICE_FUNC INLINE
void write(Eigen::Tensor<dtype, 3, Eigen::ColMajor, index_t> const& src, std::string const& path) {
  cv::imwrite(path, cv::Mat(src.dimension(0), src.dimension(1),
        static_cast<int>(CV_MAKETYPE(cv::DataType<dtype>::type, src.dimension(2))),
        const_cast<void*>(static_cast<void const*>(row_major(src).data())),
        static_cast<size_t>(0)));
}



} // namespace util

#endif // UTIL_HPP_
