<?xml version="1.0"?>
<xsl:stylesheet version="1.0" xmlns:xsl="http://www.w3.org/1999/XSL/Transform">
  <xsl:template match="/">
    <xsl:param name="NumOfFieldShowOnDP">
      <xsl:value-of select="count(//fields/field[@ShowOnDetailPane='true' and not(normalize-space(.)='(Null)')])"/>
    </xsl:param>
    <TABLE style="table-layout:fixed">
      <TR valign="top">
        <TD >
          <TABLE style="table-layout: fixed; font-size: 9pt">
            <col class="DTLPANE_FIELD"></col>
            <col class="DTLPANE_VALUE"></col>
            <col class="DTLPANE_FIELD"></col>
            <col class="DTLPANE_VALUE"></col>
            <xsl:for-each select="//fields/field[@ShowOnDetailPane='true' and not(normalize-space(.)='(Null)')]">
              <xsl:if test="position() &lt;= ceiling($NumOfFieldShowOnDP div 2)">
                <TR>
                  <TD>
                    <xsl:attribute name="title">
                      <xsl:value-of select="@description"/>
                    </xsl:attribute>  
                    <xsl:value-of select="@i18nname"/>:
                  </TD>
                  <TD>
                    <xsl:attribute name="title">
                      <xsl:value-of select="."/>
                    </xsl:attribute>
                    <b><xsl:value-of select="."/></b>
                  </TD>
                  <xsl:if test="position() &lt;= floor($NumOfFieldShowOnDP div 2)">
                    <TD>
                      <xsl:attribute name="title">
                        <xsl:value-of select="following-sibling::field[@ShowOnDetailPane='true' and not(normalize-space(.)='(Null)')][ceiling($NumOfFieldShowOnDP div 2)]/@description"/>
                      </xsl:attribute>
                      <xsl:value-of select="following-sibling::field[@ShowOnDetailPane='true' and not(normalize-space(.)='(Null)')][ceiling($NumOfFieldShowOnDP div 2)]/@i18nname"/>:
                    </TD>
                    <TD>
                      <xsl:attribute name="title">
                        <xsl:value-of select="following-sibling::field[@ShowOnDetailPane='true' and not(normalize-space(.)='(Null)')][ceiling($NumOfFieldShowOnDP div 2)]"/>
                      </xsl:attribute>
                      <b><xsl:value-of select="following-sibling::field[@ShowOnDetailPane='true' and not(normalize-space(.)='(Null)')][ceiling($NumOfFieldShowOnDP div 2)]"/></b>
                    </TD>
                  </xsl:if>
                </TR>
              </xsl:if>
            </xsl:for-each>
          </TABLE>
        </TD>
        <TD align="right" style="width: 140px;">
          <DIV ID="txtNoPhoto" style="display: none; width:135px; height:150px; overflow: hidden; border: 1px solid; text-align: center; padding: 10px; padding-top:40px; background-color: white;">Photo not available</DIV>
          <IFRAME ID="imgPhoto" onerror="javascript: this.style.display='none'; this.previousSibling.style.display='inline'; LogErrorMessage('AccessEvents.xsl', 'Error loading cardholder image at ' + this.data);" ondragstart="javascript: window.event.returnValue = false;" style="width: 135px; height: 150px;" type="text/html"><xsl:attribute name="SRC"><xsl:value-of select="fields/field[@name='CardHolderImagePath']"/></xsl:attribute></IFRAME>
        </TD>
      </TR>
    </TABLE>
  </xsl:template>
</xsl:stylesheet>