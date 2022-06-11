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
        value_ = value;
        return true;
    }

 private:
    std::ofstream stream_;
    typename T::NativeType value_;


};
CREATE_SPATIAL_COMPONENTS(CerealFileWriter)
CREATE_TEMPLATED_TRAACT_COMPONENT_FACTORY(CerealFileWriter, traact::vision, CameraCalibrationHeader)

}

BEGIN_TRAACT_PLUGIN_REGISTRATION
    REGISTER_SPATIAL_COMPONENTS(traact::component::CerealFileWriter)
    REGISTER_TEMPLATED_DEFAULT_COMPONENT(traact::component::CerealFileWriter, CameraCalibrationHeader)
END_TRAACT_PLUGIN_REGISTRATION


