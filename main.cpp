#include <iostream>
#include <windows.h>

static const char *GetChangeDisplaySettingsErrorMessage(int errorCode)
{
    switch (errorCode) {
    case DISP_CHANGE_SUCCESSFUL:
        return "The settings change was successful";
    case DISP_CHANGE_BADDUALVIEW:
        return "The settings change was unsuccessful because the system is DualView capable";
    case DISP_CHANGE_BADFLAGS:
        return "An invalid set of flags was passed in";
    case DISP_CHANGE_BADMODE:
        return "The graphics mode is not supported";
    case DISP_CHANGE_BADPARAM:
        return "An invalid parameter was passed in";
    case DISP_CHANGE_FAILED:
        return "The display driver failed the specified graphics mode";
    case DISP_CHANGE_NOTUPDATED:
        return "Unable to write settings to the registry";
    case DISP_CHANGE_RESTART:
        return "The computer must be restarted for the graphics mode to work";
    }
    return "Unknown error";
}

int main(int argc, char **argv)
{
    char programName[_MAX_FNAME] = "RefreshRateSwitcher";
    _splitpath_s(argv[0], nullptr, 0, nullptr, 0, programName, _MAX_FNAME, nullptr, 0);

	if (argc < 3) {
        std::cerr << "Usage: " << programName << " <displayNum> <refreshRate>\n\n"
            << "displayNum\n"
            << "\tNumeric index of the display: 0, 1, 2... etc.\n\n"
            << "refreshRate\n"
            << "\tRefresh rate to use for the display, in hertz. Passing 0 will reset it to the default value.\n\n"
            << "Example: " << programName << " 0 144\n"
            << std::endl;
		return EXIT_FAILURE;
	}

    auto displayNum = std::atoi(argv[1]);
    auto refreshRate = std::atoi(argv[2]);

    DISPLAY_DEVICE displayDevice = { 0 };
    bool found = false;

    for (auto iDevNum = 0; ; iDevNum++) {
        ZeroMemory(&displayDevice, sizeof(displayDevice));
        displayDevice.cb = sizeof(displayDevice);

        auto result = EnumDisplayDevices(
            nullptr,
            iDevNum,
            &displayDevice,
            DISPLAY_DEVICE_ATTACHED_TO_DESKTOP);
        if (!result) {
            break;
        }
        if (iDevNum == displayNum) {
            found = true;
            break;
        }
    }

    std::cout << "Display: " << displayDevice.DeviceString << std::endl;
    std::cout << "Connected monitors: " << std::endl;

    for (auto iDevNum = 0; ; iDevNum++) {
        DISPLAY_DEVICE monitorDevice = { 0 };
        monitorDevice.cb = sizeof(monitorDevice);

        auto result = EnumDisplayDevices(
            displayDevice.DeviceName,
            iDevNum,
            &monitorDevice,
            DISPLAY_DEVICE_ATTACHED_TO_DESKTOP);
        if (!result) {
            break;
        }
        std::cout << "\t" << monitorDevice.DeviceString << std::endl;
    }

    DEVMODE mode = { 0 };
    mode.dmSize = sizeof(mode);
    if (!EnumDisplaySettings(displayDevice.DeviceName, 0, &mode)) {
        std::cerr << "Error: Failed to obtain monitor settings" << std::endl;
        return EXIT_FAILURE;
    }
    if (!EnumDisplaySettings(displayDevice.DeviceName, ENUM_CURRENT_SETTINGS, &mode)) {
        std::cerr << "Error: Failed to obtain current monitor settings" << std::endl;
        return EXIT_FAILURE;
    }

    mode.dmFields = DM_DISPLAYFREQUENCY;
    mode.dmDisplayFrequency = refreshRate;

    auto changeResult = ChangeDisplaySettingsEx(
        displayDevice.DeviceName,
        &mode,
        nullptr,
        0,
        nullptr);
    if (changeResult != DISP_CHANGE_SUCCESSFUL) {
        std::cerr
            << "Error: Failed to change display settings: "
            << GetChangeDisplaySettingsErrorMessage(changeResult) << std::endl;
        return EXIT_FAILURE;
    }

    std::cout << "Display settings were updated successfully" << std::endl;

    return EXIT_SUCCESS;
}
