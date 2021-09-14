/*
 * Copyright (C) 2021 Huawei Device Co., Ltd.
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
#include <functional>
#include <gtest/gtest.h>
#include <cstdint>
#include <vector>
#include <sys/time.h>
#include <thread>
#include <string>
#include "global.h"
#include "ability_manager_interface.h"
#include "ability_connect_callback_proxy.h"
#include "system_ability_definition.h"
#include "want.h"
#include "input_method_ability_connection_stub.h"
#include "ability_connect_callback_proxy.h"
#include "sa_mgr_client.h"
#include "element_name.h"
#include "input_method_ability.h"
#include "message_handler.h"


using namespace testing::ext;
using namespace OHOS;
using namespace OHOS::MiscServices;
using namespace OHOS::AAFwk;

class InputMethodAbilityTest : public testing::Test
{
public:
    static void SetUpTestCase(void);
    static void TearDownTestCase(void);
    void SetUp();
    void TearDown();
};


void InputMethodAbilityTest::SetUpTestCase(void)
{
    IMSA_HILOGI("InputMethodAbilityTest::SetUpTestCase");
}

void InputMethodAbilityTest::TearDownTestCase(void)
{
    IMSA_HILOGI("InputMethodAbilityTest::TearDownTestCase");
}

void InputMethodAbilityTest::SetUp(void)
{
    IMSA_HILOGI("InputMethodAbilityTest::SetUp");
}

void InputMethodAbilityTest::TearDown(void)
{
    IMSA_HILOGI("InputMethodAbilityTest::TearDown");
}