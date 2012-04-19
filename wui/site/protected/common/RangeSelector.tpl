<com:TPanel GroupingText="<font size=5 color=white>Data range</font>" Width="100%">
  <table align="center" cellpadding="5" width="100%" border="1">
    <tr>
      <td class="AlertTag">From date:</td>
      <td class="AlertContent"><com:TDatePicker DateFormat="yyyy-MM-dd" ID="From" InputMode="DropDownList"/></td>
      <td class="AlertTag">To date:</td>
      <td class="AlertContent"><com:TDatePicker DateFormat="yyyy-MM-dd" ID="To" InputMode="DropDownList"/></td>
    </tr>
    <com:TMultiView ID="MultiView" ActiveViewIndex="1">
      <com:TView ID="View1">
      </com:TView>
      <com:TView ID="View2">
        <tr>
          <td class="AlertTag" rowspan="2">Severity:</td>
          <td class="AlertContent" rowspan="2">
            <com:TMultiView ID="SeverityType" ActiveViewIndex="1">
              <com:TView ID="Severities">
                <com:TCheckBoxList ID="CB1" RepeatColumns="3">
                  <com:TListItem ID="CBcritical" Value="high" Text="High" Selected="true" />
                  <com:TListItem ID="CBerror" Value="medium" Text="Medium" Selected="true" />
                  <com:TListItem ID="CBproblem" Value="low" Text="Low" Selected="true" />
                  <com:TListItem ID="CBwarning" Value="info" Text="Info" Selected="true" />
                  <com:TListItem ID="CBdebug" Value="debug" Text="Debug" Selected="false" />
                </com:TCheckBoxList>
              </com:TView>
              <com:TView ID="MetaAlerts">
                <com:TCheckBoxList ID="CB2" RepeatColumns="1">
                  <com:TListItem ID="CBall" Value="all" Text="All" Selected="true"/>
                  <com:TListItem ID="CBroot" Value="root" Text="Root" Selected="true"/>
                </com:TCheckBoxList>
              </com:TView>
            </com:TMultiView>
          </td>
          <td class="AlertTag">src IP:</td><td class="AlertContent"><com:TTextBox ID="srcip" Text="any" /></td>
        </tr>
        <tr>
          <td class="AlertTag">dst IP:</td><td class="AlertContent"><com:TTextBox ID="dstip" Text="any"/></td>
        </tr> 
      </com:TView>
    </com:TMultiView>
  </table>
  <com:TLabel ID="Warning" ForeColor="Orange" Visible="no"/><br/>
  <com:TTextBox ID="Type" Text="any" Visible="no"/>
  <com:TImageButton ID="Submit"
                    Text="update"
                    ImageUrl="pics/buttons/buttUpdateD.png"
                    Attributes.onMouseOver="$(this).src='pics/buttons/buttUpdateU.png'"
                    Attributes.onMouseOut="$(this).src='pics/buttons/buttUpdateD.png'"
                    ValidationGroup="ValidIP"/>
  <com:TRegularExpressionValidator
     ValidationGroup="ValidIP"
     EnableClientScript="true"
     ControlToValidate="srcip"
     Display="Dynamic"
     FocusOnError="true"
     RegularExpression="(\d{1,3}\.\d{1,3}\.\d{1,3}\.\d{1,3})|any"
     Text="Invalid source IP address." />
  <com:TRegularExpressionValidator
     ValidationGroup="ValidIP"
     EnableClientScript="true"
     ControlToValidate="dstip"
     Display="Dynamic"
     FocusOnError="true"
     RegularExpression="(\d{1,3}\.\d{1,3}\.\d{1,3}\.\d{1,3})|any"
     Text="Invalid destination IP address." />
</com:TPanel>
<br/>



