//////////////////////////////////////////////////
// Blabber [ChatTextView.cpp]
//////////////////////////////////////////////////

#include "ChatTextView.h"
#include <app/Application.h>
#include <app/AppDefs.h>
#include <string>
#include <be_apps/NetPositive/NetPositive.h>
#include <Roster.h>

using namespace std;

ChatTextView::ChatTextView(BRect frame, const char *name, BRect text_rect,
	uint32 resizing_mode, uint32 flags)
	: BTextView(frame, name, text_rect, resizing_mode, flags)
{
	_link_menu = new BPopUpMenu(NULL, false, false);
	_open_link = new BMenuItem("Open link",NULL);
	_copy_to_cb = new BMenuItem("Copy to clipboard",NULL);
	_link_menu->AddItem(_open_link);
	_link_menu->AddSeparatorItem();
	_link_menu->AddItem(_copy_to_cb);
}

ChatTextView::ChatTextView(BRect frame, const char *name, BRect text_rect,
	const BFont *font, const rgb_color *color, uint32 resizing_mode, uint32 flags)
	: BTextView(frame, name, text_rect, font, color, resizing_mode, flags)
{
	_link_menu = new BPopUpMenu(NULL, false, false);
	_open_link = new BMenuItem("Open link",NULL);
	_copy_to_cb = new BMenuItem("Copy to clipboard",NULL);
	_link_menu->AddItem(_open_link);
	_link_menu->AddSeparatorItem();
	_link_menu->AddItem(_copy_to_cb);
}

void ChatTextView::DetectUrl(std::string& url, int32 curr_offset, const char* text, BPoint pt)
{
	while (curr_offset >= 0 && !isspace(text[curr_offset])) {
		if (curr_offset + 8 <= TextLength() && text[curr_offset] == 'h'
			&& text[curr_offset + 1] == 't' && text[curr_offset + 2] == 't'
			&& text[curr_offset + 3] == 'p' && text[curr_offset + 4] == ':'
			&& text[curr_offset + 5] == '/' && text[curr_offset + 6] == '/') {

			url = text[curr_offset++];
			while(curr_offset < TextLength() && !isspace(text[curr_offset])) {
				url += text[curr_offset++];
			}

			break;
		}
	if (curr_offset + 7 <= TextLength() && text[curr_offset] == 'f'
			&& text[curr_offset + 1] == 't' && text[curr_offset + 2] == 'p'
			&& text[curr_offset + 3] == ':' && text[curr_offset + 4] == '/'
			&& text[curr_offset + 5] == '/') {

			url = text[curr_offset++];
			while(curr_offset < TextLength() && !isspace(text[curr_offset])) {
				url += text[curr_offset++];
			}

			break;
		}

		--curr_offset;
	}
	
	if (url.empty()) {
		// Ugly link search
		curr_offset = OffsetAt(pt);

		while (curr_offset >= 0 && !isspace(text[curr_offset])) {
			if (curr_offset + 5 <= TextLength() && text[curr_offset] == 'w'
				&& text[curr_offset + 1] == 'w' && text[curr_offset + 2] == 'w'
				&& text[curr_offset + 3] == '.') {
				// ignore if it's not at the beginning or has no whitespace
				if ((curr_offset - 1) >= 0 && isalnum(text[curr_offset - 1])) {
					--curr_offset;
					continue;
				}

				// is it part of a sentence
				if (isspace(text[curr_offset + 4]) || text[curr_offset + 4] == '.') {
					--curr_offset;
					continue;
				}

				url = text[curr_offset++];

				while(curr_offset < TextLength() && !isspace(text[curr_offset])) {
					url += text[curr_offset++];
				}

				// prepend http
				url = "http://" + url;

				break;
			}

			if (curr_offset + 5 <= TextLength() && text[curr_offset] == 'f'
				&& text[curr_offset + 1] == 't' && text[curr_offset + 2] == 'p'
				&& text[curr_offset + 3] == '.') {
				url = text[curr_offset++];

				while(curr_offset < TextLength() && !isspace(text[curr_offset])) {
					url += text[curr_offset++];
				}

				// prepend http
				url = "ftp://" + url;

				break;
			}

			--curr_offset;
		}
	}

	// prune punctuation
	if (!url.empty()) {
		while (url.size() > 0) {
			if (url[url.size() - 1] == ',' || url[url.size() - 1] == '!'
				|| url[url.size() - 1] == '.' || url[url.size() - 1] == ')'
				|| url[url.size() - 1] == ';' || url[url.size() - 1] == ']'
				|| url[url.size() - 1] == '>' || url[url.size() - 1] == '?'
				|| url[url.size() - 1] == '\'' || url[url.size() - 1] == '"') {
				url.erase(url.size() - 1);
			} else {
				break;
			}
		}
	}
}

