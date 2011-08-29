<com:TPanel GroupingText="<font size=5 color=white>Data range</font>" Width="100%">
  <table align="center" cellpadding="5" width="100%" border="1">
    <tr>
      <td id="AlertTag">From date:</td>
      <td id="AlertContent"><com:TDatePicker DateFormat="yyyy-MM-dd" ID="From" InputMode="DropDownList"/></td>
      <td id="AlertTag">To date:</td>
      <td id="AlertContent"><com:TDatePicker DateFormat="yyyy-MM-dd" ID="To" InputMode="DropDownList"/></td>
    </tr>
    <com:TMultiView ID="MultiView" ActiveViewIndex="1">
      <com:TView ID="View1">
      </com:TView>
      <com:TView ID="View2">
        <tr>
          <td id="AlertTag" rowspan="2">Severity:</td>
          <td id="AlertContent" rowspan="2">
            <com:TCheckBoxList ID="CB" RepeatColumns="3">
              <com:TListItem ID="CBcritical" Value="high" Text="High" Selected="true" />
              <com:TListItem ID="CBerror" Value="medium" Text="Medium" Selected="true" />
              <com:TListItem ID="CBproblem" Value="low" Text="Low" Selected="true" />
              <com:TListItem ID="CBwarning" Value="info" Text="Info" Selected="true" />
              <com:TListItem ID="CBdebug" Value="debug" Text="Debug" Selected="false" />
            </com:TCheckBoxList>
          </td>
          <td id="AlertTag">src IP:</td><td id="AlertContent"><com:TTextBox ID="srcip" Text="any" /></td>
        </tr>
        <tr>
          <td id="AlertTag">dst IP:</td><td id="AlertContent"><com:TTextBox ID="dstip" Text="any"/></td>
        </tr>    
      </com:TView>
    </com:TMultiView>
  </table>
  <com:TTextBox ID="Type" Text="any" Visible="no"/>
  <com:TButton ID="Submit" Text="update" />
</com:TPanel>
<br/>



