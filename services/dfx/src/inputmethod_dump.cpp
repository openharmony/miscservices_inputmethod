/*
 * Copyright (c) 2022-2022 Huawei Device Co., Ltd.
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#include "inputmethod_dump.h"

#include <cstdint>

#include "functional"
#include "global.h"
#include "list"
#include "string"
#include "vector"

namespace OHOS {
namespace MiscServices {
constexpr int32_t MAX_RECORED_ERROR = 10;
constexpr int32_t SUB_CMD_NAME = 0;
constexpr int32_t CMD_ONE_PARAM = 1;
constexpr const char *CMD_HELP = "-h";
constexpr const char *CMD_ALL_DUMP = "-a";
static const std::string illegalInfo = "input dump parameter error,enter '-h' for usage.\n";

void InputmethodDump::AddDumpAllMethod(const DumpNoParamFunc dumpAllMethod)
{
    if (dumpAllMethod == nullptr) {
        return;
    }
    dumpAllMethod_ = dumpAllMethod;
}

void InputmethodDump::AddErrorInfo(const std::string &error)
{
    std::lock_guard<std::mutex> lock(hidumperMutex_);
    if (errorInfo_.size() + 1 > MAX_RECORED_ERROR) {
        errorInfo_.pop_front();
        errorInfo_.push_back(error);
    } else {
        errorInfo_.push_back(error);
    }
}

bool InputmethodDump::Dump(int fd, const std::vector<std::string> &args)
{
    IMSA_HILOGI("InputmethodDump::Dump start.");
    std::string command = "";
    if (args.size() == CMD_ONE_PARAM) {
        command = args.at(SUB_CMD_NAME);
    } else {
        ShowIllealInfomation(fd);
    }
    if (command == CMD_HELP) {
        ShowHelp(fd);
    } else if (command == CMD_ALL_DUMP) {
        if (!dumpAllMethod_) {
            return false;
        }
        dumpAllMethod_(fd);
    } else {
        ShowIllealInfomation(fd);
    }
    IMSA_HILOGI("InputmethodDump::Dump command=%{public}s.", command.c_str());
    return true;
}

void InputmethodDump::ShowHelp(int fd)
{
    std::string result;
    result.append("Usage:dump  <command> [options]\n")
        .append("Description:\n")
        .append("-h show help\n")
        .append("-a dump all input methods\n");
    dprintf(fd, "%s\n", result.c_str());
}

void InputmethodDump::ShowIllealInfomation(int fd)
{
    dprintf(fd, "%s\n", illegalInfo.c_str());
}
} // namespace MiscServices
} // namespace OHOS