#pragma once
#include "DocumentID.h"

class Link
{
public:
	Link(const DocumentID& from, const DocumentID& to)
		:m_From(from),
		 m_To(to)
	{}

	DocumentID getFromDocID() const { return m_From; }
	DocumentID getToDocID() const { return m_To; }

public:
	DocumentID	m_From;
	DocumentID	m_To;
};