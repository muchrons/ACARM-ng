<div>
<!--
    <table cellspacing=0 cellpadding=0 border=0>
      <tr><td align="right">
          <com:THyperLink  NavigateUrl="<%=$this->Service->constructUrl('Home') %>">
            <com:TImageButton ImageUrl="pics/buttons/buttHomeD.png" Attributes.onMouseOver="$(this).src='pics/buttons/buttHomeU.png'"  Attributes.onMouseOut="$(this).src='pics/buttons/buttHomeD.png'" CommandName="SwitchViewIndex" CommandParameter="0"/>
          </com:THyperLink>
      </tr></td>
    </table>
    <com:TMultiView ID="MultiView" ActiveViewIndex="0">
      <com:TView ID="View1">     
        <table cellspacing=0 cellpadding=0 border=0>
          <tr><td>
              <com:TImageButton ImageUrl="pics/buttons/buttDBD.png" Attributes.onMouseOver="$(this).src='pics/buttons/buttDBU.png'"  Attributes.onMouseOut="$(this).src='pics/buttons/buttDBD.png'" CommandName="SwitchViewIndex" CommandParameter="1" />
          </tr></td> 
          <tr><td>
              <com:TImageButton ImageUrl="pics/buttons/buttDMD.png" Attributes.onMouseOver="$(this).src='pics/buttons/buttDMU.png'"  Attributes.onMouseOut="$(this).src='pics/buttons/buttDMD.png'" CommandName="SwitchViewIndex" CommandParameter="2"/>
          </tr></td> 
        </table>
      </com:TView>

      <com:TView ID="View2">     
        <table cellspacing=0 cellpadding=0 border=0>
          <tr><td>
              <com:TImageButton ImageUrl="pics/buttons/buttDBU.png" Attributes.onMouseOver="$(this).src='pics/buttons/buttDBU.png'"  Attributes.onMouseOut="$(this).src='pics/buttons/buttDBU.png'" CommandName="SwitchViewIndex" CommandParameter="1" />
          </tr></td> 
          <tr><td align="right">
              <com:THyperLink  NavigateUrl="<%=$this->Service->constructUrl('InputAlerts') %>">
                <com:TImageButton ImageUrl="pics/buttons/buttDBSourcreAlertsD.png" Attributes.onMouseOver="$(this).src='pics/buttons/buttDBSourcreAlertsU.png'"  Attributes.onMouseOut="$(this).src='pics/buttons/buttDBSourcreAlertsD.png'"/>              
              </com:THyperLink>                
          </tr></td> 
          <tr><td align="right">
              <com:THyperLink  NavigateUrl="<%=$this->Service->constructUrl('AllMetaAlerts') %>">
                <com:TImageButton ImageUrl="pics/buttons/buttDBMetaAlertsD.png" Attributes.onMouseOver="$(this).src='pics/buttons/buttDBMetaAlertsU.png'"  Attributes.onMouseOut="$(this).src='pics/buttons/buttDBMetaAlertsD.png'"/>
              </com:THyperLink>                
          </tr></td> 
          <tr><td align="right">
              <com:THyperLink  NavigateUrl="<%=$this->Service->constructUrl('Analyzers') %>">
                <com:TImageButton ImageUrl="pics/buttons/buttDBAnalyzersD.png" Attributes.onMouseOver="$(this).src='pics/buttons/buttDBAnalyzersU.png'"  Attributes.onMouseOut="$(this).src='pics/buttons/buttDBAnalyzersD.png'"/>
              </com:THyperLink>                
          </tr></td> 
          <tr><td>
              <com:TImageButton ImageUrl="pics/buttons/buttDMD.png" Attributes.onMouseOver="$(this).src='pics/buttons/buttDMU.png'"  Attributes.onMouseOut="$(this).src='pics/buttons/buttDMD.png'" CommandName="SwitchViewIndex" CommandParameter="2"/>
          </tr></td> 
        </table>
      </com:TView>

      <com:TView ID="View3">     
        <table cellspacing=0 cellpadding=0 border=0>
          <tr><td>
              <com:TImageButton ImageUrl="pics/buttons/buttDBD.png" Attributes.onMouseOver="$(this).src='pics/buttons/buttDBU.png'"  Attributes.onMouseOut="$(this).src='pics/buttons/buttDBD.png'" CommandName="SwitchViewIndex" CommandParameter="1" />
          </tr></td> 
          <tr><td>
              <com:TImageButton ImageUrl="pics/buttons/buttDMU.png" Attributes.onMouseOver="$(this).src='pics/buttons/buttDMU.png'"  Attributes.onMouseOut="$(this).src='pics/buttons/buttDMU.png'" CommandName="SwitchViewIndex" CommandParameter="2"/>
          </tr></td> 

          <tr><td align="right">
              <com:THyperLink  NavigateUrl="<%=$this->Service->constructUrl('DataMiningSeverity') %>">
                <com:TImageButton ImageUrl="pics/buttons/buttDMSeverityD.png" Attributes.onMouseOver="$(this).src='pics/buttons/buttDMSeverityU.png'"  Attributes.onMouseOut="$(this).src='pics/buttons/buttDMSeverityD.png'"/>
              </com:THyperLink>                
          </tr></td> 

          <tr><td align="right">
              <com:THyperLink  NavigateUrl="<%=$this->Service->constructUrl('DataMiningAlertTypes') %>">
                <com:TImageButton ImageUrl="pics/buttons/buttDMAlertTypesD.png" Attributes.onMouseOver="$(this).src='pics/buttons/buttDMAlertTypesU.png'"  Attributes.onMouseOut="$(this).src='pics/buttons/buttDMAlertTypesD.png'"/>
              </com:THyperLink>
          </tr></td> 

          <tr><td align="right">
              <com:THyperLink  NavigateUrl="<%=$this->Service->constructUrl('DataMiningAlertTimeSeries') %>">
                <com:TImageButton ImageUrl="pics/buttons/buttDMAlertCountD.png" Attributes.onMouseOver="$(this).src='pics/buttons/buttDMAlertCountU.png'"  Attributes.onMouseOut="$(this).src='pics/buttons/buttDMAlertCountD.png'"/>
              </com:THyperLink>
          </tr></td> 

          <tr><td align="right">
              <com:THyperLink  NavigateUrl="<%=$this->Service->constructUrl('DataMiningMetaAlertTimeSeries') %>">
                <com:TImageButton ImageUrl="pics/buttons/buttDMMetaAlertCountD.png" Attributes.onMouseOver="$(this).src='pics/buttons/buttDMMetaAlertCountU.png'"  Attributes.onMouseOut="$(this).src='pics/buttons/buttDMMetaAlertCountD.png'"/>
              </com:THyperLink>
          </tr></td> 
        </table>
      </com:TView>
    </com:TMultiView> -->

