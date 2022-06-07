/**
 *   Copyright (C) 2022  Frieder Pankratz <frieder.pankratz@gmail.com>
 *
 *   License in root folder
**/

#include <traact/component/FileReader.h>
//#include <cereal/archives/binary.hpp>
#include <cereal/archives/json.hpp>
#include <fstream>
#include "CerealSpatial.h"
#include "CerealVision.h"

namespace traact::component {

template<class T>
class CerealFileReader : public FileReader<T> {
 public:
    CerealFileReader(const std::string &name) : FileReader<T>(name, "cereal") {}

    bool readValue(typename T::NativeType &data) override {
        try {
            std::ifstream stream;
            stream.open(FileReader<T>::filename_);
            cereal::JSONInputArchive archive(stream);
            archive(data);
        } catch (...) {
            SPDLOG_ERROR("unkown exception on readValue {0}", FileReader<T>::filename_);
        }

        return true;
    }

 private:

 RTTR_ENABLE(FileReader<T>, Component)
};

}

// It is not possible to place the macro multiple times in one cpp file. When you compile your plugin with the gcc toolchain,
// make sure you use the compiler option: -fno-gnu-unique. otherwise the unregistration will not work properly.
RTTR_PLUGIN_REGISTRATION // remark the different registration macro!
{

    using namespace rttr;
    using namespace traact;
    registration::class_<component::CerealFileReader<spatial::Pose6DHeader> >("FileReader_cereal_spatial:Pose6D").constructor<
        std::string>()();
    registration::class_<component::CerealFileReader<spatial::Position2DListHeader> >(
        "FileReader_cereal_spatial:Position2DList").constructor<std::string>()();
    registration::class_<component::CerealFileReader<spatial::Position3DListHeader> >(
        "FileReader_cereal_spatial:Position3DList").constructor<std::string>()();
    registration::class_<component::CerealFileReader<vision::CameraCalibrationHeader> >(
        "FileReader_cereal_vision:CameraCalibration").constructor<std::string>()();
}