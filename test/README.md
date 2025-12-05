# How to test

To execute the described tests in this folder the [platformio-unittest-framework](https://docs.platformio.org/en/latest/advanced/unit-testing/index.html) [Unity](https://registry.platformio.org/libraries/throwtheswitch/Unity) is used.

Each test can be executed seperatly by using the __test_filter__ option in the __platformio.ini__.


In my __vscode__ application the ribbon __Testing__ is provided in which i can execute each test seperatly or all at once.


Before you can execute the tests by yourself you have to adapt the __common_test_settings.h__ file in the folder api and functional.
Here you need to adjust the __defaultsettings__ to your needs.

Important to know is that you have to connect the Interrupt-Pin of the MCP2515 to your Arduino-Board since there are some tests in which the Interrupt functionality is tested by checking these pin.
