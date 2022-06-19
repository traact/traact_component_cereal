/** Copyright (C) 2022  Frieder Pankratz <frieder.pankratz@gmail.com> **/

#ifndef TRAACTMULTI_CEREALVISION_H
#define TRAACTMULTI_CEREALVISION_H

#include <cereal/types/memory.hpp>
#include <cereal/archives/json.hpp>
#include <cereal/archives/xml.hpp>
#include <cereal/types/vector.hpp>
#include <cereal/types/map.hpp>
#include <cereal/archives/binary.hpp>
#include <type_traits>
#include <fmt/format.h>

#include <traact/vision.h>

namespace cereal {

template<class Archive>
inline
void serialize(Archive &archive,
               traact::vision::CameraCalibration &m) {
    archive(cereal::make_nvp("fx", m.fx),
            cereal::make_nvp("fy", m.fy),
            cereal::make_nvp("cx", m.cx),
            cereal::make_nvp("cy", m.cy),
            cereal::make_nvp("skew", m.skew),
            cereal::make_nvp("width", m.width),
            cereal::make_nvp("height", m.height),
            cereal::make_nvp("radial_distortion", m.radial_distortion),
            cereal::make_nvp("tangential_distortion", m.tangential_distortion)
    );
}

template<class Archive>
inline
void serialize(Archive &archive,
               traact::vision::Position2D &m) {
    archive(cereal::make_nvp("m00", m.x),
            cereal::make_nvp("m10", m.y));
}
template<class Archive>
inline
void serialize(Archive &archive,
               traact::vision::Position3D &m) {
    archive(cereal::make_nvp("m00", m.x),
            cereal::make_nvp("m10", m.y),
            cereal::make_nvp("m20", m.z));
}

template<class Archive>
inline
void serialize(Archive &archive,
               traact::vision::Image &m) {

    archive(cereal::make_nvp("foo", 0));
}

template<class Archive>
inline
void serialize(Archive &archive,
               traact::vision::Feature &m) {
    archive(cereal::make_nvp("feature_id", m.feature_id), cereal::make_nvp("constructed_from", m.constructed_from));
}

template<class Archive>
inline
void serialize(Archive &archive,
               traact::vision::FeatureList &m) {
    archive(cereal::make_nvp("feature_id", m.feature_id), cereal::make_nvp("constructed_from", m.constructed_from));
}

template<class Archive>
inline
void serialize(Archive &archive,
               cv::KeyPoint &m) {

    archive(make_nvp("point",m.pt), make_nvp("size", m.size), make_nvp("angle", m.angle), make_nvp("octave", m.octave),
            make_nvp("class_id",m.class_id), make_nvp("response",m.response));
}

}

#endif //TRAACTMULTI_CEREALVISION_H
