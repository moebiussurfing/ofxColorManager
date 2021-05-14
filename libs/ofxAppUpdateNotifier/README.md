ofxAppUpdateNotifier
====================

An OpenFrameworks addon to notify user when a new release of your app is available

* URL can be local (for tests) or distant
* Frequency of checks can be parameterized (every X hours or when application starts)
* A local file stores the last checked date, to reduce the number of checks
* New version can have a flag "critical" for your app to act in consequence

This addon requires ofxJSON (https://github.com/jefftimesten/ofxJSON)
