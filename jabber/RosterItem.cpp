//////////////////////////////////////////////////
// Blabber [RosterItem.cpp]
//////////////////////////////////////////////////

#ifndef ROSTER_ITEM_H
	#include "RosterItem.h"
#endif

#ifndef APP_LOCATION_H
	#include "AppLocation.h"
#endif

#ifndef _TRANSLATION_UTILS_H
	#include <TranslationUtils.h>
#endif

BBitmap *RosterItem::_kinda_online_icon = NULL;
BBitmap *RosterItem::_offline_icon      = NULL;
BBitmap *RosterItem::_online_icon       = NULL;
BBitmap *RosterItem::_unknown_icon      = NULL;
BBitmap *RosterItem::_aol_icon          = NULL;
BBitmap *RosterItem::_icq_icon          = NULL;
BBitmap *RosterItem::_msn_icon          = NULL;
BBitmap *RosterItem::_yahoo_icon        = NULL;

RosterItem::RosterItem(const UserID *userid)
	: BStringItem(userid->FriendlyName().c_str()) {
	_userid           = userid;
	_is_stale_pointer = false;

	// intitialize static members
	if (_offline_icon == NULL) {
		std::string graphic;

		// generate path for home icon
		graphic = AppLocation::Instance()->AbsolutePath("resources/icons/away-online.png");

		// load graphic
		_kinda_online_icon = BTranslationUtils::GetBitmap(graphic.c_str());

		// generate path for home icon
		graphic = AppLocation::Instance()->AbsolutePath("resources/icons/online.png");

		// load graphic
		_online_icon = BTranslationUtils::GetBitmap(graphic.c_str());

		// generate path for away icon
		graphic = AppLocation::Instance()->AbsolutePath("resources/icons/offline.png");

		// load graphic
		_offline_icon = BTranslationUtils::GetBitmap(graphic.c_str());

		// generate path for away icon
		graphic = AppLocation::Instance()->AbsolutePath("resources/icons/unknown.png");

		// load graphic
		_unknown_icon = BTranslationUtils::GetBitmap(graphic.c_str());

		// generate path for AOL icon
		graphic = AppLocation::Instance()->AbsolutePath("resources/icons/aol.png");

		// load graphic
		_aol_icon = BTranslationUtils::GetBitmap(graphic.c_str());

		// generate path for ICQ icon
		graphic = AppLocation::Instance()->AbsolutePath("resources/icons/icq.png");

		// load graphic
		_icq_icon = BTranslationUtils::GetBitmap(graphic.c_str());

		// generate path for Yahoo! icon
		graphic = AppLocation::Instance()->AbsolutePath("resources/icons/yahoo.png");

		// load graphic
		_yahoo_icon = BTranslationUtils::GetBitmap(graphic.c_str());

		// generate path for MSN icon
		graphic = AppLocation::Instance()->AbsolutePath("resources/icons/msn.png");

		// load graphic
		_msn_icon = BTranslationUtils::GetBitmap(graphic.c_str());
	}
}

RosterItem::~RosterItem() {
}

