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

#include "inputmethod_sysevent.h"

#include "hisysevent.h"


namespace OHOS {
namespace MiscServices {
namespace {
using HiSysEventNameSpace = OHOS::HiviewDFX::HiSysEvent;
const std::string DOMAIN_STR = std::string(HiSysEventNameSpace::Domain::INPUTMETHOD);
} // namespace

void FaultReporter(int32_t userId, std::string bundname, int32_t errCode)
{
    int ret = HiSysEventNameSpace::Write(DOMAIN_STR, "SERVICE_INIT_FAILED", HiSysEventNameSpace::EventType::FAULT,
        "USER_ID", userId, "COMPONENT_ID", bundname, "ERROR_CODE", errCode);
    if (ret != 0) {
        IMSA_HILOGE("hisysevent FaultReporter failed! ret %{public}d,errCode %{public}d", ret, errCode);
    }
}

void CreateComponentFailed(int32_t userId, int32_t errCode)
{
    int ret = HiSysEventNameSpace::Write(DOMAIN_STR, "CREATE_COMPONENT_FAILED", HiSysEventNameSpace::EventType::FAULT,
        "USER_ID", userId, "ERROR_CODE", errCode);
    if (ret != 0) {
        IMSA_HILOGE("hisysevent CreateComponentFailed failed! ret %{public}d,errCode %{public}d", ret, errCode);
    }
}

void BehaviourReporter(std::string ActiveName, const std::string &inputmethodName)
{
    int ret = HiSysEventNameSpace::Write(DOMAIN_STR, "INPUTMETHOD_USING", HiSysEventNameSpace::EventType::BEHAVIOR,
        "ACTIVE_NAME", ActiveName, "INPUTMETHOD_NAME", inputmethodName);
    if (ret != 0) {
        IMSA_HILOGE("hisysevent BehaviourReporter failed! ret %{public}d", ret);
    }
}
} // namespace MiscServices
} // namespace OHOS