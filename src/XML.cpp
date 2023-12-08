#include "XML.h"
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#ifdef XML_CREATE
#include <string>
#endif

unsigned int CXML::s_resize_count = 0x10;

CXML::CXML(const char* szName)
{
	if (szName)
	{
		unsigned int len = strlen(szName);
		m_szName = new char[len + 1];
		memcpy(m_szName, szName, len);
		m_szName[len] = '\0';
	}
	else
		m_szName = NULL;
	m_parent = NULL;
	m_parent_delete = true;
}

CXML::~CXML()
{
	if (m_szName)
		delete[] m_szName;
	unsigned int k = 0;
	for (; k < m_attribute_names.size(); ++k)
		delete[] m_attribute_names[k];
	for (k = 0; k < m_attribute_values.size(); ++k)
		delete[] m_attribute_values[k];
	for (k = 0; k < m_children.size(); ++k)
	{
		if (m_children[k]->m_parent_delete)
			delete m_children[k];
	}
}

void CXML::AddChild(CXML* node)
{
	node->m_parent_delete = false;
	m_children.push(node);
	node->m_parent = this;
}

CXML* CXML::AddChild(const char* szName)
{
	CXML* temp = new CXML(szName);
	AddChild(temp);
	return temp;
}

void CXML::SetAttribute(const char* name, const char* value)
{
	for (unsigned int k = 0; k < m_attribute_names.size(); ++k)
	{
		if (strcmp(m_attribute_names[k], name) == 0)
		{
			delete m_attribute_values[k];
			int len = strlen(value);
			char* temp = new char[len + 1];
			memcpy(temp, value, len);
			temp[len] = '\0';
			m_attribute_values[k] = temp;
			return;
		}
	}
	int len = strlen(name);
	char* temp = new char[len + 1];
	memcpy(temp, name, len);
	temp[len] = '\0';
	m_attribute_names.push(temp);

	len = strlen(value);
	temp = new char[len + 1];
	memcpy(temp, value, len);
	temp[len] = '\0';
	m_attribute_values.push(temp);
}

const char* CXML::GetAttribute(const char* name)
{
	for (unsigned int k = 0; k < m_attribute_names.size(); ++k)
	{
		if (stricmp(m_attribute_names[k], name) == 0)
			return m_attribute_values[k];
	}
	return NULL;
}

#ifdef XML_CREATE

std::string stdCreateString(CXML* node, unsigned int tabcount = 0)
{
	std::string outstr;
	unsigned int k = 0;
	for (; k < tabcount; ++k)
		outstr += '\t';
	outstr += '<';
	outstr += node->GetName();
	for (k = 0; k < node->GetAttributeCount(); ++k)
	{
		outstr += ' ';
		outstr += node->GetAttributeName(k);
		outstr += "=\"";
		outstr += node->GetAttribute(k);
		outstr += '"';
	}
	unsigned int childcount = node->GetChildCount();
	if (childcount == 0)
		outstr += "/>\n";
	else
	{
		outstr += ">\n";
		for (k = 0; k < childcount; ++k)
			outstr += stdCreateString(node->GetChild(k), tabcount + 1);
		for (k = 0; k < tabcount; ++k)
			outstr += '\t';
		outstr += "</";
		outstr += node->GetName();
		outstr += ">\n";
	}
	return outstr;
}

char* CXML::CreateString()
{
	std::string str = stdCreateString(this);
	unsigned int len = strlen(str.c_str());
	char* retstr = new char[len + 1];
	memcpy(retstr, str.c_str(), len);
	retstr[len] = NULL;
	return retstr;
}

#endif

CXML* CXML::Find(const char* tagname, const char* attr_name, const char* attr_val)
{
	CXML* temp = NULL;
	for (unsigned int k = 0; k < m_children.size(); ++k)
	{
		temp = m_children[k];
		if (stricmp(temp->GetName(), tagname) == 0)
		{
			if (attr_name)
			{
				for (unsigned int n = 0; n < temp->m_attribute_names.size(); ++n)
				{
					if (stricmp(temp->m_attribute_names[n], attr_name) == 0)
					{
						if (!attr_val || stricmp(temp->m_attribute_values[n], attr_val) == 0)
							return temp;
					}
				}
			}
			else
				return temp;
		}
		if ((temp = temp->Find(tagname, attr_name, attr_val)))
			return temp;
	}
	return NULL;
}

#define MODE_SCANNING	0
#define MODE_TAG		1

CXML* CXML::Parse(const char* szDoc)
{
	CXML* root = NULL;
	CXML* curnode = NULL;
	int mode = MODE_SCANNING;
	unsigned int doclen = strlen(szDoc);
	char* doc = new char[doclen + 1];
	memcpy(doc, szDoc, doclen);
	doc[doclen] = '\0';
	char* pos = doc;
	char* end = doc + doclen;

	while (pos < end)
	{
		if (isspace(*pos))
		{
			
		}
		else
		{
			switch (*pos)
			{
			case '<':
				{
					if (pos[1] == '?')
					{
						pos += 2;
						while (*pos != '?')
						{
							if (pos[1] == '>')
								break;
							++pos;
							if (pos == end)
								goto parse_failed;
						}
						++pos;
					}
					else if (pos[1] == '/')
					{
						if (!curnode)
							goto parse_failed;
						pos += 2;
						char* base = pos;
						while (*pos != '>')
						{
							++pos;
							if (pos == end)
								goto parse_failed;
						}
						*pos = '\0';

						if (strcmp(base, curnode->m_szName) != 0)
							goto parse_failed;
						curnode = curnode->m_parent;
					}
					else
					{
						++pos;
						char* base = pos;
						while (!isspace(*pos) && *pos != '>')
						{
							++pos;
							if (pos == end)
								goto parse_failed;
						}
						*pos = '\0';
						//++pos;
						CXML* temp = new CXML(base);
						if (!root)
							root = temp;
						else
							curnode->AddChild(temp);
						curnode = temp;
						mode = MODE_TAG;
					}
					break;
				}
			case '>':
				{
					if (mode != MODE_TAG)
						goto parse_failed;
					if (pos[-1] == '/')
						curnode = curnode->m_parent;
					mode = MODE_TAG;
					break;
				}
			case '/':
				{
					if (mode != MODE_TAG)
						goto parse_failed;
					break;
				}
			default: // attribute name
				{
					if (mode != MODE_TAG)
						goto parse_failed;
					char* name_base = pos;
					while (*pos != '=')
					{
						++pos;
						if (pos == end)
							goto parse_failed;
					}
					*pos = '\0';
					++pos;
					if (*pos != '"')
						goto parse_failed;
					++pos;
					char* value_base = pos;
					while (*pos != '"')
					{
						++pos;
						if (pos == end)
							goto parse_failed;
					}
					*pos = '\0';
					curnode->SetAttribute(name_base, value_base);
					break;
				}
			};
		}
		++pos;
	}
	delete doc;
	return root;
parse_failed:
	if (root)
		delete root;
	delete doc;
	return NULL;
}
