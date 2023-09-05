/** Copyright (C) 2022  Frieder Pankratz <frieder.pankratz@gmail.com> **/


#include <traact/component/generic/FileRecorder.h>
//#include <cereal/archives/binary.hpp>
#include <cereal/archives/json.hpp>
#include <fstream>
#include "traact/cereal/CerealSpatial.h"
#include "traact/cereal/CerealVision.h"
#include <cppfs/fs.h>
#include <cppfs/FileHandle.h>
#include <cppfs/FilePath.h>
#include <traact/spatial_convert.h>
namespace traact::component {

template<class T>
class CerealFileRecorder : public FileRecorder<T> {
 public:
    CerealFileRecorder(const std::string &name) : FileRecorder<T>(name, "cereal") {}

    [[nodiscard]] static traact::pattern::Pattern::Ptr GetPattern() {
        auto pattern = FileRecorder<T>::GetBasePattern("cereal");
        pattern->addParameter("CoordinateSystem", "Traact", spatial::CoordinateSystemNames);
        return pattern;
    }

    bool configure(const pattern::instance::PatternInstance &pattern_instance,
                   buffer::ComponentBufferConfig *data) override {
        pattern::setValueFromParameter(pattern_instance,
                                       "CoordinateSystem",
                                       target_system_,
                                       "Traact",spatial::CoordinateSystemValues);
        return FileRecorder<T>::configure(pattern_instance, data);
    }

    bool openFile() override {
        try {
            //stream_.open(FileWriter<T>::filename_);
            //archive_ = std::make_shared<cereal::JSONOutputArchive>(stream_);
        } catch (...) {
            SPDLOG_ERROR("{0}: Unspecified error when opening file", FileWriter<T>::name_);
            return false;
        }

        return true;
    }

    bool closeFile() override {
        try {
//            std::ofstream stream;
//            stream.open(FileRecorder<T>::filename_);
            cppfs::FilePath path(FileWriter<T>::filename_);
            util::createFileDirectory(path.fullPath());
            cppfs::FileHandle file = cppfs::fs::open(FileWriter<T>::filename_);
            {
                auto stream = file.createOutputStream();
                cereal::JSONOutputArchive archive(*stream);
                archive(all_data_);
            }

            //stream.close();
        } catch (...) {
            SPDLOG_ERROR("{0}: Unspecified error when closing file", FileWriter<T>::name_);
            return false;
        }

        return true;
    }

    bool saveValue(Timestamp ts, const typename T::NativeType &value) override {

        std::uint64_t ns = ts.time_since_epoch().count();
        auto data = spatial::Convert<T>::toTraact(value, target_system_);
        all_data_.emplace(std::make_pair(ns, data));

        return true;
    }

 private:
    //std::ofstream stream_;
    //std::shared_ptr<cereal::JSONOutputArchive> archive_;
    std::map<std::uint64_t, typename T::NativeType> all_data_;
    spatial::CoordinateSystems target_system_;

};

CREATE_SPATIAL_COMPONENTS(CerealFileRecorder)
CREATE_VISION_COMPONENTS(CerealFileRecorder)


}

BEGIN_TRAACT_PLUGIN_REGISTRATION
    REGISTER_SPATIAL_COMPONENTS(traact::component::CerealFileRecorder)
    REGISTER_VISION_COMPONENTS(traact::component::CerealFileRecorder)
END_TRAACT_PLUGIN_REGISTRATION
