/** Copyright (C) 2022  Frieder Pankratz <frieder.pankratz@gmail.com> **/


#include <traact/component/generic/FilePlayer.h>
//#include <cereal/archives/binary.hpp>
#include <cereal/archives/json.hpp>
#include <fstream>
#include "CerealSpatial.h"
#include "CerealVision.h"
#include <traact/spatial_convert.h>

namespace traact::component {

template<class T>
class CerealFilePlayer : public FilePlayer<T> {
 public:
    CerealFilePlayer(const std::string &name) : FilePlayer<T>(name, "cereal") {}

    [[nodiscard]] static traact::pattern::Pattern::Ptr GetPattern() {
        auto pattern = FilePlayer<T>::GetBasePattern("cereal");
        pattern->addParameter("CoordinateSystem", "Traact", spatial::CoordinateSystemNames);
        return pattern;
    }

    bool configure(const pattern::instance::PatternInstance &pattern_instance,
                           buffer::ComponentBufferConfig *data) override {
        pattern::setValueFromParameter(pattern_instance,
                                       "CoordinateSystem",
                                       target_system_,
                                       "Traact",spatial::CoordinateSystemValues);
        return FilePlayer<T>::configure(pattern_instance, data);
    }

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

        ts = kTimestampZero + TimeDuration(ns);

        return true;
    }

    bool readValue(typename T::NativeType &data) override {
        (*archive_)(data);
        data = spatial::Convert<T>::toTraact(data, target_system_);
        return true;
    }

 private:
    std::ifstream stream_;
    std::shared_ptr<cereal::JSONInputArchive> archive_;
    spatial::CoordinateSystems target_system_;

};

CREATE_SPATIAL_COMPONENTS(CerealFilePlayer)
CREATE_VISION_COMPONENTS(CerealFilePlayer)

}




BEGIN_TRAACT_PLUGIN_REGISTRATION
    REGISTER_SPATIAL_COMPONENTS(traact::component::CerealFilePlayer)
    REGISTER_VISION_COMPONENTS(traact::component::CerealFilePlayer)
END_TRAACT_PLUGIN_REGISTRATION
