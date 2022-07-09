#include "util.hpp"
#include "conv.hpp"

#include <iostream>

#include "bshiftops.hpp"
#define EIGEN_TENSORBASE_PLUGIN tensorops.hpp
#include <unsupported/Eigen/CXX11/TensorSymmetry>



typedef Eigen::Sizes<IMAGE_H, IMAGE_W, 3> kImageSize;
typedef Eigen::TensorFixedSize<uint8_t, kImageSize, Eigen::RowMajor, kIndexType> ImageRowMajor;
typedef Eigen::TensorFixedSize<uint8_t, kImageSize, Eigen::ColMajor, kIndexType> ImageColMajor;



int main(int argc, char** argv) {
  if (argc != 3) {
    std::cerr << "Usage: `" << argv[0] << " <src-image-path> <dst-image-path>`\n";
    return 1;
  }
  cv::Mat const src{cv::imread(argv[1])};
  ImageRowMajor rm{util::wrap<IMAGE_H, IMAGE_W>(src)};
  ImageColMajor cm{util::col_major(rm)};
  auto poo = conv::gauss_half(cm);
  // util::write(cm, argv[1]);
  util::write(poo, argv[2]);
  return 0;
}
