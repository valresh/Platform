<?xml version="1.0"?>
<xsl:stylesheet version="1.0" xmlns:xsl="http://www.w3.org/1999/XSL/Transform">
  <xsl:template match="/">
    <xsl:param name="NumOfFieldShowOnDP">
      <xsl:value-of select="count(//fields/field[@ShowOnDetailPane='true' and not(normalize-space(.)='(Null)')])"/>
    </xsl:param>

    <TABLE style="table-layout: fixed; width: 100%;">
      <col class="DTLPANE_FIELD"></col>
      <col class="DTLPANE_VALUE"></col>
      <col class="DTLPANE_FIELD"></col>
      <col class="DTLPANE_VALUE"></col>
      <xsl:for-each select="//fields/field[@ShowOnDetailPane='true' and not(normalize-space(.)='(Null)')]">
        <xsl:if test="position() &lt;= ceiling($NumOfFieldShowOnDP div 2)">
         <TR>
         <TD style="width: 50%">
         <TABLE style="font-size: 9pt; word-wrap: break-word; width: 100%;">
          <TR>
            <TD class="detailsGeneralField">
              <xsl:attribute name="title">
                <xsl:value-of select="@description"/>
              </xsl:attribute>  
              <xsl:value-of select="@i18nname"/>:
            </TD>
            <TD class="detailsGeneralValue">
              <xsl:attribute name="title">
                <xsl:value-of select="."/>
              </xsl:attribute>
              <b><xsl:value-of select="."/></b>
            </TD>
          </TR>
         </TABLE>
         </TD>
         <TD style="width: 50%">
         <TABLE style="font-size: 9pt; word-wrap: break-word; width: 100%;">
          <TR>
            <xsl:if test="position() &lt;= floor($NumOfFieldShowOnDP div 2)">
            <TD class="detailsGeneralField">
              <xsl:attribute name="title">
                <xsl:value-of select="following-sibling::field[@ShowOnDetailPane='true' and not(normalize-space(.)='(Null)')][ceiling($NumOfFieldShowOnDP div 2)]/@description"/>
              </xsl:attribute>
              <xsl:value-of select="following-sibling::field[@ShowOnDetailPane='true' and not(normalize-space(.)='(Null)')][ceiling($NumOfFieldShowOnDP div 2)]/@i18nname"/>:
            </TD>
            <TD class="detailsGeneralValue">
              <xsl:attribute name="title">
                <xsl:value-of select="following-sibling::field[@ShowOnDetailPane='true' and not(normalize-space(.)='(Null)')][ceiling($NumOfFieldShowOnDP div 2)]"/>
              </xsl:attribute>
              <b><xsl:value-of select="following-sibling::field[@ShowOnDetailPane='true' and not(normalize-space(.)='(Null)')][ceiling($NumOfFieldShowOnDP div 2)]"/></b>
            </TD>
            </xsl:if>
          </TR>
         </TABLE>
         </TD>
         </TR>
        </xsl:if>
      </xsl:for-each>
    </TABLE>
  </xsl:template>  
</xsl:stylesheet>

