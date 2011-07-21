<!-- TODO: Image highliting disabled due to issue with Firefox5 -->
<div>
<table cellspacing="0" cellpadding="0" border="0">
  <tr>
    <td align="right">
      <com:THyperLink  NavigateUrl="<%=$this->Service->constructUrl('Home') %>"> 
        <img src="pics/buttons/buttHomeD.png">
          <!--        <com:TImageButton ImageUrl="pics/buttons/buttHomeD.png" Attributes.onMouseOver="$(this).src='pics/buttons/buttHomeU.png'"  Attributes.onMouseOut="$(this).src='pics/buttons/buttHomeD.png'" CommandName="SwitchViewIndex" CommandParameter="0"/> -->
      </com:THyperLink>
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
        <img src="pics/buttons/buttDBSourcreAlertsD.png">
<!--         <com:TImageButton ImageUrl="pics/buttons/buttDBSourcreAlertsD.png" Attributes.onMouseOver="$(this).src='pics/buttons/buttDBSourcreAlertsU.png'"  Attributes.onMouseOut="$(this).src='pics/buttons/buttDBSourcreAlertsD.png'"/>   -->
      </com:THyperLink>
    </td>
  </tr>
  <tr>
    <td align="right">
      <com:THyperLink  NavigateUrl="<%=$this->Service->constructUrl('MetaAlerts') %>">
        <img src="pics/buttons/buttDBMetaAlertsD.png">
<!--        <com:TImageButton ImageUrl="pics/buttons/buttDBMetaAlertsD.png" Attributes.onMouseOver="$(this).src='pics/buttons/buttDBMetaAlertsU.png'"  Attributes.onMouseOut="$(this).src='pics/buttons/buttDBMetaAlertsD.png'"/> -->
      </com:THyperLink>                
    </td>
  </tr>
  <tr>
    <td align="right">
      <com:THyperLink  NavigateUrl="<%=$this->Service->constructUrl('MetaAlertsRoot') %>">
        <img src="pics/buttons/buttDBRootMetaAlertsD.png">
<!--        <com:TImageButton ImageUrl="pics/buttons/buttDBRootMetaAlertsD.png" Attributes.onMouseOver="$(this).src='pics/buttons/buttDBRootMetaAlertsU.png'"  Attributes.onMouseOut="$(this).src='pics/buttons/buttDBRootMetaAlertsD.png'"/>-->
      </com:THyperLink>                
    </td>
  </tr>
  <tr>
    <td align="right">
      <com:THyperLink  NavigateUrl="<%=$this->Service->constructUrl('Analyzers') %>">
        <img src="pics/buttons/buttDBAnalyzersD.png">
<!--         <com:TImageButton ImageUrl="pics/buttons/buttDBAnalyzersD.png" Attributes.onMouseOver="$(this).src='pics/buttons/buttDBAnalyzersU.png'"  Attributes.onMouseOut="$(this).src='pics/buttons/buttDBAnalyzersD.png'"/> -->
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
      <com:THyperLink  NavigateUrl="<%=$this->Service->constructUrl('DataMiningSeverity') %>">
        <img src="pics/buttons/buttDMSeverityD.png">
<!--         <com:TImageButton ImageUrl="pics/buttons/buttDMSeverityD.png" Attributes.onMouseOver="$(this).src='pics/buttons/buttDMSeverityU.png'"  Attributes.onMouseOut="$(this).src='pics/buttons/buttDMSeverityD.png'"/> -->
      </com:THyperLink>                
    </td>
  </tr>
  <tr>
    <td align="right">
      <com:THyperLink  NavigateUrl="<%=$this->Service->constructUrl('DataMiningAlertTypes') %>">
        <img src="pics/buttons/buttDMAlertTypesD.png">
<!--         <com:TImageButton ImageUrl="pics/buttons/buttDMAlertTypesD.png" Attributes.onMouseOver="$(this).src='pics/buttons/buttDMAlertTypesU.png'"  Attributes.onMouseOut="$(this).src='pics/buttons/buttDMAlertTypesD.png'"/> -->
      </com:THyperLink>
    </td>
  </tr>
  <tr>
    <td align="right">
      <com:THyperLink  NavigateUrl="<%=$this->Service->constructUrl('DataMiningAlertTimeSeries') %>">
        <img src="pics/buttons/buttDMAlertCountD.png">
<!--         <com:TImageButton ImageUrl="pics/buttons/buttDMAlertCountD.png" Attributes.onMouseOver="$(this).src='pics/buttons/buttDMAlertCountU.png'"  Attributes.onMouseOut="$(this).src='pics/buttons/buttDMAlertCountD.png'"/> -->
      </com:THyperLink>
    </td>
  </tr>
  <tr>
    <td align="right">
      <com:THyperLink  NavigateUrl="<%=$this->Service->constructUrl('DataMiningMetaAlertTimeSeries') %>">
        <img src="pics/buttons/buttDMMetaAlertCountD.png">
<!--         <com:TImageButton ImageUrl="pics/buttons/buttDMMetaAlertCountD.png" Attributes.onMouseOver="$(this).src='pics/buttons/buttDMMetaAlertCountU.png'"  Attributes.onMouseOut="$(this).src='pics/buttons/buttDMMetaAlertCountD.png'"/> -->
      </com:THyperLink>
    </td>
  </tr>
  <tr>
  </tr>
  <tr>
    <td align="right">
      <com:THyperLink  NavigateUrl="<%=$this->Service->constructUrl('DataMiningHeatMap') %>">
        <img src="pics/buttons/buttHeatmapD.png">
<!--         <com:TImageButton ImageUrl="pics/buttons/buttHeatmapD.png" Attributes.onMouseOver="$(this).src='pics/buttons/buttHeatmapU.png'"  Attributes.onMouseOut="$(this).src='pics/buttons/buttHeatmapD.png'"/> -->
      </com:THyperLink>
    </td>
  </tr>
  <tr>
    <td>
      <com:THyperLink  NavigateUrl="<%=$this->Service->constructUrl('WUILogs') %>">
        <img src="pics/buttons/buttWUILogsD.png">
<!--         <com:TImageButton ImageUrl="pics/buttons/buttWUILogsD.png" Attributes.onMouseOver="$(this).src='pics/buttons/buttWUILogsU.png'"  Attributes.onMouseOut="$(this).src='pics/buttons/buttWUILogsD.png'"/> -->
      </com:THyperLink>                
    </td>
  </tr> 
</table>
</div>
