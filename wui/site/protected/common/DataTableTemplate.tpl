<div width=100%>
<table border=0 width=100%>
<!-- TODO: i think this header should not be the part of this control -->
<tr><td><b><com:TLabel ID="TopCaption"/></b></td><td align=right>Per page:

<com:TDropDownList
    AutoPostBack="true"
    OnSelectedIndexChanged="selectionChanged">
    <com:TListItem Value="10" Text="10" Selected="true"/>
    <com:TListItem Value="20" Text="20"/>
    <com:TListItem Value="50" Text="50" />
    <com:TListItem Value="100" Text="100" />
    <com:TListItem Value="1000" Text="Jillion" />
</com:TDropDownList>

</td></tr>
<tr><td colspan=2>
Page: <com:TPager ID="Pager"
          ControlToPaginate="DataGrid"
          PageButtonCount="10"
          Mode="Numeric"
          OnPageIndexChanged="changePage" />

<com:TDataGrid
    ID="DataGrid"
    Width="100%"
    DataKeyField="id"
    AllowPaging="true"
    AllowCustomPaging="true"
    PageSize="10"
    AutoGenerateColumns="true"
    CellPadding="2"
    HeaderStyle.BackColor="black"
    HeaderStyle.ForeColor="white"
    ItemStyle.BackColor="#BFCFFF"
    ItemStyle.Font.Italic="true"
    AlternatingItemStyle.BackColor="#e6ECFF" >
</com:TDataGrid>

Choose page:

<com:TPager ID="Pager3"
    ControlToPaginate="DataGrid"
    Mode="DropDownList"
    OnPageIndexChanged="changePage"/>

</td></tr>
</table>

</div>

