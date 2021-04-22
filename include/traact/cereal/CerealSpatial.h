/*  BSD 3-Clause License
 *
 *  Copyright (c) 2020, FriederPankratz <frieder.pankratz@gmail.com>
 *  All rights reserved.
 *
 *  Redistribution and use in source and binary forms, with or without
 *  modification, are permitted provided that the following conditions are met:
 *
 *  1. Redistributions of source code must retain the above copyright notice, this
 *     list of conditions and the following disclaimer.
 *
 *  2. Redistributions in binary form must reproduce the above copyright notice,
 *     this list of conditions and the following disclaimer in the documentation
 *     and/or other materials provided with the distribution.
 *
 *  3. Neither the name of the copyright holder nor the names of its
 *     contributors may be used to endorse or promote products derived from
 *     this software without specific prior written permission.
 *
 *  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 *  AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 *  IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 *  DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
 *  FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 *  DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
 *  SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
 *  CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
 *  OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
 *  OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
**/

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




    template <class Archive, class _Scalar, int _Rows, int _Cols, int _Options, int _MaxRows, int _MaxCols>
    inline
    typename std::enable_if<!cereal::traits::is_output_serializable<cereal::BinaryData<_Scalar>, Archive>::value, void>::type
    save(Archive& ar, const Eigen::Matrix<_Scalar, _Rows, _Cols, _Options, _MaxRows, _MaxCols>& m)
    {
        for (int i=0; i < _Cols; ++i)
            for (int j=0; j < _Rows; ++j)
                ar(cereal::make_nvp(fmt::format("m{0}{1}", j, i), m(j,i)));
    };

    template <class Archive, class _Scalar, int _Rows, int _Cols, int _Options, int _MaxRows, int _MaxCols>
    inline
    typename std::enable_if<cereal::traits::is_output_serializable<cereal::BinaryData<_Scalar>, Archive>::value, void>::type
    save(Archive& ar, const Eigen::Matrix<_Scalar, _Rows, _Cols, _Options, _MaxRows, _MaxCols>& matrix)
    {
        auto rows = static_cast<std::int32_t>(matrix.rows());
        auto cols = static_cast<std::int32_t>(matrix.cols());
        ar(CEREAL_NVP(rows));
        ar(CEREAL_NVP(cols));
        ar(binary_data(matrix.data(), rows * cols * sizeof(_Scalar)));
    };



    template <class Archive, class _Scalar, int _Rows, int _Cols, int _Options, int _MaxRows, int _MaxCols>
    inline
    typename std::enable_if<!cereal::traits::is_input_serializable<cereal::BinaryData<_Scalar>, Archive>::value, void>::type
    load(Archive& ar, Eigen::Matrix<_Scalar, _Rows, _Cols, _Options, _MaxRows, _MaxCols>& m)
    {
        for (int i=0; i < _Cols; ++i)
            for (int j=0; j < _Rows; ++j)
                ar(cereal::make_nvp(fmt::format("m{0}{1}", j, i), m(j,i)));
    };

    template <class Archive, class _Scalar, int _Rows, int _Cols, int _Options, int _MaxRows, int _MaxCols>
    inline
    typename std::enable_if<cereal::traits::is_input_serializable<cereal::BinaryData<_Scalar>, Archive>::value, void>::type
    load(Archive& ar, Eigen::Matrix<_Scalar, _Rows, _Cols, _Options, _MaxRows, _MaxCols>& matrix)
    {
        std::int32_t rows;
        std::int32_t cols;
        ar(CEREAL_NVP(rows));
        ar(CEREAL_NVP(cols));

        matrix.resize(rows, cols);

        ar(binary_data(matrix.data(), static_cast<std::size_t>(rows * cols * sizeof(_Scalar))));
    };

    template<class Archive, class Scalar_>
    inline
    void serialize(Archive & archive,
                   Eigen::Quaternion<Scalar_>& m)
    {
        archive( cereal::make_nvp("x", m.x()), cereal::make_nvp("y", m.y()),
                 cereal::make_nvp("z", m.z()), cereal::make_nvp("w", m.w()) );
    };




    //template<class Archive, class _Scalar, int _Dim, int _Mode, int _Options>
    template<class Archive>
    void save(Archive & archive,
                   const Eigen::Affine3d & m)
    {
        Eigen::Vector3d trans = m.translation();
        Eigen::Quaterniond rot = Eigen::Quaterniond(m.rotation());
        archive( cereal::make_nvp("translation", trans),
                 cereal::make_nvp("rotation", rot));
    };

    //template<class Archive, class _Scalar, int _Dim, int _Mode, int _Options>
    template<class Archive>
    void load(Archive & archive,
              Eigen::Affine3d & m)
    {
        Eigen::Vector3d trans;
        Eigen::Quaterniond rot;
        archive( cereal::make_nvp("translation", trans),
                 cereal::make_nvp("rotation", rot));

        m.setIdentity();
        m.translate(trans);
        m.rotate(rot);
    };


}



#endif //TRAACTMULTI_CEREALSPATIAL_H
