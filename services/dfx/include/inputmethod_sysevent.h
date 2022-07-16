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

#ifndef INPUTMETHOD_SYSEVENT_H
#define INPUTMETHOD_SYSEVENT_H

#include <string>
#include "global.h"

namespace OHOS {
namespace MiscServices {
void FaultReporter(int32_t userId, std::string bundname, int32_t errCode);
void CreateComponentFailed(int32_t userId, int32_t errCode);
void BehaviourReporter(std::string ActiveName, const std::string &inputmethodName);
} // namespace MiscServices
} // namespace OHOS
#endif // INPUTMETHOD_SYSEVENT_H
