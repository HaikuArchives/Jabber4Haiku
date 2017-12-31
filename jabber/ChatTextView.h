//////////////////////////////////////////////////
// Blabber [ChatTextView.h]
//     Handles MouseDown.
//////////////////////////////////////////////////

#ifndef CHAT_TEXT_VIEW_H
#define CHAT_TEXT_VIEW_H

#include <Clipboard.h>

#include <Cursor.h>

//#include "BlabberApp.h"

#ifndef BLABBER_MAIN_WINDOW_H
	#include "BlabberMainWindow.h"
#endif

#ifndef _TEXT_VIEW_H
	#include <interface/TextView.h>
#endif

#ifndef _POP_UP_MENU_H
	#include <interface/PopUpMenu.h>
#endif

#ifndef _LIST_ITEM_H
	#include <interface/ListItem.h>
#endif

#ifndef ROSTER_ITEM_H
	#include "RosterItem.h"
#endif
#ifndef _MENU_ITEM_H
	#include <MenuItem.h>
#endif

#ifndef _OUTLINE_LIST_VIEW_H
	#include <interface/OutlineListView.h>
#endif

class ChatTextView : public BTextView {
public:
	ChatTextView(BRect frame, const char *name, BRect text_rect, uint32 resizing_mode, uint32 flags);
	ChatTextView(BRect frame, const char *name, BRect text_rect, const BFont *font, const rgb_color *color, uint32 resizing_mode, uint32 flags);
	void MouseMoved(BPoint pt, uint32 code, const BMessage* dragMessage);
	void MouseDown(BPoint pt);
private:
	bool right_button_click;
	BPopUpMenu       *_link_menu;
	BMenuItem		  *_open_link;
	BMenuItem         *_copy_to_cb;
};

#endif
