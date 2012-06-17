#include "stdafx.h"
#include "Notification.h"


Notification::Notification(Notification::NotifyMesg incoming)
{
	//stamp it with the current time.
	TimeStamp = System::DateTime::Now;
	//populate the notification.
	notifyMesg = incoming;
	//by default, all notifications are displayed.
	IsDisplayed = true;
	switch(notifyMesg)
	{
	//alert-type
	case NotifyMesg::PatientLeftEvacArea:
		notifyType = NotifyType::Alert;
		NotifyText = "PATIENT LEFT EVAC AREA!";
		Contradict = NotifyMesg::AllClear;
		break;
	//warning-type
	case NotifyMesg::SystemNonOperational:
		notifyType = NotifyType::Warning;
		NotifyText = "WENDE SYSTEM NOT OPERATIONAL";
		Contradict = NotifyMesg::SystemOperational;
		break;
	//information-type
	case NotifyMesg::SystemOperational:
		notifyType = NotifyType::Information;
		NotifyText = "WENDE SYSTEM OPERATIONAL";
		Contradict = NotifyMesg::SystemNonOperational;
		break;
	case NotifyMesg::TrialSuccess:
		notifyType = NotifyType::Information;
		NotifyText = "TRIAL SUCCESS";
		Contradict = NotifyMesg::TrialSuccess;
		break;
	case NotifyMesg::TrialFailed:
		notifyType = NotifyType::Alert;
		NotifyText = "TRIAL FAILURE";
		Contradict = NotifyMesg::AllClear;
		break;
	case NotifyMesg::AllClear:
		notifyType = NotifyType::Information;
		NotifyText = "";
		Contradict = NotifyMesg::PatientLeftEvacArea;
		IsDisplayed = false;
		break;
	default:
		break;
	}
	//decorate notification based on type
	switch(notifyType)
	{
	case NotifyType::Alert:
		bgColor = System::Drawing::Color::DarkRed;
		fgColor = System::Drawing::Color::White;
		break;
	case NotifyType::Warning:
		bgColor = System::Drawing::Color::Goldenrod;
		fgColor = System::Drawing::Color::Black;
		break;
	case NotifyType::Information:
		bgColor = System::Drawing::Color::DarkGreen;
		fgColor = System::Drawing::Color::Black;
		break;
	default:
		break;
	}
}

Notification::~Notification()
{
}

