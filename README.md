RefreshRateSwitcher
===================

This program allows you to dynamically change the refresh rate of your monitor via command line on Windows 7/8/10+.

For example, it can be useful if your cheap-ass monitor makes an annoying squeaky sound when running at the maximum
rate and you want to only use that mode while gaming.

Usage:

```
RefreshRateSwitcher <displayNum> <refreshRate>

<displayNum>
        Numeric index of the display: 0, 1, 2... etc.

<refreshRate>
        Refresh rate to use for the display, in hertz. Passing 0 will reset it to the default value.
```

Example:

```
RefreshRateSwitcher 0 144
```

Example AutoHotkey script:

```autohotkey
+#G::
	RunWait, "C:\Tools\RefreshRateSwitcher.exe" 0 144
	return

+#!G::
	RunWait, "C:\Tools\RefreshRateSwitcher.exe" 0 100
	return
```
