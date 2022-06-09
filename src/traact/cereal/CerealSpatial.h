/** Copyright (C) 2022  Frieder Pankratz <frieder.pankratz@gmail.com> **/

#ifndef TRAACTMULTI_CEREALSPATIAL_H
#define TRAACTMULTI_CEREALSPATIAL_H

#include <cereal/types/memory.hpp>
#include <cereal/types/vector.hpp>
#include <cereal/types/map.hpp>
#include <cereal/archives/binary.hpp>
#include <type_traits>
#include <fmt/format.h>

#include <traact/spatial.h>

namespace cereal {

template<class Archive, class _Scalar, int _Rows, int _Cols, int _Options, int _MaxRows, int _MaxCols>
inline
typename std::enable_if<!cereal::traits::is_output_serializable<cereal::BinaryData<_Scalar>, Archive>::value,
                        void>::type
save(Archive &ar, const Eigen::Matrix<_Scalar, _Rows, _Cols, _Options, _MaxRows, _MaxCols> &m) {
    for (int i = 0; i < _Cols; ++i)
        for (int j = 0; j < _Rows; ++j)
            ar(cereal::make_nvp(fmt::format("m{0}{1}", j, i), m(j, i)));
};

template<class Archive, class _Scalar, int _Rows, int _Cols, int _Options, int _MaxRows, int _MaxCols>
inline
typename std::enable_if<cereal::traits::is_output_serializable<cereal::BinaryData<_Scalar>, Archive>::value, void>::type
save(Archive &ar, const Eigen::Matrix<_Scalar, _Rows, _Cols, _Options, _MaxRows, _MaxCols> &matrix) {
    auto rows = static_cast<std::int32_t>(matrix.rows());
    auto cols = static_cast<std::int32_t>(matrix.cols());
    ar(CEREAL_NVP(rows));
    ar(CEREAL_NVP(cols));
    ar(binary_data(matrix.data(), rows * cols * sizeof(_Scalar)));
};

template<class Archive, class _Scalar, int _Rows, int _Cols, int _Options, int _MaxRows, int _MaxCols>
inline
typename std::enable_if<!cereal::traits::is_input_serializable<cereal::BinaryData<_Scalar>, Archive>::value, void>::type
load(Archive &ar, Eigen::Matrix<_Scalar, _Rows, _Cols, _Options, _MaxRows, _MaxCols> &m) {
    for (int i = 0; i < _Cols; ++i)
        for (int j = 0; j < _Rows; ++j)
            ar(cereal::make_nvp(fmt::format("m{0}{1}", j, i), m(j, i)));
};

template<class Archive, class _Scalar, int _Rows, int _Cols, int _Options, int _MaxRows, int _MaxCols>
inline
typename std::enable_if<cereal::traits::is_input_serializable<cereal::BinaryData<_Scalar>, Archive>::value, void>::type
load(Archive &ar, Eigen::Matrix<_Scalar, _Rows, _Cols, _Options, _MaxRows, _MaxCols> &matrix) {
    std::int32_t rows;
    std::int32_t cols;
    ar(CEREAL_NVP(rows));
    ar(CEREAL_NVP(cols));

    matrix.resize(rows, cols);

    ar(binary_data(matrix.data(), static_cast<size_t>(rows * cols * sizeof(_Scalar))));
};

template<class Archive, class Scalar_>
inline
void serialize(Archive &archive,
               Eigen::Quaternion<Scalar_> &m) {
    archive(cereal::make_nvp("x", m.x()), cereal::make_nvp("y", m.y()),
            cereal::make_nvp("z", m.z()), cereal::make_nvp("w", m.w()));
};

//template<class Archive, class _Scalar, int _Dim, int _Mode, int _Options>
template<class Archive>
void save(Archive &archive,
          const Eigen::Affine3d &m) {
    Eigen::Vector3d trans = m.translation();
    Eigen::Quaterniond rot = Eigen::Quaterniond(m.rotation());
    archive(cereal::make_nvp("translation", trans),
            cereal::make_nvp("rotation", rot));
};

//template<class Archive, class _Scalar, int _Dim, int _Mode, int _Options>
template<class Archive>
void load(Archive &archive,
          Eigen::Affine3d &m) {
    Eigen::Vector3d trans;
    Eigen::Quaterniond rot;
    archive(cereal::make_nvp("translation", trans),
            cereal::make_nvp("rotation", rot));

    m.setIdentity();
    m.translate(trans);
    m.rotate(rot);
};

template<class Archive, class Scalar_>
inline
void serialize(Archive &archive,
               Eigen::Translation<Scalar_, 2> &m) {
    archive(cereal::make_nvp("x", m.x()), cereal::make_nvp("y", m.y()));
};

template<class Archive, class Scalar_>
inline
void serialize(Archive &archive,
               Eigen::Translation<Scalar_, 3> &m) {
    archive(cereal::make_nvp("x", m.x()), cereal::make_nvp("y", m.y()), cereal::make_nvp("z", m.z()));
};

}

#endif //TRAACTMULTI_CEREALSPATIAL_H
