<div width="100%">
<table border="0" width="100%">

<tr><td><com:TLabel ID="TopCaption" ForeColor="White" Font.Size="14pt"/></td><td align="right"><com:TLabel Text="Per page:" ForeColor="White" Font.Size="14pt"/>

<com:TDropDownList
   AutoPostBack="true"
   OnSelectedIndexChanged="selectionChanged">
  <com:TListItem Value="10" Text="10" />
  <com:TListItem Value="20" Text="20" Selected="true" />
  <com:TListItem Value="50" Text="50" />
  <com:TListItem Value="100" Text="100" />
  <com:TListItem Value="1000" Text="Jillion" />
</com:TDropDownList>

</td></tr>
<tr><td colspan=2>
<com:TPager ID="Pager"
            ControlToPaginate="DataGrid"
            Mode="Numeric"
            OnPageIndexChanged="changePage" 
            ForeColor="White" 
            BackColor="Grey"/>

<com:TDataGrid
   ID="DataGrid"
   Width="100%"
   DataKeyField="id"
   AllowPaging="true"
   AllowCustomPaging="true"
   PageSize="20"
   AutoGenerateColumns="false"
   CellPadding="2"
   CellSpacing="1"
   HeaderStyle.BackColor="black"
   HeaderStyle.ForeColor="white"
   ItemStyle.BackColor="#c6c6d2"
   ItemStyle.Font.Italic="true"
   AllowSorting="false"
   AlternatingItemStyle.BackColor="#b2b2d0" >
</com:TDataGrid>

<center>
<com:TLabel
   ID="Labelka"
   Text="Your query returned no data."
   ForeColor="white"
   BackColor="black"
   Visible="false"
   Font.Name="Arial"
   Font.Size="14pt"
   />
</center>
</td></tr>
</table>

</div>

