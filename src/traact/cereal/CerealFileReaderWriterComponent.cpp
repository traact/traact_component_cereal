/** Copyright (C) 2022  Frieder Pankratz <frieder.pankratz@gmail.com> **/

#include <traact/component/generic/FileReaderWriter.h>
//#include <cereal/archives/binary.hpp>
#include <cereal/archives/json.hpp>
#include <fstream>
#include <cppfs/FilePath.h>
#include <cppfs/FileHandle.h>
#include <cppfs/fs.h>
#include "CerealSpatial.h"
#include "CerealVision.h"

namespace traact::component {

template<class T>
class CerealFileReaderWriterRead : public FileReaderWriterRead<T> {
 public:
    CerealFileReaderWriterRead(const std::string &name) : FileReaderWriterRead<T>(name, "cereal") {}

    [[nodiscard]] static traact::pattern::Pattern::Ptr GetPattern() {
        return FileReaderWriterRead<T>::GetBasePattern("cereal");
    }

    bool readValue(typename T::NativeType &data) override {
        try {
            std::ifstream stream;
            stream.open(FileReaderWriterRead<T>::filename_);
            cereal::JSONInputArchive archive(stream);
            archive(data);
        } catch (...) {
            SPDLOG_ERROR("unknown exception on readValue {0}", FileReaderWriterRead<T>::filename_);
        }

        return true;
    }

 private:


};

template<class T>
class CerealFileReaderWriterWrite : public FileReaderWriterWrite<T> {
 public:
    CerealFileReaderWriterWrite(const std::string &name) : FileReaderWriterWrite<T>(name, "cereal") {}

    [[nodiscard]] static traact::pattern::Pattern::Ptr GetPattern() {
        return FileReaderWriterWrite<T>::GetBasePattern("cereal");
    }

    bool saveValue(Timestamp ts, const typename T::NativeType &value) override {
        try {

            cppfs::FilePath path(FileReaderWriterWrite<T>::filename_);
            util::createFileDirectory(path.fullPath());
            cppfs::FileHandle file = cppfs::fs::open(FileReaderWriterWrite<T>::filename_);

            if(FileReaderWriterWrite<T>::value_module_->value_.has_value())
            {
                auto stream = file.createOutputStream();
                SPDLOG_TRACE("write cereal file: {0}", FileReaderWriterWrite<T>::filename_);
                cereal::JSONOutputArchive archive_(*stream);
                archive_(FileReaderWriterWrite<T>::value_module_->value_.value());
            } else {
                SPDLOG_TRACE("no data to write cereal file: {0}", FileReaderWriterWrite<T>::filename_);
            }
        } catch (...) {
            SPDLOG_ERROR("{0} : could not write file", Component::name_);
        }

        return true;
    }



};

CREATE_SPATIAL_COMPONENTS(CerealFileReaderWriterRead)
CREATE_VISION_COMPONENTS(CerealFileReaderWriterRead)
CREATE_SPATIAL_COMPONENTS(CerealFileReaderWriterWrite)
CREATE_VISION_COMPONENTS(CerealFileReaderWriterWrite)

}

BEGIN_TRAACT_PLUGIN_REGISTRATION
    REGISTER_SPATIAL_COMPONENTS(traact::component::CerealFileReaderWriterRead)
    REGISTER_VISION_COMPONENTS(traact::component::CerealFileReaderWriterRead)
    REGISTER_SPATIAL_COMPONENTS(traact::component::CerealFileReaderWriterWrite)
    REGISTER_VISION_COMPONENTS(traact::component::CerealFileReaderWriterWrite)
END_TRAACT_PLUGIN_REGISTRATION
