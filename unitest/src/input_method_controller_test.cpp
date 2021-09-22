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
#include "utils.h"

#include "input_method_controller.h"
#include "i_input_method_system_ability.h"
#include "i_input_method_agent.h"
#include "input_data_channel_stub.h"
#include "input_client_stub.h"
#include "iservice_registry.h"
#include "system_ability_definition.h"

using namespace testing::ext;
using namespace OHOS;
using namespace OHOS::MiscServices;

class TextListener : public OnTextChangedListener {
public:
    TextListener() {}
    ~TextListener() {}
    void InsertText(const std::u16string& text) {
        IMSA_HILOGI("IMC TEST TextListener InsertText: %{public}s", MiscServices::Utils::to_utf8(text).c_str());
    }

    void DeleteBackward(int32_t length){
        IMSA_HILOGI("IMC TEST TextListener DeleteBackward length: %{public}d", length);
    }

    void SetKeyboardStatus(bool status) {
        IMSA_HILOGI("IMC TEST TextListener SetKeyboardStatus %{public}d", status);
    }
};
class InputMethodControllerTest : public testing::Test {
public:
    static void SetUpTestCase(void);
    static void TearDownTestCase(void);
    void SetUp();
    void TearDown();
};

void InputMethodControllerTest::SetUpTestCase(void) {
    IMSA_HILOGI("InputMethodControllerTest::SetUpTestCase");
}

void InputMethodControllerTest::TearDownTestCase(void) {
    IMSA_HILOGI("InputMethodControllerTest::TearDownTestCase");
}

void InputMethodControllerTest::SetUp(void) {
    IMSA_HILOGI("InputMethodControllerTest::SetUp");
}

void InputMethodControllerTest::TearDown(void) {
    IMSA_HILOGI("InputMethodControllerTest::TearDown");
}

/**
* @tc.name: Imc001
* @tc.desc: Bind IMSA.
* @tc.type: FUNC
*/
HWTEST_F(InputMethodControllerTest, Imc001, TestSize.Level0) {
    IMSA_HILOGI("IMC TEST START");
    sptr<InputMethodController> imc = InputMethodController::GetInstance();
    ASSERT_TRUE(imc!=nullptr);
    sptr<OnTextChangedListener> textListener = new TextListener();

    IMSA_HILOGI("IMC Attach START");
    imc->Attach();
    int waitForStatusOk =2;
    sleep(waitForStatusOk);

    IMSA_HILOGI("IMC ShowTextInput START");
    imc->ShowTextInput(textListener);
    sleep(10);

    IMSA_HILOGI("IMC HideTextInput START");
    imc->HideTextInput();
    sleep(waitForStatusOk);

    IMSA_HILOGI("IMC Close START");
    imc->Close();
    sleep(waitForStatusOk);
    IMSA_HILOGI("IMC TEST OVER");
}