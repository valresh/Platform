<?xml version="1.0"?>
<xsl:stylesheet version="1.0" xmlns:xsl="http://www.w3.org/1999/XSL/Transform">
    <xsl:template match="/">
    <xsl:if test="/Error">
        <div id = "divSysAlmHlpErr" class="almHlpErr"><xsl:value-of select="/Error"/></div>
    </xsl:if>
    <xsl:if test="not(/Error)">
            <div class="almHlpArea">
               <div id = "divSysAlmHlpCauseCell" class="almHlpItem" >
                   <div id = "divSysAlmHlpCauseHead" class = "almHlpItemHead">Cause</div>
                   <div id = "divSysAlmHlpCauseContent" class = "sysAlmHlpContent">
                       <xsl:value-of select="//Cause"/> 
                   </div>
               </div> 
              <div id = "divSysAlmHlpActionCell" class="almHlpItem">
                  <div id = "divSysAlmHlpActionHeader" class = "almHlpItemHead">Action</div>
                  <div id = "divSysAlmHlpActionContent" class = "sysAlmHlpContent">
                      <xsl:value-of select="//CorrectiveAction"/> 
                  </div>
              </div>			   
               <div id = "divSysAlmHlpConseqCell" class="almHlpItem">
                   <div id = "divSysAlmHlpConseqHead" class = "almHlpItemHead">Consequence</div>
                   <div id = "divSysAlmHlpConseqContent" class = "sysAlmHlpContent">
                       <xsl:value-of select="//Consequence"/>
                   </div>
               </div>
              <div id = "divSysAlmHlpTimeCell" class="almHlpItem">
                  <div id = "divSysAlmHlpTimeHeader" class = "almHlpItemHead">Time To Respond</div>
                  <div id = "divSysAlmHlpTimeContent" class = "sysAlmHlpContent">
                      <xsl:value-of select="//TimeToRespond"/>
                  </div>
              </div>
            </div>
    </xsl:if>
    </xsl:template>
</xsl:stylesheet>
