<div>
  <table cellspacing=0 cellpadding=0>
    <tr> <td> <a href="<%=$this->Service->constructUrl('Home') %>">Home</a> </td> </tr>
    <tr> <td> <a href="<%=$this->Service->constructUrl('Reports') %>">Reports (roots)</a> </td> </tr>
    <tr> <td> <a href="<%=$this->Service->constructUrl('InputAlerts') %>">Alerts (source)</a> </td> </tr>
    <tr> <td> <a href="<%=$this->Service->constructUrl('AllMetaAlerts') %>">Alerts and all reports</a> </td> </tr>
    <tr> <td> <a href="<%=$this->Service->constructUrl('Analyzers') %>">Analyzers</a> </td> </tr>
    <tr> <td> <a href="<%=$this->Service->constructUrl('WebLogs') %>">Web logs</a> </td> </tr>
    <tr> <td> <a href="<%=$this->Service->constructUrl('DataMiningSeverity') %>">Data Mining Severity</a> </td> </tr>
    <tr> <td> <a href="<%=$this->Service->constructUrl('DataMiningAlertTypes') %>">Data Mining Types</a> </td> </tr>
    <tr> <td> <a href="<%=$this->Service->constructUrl('DataMiningAlertTimeSeries') %>">Alert Time Series</a> </td> </tr>
    <tr> <td> <a href="<%=$this->Service->constructUrl('DataMiningMetaAlertTimeSeries') %>">Meta Alert Time Series</a> </td> </tr>


<!--
    <tr> <td> <com:TImageButton ID="Home" ImageUrl="pics/buttons/buttHomeD.png" Attributes.onMouseOver="$(this).src='pics/buttons/buttHomeU.png'" Attributes.onMouseOut="$(this).src='pics/buttons/buttHomeD.png'" OnClick="buttonClicked" /> </td></tr>
    <tr> <td> <com:TImageButton ImageUrl="pics/buttons/buttReportsD.png" OnClick="buttonClicked" /> </td></tr>
    <tr> <td> <com:TImageButton ImageUrl="pics/buttons/buttSourceAlertsD.png" OnClick="buttonClicked" /> </td></tr
    <tr> <td> <com:TImageButton ImageUrl="pics/buttons/buttAnalyzersD.png" OnClick="buttonClicked" /> </td></tr>
    <tr> <td> <com:TImageButton ImageUrl="pics/buttons/buttWeblogsD.png" OnClick="buttonClicked" /> </td></tr>
-->
<com:TLabel ID="Result" Text="Nothing"/>
  </table>
</div>
