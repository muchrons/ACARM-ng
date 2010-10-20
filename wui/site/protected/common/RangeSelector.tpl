<com:TPanel GroupingText="<font size=5 color=white>Data range</font>" Width="100%">
  <table align="center" cellpadding="5" width="100%" border="1">
    <tr>
      <td id="AlertTag">From date:</td>
      <td id="AlertContent"><com:TDatePicker InputMode="DropDownList" Timestamp=<%= @strtotime("-1 month") %>/></td>
      <td id="AlertTag">To date:</td>
      <td id="AlertContent"><com:TDatePicker InputMode="DropDownList" Timestamp=<%= @strtotime("today") %>/></td>
    </tr>
    <tr>
      <td id="AlertTag" rowspan="2">Severity:</td>
      <td id="AlertContent" rowspan="2">
        <com:TCheckBoxList RepeatColumns="3">
          <com:TListItem Value="value 1" Text="Critical" Selected="true" />
          <com:TListItem Value="value 2" Text="Error" Selected="true" />
          <com:TListItem Value="value 3" Text="Problem" Selected="true" />
          <com:TListItem Value="value 5" Text="Debug" Selected="false" />
          <com:TListItem Value="value 3" Text="Warning" Selected="true" />
          <com:TListItem Value="value 4" Text="Info" Selected="false" />
        </com:TCheckBoxList>
      </td>
      <td id="AlertTag">src IP:</td><td id="AlertContent"><com:TTextBox ID="srcip" Text="any" /></td>
    </tr>
    <tr>
      <td id="AlertTag">dst IP:</td><td id="AlertContent"><com:TTextBox Text="anyy" ID="dstip" /></td>
    </tr>    
  </table>
<com:TButton Text="show" />
</com:TPanel>
<br/>
