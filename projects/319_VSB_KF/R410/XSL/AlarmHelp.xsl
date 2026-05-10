<?xml version="1.0"?>
<xsl:stylesheet version="1.0" xmlns:xsl="http://www.w3.org/1999/XSL/Transform">
	<xsl:template match="/">
	<xsl:choose>
	<xsl:when test="/Error">
		<div class="almHlpErr"><xsl:value-of select="/Error"/></div>
	</xsl:when>
	<xsl:when test="/VariableTable/Variable/Error">
		<div class="almHlpErr"><xsl:value-of select="/VariableTable/Variable/Error"/></div>
	</xsl:when>
	<xsl:when test="VariableTable = ''">
		<div class="almHlpErr">ACM not configured</div>
	</xsl:when>
	<xsl:otherwise>
		<div class="amlHlpHeader">
			<xsl:choose>
				<xsl:when test="contains(//AlarmTag/@Name,'.')">
					<xsl:value-of select="substring-before(//AlarmTag/@Name,'.')"/>
				</xsl:when>
				<xsl:otherwise>
					<xsl:value-of select="//AlarmTag/@Name"/>
				</xsl:otherwise>
			</xsl:choose>	
		</div>
		<div class="almHlpArea">
			<div class="almHlpSmallItem">
				<div class="almHlpSmallItemLabel">Block:</div>
				<div class="almHlpSmallItemValue"><xsl:value-of select="substring-after(//AlarmTag/@Name,'.')"/></div>
			</div>
			<div class="almHlpSmallItem">
				<div class="almHlpSmallItemLabel">Boundary:</div>
				<div class="almHlpSmallItemValue"><xsl:value-of select="//Limit/@LimitName"/></div>
			</div>
			<div class="almHlpSmallItem">
				<div class="almHlpSmallItemLabel">Consequence:</div>
				<div class="almHlpSmallItemValue"><xsl:value-of select="//Consequence"/></div>
			</div>
			<div class="almHlpSmallItem">
				<div class="almHlpSmallItemLabel">Condition:</div>
				<div class="almHlpSmallItemValue"><xsl:value-of select="//Limit/@AlarmCondition"/></div>
			</div>
			<div class="almHlpSmallItem">
				<div class="almHlpSmallItemLabel">Time To Respond:</div>
				<div class="almHlpSmallItemValue"><xsl:value-of select="//TimeToRespond"/></div>
			</div>
			<div class="almHlpSmallItem">
				<div class="almHlpSmallItemLabel">Alarm Limit:</div>
				<div class="almHlpSmallItemValue"><xsl:value-of select="//Value"/></div>
			</div>
			<div class="almHlpSmallItem">
				<div class="almHlpSmallItemLabel">Shutdown Pre Alarm:</div>
				<div class="almHlpSmallItemValue"><xsl:value-of select="//SDPreAlarm"/></div>
			</div>
			<div class="almHlpSmallItem">
				<div class="almHlpSmallItemLabel">Mode:</div>
				<div class="almHlpSmallItemValue"><xsl:value-of select="//Mode/@Name"/></div>
			</div>
			<xsl:for-each select="//LimitNote">
				<div class="almHlpItem">
					<div class="almHlpItemHead"><xsl:value-of select="@Label"/>:</div>
					<div class="almHlpItemContent"><xsl:value-of select="."/></div>
				</div>
			</xsl:for-each>
		</div>

		<div class ="almHlpMoreDetails"> 
			<a>
				<xsl:attribute name="href">
					<xsl:value-of select="//ACMWeb/@OperDetailURL"/>
				</xsl:attribute>
				More Details
			</a>
		</div>
	</xsl:otherwise>
	</xsl:choose>
	</xsl:template>
</xsl:stylesheet>