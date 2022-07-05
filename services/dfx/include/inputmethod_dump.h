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

#ifndef INPUTMETHOD_DUMP_H
#define INPUTMETHOD_DUMP_H

#include <stdlib.h>
#include <functional>
#include <list>
#include <string>
#include <vector>
#include "singleton.h"

namespace OHOS {
namespace MiscServices {
class InputmethodDump : public Singleton<InputmethodDump> {
public:
    using DumpNoParamFunc = std::function<void(int)>;
    InputmethodDump() = default;
    virtual ~InputmethodDump() = default;
    void AddDumpAllMethod(const DumpNoParamFunc dumpAllMethod);
    void AddErrorInfo(const std::string &error);
    bool Dump(int fd, const std::vector<std::string> &args);

private:
    void ShowHelp(int fd);
    void ShowIllealInfomation(int fd);
    mutable std::mutex hidumperMutex_;
    std::list<std::string> errorInfo_;
    DumpNoParamFunc dumpAllMethod_;
};
} // namespace MiscServices
} // namespace OHOS
#endif // INPUTMETHOD_DUMP_H