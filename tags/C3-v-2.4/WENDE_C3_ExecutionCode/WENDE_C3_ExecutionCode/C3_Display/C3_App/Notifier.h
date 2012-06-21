#include "stdafx.h"
#include "Notification.h"
#ifndef NOTIFIER_H
#define NOTIFIER_H

ref class Notifier {
public:
	void RegisterNewNotification(Notification ^);
	//ref class Notification^ GetDisplayNotification();
	Notifier();
	~Notifier();
private:
	ref class Notification^ FindNotifyMesg(Notification::NotifyMesg);
	array<ref class Notification ^>^ NotifyList;
};

#endif //NOTIFIER_H