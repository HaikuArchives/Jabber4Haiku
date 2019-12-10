/*
 * Copyright (C) 2019 Adrien Destugues <pulkomandy@pulkomandy.tk>
 *
 * Distributed under terms of the MIT license.
 */

#include "media.h"
#include <gloox/tag.h>

#include "Extensions.h"


Media::Media(const gloox::Tag* tag)
	: gloox::StanzaExtension(ExtMedia)
{
    if(!tag || tag->name() != "media" || tag->xmlns() != XMLNS_MEDIA)
      return;

	// Get the URI
	gloox::Tag* uri = tag->findChild("uri");
	fType = uri->findAttribute("type");
	fURI = uri->cdata();
}


const std::string& Media::filterString() const
{
	static const std::string filter = "/iq/query/x/field/media[@xmlns='" + XMLNS_MEDIA + "']";
	return filter;
}


gloox::Tag* Media::tag() const
{
	gloox::Tag* t = new gloox::Tag("media");
	t->setXmlns(XMLNS_MEDIA);
	gloox::Tag* uri = new gloox::Tag("uri");
	uri->setCData(fURI);
        uri->addAttribute("type", fType);
	t->addChild(uri);
	return t;
}
