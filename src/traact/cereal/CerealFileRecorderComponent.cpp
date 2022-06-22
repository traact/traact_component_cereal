/** Copyright (C) 2022  Frieder Pankratz <frieder.pankratz@gmail.com> **/


#include <traact/component/generic/FileRecorder.h>
//#include <cereal/archives/binary.hpp>
#include <cereal/archives/json.hpp>
#include <fstream>
#include "CerealSpatial.h"
#include "CerealVision.h"
#include <cppfs/fs.h>
#include <cppfs/FileHandle.h>
#include <cppfs/FilePath.h>
namespace traact::component {

template<class T>
class CerealFileRecorder : public FileRecorder<T> {
 public:
    CerealFileRecorder(const std::string &name) : FileRecorder<T>(name, "cereal") {}

    [[nodiscard]] static traact::pattern::Pattern::Ptr GetPattern() {
        return FileRecorder<T>::GetBasePattern("cereal");
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
        all_data_.emplace(std::make_pair(ns, value));

        return true;
    }

 private:
    //std::ofstream stream_;
    //std::shared_ptr<cereal::JSONOutputArchive> archive_;
    std::map<std::uint64_t, typename T::NativeType> all_data_;

};

CREATE_SPATIAL_COMPONENTS(CerealFileRecorder)
CREATE_VISION_COMPONENTS(CerealFileRecorder)


}

BEGIN_TRAACT_PLUGIN_REGISTRATION
    REGISTER_SPATIAL_COMPONENTS(traact::component::CerealFileRecorder)
    REGISTER_VISION_COMPONENTS(traact::component::CerealFileRecorder)
END_TRAACT_PLUGIN_REGISTRATION
