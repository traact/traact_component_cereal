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

#include <traact/vision_datatypes.h>

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

}

#endif //TRAACTMULTI_CEREALVISION_H
