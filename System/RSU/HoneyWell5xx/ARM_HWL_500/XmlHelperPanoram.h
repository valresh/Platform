#pragma once
#ifndef XmlHelperPanoram_H
#define XmlHelperPanoram_H

#include "math.h"
//#import "msxml3.dll"
#define MAX_POINTS_IN_ONE_FILE 100
#define MAX_POINTS_IN_ARRAY 60000

//#include "Windows.h"
#include "linuxTypes/cstring.h"

#include "crosswindows.h"
#include <crossplatform.h>


#include <filesystem>
#include <iostream>

class CXmlHelperPanoram
{
public:

    //TODO need some XML Linux parser
    // MSXML2::IXMLDOMDocumentPtr m_pDoc;
    cross::CString m_strFilePath;

    // void operator =(CXmlHelperPanoram& xml)
    // {
    //     //		m_pDoc = xml.m_pDoc;
    //     m_strFilePath = xml.m_strFilePath;
    // }

    //    CXmlHelperPanoram(){}
    // virtual ~CXmlHelperPanoram()
    // {
    //     //	m_pDoc = NULL;
    // }

    BOOL CreateXmlDocument(LPCTSTR szLoadFile=NULL)
    {
        HRESULT hr = E_FAIL;
        // try
        // {
        // 	if(m_pDoc != NULL)
        // 		m_pDoc = NULL;

        // 	hr = m_pDoc.CreateInstance(__uuidof(MSXML2::DOMDocument30));
        // 	if (FAILED(hr))
        // 		throw _com_error(hr);

        // 	if(szLoadFile != NULL)
        // 	{
        // 		m_pDoc->async = VARIANT_FALSE;
        // 		if(VARIANT_TRUE != m_pDoc->load(szLoadFile))
        // 			throw _com_error(E_FAIL);
        // 		m_strFilePath = szLoadFile;
        // 	}
        // 	else
        // 	{
        // 		MSXML2::IXMLDOMProcessingInstructionPtr pPInst = m_pDoc->createProcessingInstruction("xml", "version='1.0'");
        // 		MSXML2::IXMLDOMAttributePtr pAttr = m_pDoc->createAttribute("encoding");
        // 		pAttr->nodeValue = "Windows-1251";
        // 		pPInst->attributes->setNamedItem(pAttr);
        // 		m_pDoc->appendChild(pPInst);
        // 	}
        // }
        // catch(_com_error e)
        // {
        // 	hr = e.Error();
        // 	return FALSE;
        // }

        return TRUE;
    }

    BOOL LoadXmlDocument(LPCTSTR szFileName)
    {
        // try
        // {
        // 	if(m_pDoc == NULL)
        // 		return FALSE;

        // 	m_pDoc->async = VARIANT_FALSE;
        // 	if(VARIANT_TRUE != m_pDoc->load(szFileName))
        // 		throw _com_error(E_FAIL);
        // 	m_strFilePath = szFileName;
        // }
        // catch(_com_error e)
        // {
        // 	return FALSE;
        // }

        return TRUE;
    }

    cross::CString GetFilePath()
    {
        return m_strFilePath;
    }

    BOOL SaveXmlDocument(LPCTSTR szFileName=NULL)
    {
        // try
        // {
        // 	if(m_pDoc == NULL)
        // 		return FALSE;

        // 	CString strPath = szFileName;
        // 	if(strPath.IsEmpty())
        // 		strPath = m_strFilePath;

        // 	if(S_OK != m_pDoc->save(_variant_t(strPath)))
        // 		throw _com_error(E_FAIL);

        // 	m_strFilePath = strPath;
        // }
        // catch(_com_error e)
        // {
        // 	//CString strError;
        // 	//strError.Format(_T("������ ��� ������ �����\n%s"), szFileName);
        // 	//AfxMessageBox(strError);
        // 	return FALSE;
        // }

        return TRUE;
    }

    void CloseXmlDocument()
    {
        // if(m_pDoc != NULL)
        // 	m_pDoc = NULL;
    }

    // MSXML2::IXMLDOMElementPtr AddNode(LPCTSTR szNodeName, MSXML2::IXMLDOMNodePtr pParentNode=NULL)
    // {
    // 	MSXML2::IXMLDOMElementPtr pNode = m_pDoc->createNode((short)(MSXML2::NODE_ELEMENT),szNodeName,"");
    // 	MSXML2::IXMLDOMElementPtr pResultNode = NULL;
    // 	if(pParentNode != NULL)
    // 		pResultNode = pParentNode->appendChild(pNode);
    // 	else
    // 		pResultNode = m_pDoc->appendChild(pNode);

