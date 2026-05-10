'/************************************************************************/
'/*****     COPYRIGHT 2004           HONEYWELL LIMITED AUSTRALIA      ****/
'/************************************************************************/
' PntBrowserScrplt.vbs
' $Revision: 2 $
Function JsTrim(str)
	JsTrim = Trim(str)
End Function


Sub TransformTagSelectionChangeXML(DetailsXML)

	If (DetailsXML <> "") Then
		Dim xmlDoc
		Set xmlDoc = CreateObject("Msxml2.DOMDocument.6.0")
		xmlDoc.async = false
		xmlDoc.loadXML(DetailsXML)
		xmlDoc.setProperty "SelectionLanguage", "XPath"

        
        Dim g_sLastCat

		g_sLastCat = CStr(xmlDoc.selectSingleNode("fields/field[@name='Category']").text)

        msgbox(g_sLastCat)
	End If

End Sub

Sub vbHandle_OnTagSelectionChange(DetailsXML, bSelectionChange, bSelectedRowDataChange)
	If (bSelectionChange or bSelectedRowDataChange) Then
		g_TagSelectionChangeXML = DetailsXML
		If (g_TagSelectionChangeXML <> "") Then
			TransformTagSelectionChangeXML(g_TagSelectionChangeXML)
		End If
	End If
End Sub

Sub PopulateDetailsPane(DetailsXML, bSelectionChange, bSelectedRowDataChange)
	If (bSelectionChange or bSelectedRowDataChange) Then
		g_DefaultDetailsPaneHTML = ""
		g_DetailsPaneXML = DetailsXML
		g_EventCategory = 0
	End If
	If (bSelectionChange) Then
		g_ExistCommentsHTML = ""
	End If
	If (DetailsPane.style.display <> "none" and (bSelectedRowDataChange or bSelectionChange)) Then
		If (g_DetailsPaneXML <> "") Then
			TransformDetailsPaneHTML(g_DetailsPaneXML)
			DefaultData.innerHTML = g_DefaultDetailsPaneHTML
			DefaultData.style.display = "inline"
			divDetailsSidePanel.style.visibility = "visible"
		End If
	End If
	If (g_SummaryType = SUMMARY_EVENT or g_SummaryType = SUMMARY_ALARM or g_SummaryType = SUMMARY_SYSTEM_ALARM or g_SummaryType = SUMMARY_ALERT) Then
		If (DetailsPane.style.display <> "none" and CommentsData.style.display <> "none" and bSelectionChange) Then
			RequestComments()
		End If
	End If
	If (g_SummaryType = SUMMARY_ALARM or g_SummaryType = SUMMARY_SYSTEM_ALARM or g_SummaryType = SUMMARY_ALERT) Then
		CheckLiveValueUpdates()
	End If
End Sub

'/************************************************************************/
'/*****     COPYRIGHT 2004           HONEYWELL LIMITED AUSTRALIA      ****/
'/************************************************************************/


