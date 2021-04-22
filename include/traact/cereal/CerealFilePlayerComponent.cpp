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


#include <traact/component/FilePlayer.h>
//#include <cereal/archives/binary.hpp>
#include <cereal/archives/json.hpp>
#include <fstream>
#include "CerealSpatial.h"
#include "CerealVision.h"

namespace traact::component {

    template<class T>
    class CerealFilePlayer : public FilePlayer<T> {
    public:
        CerealFilePlayer(const std::string &name) : FilePlayer<T>(name, "cereal") {}

        bool OpenFile() override {
            try{
                stream_.open(FilePlayer<T>::filename_);
                archive_ = std::make_shared<cereal::JSONInputArchive>(stream_);
            }catch (...){
                spdlog::error("{0}: Unspecified error when opening file", FilePlayer<T>::name_);
                return false;
            }

            return true;
        }

        bool CloseFile() override {
            try{
                archive_.reset();
                stream_.close();
            } catch (...){
                spdlog::error("{0}: Unspecified error when closing file", FilePlayer<T>::name_);
                return false;
            }

            return true;
        }

        bool ReadTimestamp(TimestampType &ts) override {
            std::uint64_t ns;
            (*archive_)(ns);

            ts = TimestampType::min() + TimeDurationType(ns);

            return true;
        }

        bool ReadValue(typename T::NativeType &data) override {
            (*archive_)(data);
            return true;
        }


    private:
        std::ifstream stream_;
        std::shared_ptr<cereal::JSONInputArchive> archive_;
    RTTR_ENABLE(FilePlayer<T>, Component)
    };


}

// It is not possible to place the macro multiple times in one cpp file. When you compile your plugin with the gcc toolchain,
// make sure you use the compiler option: -fno-gnu-unique. otherwise the unregistration will not work properly.
RTTR_PLUGIN_REGISTRATION // remark the different registration macro!
{

    using namespace rttr;
    using namespace traact;
    registration::class_<component::CerealFilePlayer<spatial::Pose6DHeader> >("FilePlayer_cereal_spatial:Pose6D").constructor<std::string>()();
    registration::class_<component::CerealFilePlayer<spatial::Position2DListHeader> >("FilePlayer_cereal_spatial:Position2DList").constructor<std::string>()();
    registration::class_<component::CerealFilePlayer<spatial::Position3DListHeader> >("FilePlayer_cereal_spatial:Position3DList").constructor<std::string>()();
}