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
#include <gtest/gtest.h>
#include <sys/time.h>

#include <cstdint>
#include <functional>
#include <string>
#include <thread>
#include <vector>

#include "global.h"
#include "input_method_controller.h"
#include "input_method_property.h"
#include "message_handler.h"

using namespace testing::ext;
namespace OHOS {
namespace MiscServices {
class InputMethodSwitchTest : public testing::Test {
public:
    static void SetUpTestCase(void);
    static void TearDownTestCase(void);
    void SetUp();
    void TearDown();
};

void InputMethodSwitchTest::SetUpTestCase(void)
{
    IMSA_HILOGI("InputMethodSwitchTest::SetUpTestCase");
}

void InputMethodSwitchTest::TearDownTestCase(void)
{
    IMSA_HILOGI("InputMethodSwitchTest::TearDownTestCase");
}

void InputMethodSwitchTest::SetUp(void)
{
    IMSA_HILOGI("InputMethodSwitchTest::SetUp");
}

void InputMethodSwitchTest::TearDown(void)
{
    IMSA_HILOGI("InputMethodSwitchTest::TearDown");
}

/**
* @tc.name: intputMethodSwitch_sub_001
* @tc.desc: Checkout SwitchInputMethod.
* @tc.type: FUNC
*/
HWTEST_F(InputMethodSwitchTest, intputMethodSwitch_sub_001, TestSize.Level0)
{
    InputMethodProperty *target = new InputMethodProperty();
    std::string packageName = "com.ohos.inputApp";
    std::string imeId = "InputDemoService";
    target->mPackageName = std::wstring_convert<std::codecvt_utf8_utf16<char16_t>, char16_t>{}.from_bytes(packageName);
    target->mImeId = std::wstring_convert<std::codecvt_utf8_utf16<char16_t>, char16_t>{}.from_bytes(imeId);
    int32_t ret = InputMethodController::GetInstance()->SwitchInputMethod(target);
    delete target;
    EXPECT_TRUE(0 == ret);
}
} // namespace MiscServices
} // namespace OHOS
