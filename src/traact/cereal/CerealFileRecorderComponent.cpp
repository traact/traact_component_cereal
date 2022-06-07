/**
 *   Copyright (C) 2022  Frieder Pankratz <frieder.pankratz@gmail.com>
 *
 *   License in root folder
**/


#include <traact/component/FileRecorder.h>
//#include <cereal/archives/binary.hpp>
#include <cereal/archives/json.hpp>
#include <fstream>
#include "CerealSpatial.h"
#include "CerealVision.h"

namespace traact::component {

template<class T>
class CerealFileRecorder : public FileRecorder<T> {
 public:
    CerealFileRecorder(const std::string &name) : FileRecorder<T>(name, "cereal") {}

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
            std::ofstream stream;
            stream.open(FileWriter<T>::filename_);
            {
                cereal::JSONOutputArchive archive(stream);
                archive(all_data_);
            }

            stream.close();
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
 RTTR_ENABLE(FileWriter<T>, Component)
};

}

// It is not possible to place the macro multiple times in one cpp file. When you compile your plugin with the gcc toolchain,
// make sure you use the compiler option: -fno-gnu-unique. otherwise the unregistration will not work properly.
RTTR_PLUGIN_REGISTRATION // remark the different registration macro!
{

    using namespace rttr;
    using namespace traact;
    registration::class_<component::CerealFileRecorder<spatial::Pose6DHeader> >("FileRecorder_cereal_spatial:Pose6D").constructor<
        std::string>()();
    registration::class_<component::CerealFileRecorder<spatial::Position2DListHeader> >(
        "FileRecorder_cereal_spatial:Position2DList").constructor<std::string>()();
    registration::class_<component::CerealFileRecorder<spatial::Position3DListHeader> >(
        "FileRecorder_cereal_spatial:Position3DList").constructor<std::string>()();
}