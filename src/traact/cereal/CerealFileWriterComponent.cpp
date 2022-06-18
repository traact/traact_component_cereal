/** Copyright (C) 2022  Frieder Pankratz <frieder.pankratz@gmail.com> **/

#include <traact/component/generic/FileWriter.h>
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

    [[nodiscard]] static traact::pattern::Pattern::Ptr GetPattern() {
        return FileWriter<T>::GetBasePattern("cereal");
    }

    bool openFile() override {
        return true;
    }

    bool closeFile() override {
        try {

            stream_.open(FileWriter<T>::filename_);

            if(value_.has_value())
            {
                SPDLOG_TRACE("write cereal file: {0}", FileWriter<T>::filename_);
                cereal::JSONOutputArchive archive_(stream_);
                archive_(value_.value());
            } else {
                SPDLOG_TRACE("no data to write cereal file: {0}", FileWriter<T>::filename_);
            }

            stream_.flush();
            stream_.close();

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
    std::ofstream stream_;
    std::optional<typename T::NativeType> value_;


};
CREATE_SPATIAL_COMPONENTS(CerealFileWriter)
CREATE_VISION_COMPONENTS(CerealFileWriter)

}

BEGIN_TRAACT_PLUGIN_REGISTRATION
    REGISTER_SPATIAL_COMPONENTS(traact::component::CerealFileWriter)
    REGISTER_VISION_COMPONENTS(traact::component::CerealFileWriter)
END_TRAACT_PLUGIN_REGISTRATION


