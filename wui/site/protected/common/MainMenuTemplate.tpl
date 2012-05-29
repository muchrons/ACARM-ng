<div>
<table cellspacing="0" cellpadding="0" border="0">
  <tr>
    <td align="right">
      <com:THyperLink  NavigateUrl="<%=$this->Service->constructUrl('Home') %>"> 
        <com:TImage ImageUrl="pics/buttons/buttHomeD.png"
                    Attributes.onMouseOver="$(this).src='pics/buttons/buttHomeU.png'"
                    Attributes.onMouseOut="$(this).src='pics/buttons/buttHomeD.png'"/> 
      </com:THyperLink>
    <br/>
    </td>
  </tr>
  <tr>
    <td>
      <com:TImage ImageUrl="pics/buttons/buttDBD.png"/>
    </td>
  </tr> 
  <tr>
    <td align="right">
      <com:THyperLink  NavigateUrl="<%=$this->Service->constructUrl('Alerts') %>">
        <com:TImage ImageUrl="pics/buttons/buttDBSourcreAlertsD.png"
                    Attributes.onMouseOver="$(this).src='pics/buttons/buttDBSourcreAlertsU.png'"
                    Attributes.onMouseOut="$(this).src='pics/buttons/buttDBSourcreAlertsD.png'"/>
      </com:THyperLink>
    </td>
  </tr>
  <tr>
    <td align="right">
      <com:THyperLink  NavigateUrl="<%=$this->Service->constructUrl('MetaAlerts') %>">
        <com:TImage ImageUrl="pics/buttons/buttDBMetaAlertsD.png"
                    Attributes.onMouseOver="$(this).src='pics/buttons/buttDBMetaAlertsU.png'"
                    Attributes.onMouseOut="$(this).src='pics/buttons/buttDBMetaAlertsD.png'"/>
      </com:THyperLink>                
    </td>
  </tr>
  <tr>
    <td align="right">
      <com:THyperLink  NavigateUrl="<%=$this->Service->constructUrl('MetaAlertsRoot') %>">
        <com:TImage ImageUrl="pics/buttons/buttDBRootMetaAlertsD.png"
                    Attributes.onMouseOver="$(this).src='pics/buttons/buttDBRootMetaAlertsU.png'"
                    Attributes.onMouseOut="$(this).src='pics/buttons/buttDBRootMetaAlertsD.png'"/>
      </com:THyperLink>                
    </td>
  </tr>
  <tr>
    <td align="right">
      <com:THyperLink  NavigateUrl="<%=$this->Service->constructUrl('Analyzers') %>">
        <com:TImage ImageUrl="pics/buttons/buttDBAnalyzersD.png"
                    Attributes.onMouseOver="$(this).src='pics/buttons/buttDBAnalyzersU.png'"
                    Attributes.onMouseOut="$(this).src='pics/buttons/buttDBAnalyzersD.png'"/> 
      </com:THyperLink>                
    </td>
  </tr> 
  <tr>
    <td>
      <com:TImage ImageUrl="pics/buttons/buttDMD.png"/>
    </td>
  </tr> 
  <tr>
    <td align="right">
      <com:THyperLink  NavigateUrl="<%=$this->Service->constructUrl('DataMiningAlertTypes') %>">
        <com:TImage ImageUrl="pics/buttons/buttDMAlertTypesD.png"
                    Attributes.onMouseOver="$(this).src='pics/buttons/buttDMAlertTypesU.png'"
                    Attributes.onMouseOut="$(this).src='pics/buttons/buttDMAlertTypesD.png'"/> 
      </com:THyperLink>
    </td>
  </tr>
  <tr>
    <td align="right">
      <com:THyperLink  NavigateUrl="<%=$this->Service->constructUrl('DataMiningSeverity') %>">
        <com:TImage ImageUrl="pics/buttons/buttDMSeverityD.png"
                    Attributes.onMouseOver="$(this).src='pics/buttons/buttDMSeverityU.png'"
                    Attributes.onMouseOut="$(this).src='pics/buttons/buttDMSeverityD.png'"/> 
      </com:THyperLink>                
    </td>
  </tr>
  <tr>
    <td align="right">
      <com:THyperLink  NavigateUrl="<%=$this->Service->constructUrl('DataMiningTimeSeries',array('type'=>'Alert')) %>">
        <com:TImage ImageUrl="pics/buttons/buttDMAlertCountD.png"
                    Attributes.onMouseOver="$(this).src='pics/buttons/buttDMAlertCountU.png'"
                    Attributes.onMouseOut="$(this).src='pics/buttons/buttDMAlertCountD.png'"/> 
      </com:THyperLink>
    </td>
  </tr>
  <tr>
    <td align="right">
      <com:THyperLink  NavigateUrl=<%=$this->Service->constructUrl('DataMiningTimeSeries',array('type'=>'MetaAlert')) %>>
        <com:TImage ImageUrl="pics/buttons/buttDMMetaAlertCountD.png"
                    Attributes.onMouseOver="$(this).src='pics/buttons/buttDMMetaAlertCountU.png'"
                    Attributes.onMouseOut="$(this).src='pics/buttons/buttDMMetaAlertCountD.png'"/> 
      </com:THyperLink>
    </td>
  </tr>
  <tr>
  </tr>
  <tr>
    <td align="right">
      <com:THyperLink  NavigateUrl="<%=$this->Service->constructUrl('DataMiningHeatMap') %>">
        <com:TImage ImageUrl="pics/buttons/buttHeatmapD.png"
                    Attributes.onMouseOver="$(this).src='pics/buttons/buttHeatmapU.png'"
                    Attributes.onMouseOut="$(this).src='pics/buttons/buttHeatmapD.png'"/> 
      </com:THyperLink>
    </td>
  </tr>
  <tr>
    <td>
      <com:TImage ImageUrl="pics/buttons/buttMiscellaneousD.png"/>
    </td>
  </tr> 
  <tr>
    <td align="right">
      <com:THyperLink  NavigateUrl="<%=$this->Service->constructUrl('WUILogs') %>">
        <com:TImage ImageUrl="pics/buttons/buttWUILogsD.png"
                    Attributes.onMouseOver="$(this).src='pics/buttons/buttWUILogsU.png'"
                    Attributes.onMouseOut="$(this).src='pics/buttons/buttWUILogsD.png'"/>
      </com:THyperLink>                
    </td>
  </tr> 
  <tr>
    <td align="right">
      <com:THyperLink  NavigateUrl="<%=$this->Service->constructUrl('Credits') %>">
        <com:TImage ImageUrl="pics/buttons/buttCreditsD.png"
                    Attributes.onMouseOver="$(this).src='pics/buttons/buttCreditsU.png'"
                    Attributes.onMouseOut="$(this).src='pics/buttons/buttCreditsD.png'"/>
      </com:THyperLink>                
    </td>
  </tr> 
</table>
</div>
