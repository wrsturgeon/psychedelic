#ifndef BSHIFTOPS_HPP_
#define BSHIFTOPS_HPP_

#include <unsupported/Eigen/CXX11/TensorSymmetry>



template<typename LhsScalar,typename RhsScalar>
struct scalar_rshift_op : Eigen::internal::binary_op_base<LhsScalar,RhsScalar> {
  EIGEN_DEVICE_FUNC EIGEN_STRONG_INLINE LhsScalar operator() (const LhsScalar& a, const RhsScalar& b) const { return a >> b; }
};



#endif // BSHIFTOPS_HPP_
