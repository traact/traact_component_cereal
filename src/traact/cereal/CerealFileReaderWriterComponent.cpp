/** Copyright (C) 2022  Frieder Pankratz <frieder.pankratz@gmail.com> **/

#include <traact/component/generic/FileReaderWriter.h>
//#include <cereal/archives/binary.hpp>
#include <cereal/archives/json.hpp>
#include <fstream>
#include <cppfs/FilePath.h>
#include <cppfs/FileHandle.h>
#include <cppfs/fs.h>
#include "traact/cereal/CerealSpatial.h"
#include "traact/cereal/CerealVision.h"
#include <traact/spatial_convert.h>

namespace traact::component {

template<class T>
class CerealFileReaderWriterRead : public FileReaderWriterRead<T> {
 public:
    CerealFileReaderWriterRead(const std::string &name) : FileReaderWriterRead<T>(name, "cereal") {}

    [[nodiscard]] static traact::pattern::Pattern::Ptr GetPattern() {
        auto pattern = FileReaderWriterRead<T>::GetBasePattern("cereal");
        pattern->addParameter("CoordinateSystem", "Traact", spatial::CoordinateSystemNames);
        return pattern;
    }
    virtual bool configure(const pattern::instance::PatternInstance &pattern_instance,
                           buffer::ComponentBufferConfig *data) override {
        pattern::setValueFromParameter(pattern_instance,
                                       "CoordinateSystem",
                                       target_system_,
                                       "Traact",spatial::CoordinateSystemValues);
        return FileReaderWriterRead<T>::configure(pattern_instance, data);
    }

    bool readValue(typename T::NativeType &data) override {
        try {
            std::ifstream stream;
            stream.open(FileReaderWriterRead<T>::filename_);
            cereal::JSONInputArchive archive(stream);
            archive(data);
            data = spatial::Convert<T>::toTraact(data, target_system_);
        } catch (...) {
            SPDLOG_ERROR("unknown exception on readValue {0}", FileReaderWriterRead<T>::filename_);
        }

        return true;
    }

 private:
    spatial::CoordinateSystems target_system_;

};

template<class T>
class CerealFileReaderWriterWrite : public FileReaderWriterWrite<T> {
 public:
    CerealFileReaderWriterWrite(const std::string &name) : FileReaderWriterWrite<T>(name, "cereal") {}

    [[nodiscard]] static traact::pattern::Pattern::Ptr GetPattern() {
        auto pattern = FileReaderWriterWrite<T>::GetBasePattern("cereal");
        pattern->addParameter("CoordinateSystem", "Traact", spatial::CoordinateSystemNames);
        return pattern;
    }

    virtual bool configure(const pattern::instance::PatternInstance &pattern_instance,
                           buffer::ComponentBufferConfig *data) override {
        pattern::setValueFromParameter(pattern_instance,
                                       "CoordinateSystem",
                                       target_system_,
                                       "Traact",spatial::CoordinateSystemValues);
        return FileReaderWriterModuleComponent<T>::configure(pattern_instance, data);
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
                typename T::NativeType result = spatial::Convert<T>::fromTraact(FileReaderWriterWrite<T>::value_module_->value_.value(), target_system_);
                archive_(result);
            } else {
                SPDLOG_TRACE("no data to write cereal file: {0}", FileReaderWriterWrite<T>::filename_);
            }
        } catch (...) {
            SPDLOG_ERROR("{0} : could not write file", Component::name_);
        }

        return true;
    }
 private:
    spatial::CoordinateSystems target_system_;

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
