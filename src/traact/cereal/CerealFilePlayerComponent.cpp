/**
 *   Copyright (C) 2022  Frieder Pankratz <frieder.pankratz@gmail.com>
 *
 *   License in root folder
**/


#include <traact/component/FilePlayer.h>
//#include <cereal/archives/binary.hpp>
#include <cereal/archives/json.hpp>
#include <fstream>
#include "CerealSpatial.h"
#include "CerealVision.h"

namespace traact::component {

template<class T>
class CerealFilePlayer : public FilePlayer<T> {
 public:
    CerealFilePlayer(const std::string &name) : FilePlayer<T>(name, "cereal") {}

    bool openFile() override {
        try {
            stream_.open(FilePlayer<T>::filename_);
            archive_ = std::make_shared<cereal::JSONInputArchive>(stream_);
        } catch (...) {
            SPDLOG_ERROR("{0}: Unspecified error when opening file", FilePlayer<T>::name_);
            return false;
        }

        return true;
    }

    bool closeFile() override {
        try {
            archive_.reset();
            stream_.close();
        } catch (...) {
            SPDLOG_ERROR("{0}: Unspecified error when closing file", FilePlayer<T>::name_);
            return false;
        }

        return true;
    }

    bool readTimestamp(Timestamp &ts) override {
        std::uint64_t ns;
        (*archive_)(ns);

        ts = Timestamp::min() + TimeDuration(ns);

        return true;
    }

    bool readValue(typename T::NativeType &data) override {
        (*archive_)(data);
        return true;
    }

 private:
    std::ifstream stream_;
    std::shared_ptr<cereal::JSONInputArchive> archive_;
 RTTR_ENABLE(FilePlayer<T>, Component)
};

}

// It is not possible to place the macro multiple times in one cpp file. When you compile your plugin with the gcc toolchain,
// make sure you use the compiler option: -fno-gnu-unique. otherwise the unregistration will not work properly.
RTTR_PLUGIN_REGISTRATION // remark the different registration macro!
{

    using namespace rttr;
    using namespace traact;
    registration::class_<component::CerealFilePlayer<spatial::Pose6DHeader> >("FilePlayer_cereal_spatial:Pose6D").constructor<
        std::string>()();
    registration::class_<component::CerealFilePlayer<spatial::Position2DListHeader> >(
        "FilePlayer_cereal_spatial:Position2DList").constructor<std::string>()();
    registration::class_<component::CerealFilePlayer<spatial::Position3DListHeader> >(
        "FilePlayer_cereal_spatial:Position3DList").constructor<std::string>()();
}