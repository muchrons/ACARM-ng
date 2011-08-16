<div width="100%">
<table border="0" width="100%">

<tr><td><com:TLabel ID="TopCaption" ForeColor="White" Font.Size="14pt"/></td></td></tr>
<tr><td>

<com:TDataGrid
    ID="DataGrid"
    Width="100%"
    DataKeyField="id"
    AllowPaging="false"
    AllowCustomPaging="false"
    AutoGenerateColumns="false"
    CellPadding="2"
    CellSpacing="1"
    HeaderStyle.BackColor="black"
    HeaderStyle.ForeColor="white"
    ItemStyle.BackColor="#c6c6d2"
    ItemStyle.Font.Italic="true"
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

