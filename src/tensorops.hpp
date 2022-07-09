// No #include guards; see https://eigen.tuxfamily.org/dox/TopicCustomizing_Plugins.html

template<typename OtherDerived> EIGEN_DEVICE_FUNC INLINE
const TensorCwiseBinaryOp<scalar_rshift_op<Scalar>, const Derived, const OtherDerived>
operator>>(const OtherDerived& other) const {
  return binaryExpr(other.derived(), scalar_rshift_op<Scalar>());
}
