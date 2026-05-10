<?xml version="1.0" encoding="UTF-8"?>
<xsl:stylesheet version="1.0" xmlns:xsl="http://www.w3.org/1999/XSL/Transform">
	<xsl:output method="html" version="1.0" encoding="UTF-8" indent="yes"/>

	<xsl:param name="touchPadding">
		<xsl:text>5</xsl:text>
	</xsl:param>

	<xsl:template match="/">
		<HTML>
		<HEAD>
		<TITLE>Popup Window</TITLE>
		</HEAD>	
		<xsl:apply-templates select="menu/script"/>
		<xsl:apply-templates select="menu/toolbar/script"/>
		<xsl:apply-templates select="menu/menuitem/script[@type='onclick']"/>	
		<xsl:apply-templates select="menu/menuitem/script[@type='onpage']"/>
		<xsl:apply-templates select="menu/toolbar/menuitem/script[@type='onclick']"/>
		<xsl:apply-templates select="menu/toolbar/menuitem/script[@type='onpage']"/>

		<BODY scroll="no" style="border:0px">
		<STYLE>
		.menuItem <xsl:value-of select="menu/@menuItemStyle"/>
		.highlightItem <xsl:value-of select="menu/@highlightItemStyle"/>
		.tooltip {
			border-top: 1pt outset grey;
			border-left: 1pt outset grey;
			border-right: 1pt outset grey;
			border-bottom: 0pt;
			text-align: left;
			background-color: #FFFFCC;
			font-family:sans-serif;
			font-size:9pt;
			padding:6pt,4pt,6pt,4pt;
		}

		.touchItem {
		<xsl:if test="menu/@touchpanel">
			padding: <xsl:value-of select="$touchPadding"/>pt;
		</xsl:if>
		}
		</STYLE>
		<TABLE id="menu1" style="position:absolute; left:1px; top:1px; border:1pt outset grey;" BGCOLOR="menu" CELLPADDING="0" CELLSPACING="0">
			<xsl:if test="menu/@width">
				<xsl:if test="menu/@touchpanel">
					<xsl:attribute name="WIDTH"><xsl:value-of select="menu/@width+10*$touchPadding"/></xsl:attribute>
				</xsl:if>
				<xsl:if test="not(menu/@touchpanel)">
					<xsl:attribute name="WIDTH"><xsl:value-of select="menu/@width"/></xsl:attribute>
				</xsl:if>
			</xsl:if>
			<xsl:if test="menu/@height">
				<xsl:attribute name="HEIGHT"><xsl:value-of select="menu/@height"/></xsl:attribute>
			</xsl:if>
			<xsl:apply-templates select="menu/menuitem | menu/html | menu/separator | menu/menuitemtt"/>
			<xsl:apply-templates select="menu/toolbar"/>
		</TABLE>
		</BODY>
		</HTML>
	</xsl:template>
	
	<xsl:template match="menuitemtt">
		<xsl:if test="../../menu/@touchpanel">
			<DIV class="tooltip">
				<xsl:text disable-output-escaping="yes">&amp;nbsp;</xsl:text><xsl:apply-templates select="text"/>
			</DIV>
		</xsl:if>
	</xsl:template>

	<xsl:template match="toolbar">
			<xsl:attribute name="id"><xsl:value-of select="./@id"/></xsl:attribute>
				<xsl:apply-templates select="menuitem | html | separator"/>
	</xsl:template>
	
	<xsl:template match="toolbaritem">
		<TD class='menuItem touchItem'>
			<xsl:attribute name="id"><xsl:value-of select="./@id"/></xsl:attribute>
			<xsl:attribute name="onmouseout">this.className='menuItem touchItem'; this.className='menuItem touchItem'</xsl:attribute>
			<xsl:attribute name="onmouseover">this.className='highlightItem touchItem'; if (!this.disabled) this.setActive(); this.className='highlightItem touchItem'; if (!this.disabled) this.setActive()</xsl:attribute>
			<xsl:apply-templates select="image"/>
		</TD>
	</xsl:template>
	
	<xsl:template match="menuitem">
		<TR class="row">
			<xsl:attribute name="id"><xsl:value-of select="./@id"/></xsl:attribute>
			<xsl:attribute name="onmouseout">this.children[0].className='menuItem touchItem'; this.children[1].className='menuItem touchItem'</xsl:attribute>
			<xsl:attribute name="onmouseover">this.children[0].className='highlightItem touchItem'; if (!this.disabled) this.children[0].setActive(); this.children[1].className='highlightItem touchItem'; if (!this.disabled) this.children[1].setActive()</xsl:attribute>
			<TD class='menuItem touchItem' id="image">
				<xsl:apply-templates select="image"/>
			</TD>
			
			<TD class="menuItem touchItem" id="text">
				<xsl:text disable-output-escaping="yes">&amp;nbsp;</xsl:text><xsl:value-of disable-output-escaping="yes" select="text"/>
			</TD>
		</TR>
	</xsl:template>
	
	<xsl:template match="html">
		<TR>
			<TD colspan="2">
				<xsl:value-of disable-output-escaping="yes" select="."/>
			</TD>
		</TR>
	</xsl:template>

	<xsl:template match="separator">
		<TR>
			<TD colspan="2">
				<xsl:attribute name="id"><xsl:value-of select="./@id"/></xsl:attribute>
				<hr></hr>		
			</TD>
		</TR>
	</xsl:template>	

	<xsl:template match="menuitem/script[@type='onclick']">
		<script>
			<xsl:attribute name="event">onclick</xsl:attribute>
			<xsl:attribute name="for"><xsl:value-of select="../@id"></xsl:value-of></xsl:attribute>
			<xsl:value-of disable-output-escaping="yes" select="."/>return true
		</script>		
	</xsl:template>

	<xsl:template match="menuitem/script[@type='onpage']">
		<script>
			<xsl:attribute name="event">onpage</xsl:attribute>
			<xsl:attribute name="for"><xsl:value-of select="../@id"></xsl:value-of></xsl:attribute>
			<xsl:value-of disable-output-escaping="yes" select="."/>
		</script>		
	</xsl:template>

	<xsl:template match="toolbar/toolbaritem/script[@type='onclick']">
		<script>
			<xsl:attribute name="event">onclick</xsl:attribute>
			<xsl:attribute name="for"><xsl:value-of select="../@id"></xsl:value-of></xsl:attribute>
			<xsl:value-of disable-output-escaping="yes" select="."/>return true
		</script>		
	</xsl:template>

	<xsl:template match="toolbar/toolbaritem/script[@type='onpage']">
		<script>
			<xsl:attribute name="event">onpage</xsl:attribute>
			<xsl:attribute name="for"><xsl:value-of select="../@id"></xsl:value-of></xsl:attribute>
			<xsl:value-of disable-output-escaping="yes" select="."/>
		</script>		
	</xsl:template>

	<xsl:template match="script">
		<script>
			<xsl:value-of disable-output-escaping="yes" select="."/>
		</script>		
	</xsl:template>	
	
	<xsl:template match="image">
		<img ALIGN="absbottom">
			<xsl:attribute name="src">
				<xsl:value-of disable-output-escaping="yes" select="."/>
			</xsl:attribute>
		</img>
	</xsl:template>

	<!-- Replace '\n', '\r' or '\r\n' with <br/> -->

	<xsl:param name="br">
		<xsl:text>&#10;</xsl:text>
	</xsl:param>
		
	<xsl:template match="text" name="insertBreaks">		
		<xsl:param name="pText" select="."/>

		<xsl:choose>
			<xsl:when test="not(contains($pText, $br))">
				<xsl:value-of disable-output-escaping="yes" select="$pText"/>
			</xsl:when>
			<xsl:otherwise>
				<xsl:value-of disable-output-escaping="yes" select="substring-before($pText, $br)"/>
				<br/><xsl:text disable-output-escaping="yes">&amp;nbsp;</xsl:text>
				<xsl:call-template name="insertBreaks">
					<xsl:with-param name="pText" select="substring-after($pText, $br)"/>
				</xsl:call-template>
			</xsl:otherwise>
		</xsl:choose>				
	</xsl:template>
</xsl:stylesheet>
