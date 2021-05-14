#include "ofxAppUpdateNotifier.h"
#include "ofxJSON.h"
#include <time.h>

using namespace AppUpdateNotifier;


void ofxAppUpdateNotifier::init(int currentVersionNumber, string url, int frequencyInHours, string filename)
{
	_currentVersionNumber = currentVersionNumber;
	_url = url;
	_frequency = frequencyInHours * 3600;
	_filename = filename;

	if (_frequency == 0)
	{
		checkNewVersion();
	}
	else
	{
		// get last checked date from file
		ofBuffer buffer = ofBufferFromFile(_filename);		
		if (buffer.size() > 0)
		{
			string lastCheckedDateStr = buffer.getText();
			_lastCheckedDate = difftime(ofToInt(lastCheckedDateStr), time(NULL));	// negative
		}
		else
		{
			_lastCheckedDate = -_frequency;	// check now
		}

		startThread();
	}	
}

//--------------------------------------------------------------
void ofxAppUpdateNotifier::threadedFunction()
{
	while (isThreadRunning())
	{
		if (ofGetElapsedTimef() - _lastCheckedDate >= _frequency)
			checkNewVersion();
		else
			ofSleepMillis(1000 * (_frequency + _lastCheckedDate));
	}
}

//--------------------------------------------------------------
void ofxAppUpdateNotifier::exit()
{
	stopThread();

	// write last checked date to file on disk
	ofstream file(ofToDataPath(_filename).c_str());
	file << time(NULL);
	file.close();
}

//--------------------------------------------------------------
void ofxAppUpdateNotifier::checkNewVersion()
{
	ofxJSONElement response;
	if (!response.open(_url))
	{
		ofLogNotice("ofxAppUpdateNotifier::checkNewVersion") << "Failed to parse JSON";
	}
	else
	{
		_lastCheckedDate = ofGetElapsedTimef();
		int versionNumber = response["number"].asInt();
		if (versionNumber > _currentVersionNumber)
		{
			Version version;
			version.number = versionNumber;
			version.displayNumber = response["displayNumber"].asString();			
			version.isCritical = response["isCritial"].asBool();
			version.url = response["url"].asString();

			ofNotifyEvent(newVersionAvailable, version, this);

			stopThread();
		}		
	}

	sleep(1000 * _frequency);
}