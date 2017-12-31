//////////////////////////////////////////////////
// Blabber [BlabberMainWindow.h]
//     Central information display.
//////////////////////////////////////////////////

#ifndef BLABBER_MAIN_WINDOW_H
#define BLABBER_MAIN_WINDOW_H

#include <InterfaceKit.h>

#ifndef _BUTTON_H
	#include <interface/Button.h>
#endif

#ifndef _CHECK_BOX_H
	#include <interface/CheckBox.h>
#endif

#ifndef _MENU_ITEM_H
	#include <interface/MenuItem.h>
#endif

#ifndef _POP_UP_MENU_H
	#include <interface/PopUpMenu.h>
#endif

#ifndef _TAB_VIEW_H
	#include <interface/TabView.h>
#endif

#ifndef _TEXT_CONTROL_H
	#include <interface/TextControl.h>
#endif

#ifndef _WINDOW_H
	#include <interface/Window.h>
#endif

#ifndef BITMAP_BUTTON_H
	#include "BitmapButton.h"
#endif

#ifndef PICTURE_VIEW_H
	#include "PictureView.h"
#endif

#ifndef ROSTER_VIEW_H
	#include "RosterView.h"
#endif

#ifndef STATUS_VIEW_H
	#include "StatusView.h"
#endif

class BlabberMainWindow : public BWindow {
public:
	static BlabberMainWindow  *Instance();
	                          ~BlabberMainWindow();

	virtual void               MessageReceived(BMessage *msg);
	virtual void               MenusBeginning();
	virtual bool               QuitRequested();

	bool                       ValidateLogin();
	void                       ShowLogin();
	void                       SetCustomStatus(std::string status);
	BApplication* AppPtr;
protected:
	                           BlabberMainWindow(BRect frame);
		                           
private:
	// singleton
	static BlabberMainWindow *_instance;
	
	// GUI
	BView                    *_login_full_view;

	PictureView              *_login_bulb;
	BTextControl             *_login_realname;
	BTextControl             *_login_username;
	BTextControl             *_login_password;

	BTextControl			 *_ssl_server;
	BTextControl			 *_ssl_port;

	BCheckBox                *_login_new_account;
	BCheckBox                *_login_auto_login;
	BCheckBox				 *_ssl_enabled;
	
	BButton                  *_login_login;

	BMenuBar                 *_menubar;
	BMenu                    *_file_menu;
	BMenu                    *_edit_menu;
	BMenu                    *_status_menu;
	BMenu                    *_channel_menu;
	BMenu                    *_talk_menu;

	BMenu                    *_common_status_menu;
	BMenu                    *_help_menu;
	BMenuItem                *_connect_item;
	BMenuItem                *_disconnect_item;
	BMenuItem                *_about_item;
	BMenuItem                *_quit_item;
	BMenuItem                *_add_buddy_item;
	BMenuItem                *_change_buddy_item;
	BMenuItem                *_remove_buddy_item;
	BMenuItem                *_user_info_item;
	BMenuItem                *_user_chatlog_item;
	BMenuItem                *_chat_item;
	BMenuItem                *_away_item;
	BMenuItem                *_dnd_item;
	BMenuItem                *_xa_item;
	BMenuItem                *_school_item;
	BMenuItem                *_work_item;
	BMenuItem                *_lunch_item;
	BMenuItem                *_dinner_item;
	BMenuItem                *_sleep_item;
	BMenuItem                *_custom_item;

	BMenuItem                *_rotate_chat_forward_item;
	BMenuItem                *_rotate_chat_backward_item;
	BMenuItem                *_send_message_item;
	BMenuItem                *_send_chat_item;
	BMenuItem                *_send_groupchat_item;

	BMenuItem                *_preferences_item;
	BMenuItem                *_beos_user_item;
	BMenuItem                *_riv_item;
	BMenuItem                *_jabber_org_item;
	BMenuItem                *_jabber_central_org_item;
	BMenuItem                *_jabber_view_com_item;
	BMenuItem                *_user_guide_item;
	BMenuItem                *_faq_item;

	int32                     _deskbar_id;
	
	BView                    *_full_view;

	PictureView              *_title;
	StatusView               *_status_view;
	
	BPopUpMenu               *_online_status_selection;
	BMenuField               *_online_status;

	RosterView               *_roster;
	BScrollView              *_roster_scroller;
	
	BitmapButton             *_add_buddy, *_remove_buddy, *_send_chat, *_send_message;
};

#endif