    // 	return pResultNode;
    // }

    // MSXML2::IXMLDOMElementPtr AddTxtNode(LPCTSTR szNodeName, LPCTSTR szText, MSXML2::IXMLDOMNodePtr pParentNode=NULL)
    // {
    // 	MSXML2::IXMLDOMElementPtr pNode = m_pDoc->createNode((short)(MSXML2::NODE_ELEMENT),szNodeName,"");
    // 	pNode->put_text(_bstr_t(szText));
    // 	MSXML2::IXMLDOMElementPtr pResultNode = NULL;
    // 	if(pParentNode != NULL)
    // 		pResultNode = pParentNode->appendChild(pNode);
    // 	else
    // 		pResultNode = m_pDoc->appendChild(pNode);

    // 	return pResultNode;
    // }

    // void RemoveNode(MSXML2::IXMLDOMNodePtr pNode, MSXML2::IXMLDOMNodePtr pParentNode=NULL)
    // {
    // 	if(pParentNode == NULL)
    // 		pParentNode = GetRootNode();

    // 	pParentNode->removeChild(pNode);
    // }

    // CString GetNodeName(MSXML2::IXMLDOMNodePtr pNode)
    // {
    // 	CString str = pNode->nodeName;
    // 	return str;
    // }

    // MSXML2::IXMLDOMElementPtr GetRootNode()
    // {
    // 	return m_pDoc->documentElement;
    // }

    // void SetRootNode(MSXML2::IXMLDOMElementPtr pNode)
    // {
    // 	m_pDoc->documentElement = pNode;
    // }

    // MSXML2::IXMLDOMNodePtr FindNode(MSXML2::IXMLDOMNodePtr pParentNode, LPCTSTR szAttrName, LPCTSTR szAttrValue)
    // {
    // 	MSXML2::IXMLDOMNodePtr pNode = pParentNode->firstChild;
    // 	while(pNode != NULL)
    // 	{
    //		if(GetAttr(pNode, szAttrName).CompareNoCase(szAttrValue) == 0)
    // 			return pNode;

    // 		pNode = pNode->nextSibling;
    // 	}

    // 	return NULL;
    // }

    // bool FindNode(MSXML2::IXMLDOMNodePtr* nodePtrArray, MSXML2::IXMLDOMNodePtr pParentNode, LPCTSTR szAttrName, LPCTSTR szAttrValue,int& nCount, int nMax)
    // {
    // 	nCount=0;
    // 	MSXML2::IXMLDOMNodePtr pNode = pParentNode->firstChild;
    // 	while(pNode != NULL)
    // 	{
    // 		if(GetAttr(pNode, szAttrName).CompareNoCase(szAttrValue) == 0)
    // 		{
    // 			nodePtrArray[nCount]=pNode;
    // 			nCount++;
    // 			if (nCount==nMax) break;
    // 		}

    // 		pNode = pNode->nextSibling;
    // 	}
    // 	if (nCount>0)
    // 		return true;
    // 	else
    // 		return false;
    // }

    // MSXML2::IXMLDOMNodePtr FindNode(MSXML2::IXMLDOMNodePtr pParentNode, LPCTSTR szAttrName, LPCTSTR szAttrValue,
    // 	LPCTSTR szAttrName2, LPCTSTR szAttrValue2)
    // {
    // 	MSXML2::IXMLDOMNodePtr pNode = pParentNode->firstChild;
    // 	while(pNode != NULL)
    // 	{
    // 		if((GetAttr(pNode, szAttrName).CompareNoCase(szAttrValue) == 0)&&
    // 			(GetAttr(pNode, szAttrName2).CompareNoCase(szAttrValue2) == 0))
    // 			return pNode;

    // 		pNode = pNode->nextSibling;
    // 	}

    // 	return NULL;
    // }

    // MSXML2::IXMLDOMNodePtr GetFirstChildNode(MSXML2::IXMLDOMNodePtr pParentNode)
    // {
    // 	return pParentNode->firstChild;
    // }

    // MSXML2::IXMLDOMNodePtr GetNextSiblingNode(MSXML2::IXMLDOMNodePtr pNode)
    // {
    // 	return pNode->nextSibling;
    // }


    // MSXML2::IXMLDOMNodePtr FindNode(MSXML2::IXMLDOMNodePtr pParentNode, LPCTSTR szNodeName)
    // {
    // 	return pParentNode->selectSingleNode(szNodeName);
    // }

