// No #include guards; see https://eigen.tuxfamily.org/dox/TopicCustomizing_Plugins.html

template<typename other_t> EIGEN_DEVICE_FUNC INLINE
TensorCwiseBinaryOp<scalar_rshift_op<Scalar>, Derived const, other_t const> const
operator>>(other_t const& other) const {
  return binaryExpr(other.derived(), scalar_rshift_op<Scalar>());
}

EIGEN_DEVICE_FUNC INLINE
TensorCwiseUnaryOp<internal::bind2nd_op<scalar_rshift_op<Scalar, uint8_t>>, Derived const> const
operator>>(Scalar rhs) const {
  return unaryExpr(internal::bind2nd_op<scalar_rshift_op<Scalar, uint8_t>>(rhs));
}
