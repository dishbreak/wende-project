#include "stdafx.h"
#ifndef NOTIFICATION_H
#define NOTIFICATION_H
ref class Notification {
	//
public:	
	enum class NotifyMesg {
		PatientLeftEvacArea,
		SystemOperational,
		SystemNonOperational,
		TrialSuccess,
		TrialFailed,
		AllClear
	};
	enum class NotifyType {
		Alert,
		Warning,
		Information
	};
	enum class NotifyType notifyType;
	enum class NotifyMesg notifyMesg;
	enum class NotifyMesg Contradict;
	bool IsDisplayed;
	System::String ^ NotifyText;
	System::DateTime TimeStamp;
	System::Drawing::Color bgColor;
	System::Drawing::Color fgColor;
	Notification(NotifyMesg);
	~Notification();
};

#endif //NOTIFICATION_H