void ChatTextView::MouseMoved(BPoint pt, uint32 code, const BMessage* dragMessage)
{
	std::string url;
	const char *text = Text();
	// base
	BTextView::MouseMoved(pt, code, dragMessage);
	// Ugly link search
	int32 curr_offset = OffsetAt(pt);
	// no more looking at spaces
	DetectUrl(url, curr_offset, text, pt);
	if(!url.empty())
	{
		BCursor* buffer = new BCursor(B_CURSOR_ID_FOLLOW_LINK);
		be_app->SetCursor(buffer);
		delete buffer;
	}
	else
	{
		BCursor* buffer = new BCursor(B_CURSOR_ID_I_BEAM);
		be_app->SetCursor(buffer);
		delete buffer;
	}
}

void ChatTextView::MouseDown(BPoint pt)
{
	const char *text = Text();
	std::string url;
	BMenuItem* selected;
	uint32 buttons = 0;
	GetMouse(&pt, &buttons, true);
	// base
	BTextView::MouseDown(pt);
	// Ugly link search
	int32 curr_offset = OffsetAt(pt);

	// no more looking at spaces
	DetectUrl(url, curr_offset, text, pt);
	//Open link, without pop-up menu
	if(buttons & B_PRIMARY_MOUSE_BUTTON)
	{
	 	if(!url.empty())
	 	{
	 		char *argv[] = {const_cast<char *>(url.c_str()), NULL};
			if (!be_roster->IsRunning("text/html"))
			{
				be_roster->Launch("text/html", 1, argv);
			}
			else 
			{
				BMessenger messenger("text/html");
				BMessage msg(B_NETPOSITIVE_OPEN_URL);
				msg.AddString("be:url", url.c_str());
				messenger.SendMessage(&msg);
			}
	 	}
	}
	//check menu selected item
	if(buttons & B_SECONDARY_MOUSE_BUTTON)
	{
		if(!url.empty())
		{
			BPoint screen_point(pt);	
			ConvertToScreen(&screen_point);	
			screen_point.x+=150;
			selected = _link_menu->Go(screen_point);
			//open link
		if(selected == _open_link)
		{
				// load up browser!!
			if (!url.empty()) 
			{
				char *argv[] = {const_cast<char *>(url.c_str()), NULL};
				if (!be_roster->IsRunning("text/html"))
				{
					be_roster->Launch("text/html", 1, argv);
				}
				else {
					BMessenger messenger("text/html");
					BMessage msg(B_NETPOSITIVE_OPEN_URL);
					msg.AddString("be:url", url.c_str());
					messenger.SendMessage(&msg);
				}
			}
		}
		//copy to clipboard
			if(selected == _copy_to_cb)
			{
				be_clipboard->Lock();
				if(be_clipboard->Lock()) 
				{
					be_clipboard->Clear();
					BMessage* clip = be_clipboard->Data();	
					clip->AddData("text/plain", B_MIME_TYPE, url.c_str(), strlen(url.c_str()));
					be_clipboard->Commit();
					be_clipboard->Unlock();
				}
			}	
		}
	}
}
