# miscservices_inputmethod

####Introduction

Input Method Framework, is used to connect the application and input method. the application can input text through the input method

####Warehouse path

/base/miscservices/inputmethod

####Introduction to framework code

The input method framework currently has four modules, as follows:

1. Application client

Path: /base/miscservices/inputmethod/frameworks/inputmethod_controller

Function: realize the service delivery of application and input method framework, including the binding between application and input method service, application display and hiding request for input method, etc

2. Input method client

Path: /base/miscservices/inputmethod/frameworks/inputmethod_ability

Function: the intermediate bridge between input method framework service and input method delivery, including monitoring the current status of input method, etc

3. Input method service

Path: /base/miscservices/inputmethod/services

Function: as the core of the input method framework, the main processing logic of the input method is completed here

4. Input method JS interface

Path: /base/miscservices/inputmethod/interfaces/kits/JS

Function: the temporarily exposed JS interface is mainly reserved for calling input methods

####Main functions supported by the framework

1. Click in the edit attribute control to invoke the default input method application through the input method framework

2. Typing can be carried out through the input method application, and characters can be input to the application client on the screen

####Participation contribution

1. Fork warehouse
2. Submission code
3. Create a new pull request
4. Commit is complete
