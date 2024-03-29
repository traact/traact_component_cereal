/** Copyright (C) 2022  Frieder Pankratz <frieder.pankratz@gmail.com> **/

#include <traact/component/generic/FileWriter.h>
//#include <cereal/archives/binary.hpp>
#include <cereal/archives/json.hpp>
#include <fstream>
#include "traact/cereal/CerealSpatial.h"
#include "traact/cereal/CerealVision.h"
#include <cppfs/fs.h>
#include <cppfs/FileHandle.h>
#include <cppfs/FilePath.h>
#include <traact/util/FileUtil.h>
#include <traact/spatial_convert.h>
namespace traact::component {

template<class T>
class CerealFileWriter : public FileWriter<T> {
 public:
    CerealFileWriter(const std::string &name) : FileWriter<T>(name, "cereal") {}

    [[nodiscard]] static traact::pattern::Pattern::Ptr GetPattern() {
        auto pattern = FileWriter<T>::GetBasePattern("cereal");
        pattern->addParameter("CoordinateSystem", "Traact", spatial::CoordinateSystemNames);
        return pattern;
    }

    bool configure(const pattern::instance::PatternInstance &pattern_instance,
                   buffer::ComponentBufferConfig *data) override {
        pattern::setValueFromParameter(pattern_instance,
                                       "CoordinateSystem",
                                       target_system_,
                                       "Traact",spatial::CoordinateSystemValues);
        return FileWriter<T>::configure(pattern_instance, data);
    }

    bool openFile() override {
        return true;
    }

    bool closeFile() override {
        try {

            //stream_.open(FileWriter<T>::filename_);
            cppfs::FilePath path(FileWriter<T>::filename_);
            util::createFileDirectory(path.fullPath());
            cppfs::FileHandle file = cppfs::fs::open(FileWriter<T>::filename_);

            if(value_.has_value())
            {
                auto stream = file.createOutputStream();
                SPDLOG_TRACE("write cereal file: {0}", FileWriter<T>::filename_);
                cereal::JSONOutputArchive archive_(*stream);
                auto data = spatial::Convert<T>::toTraact(value_.value(), target_system_);
                archive_(data);
            } else {
                SPDLOG_TRACE("no data to write cereal file: {0}", FileWriter<T>::filename_);
            }

            //stream_.flush();
            //stream_.close();

        } catch (...) {
            SPDLOG_ERROR("{0} : could not write file", Component::name_);
        }

        return true;
    }

    bool saveValue(Timestamp ts, const typename T::NativeType &value) override {
        value_ = value;
        return true;
    }

 private:
    //std::ofstream stream_;
    std::optional<typename T::NativeType> value_;
    spatial::CoordinateSystems target_system_;


};
CREATE_SPATIAL_COMPONENTS(CerealFileWriter)
CREATE_VISION_COMPONENTS(CerealFileWriter)

}

BEGIN_TRAACT_PLUGIN_REGISTRATION
    REGISTER_SPATIAL_COMPONENTS(traact::component::CerealFileWriter)
    REGISTER_VISION_COMPONENTS(traact::component::CerealFileWriter)
END_TRAACT_PLUGIN_REGISTRATION