    // CString GetAttr(MSXML2::IXMLDOMNodePtr pNode, LPCTSTR szName)
    // {
    // 	MSXML2::IXMLDOMNamedNodeMapPtr Attributes = pNode->attributes;
    // 	if(Attributes == NULL)
    // 		return _T("");

    // 	MSXML2::IXMLDOMAttributePtr pAttr = Attributes->getNamedItem(szName);
    // 	if(pAttr == NULL)
    // 		return _T("");

    // 	return pAttr->value;
    // }

    // int GetIntAttr(MSXML2::IXMLDOMNodePtr pNode, LPCTSTR szName)
    // {
    // 	CString& str = GetAttr(pNode, szName);
    // 	if(str.IsEmpty())
    // 		return 0;
    // 	return atoi(str);
    // }

    // __int64 GetINT64Attr(MSXML2::IXMLDOMNodePtr pNode, LPCTSTR szName)
    // {
    // 	CString& str = GetAttr(pNode, szName);
    // 	if(str.IsEmpty())
    // 		return 0;

    // 	__int64 x;
    // 	sscanf_s(str, "%I64d", &x);
    // 	return x;
    // }

    // DWORD GetHexAttr(MSXML2::IXMLDOMNodePtr pNode, LPCTSTR szName)
    // {
    // 	CString& str = GetAttr(pNode, szName);
    // 	if(str.IsEmpty())
    // 		return 0;
    // 	char* stop = NULL;
    // 	return (DWORD)strtol(str, &stop, 16);
    // }

    // DWORD GetDWORDAttr(MSXML2::IXMLDOMNodePtr pNode, LPCTSTR szName)
    // {
    // 	return (DWORD)GetIntAttr(pNode, szName);
    // }

    // double GetDblAttr(MSXML2::IXMLDOMNodePtr pNode, LPCTSTR szName)
    // {
    // 	CString& str = GetAttr(pNode, szName);
    // 	if(str.IsEmpty())
    // 		return 0;

    // 	return atof(str);
    // }

    // bool IsAttrExist(MSXML2::IXMLDOMNodePtr pNode, LPCTSTR szName)
    // {
    // 	CString& str = GetAttr(pNode, szName);
    // 	if(str.IsEmpty())
    // 		return false;
    // 	return true;
    // }

    // void AddAttr(MSXML2::IXMLDOMNodePtr pNode, LPCTSTR szName, LPCTSTR szValue)
    // {
    // 	MSXML2::IXMLDOMAttributePtr pAttr = m_pDoc->createAttribute(szName);
    // 	pAttr->value = szValue;
    // 	pNode->attributes->setNamedItem(pAttr);
    // }

    // void AddDblAttr(MSXML2::IXMLDOMNodePtr pNode, LPCTSTR szName, double Value)
    // {
    // 	//char szBuffer[256];
    // 	//sprintf(szBuffer, "%.2f", Value);
    // 	//AddAttr(pNode, szName, szBuffer);
    // 	AddAttr(pNode, szName, DoubleToString(Value));
    // }

    // void AddIntAttr(MSXML2::IXMLDOMNodePtr pNode, LPCTSTR szName, int Value)
    // {
    // 	char szBuffer[256];
    // 	sprintf_s(szBuffer, "%d", Value);
    // 	AddAttr(pNode, szName, szBuffer);
    // }

    // void AddINT64Attr(MSXML2::IXMLDOMNodePtr pNode, LPCTSTR szName, __int64 Value)
    // {
    // 	char szBuffer[256];
    // 	sprintf_s(szBuffer, "%I64d", Value);
    // 	AddAttr(pNode, szName, szBuffer);
    // }

    // void AddDWORDAttr(MSXML2::IXMLDOMNodePtr pNode, LPCTSTR szName, DWORD Value)
    // {
    // 	AddIntAttr(pNode, szName, Value);
    // }

    cross::CString DoubleToString(double Val)
    {
        cross::CString strVal;

        int iVal = (int)Val;
        if(((double)iVal - Val) == 0)
            strVal.Format("%d",  std::to_string(iVal).c_str());
        else
        {
            cross::CString strFmt;
            double absVal = fabs(Val);
            if(absVal >= 1000)
                strFmt = "%.0f";
            else if(absVal >= 100)
                strFmt = "%.1f";
            else if(absVal >= 10)
                strFmt = "%.2f";
            else if(absVal >= 1)
                strFmt = "%.3f";
            else if(absVal <= 1E-5)
                strFmt = "%E";
            else if(absVal <= 1E-4)
                strFmt = "%.8f";
            else if(absVal <= 1E-3)
                strFmt = "%.7f";
            else if(absVal <= 1E-2)
                strFmt = "%.6f";
            else if(absVal <= 1E-1)
                strFmt = "%.5f";
            else
                strFmt = "%.4f";

            strVal.Format(strFmt,  std::to_string(Val).c_str());
        }
        //strVal.Format("%f", Val);
        return strVal;
    };

};

