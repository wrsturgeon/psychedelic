#ifndef BSHIFTOPS_HPP_
#define BSHIFTOPS_HPP_

#include <unsupported/Eigen/CXX11/TensorSymmetry>



template<typename lhs_t, typename rhs_t>
struct scalar_rshift_op : Eigen::internal::binary_op_base<lhs_t, rhs_t> {
  EIGEN_DEVICE_FUNC INLINE lhs_t operator() (lhs_t const& a, rhs_t const& b) const { return a >> b; }
};



#endif // BSHIFTOPS_HPP_
