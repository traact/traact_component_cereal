/*  BSD 3-Clause License
 *
 *  Copyright (c) 2020, FriederPankratz <frieder.pankratz@gmail.com>
 *  All rights reserved.
 *
 *  Redistribution and use in source and binary forms, with or without
 *  modification, are permitted provided that the following conditions are met:
 *
 *  1. Redistributions of source code must retain the above copyright notice, this
 *     list of conditions and the following disclaimer.
 *
 *  2. Redistributions in binary form must reproduce the above copyright notice,
 *     this list of conditions and the following disclaimer in the documentation
 *     and/or other materials provided with the distribution.
 *
 *  3. Neither the name of the copyright holder nor the names of its
 *     contributors may be used to endorse or promote products derived from
 *     this software without specific prior written permission.
 *
 *  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 *  AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 *  IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 *  DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
 *  FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 *  DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
 *  SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
 *  CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
 *  OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
 *  OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
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

        bool OpenFile() override {
//            try{
//                stream_.open(FileWriter<T>::filename_);
//                //archive_ = std::make_shared<cereal::JSONOutputArchive>(stream_);
//            }catch (...){
//                spdlog::error("Unspecified error when opening file for component {0}", FileWriter<T>::name_);
//                return false;
//            }

            return true;
        }

        bool CloseFile() override {
//            try{
//                //archive_.reset();
//                stream_.close();
//            } catch (...){
//              spdlog::error("Unspecified error when closing file for component {0}", FileWriter<T>::name_);
//                return false;
//            }

            return true;
        }

        bool saveValue(TimestampType ts, const typename T::NativeType &value) override {

            try{
                //stream_.seekp(0);
                stream_.open(FileWriter<T>::filename_);

                {
                    cereal::JSONOutputArchive archive_(stream_);
                    archive_(value);
                    //archive_( ts.time_since_epoch().count());
                }


                stream_.flush();
                stream_.close();

            } catch ( ... ) {
                spdlog::error("{0} : could not write file", Component::name_);
            }


            return true;
        }

    private:
        std::ofstream stream_;
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
    registration::class_<component::CerealFileWriter<spatial::Pose6DHeader> >("FileWriter_cereal_spatial:Pose6D").constructor<std::string>()();
    registration::class_<component::CerealFileWriter<spatial::Position2DListHeader> >("FileWriter_cereal_spatial:Position2DList").constructor<std::string>()();
    registration::class_<component::CerealFileWriter<spatial::Position3DListHeader> >("FileWriter_cereal_spatial:Position3DList").constructor<std::string>()();
    registration::class_<component::CerealFileWriter<vision::CameraCalibrationHeader> >("FileWriter_cereal_vision:CameraCalibration").constructor<std::string>()();
}