void RosterItem::DrawItem(BView *owner, BRect frame, bool complete) {
	// protection
	if (StalePointer()) {
		return;
	}

	// get online status
	UserID::online_status status = _userid->OnlineStatus();
	std::string                exact_status = _userid->ExactOnlineStatus();

	// text characteristics
	owner->SetFont(be_plain_font);
	owner->SetFontSize(10.0);

	// clear rectangle
	if (IsSelected()) {
		if (status == UserID::ONLINE) {
			if(exact_status == "xa" || exact_status == "away")
			{
				owner->SetHighColor(255, 179, 0, 255);
			}
			else if(exact_status == "dnd") 
			{
				owner->SetHighColor(213, 158, 158, 255);
			}
			else {
				owner->SetHighColor(158, 213, 158, 255);
			}
		} else if (status == UserID::OFFLINE) {
			owner->SetHighColor(213, 213, 213, 255);
		} else {
			owner->SetHighColor(200, 200, 255, 255);
		}
	} else {
		owner->SetHighColor(owner->ViewColor());
	}

	owner->FillRect(frame);

	// draw a graphic
	owner->SetDrawingMode(B_OP_ALPHA);
	owner->SetBlendingMode(B_PIXEL_ALPHA, B_ALPHA_OVERLAY);

	if (status == UserID::ONLINE) {
		if (exact_status == "xa" || exact_status == "away" || exact_status == "dnd") {
			if (_kinda_online_icon) {
				owner->DrawBitmapAsync(_kinda_online_icon, BPoint(frame.left + 1, frame.top + 4));
			}
		} else {
			if (_online_icon) {
				owner->DrawBitmapAsync(_online_icon, BPoint(frame.left + 1, frame.top + 4));
			}
		}
	} else if (status == UserID::OFFLINE) {
		if (_offline_icon) {
			owner->DrawBitmapAsync(_offline_icon, BPoint(frame.left + 1, frame.top + 4));
		}
	} else {
		if (_unknown_icon) {
			owner->DrawBitmapAsync(_unknown_icon, BPoint(frame.left + 1, frame.top + 4));
		}
	}

	float height;

	// construct name
	std::string name = GetUserID()->FriendlyName();

	if (name.empty()) {
		name = GetUserID()->Handle();

		if (name.empty()) {
			name = "<anonymous>";
		}
	}

	BFont statusFont;
			
	// font color is based on online status
	if (status == UserID::ONLINE) {
		if (exact_status == "dnd") {
			owner->SetHighColor(255, 0, 0, 255);
			owner->GetFont(&statusFont);
			statusFont.SetFace(B_ITALIC_FACE);
			owner->SetFont(&statusFont);
		}
		else if(exact_status == "xa" || exact_status == "away")
		{
			owner->SetHighColor(215, 107, 0, 255);
			owner->GetFont(&statusFont);
			statusFont.SetFace(B_ITALIC_FACE);
			owner->SetFont(&statusFont);
		}
		 else {
			owner->SetHighColor(0, 180, 0, 255);
		}
	} else if (status == UserID::OFFLINE) {
		owner->SetHighColor(120, 120, 120, 255); //gray
	} else {
		owner->SetHighColor(0, 0, 255, 255); //blue
	}

	// construct text positioning
	font_height fh;
	owner->GetFontHeight(&fh);

	height = fh.ascent + fh.descent;

	// draw name
	owner->DrawString(name.c_str(), BPoint(frame.left + 13, frame.bottom - ((frame.Height() - height) / 2) - fh.descent));

	// draw show
	if (!GetUserID()->MoreExactOnlineStatus().empty()) {
		owner->SetHighColor(0, 0, 0, 255);

		owner->DrawString(" [");
		owner->DrawString(GetUserID()->MoreExactOnlineStatus().c_str());
		owner->DrawString("]");
	}

	// draw external chat icon
	if (GetUserID()->UserType() == UserID::AIM) {
		if (_aol_icon) {
			owner->DrawBitmapAsync(_aol_icon, BPoint(owner->PenLocation().x + 2.0, frame.top + 2));
		}
	} else if (GetUserID()->UserType() == UserID::YAHOO) {
		if (_yahoo_icon) {
			owner->DrawBitmapAsync(_yahoo_icon, BPoint(owner->PenLocation().x + 3.0, frame.top + 3));
		}
	} else if (GetUserID()->UserType() == UserID::ICQ) {
		if (_icq_icon) {
			owner->DrawBitmapAsync(_icq_icon, BPoint(owner->PenLocation().x + 2.0, frame.top + 2));
		}
	} else if (GetUserID()->UserType() == UserID::MSN) {
		if (_msn_icon) {
			owner->DrawBitmapAsync(_msn_icon, BPoint(owner->PenLocation().x + 2.0, frame.top + 2));
		}
	}
	
	owner->SetFont(be_plain_font);
	owner->SetFontSize(10.0);

}

void RosterItem::Update(BView *owner, const BFont *font) {
	BListItem::Update(owner, font);

	// set height to accomodate graphics and text
	SetHeight(16.0);
}

bool RosterItem::StalePointer() const {
	return _is_stale_pointer;
}

const UserID *RosterItem::GetUserID() const {
	if (StalePointer()) {
		return NULL;
	} else {
		return _userid;
	}
}

void RosterItem::SetStalePointer(bool is_stale) {
	_is_stale_pointer = is_stale;
}
