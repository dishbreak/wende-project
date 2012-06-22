#include "stdafx.h"
#include "Notifier.h"

Notifier::Notifier()
{
	//
}

Notifier::~Notifier()
{
	//
}

void Notifier::RegisterNewNotification(Notification ^ newNote) {
	//TODO: check to see if there's an existing notification
	Notification ^ search = FindNotifyMesg(newNote->notifyMesg);
	if (search != nullptr)
	{
		//update TimeStamp
		search->TimeStamp = newNote->TimeStamp;
	}
	//Add the notification
	NotifyList[NotifyList->GetLength(0)+1] = newNote;
}

ref class Notification^ Notifier::FindNotifyMesg(Notification::NotifyMesg searchKey) {
	ref class Notification ^result = nullptr;
	for each (Notification ^ curNote in NotifyList) {
		if(curNote->notifyMesg == searchKey) {
			return curNote;
		}
	}
	return result;
}