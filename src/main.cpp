#include "util.hpp"
#include "conv.hpp"

#include <iostream>

#include "bshiftops.hpp"
#define EIGEN_TENSORBASE_PLUGIN tensorops.hpp
#include <unsupported/Eigen/CXX11/TensorSymmetry>



static constexpr kIndexType kImageHeight = IMAGE_H;
static constexpr kIndexType kImageWidth = IMAGE_W;
typedef Eigen::Sizes<kImageHeight, kImageWidth, 3> kImageSize;
typedef Eigen::TensorFixedSize<uint8_t, kImageSize, Eigen::RowMajor, kIndexType> ImageRowMajor;
typedef Eigen::TensorFixedSize<uint8_t, kImageSize, Eigen::ColMajor, kIndexType> ImageColMajor;



int main(int argc, char** argv) {
  if (argc != 3) {
    std::cerr << "Usage: `" << argv[0] << " <src-image-path> <dst-image-path>`\n";
    return 1;
  }
  cv::Mat const src{cv::imread(argv[1])};
  ImageRowMajor rm{util::wrap<kImageHeight, kImageWidth>(src)};
  ImageColMajor cm{util::col_major(rm)};
  util::write(cm, argv[1]);
  // util::write(conv::gauss_half(cm), argv[2]);
  return 0;
}
