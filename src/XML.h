#if !defined(AFX_XML_H__D9F1340C_4C0C_4F5C_A646_46D6D74C3489__INCLUDED_)
#define AFX_XML_H__D9F1340C_4C0C_4F5C_A646_46D6D74C3489__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <stdlib.h>
#include "XArray.h"

class CXML 
{
public:
	CXML(const char* szName = NULL);
	virtual ~CXML();

	const char* GetName() { return m_szName; }
	void AddChild(CXML* node);
	CXML* AddChild(const char* szName);
	CXML* GetChild(unsigned int index) { return m_children[index]; }
	unsigned int GetChildCount() { return m_children.size(); }
	void SetAttribute(const char* name, const char* value);
	const char* GetAttributeName(unsigned int index) { return m_attribute_names[index]; }
	const char* GetAttribute(const char* name);
	const char* GetAttribute(unsigned int index) { return m_attribute_values[index]; }
	unsigned int GetAttributeCount() { return m_attribute_names.size(); }
	CXML* GetParent() { return m_parent; }
	CXML* Find(const char* tagname, const char* attr_name = NULL, const char* attr_val = NULL);

#ifdef XML_CREATE
	char* CreateString();
#endif

	static CXML* Parse(const char* szDoc);
	static unsigned int s_resize_count;

protected:
	char* m_szName;
	XArray<CXML*> m_children;
	XArray<char*> m_attribute_names;
	XArray<char*> m_attribute_values;
	CXML* m_parent;
	bool m_parent_delete;
};

#endif
