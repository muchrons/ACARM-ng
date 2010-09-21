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

<!-- Buttons
    <tr> <td> <com:TImageButton ImageUrl=<%~ buttHomeD.png %>  OnClick="buttonClicked" /> </td></tr>
    <tr> <td> <com:TImageButton ImageUrl=<%~ buttReportsD.png %> OnClick="buttonClicked" /> </td></tr>
    <tr> <td> <com:TImageButton ImageUrl=<%~ buttSourceAlertsD.png %> OnClick="buttonClicked" /> </td></tr
    <tr> <td> <com:TImageButton ImageUrl=<%~ buttAnalyzersD.png %> OnClick="buttonClicked" /> </td></tr>
    <tr> <td> <com:TImageButton ImageUrl=<%~ buttWeblogsD.png %> OnClick="buttonClicked" /> </td></tr>
-->
<com:TLabel ID="Result" Text="Nothing"/>
  </table>
</div>
