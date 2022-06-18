/** Copyright (C) 2022  Frieder Pankratz <frieder.pankratz@gmail.com> **/

#include <traact/component/generic/FileReader.h>
//#include <cereal/archives/binary.hpp>
#include <cereal/archives/json.hpp>
#include <fstream>
#include "CerealSpatial.h"
#include "CerealVision.h"

namespace traact::component {

template<class T>
class CerealFileReader : public FileReader<T> {
 public:
    CerealFileReader(const std::string &name) : FileReader<T>(name, "cereal") {}

    [[nodiscard]] static traact::pattern::Pattern::Ptr GetPattern() {
        return FileReader<T>::GetBasePattern("cereal");
    }

    bool readValue(typename T::NativeType &data) override {
        try {
            std::ifstream stream;
            stream.open(FileReader<T>::filename_);
            cereal::JSONInputArchive archive(stream);
            archive(data);
        } catch (...) {
            SPDLOG_ERROR("unknown exception on readValue {0}", FileReader<T>::filename_);
        }

        return true;
    }

 private:


};

CREATE_SPATIAL_COMPONENTS(CerealFileReader)
CREATE_VISION_COMPONENTS(CerealFileReader)

}

BEGIN_TRAACT_PLUGIN_REGISTRATION
    REGISTER_SPATIAL_COMPONENTS(traact::component::CerealFileReader)
    REGISTER_VISION_COMPONENTS(traact::component::CerealFileReader)
END_TRAACT_PLUGIN_REGISTRATION
