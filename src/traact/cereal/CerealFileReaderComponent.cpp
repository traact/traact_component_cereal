/** Copyright (C) 2022  Frieder Pankratz <frieder.pankratz@gmail.com> **/

#include <traact/component/generic/FileReader.h>
//#include <cereal/archives/binary.hpp>
#include <cereal/archives/json.hpp>
#include <fstream>
#include "traact/cereal/CerealSpatial.h"
#include "traact/cereal/CerealVision.h"
#include <traact/spatial_convert.h>
namespace traact::component {

template<class T>
class CerealFileReader : public FileReader<T> {
 public:
    CerealFileReader(const std::string &name) : FileReader<T>(name, "cereal") {}

    [[nodiscard]] static traact::pattern::Pattern::Ptr GetPattern() {
        auto pattern = FileReader<T>::GetBasePattern("cereal");
        pattern->addParameter("CoordinateSystem", "Traact", spatial::CoordinateSystemNames);
        return pattern;
    }

    bool configure(const pattern::instance::PatternInstance &pattern_instance,
                   buffer::ComponentBufferConfig *data) override {
        pattern::setValueFromParameter(pattern_instance,
                                       "CoordinateSystem",
                                       target_system_,
                                       "Traact",spatial::CoordinateSystemValues);
        return FileReader<T>::configure(pattern_instance, data);
    }

    bool readValue(typename T::NativeType &data) override {
        try {
            std::ifstream stream;
            stream.open(FileReader<T>::filename_);
            cereal::JSONInputArchive archive(stream);
            archive(data);
            data = spatial::Convert<T>::toTraact(data, target_system_);
        } catch (...) {
            SPDLOG_ERROR("unknown exception on readValue {0}", FileReader<T>::filename_);
        }

        return true;
    }

 private:
    spatial::CoordinateSystems target_system_;


};

CREATE_SPATIAL_COMPONENTS(CerealFileReader)
CREATE_VISION_COMPONENTS(CerealFileReader)

}

BEGIN_TRAACT_PLUGIN_REGISTRATION
    REGISTER_SPATIAL_COMPONENTS(traact::component::CerealFileReader)
    REGISTER_VISION_COMPONENTS(traact::component::CerealFileReader)
END_TRAACT_PLUGIN_REGISTRATION
