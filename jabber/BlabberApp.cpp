//////////////////////////////////////////////////
// Blabber [BlabberApp.cpp]
//////////////////////////////////////////////////

#ifndef __CSTDLIB__
	#include <cstdlib>
#endif

#include <InterfaceKit.h>

#ifndef BLABBER_APP_H
	#include "BlabberApp.h"
#endif

#ifndef BLABBER_MAIN_WINDOW_H
	#include "BlabberMainWindow.h"
#endif

#ifndef BLABBER_SETTINGS_H
	#include "BlabberSettings.h"
#endif

#ifndef JABBER_SPEAK_H
	#include "JabberSpeak.h"
#endif

#ifndef MESSAGE_REPEATER_H
	#include "MessageRepeater.h"
#endif

#ifndef MODAL_ALERT_FACTORY_H
	#include "ModalAlertFactory.h"
#endif

// remove as soon as we decide to drop R5 support...
#ifdef __BEOS__
const char* kAppMIMEType = "application/jabber";
#else
const char* kAppMIMEType = "application/x-vnd.Haiku-Jabber";
#endif

BlabberApp::BlabberApp()
	: BApplication(kAppMIMEType) {
	
	// launch the message repeater looper
	MessageRepeater::Instance()->Run();

	// display opening window
	BlabberMainWindow::Instance()->Show();
	BlabberMainWindow::Instance()->AppPtr = this;
	// start up the network queue
	JabberSpeak::Instance()->Run();

	// launch initial login process
	JabberSpeak::Instance()->SendConnect();
}

BlabberApp::~BlabberApp() {
	// take down the message repeater
	MessageRepeater::Instance()->Lock();
	MessageRepeater::Instance()->Quit();

	// take down the network queue
	JabberSpeak::Instance()->Lock();
	JabberSpeak::Instance()->Quit();
	
	// clean up the settings module
	delete BlabberSettings::Instance();
	

}
