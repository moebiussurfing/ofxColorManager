// ofxAppUpdateNotifier
// Raise an event when a new version of the app is available

#pragma once

#include "ofMain.h"


namespace AppUpdateNotifier
{
	struct Version
	{
	public:
		int number;
		string displayNumber;
		bool isCritical;
		string url;
	};
}


class ofxAppUpdateNotifier : public ofThread
{
public:
	void init(int currentVersionNumber, string url, int frequencyInHours, string filename = "AppUpdateLastCheckedDate");	
	void exit();

	ofEvent<AppUpdateNotifier::Version> newVersionAvailable;

private:
	void threadedFunction();
	void checkNewVersion();

	int _currentVersionNumber;
	string _url;
	int _frequency;
	float _lastCheckedDate;
	string _filename;
};