class CXMLOperations
{
public:


    bool FileExists(cross::CString strXMLFileName)
    {
        namespace fs = std::filesystem;

        std::filesystem::path path(strXMLFileName.getStdString());
        auto status = fs::status(path);

        // int Code = GetFileAttributes(strXMLFileName);
        // return ((Code!=INVALID_FILE_ATTRIBUTES) && ((FILE_ATTRIBUTE_DIRECTORY & Code) == 0));
        // return fs::exists(strXMLFileName);

        if(status.type()== fs::file_type::regular)
            return true;

        return false;
    }

    void ReadAllPointsInXMLFilesAndSaveInArrow(cross::CString strColumnTxt[MAX_POINTS_IN_ARRAY][3], int& nCountOfPoints, std::string strSpecialPanoramDir)
    {
        // 	//CString strXMLFileDirectory=GetBasePath();
        // 	//strXMLFileDirectory+="Panorams\\";
        // 	CString strDir(strSpecialPanoramDir.c_str());
        //   //   if (strDir!=CString(""))
        // 		// strXMLFileDirectory=strDir;
        // 	CString strMask="*.xml";
        // //	CString strFindString=strXMLFileDirectory+strMask;
        // 	const UINT MAXmlFiles=22000;
        // 	CString strArrayXMLNames[MAXmlFiles];
        // 	HANDLE hFind;
        //     WIN32_FIND_DATA find_data;
        // //	hFind=FindFirstFile(strFindString,&find_data);
        // 	int nCount=0;
        // 	if (hFind != INVALID_HANDLE_VALUE)
        // 	{
        //         do
        //         {
        //             if(!(find_data.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY))
        // 			{
        //                 strArrayXMLNames[nCount]=find_data.cFileName;
        // 				nCount++;
        // 				if (nCount==MAXmlFiles) break;
        // 			}
        //         }
        // 		while(FindNextFile(hFind,&find_data));
        // 	}
        // //	CString strFile=strXMLFileDirectory+"points.csv";
        // 	//FILE * F;
        // 	//fopen_s ( &F, strFile, "wt" );
        // 	//if (F==NULL) return;
        // 	nCountOfPoints=0;
        // 	for (int k=0;k<nCount;k++)
        // 	{
        // 		CString strXMLFileName=strXMLFileDirectory+strArrayXMLNames[k];
        // 		CString strShortXMLName=strArrayXMLNames[k];
        // 		if (FileExists(strXMLFileName))
        // 		{
        // 			CXmlHelperPanoram xmlDoc;
        // 			if(!xmlDoc.CreateXmlDocument(strXMLFileName)) return;
        // 			MSXML2::IXMLDOMNodePtr pTable = xmlDoc.GetRootNode();
        // 			MSXML2::IXMLDOMNodePtr nodePtrArray[MAX_POINTS_IN_ONE_FILE];
        // 			int nCount2=0;
        // 			if (xmlDoc.FindNode(nodePtrArray,pTable,"point", "1",nCount2,MAX_POINTS_IN_ONE_FILE))
        // 			{
        // 				for (int i=0;i<nCount2;i++)
        // 				{
        // 					MSXML2::IXMLDOMNodePtr find=nodePtrArray[i];
        // 					if (find!=NULL)
        // 					{
        // 						CString strType = xmlDoc.GetAttr(find, "type");
        // 						CString strName = xmlDoc.GetAttr(find, "name");
        // 						strColumnTxt[nCountOfPoints][0]=strType;
        // 						strColumnTxt[nCountOfPoints][1]=strName;
        // 						strColumnTxt[nCountOfPoints][2]=strShortXMLName;
        // 						nCountOfPoints++;
        // 						if (nCountOfPoints==MAX_POINTS_IN_ARRAY)
        // 						{
        // 							xmlDoc.CloseXmlDocument();
        // 							return;
        // 						}
        // 					}
        // 				}
        // 			}
        // 			xmlDoc.CloseXmlDocument();
        // 		}
        // 	}
        //fclose ( F );
    }

};
#endif
