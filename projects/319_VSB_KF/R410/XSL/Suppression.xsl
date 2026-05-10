<xsl:stylesheet version="1.0" xmlns:xsl="http://www.w3.org/1999/XSL/Transform">

  <xsl:template match="/">
    <!--Create the column headers-->
    <DIV class="detailsSuppressionHeaderRow">
      <SPAN class="detailsSuppressionHeader">Source</SPAN>
      <SPAN class="detailsSuppressionHeader">Block</SPAN>
      <SPAN class="detailsSuppressionHeader">Condition</SPAN>
      <SPAN class="detailsSuppressionHeader">Group</SPAN>
    </DIV>
    <!--Iterate over each relationship record and create a new row-->
    <xsl:for-each select="//relationship">
      <!--Sort the relationships so they are in alphabetical order by Source, then Block, then Condition-->
      <xsl:sort data-type="text" order="ascending" select="@Source"/>
      <xsl:sort data-type="text" order="ascending" select="@Block"/>
      <xsl:sort data-type="text" order="ascending" select="@Condition"/>
      <!--Create a new row-->
      <DIV class="detailsSuppressionListRow">
        <!--Create the Source cell-->
        <SPAN class="detailsSuppressionValue">
          <!--Set the value of the Source tooltip-->
          <xsl:attribute name="title">
            <xsl:value-of select="@Source"/>
          </xsl:attribute>
          <!--Set the value of the Source cell-->
          <xsl:value-of select="@Source"/>
        </SPAN>
        <!--Create the Block cell-->
        <SPAN class="detailsSuppressionValue">
          <!--Set the value of the Block tooltip-->
          <xsl:attribute name="title">
            <xsl:value-of select="@Block"/>
          </xsl:attribute>
          <!--Set the value of the Block cell-->
          <xsl:value-of select="@Block"/>
        </SPAN>
        <!--Create the Condition cell-->
        <SPAN class="detailsSuppressionValue">
          <!--Set the value of the Condition tooltip-->  
          <xsl:attribute name="title">
            <xsl:value-of select="@Condition"/>
          </xsl:attribute>
          <!--Set the value of the Condition cell-->
          <xsl:value-of select="@Condition"/>
        </SPAN>
        <!--Create the Group cell-->
        <SPAN class="detailsSuppressionValue">
          <!--Set the value of the Group tooltip-->
          <xsl:attribute name="title">
            <xsl:value-of select="@Group"/>
          </xsl:attribute>
          <!--Set the value of the Group cell-->
          <xsl:value-of select="@Group"/>
        </SPAN>
      </DIV>
    </xsl:for-each>
  </xsl:template>
</xsl:stylesheet>
