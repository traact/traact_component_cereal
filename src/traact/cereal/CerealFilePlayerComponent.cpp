/** Copyright (C) 2022  Frieder Pankratz <frieder.pankratz@gmail.com> **/


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
 TRAACT_PLUGIN_ENABLE(FilePlayer<T>, Component)
};

}

BEGIN_TRAACT_PLUGIN_REGISTRATION
    REGISTER_SPATIAL_COMPONENTS(traact::component::CerealFilePlayer)
    REGISTER_COMPONENT(traact::component::CerealFilePlayer<traact::vision::CameraCalibrationHeader>)
END_TRAACT_PLUGIN_REGISTRATION
