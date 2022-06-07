/**
 *   Copyright (C) 2022  Frieder Pankratz <frieder.pankratz@gmail.com>
 *
 *   License in root folder
**/

#include <traact/component/FileWriter.h>
//#include <cereal/archives/binary.hpp>
#include <cereal/archives/json.hpp>
#include <fstream>
#include "CerealSpatial.h"
#include "CerealVision.h"

namespace traact::component {

template<class T>
class CerealFileWriter : public FileWriter<T> {
 public:
    CerealFileWriter(const std::string &name) : FileWriter<T>(name, "cereal") {}

    bool openFile() override {
//            try{
//                stream_.open(FileWriter<T>::filename_);
//                //archive_ = std::make_shared<cereal::JSONOutputArchive>(stream_);
//            }catch (...){
//                SPDLOG_ERROR("Unspecified error when opening file for component {0}", FileWriter<T>::name_);
//                return false;
//            }

        return true;
    }

    bool closeFile() override {
        try {
            //stream_.seekp(0);
            stream_.open(FileWriter<T>::filename_);

            {
                cereal::JSONOutputArchive archive_(stream_);
                archive_(value_);
                //archive_( ts.time_since_epoch().count());
            }

            stream_.flush();
            stream_.close();

        } catch (...) {
            SPDLOG_ERROR("{0} : could not write file", Component::name_);
        }

        return true;
    }

    bool saveValue(Timestamp ts, const typename T::NativeType &value) override {

//            try{
//                //stream_.seekp(0);
//                stream_.open(FileWriter<T>::filename_);
//
//                {
//                    cereal::JSONOutputArchive archive_(stream_);
//                    archive_(value);
//                    //archive_( ts.time_since_epoch().count());
//                }
//
//
//                stream_.flush();
//                stream_.close();
//
//            } catch ( ... ) {
//                SPDLOG_ERROR("{0} : could not write file", Component::name_);
//            }
        value_ = value;

        return true;
    }

 private:
    std::ofstream stream_;
    typename T::NativeType value_;
    //std::shared_ptr<cereal::JSONOutputArchive> archive_;
 RTTR_ENABLE(FileWriter<T>, Component)
};

}

// It is not possible to place the macro multiple times in one cpp file. When you compile your plugin with the gcc toolchain,
// make sure you use the compiler option: -fno-gnu-unique. otherwise the unregistration will not work properly.
RTTR_PLUGIN_REGISTRATION // remark the different registration macro!
{

    using namespace rttr;
    using namespace traact;
    registration::class_<component::CerealFileWriter<spatial::Pose6DHeader> >("FileWriter_cereal_spatial:Pose6D").constructor<
        std::string>()();
    registration::class_<component::CerealFileWriter<spatial::Position2DListHeader> >(
        "FileWriter_cereal_spatial:Position2DList").constructor<std::string>()();
    registration::class_<component::CerealFileWriter<spatial::Position3DListHeader> >(
        "FileWriter_cereal_spatial:Position3DList").constructor<std::string>()();
    registration::class_<component::CerealFileWriter<vision::CameraCalibrationHeader> >(
        "FileWriter_cereal_vision:CameraCalibration").constructor<std::string>()();
}