<table cellspacing=0 cellpadding=0 border=0>
  <tr>
    <td align="right">
      <com:THyperLink  NavigateUrl="<%=$this->Service->constructUrl('Home') %>">
        <com:TImageButton ImageUrl="pics/buttons/buttHomeD.png" Attributes.onMouseOver="$(this).src='pics/buttons/buttHomeU.png'"  Attributes.onMouseOut="$(this).src='pics/buttons/buttHomeD.png'" CommandName="SwitchViewIndex" CommandParameter="0"/>
      </com:THyperLink>
    </td>
  </tr>
  <tr>
    <td>
      <com:TImageButton ImageUrl="pics/buttons/buttDBD.png" Attributes.onMouseOver="$(this).src='pics/buttons/buttDBU.png'"  Attributes.onMouseOut="$(this).src='pics/buttons/buttDBD.png'" CommandName="SwitchViewIndex" CommandParameter="1" />
    </td>
  </tr> 
  <tr>
    <td align="right">
      <com:THyperLink  NavigateUrl="<%=$this->Service->constructUrl('Alerts') %>">
        <com:TImageButton ImageUrl="pics/buttons/buttDBSourcreAlertsD.png" Attributes.onMouseOver="$(this).src='pics/buttons/buttDBSourcreAlertsU.png'"  Attributes.onMouseOut="$(this).src='pics/buttons/buttDBSourcreAlertsD.png'"/>              
      </com:THyperLink>                
    </td>
  </tr>
  <tr>
    <td align="right">
      <com:THyperLink  NavigateUrl="<%=$this->Service->constructUrl('MetaAlerts') %>">
        <com:TImageButton ImageUrl="pics/buttons/buttDBMetaAlertsD.png" Attributes.onMouseOver="$(this).src='pics/buttons/buttDBMetaAlertsU.png'"  Attributes.onMouseOut="$(this).src='pics/buttons/buttDBMetaAlertsD.png'"/>
      </com:THyperLink>                
    </td>
  </tr>
  <tr>
    <td align="right">
      <com:THyperLink  NavigateUrl="<%=$this->Service->constructUrl('Analyzers') %>">
        <com:TImageButton ImageUrl="pics/buttons/buttDBAnalyzersD.png" Attributes.onMouseOver="$(this).src='pics/buttons/buttDBAnalyzersU.png'"  Attributes.onMouseOut="$(this).src='pics/buttons/buttDBAnalyzersD.png'"/>
      </com:THyperLink>                
    </td>
  </tr> 
  <tr>
    <td>
      <com:TImageButton ImageUrl="pics/buttons/buttDMD.png" Attributes.onMouseOver="$(this).src='pics/buttons/buttDMU.png'"  Attributes.onMouseOut="$(this).src='pics/buttons/buttDMD.png'" CommandName="SwitchViewIndex" CommandParameter="2"/>
    </td>
  </tr> 
  <tr>
    <td align="right">
      <com:THyperLink  NavigateUrl="<%=$this->Service->constructUrl('DataMiningSeverity') %>">
        <com:TImageButton ImageUrl="pics/buttons/buttDMSeverityD.png" Attributes.onMouseOver="$(this).src='pics/buttons/buttDMSeverityU.png'"  Attributes.onMouseOut="$(this).src='pics/buttons/buttDMSeverityD.png'"/>
      </com:THyperLink>                
    </td>
  </tr>
  <tr>
    <td align="right">
      <com:THyperLink  NavigateUrl="<%=$this->Service->constructUrl('DataMiningAlertTypes') %>">
        <com:TImageButton ImageUrl="pics/buttons/buttDMAlertTypesD.png" Attributes.onMouseOver="$(this).src='pics/buttons/buttDMAlertTypesU.png'"  Attributes.onMouseOut="$(this).src='pics/buttons/buttDMAlertTypesD.png'"/>
      </com:THyperLink>
    </td>
  </tr>
  <tr>
    <td align="right">
      <com:THyperLink  NavigateUrl="<%=$this->Service->constructUrl('DataMiningAlertTimeSeries') %>">
        <com:TImageButton ImageUrl="pics/buttons/buttDMAlertCountD.png" Attributes.onMouseOver="$(this).src='pics/buttons/buttDMAlertCountU.png'"  Attributes.onMouseOut="$(this).src='pics/buttons/buttDMAlertCountD.png'"/>
      </com:THyperLink>
    </td>
  </tr>
  <tr>
    <td align="right">
      <com:THyperLink  NavigateUrl="<%=$this->Service->constructUrl('DataMiningMetaAlertTimeSeries') %>">
        <com:TImageButton ImageUrl="pics/buttons/buttDMMetaAlertCountD.png" Attributes.onMouseOver="$(this).src='pics/buttons/buttDMMetaAlertCountU.png'"  Attributes.onMouseOut="$(this).src='pics/buttons/buttDMMetaAlertCountD.png'"/>
      </com:THyperLink>
    </td>
  </tr>
  <tr>
    <td align="right">
      <com:THyperLink  NavigateUrl="<%=$this->Service->constructUrl('Settings') %>">
        <com:TImageButton ImageUrl="pics/buttons/buttSettingsD.png" Attributes.onMouseOver="$(this).src='pics/buttons/buttSettingsU.png'"  Attributes.onMouseOut="$(this).src='pics/buttons/buttSettingsD.png'"/>
      </com:THyperLink>
    </td>
  </tr>
  <tr>
    <td align="right">
      <com:THyperLink  NavigateUrl="<%=$this->Service->constructUrl('DataMiningHeatMap') %>">
        <com:TImageButton ImageUrl="pics/buttons/buttHeatmapD.png" Attributes.onMouseOver="$(this).src='pics/buttons/buttHeatmapU.png'"  Attributes.onMouseOut="$(this).src='pics/buttons/buttHeatmapD.png'"/>
      </com:THyperLink>
    </td>
  </tr>
</table>
</div>
