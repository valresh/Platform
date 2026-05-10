<xsl:stylesheet version="1.0" xmlns:xsl="http://www.w3.org/1999/XSL/Transform"
    xmlns:msxsl="urn:schemas-microsoft-com:xslt"
    xmlns:user="urn:custom-scripts"
    xmlns:userjs="urn:custom-javascript"
    xmlns:rs="urn:schemas-microsoft-com:rowset" 
    xmlns:z="#RowsetSchema" >

  <msxsl:script language="JScript" implements-prefix="userjs">
    <![CDATA[
     function FiletimeToVBtime(ftime)
     {
        return ftime.item(0).text / 864000000000 - 109205;
     }
    ]]>
  </msxsl:script>

  <msxsl:script language="VBScript" implements-prefix="user">
    <![CDATA[
     Function VBtimeToString(vbtime)
        VBtimeToString = CStr(CDate(CDbl(vbtime)))
     End Function
     ]]>
  </msxsl:script>

  <xsl:template match="/">
    <xsl:for-each select="//rs:data/z:row">
	<xsl:sort data-type="number" order="descending" select="@Time"/>
      <DIV style="font-family: 'Helvetica'; font-size: 9pt; font-weight: bold; position: relative; padding-top: 8px;">
        <SPAN style="overflow: hidden; text-overflow: ellipsis; position: relative; width: 150px; white-space: nowrap;">
        <!-- VBScript can not support 64bit integers so we send the text to a JScript function for the numeric manipulation work first -->
          <xsl:value-of select="user:VBtimeToString(userjs:FiletimeToVBtime(@Time))"/>
        </SPAN>
        <SPAN style="padding-left:5px;">
          <xsl:value-of select="@Actor"/>
        </SPAN>
      </DIV>
      <DIV style="font-family: 'Helvetica'; font-size: 9pt;">
        <xsl:value-of select="@Comment"/>
      </DIV>
    </xsl:for-each>
  </xsl:template>
</xsl:stylesheet>
