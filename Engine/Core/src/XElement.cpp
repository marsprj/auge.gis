#include "AugeXML.h"
#include "AugeCore.h"
#include "string.h"

namespace auge
{

	XElement::XElement(xmlNodePtr pxNode):
	XNode(pxNode)
	{

	}

	XElement::~XElement()
	{

	}

	//////////////////////////////////////////////////////////////////////////
	void XElement::SetNamespaceDeclaration(const char* ns_uri, const char* ns_prefix)
	{
		xmlNewNs(m_pxNode, (const xmlChar*)ns_uri,
			(const xmlChar*)ns_prefix);
	}

	const char* XElement::GetNamespaceUriForPrefix(const char* ns_prefix)
	{
		const char* uri = NULL;
		const xmlNs* ns = xmlSearchNs(m_pxNode->doc, const_cast<xmlNode*>(m_pxNode), (xmlChar*)ns_prefix);
		if(ns!=NULL)
		{
			uri = (const char*)ns->prefix;
		}
		return uri;
	}

	XAttribute*	XElement::SetAttribute(const char* name, const char* value, const char* ns_prefix)
	{
		xmlAttr* attr = NULL;

		char* name_utf8	 = (char*)name;
		char* value_utf8 = (char*)value;
		char* ns_prefix_utf8 = (char*)ns_prefix;

		//Ignore the namespace if none was specified:
		if((ns_prefix==NULL)||strlen(ns_prefix)==0)
		{
			attr = xmlSetProp(m_pxNode, (const xmlChar*)name_utf8, (const xmlChar*)value_utf8);
		}
		else
		{
			//If the namespace exists, then use it:
			xmlNs* ns = xmlSearchNs(m_pxNode->doc, m_pxNode, (const xmlChar*)ns_prefix_utf8);
			if (ns)
			{
				attr = xmlSetNsProp(m_pxNode, ns, (const xmlChar*)name_utf8, (const xmlChar*)value_utf8);
			}
			else
			{
			}
		}

		if(attr)
			return reinterpret_cast<XAttribute*>(attr->_private);
		return NULL;
	}

	XAttribute*	XElement::GetAttribute(const char* name, const char* ns_prefix)
	{
		if(name==NULL)
		{
			return NULL;
		}

		const char* name_utf8 = auge_encoding_convert("GBK", "UTF-8", name, strlen(name));
		if(name_utf8==NULL)
		{
			return NULL;
		}

		if(ns_prefix==NULL)
		{
			xmlAttr* attr = xmlHasProp(const_cast<xmlNode*>(m_pxNode), (const xmlChar*)name_utf8);
			if( attr )
			{
				return reinterpret_cast<XAttribute*>(attr->_private);
			}
		}
		else
		{
			const char* ns_uri = GetNamespaceUriForPrefix(ns_prefix);
			xmlAttr* attr = xmlHasNsProp(const_cast<xmlNode*>(m_pxNode), 
				(const xmlChar*)name_utf8,
				(const xmlChar*)ns_uri);
			if( attr )
			{
				return reinterpret_cast<XAttribute*>(attr->_private);
			}
		}

		return NULL;
	}

	void XElement::SetChildText(const char* content)
	{
		XTextNode* node = GetChildText();
		if(node!=NULL)
			node->SetContent(content);
		else
			AddChildText(content);
	}

	XCommentNode* XElement::AddChildComment(const char* content)
	{
		char* content_utf8 = (char*)content;
		xmlNode* node = xmlNewComment((const xmlChar*)content_utf8);

		// Use the result, because node can be freed when merging text nodes:
		node = xmlAddChild(m_pxNode, node);
		return static_cast<XCommentNode*>(node->_private);
	}

	XTextNode* XElement::AddChildText(const char* content)
	{
		if(m_pxNode->type == XML_ELEMENT_NODE)
		{
			char* content_utf8 = (char*)content;
			xmlNode* node = xmlNewText((const xmlChar*)content_utf8);

			// Use the result, because node can be freed when merging text nodes:
			node = xmlAddChild(m_pxNode, node); 

			return static_cast<XTextNode*>(node->_private);
		}
		return 0;
	}

	XTextNode* XElement::GetChildText()
	{
		// TODO: This only returns the first content node.
		// What should we do instead? Update the documentation if we change this. murrayc.
		for(xmlNode* child = m_pxNode->children; child; child = child->next)
		{
			if(child->type == XML_TEXT_NODE)
			{
				return static_cast<XTextNode*>(child->_private);
			}
		}

		return NULL;
	}

}
