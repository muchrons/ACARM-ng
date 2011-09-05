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

<com:TBoundColumn
   ID="Count"
   HeaderText="Count"
   HeaderStyle.Width="80px"
   ItemStyle.HorizontalAlign="Right"
   DataField="Count"
   />

<com:TTemplateColumn 
   ID="CbColumn"
   HeaderText="Select"
   HeaderStyle.Width="30px"
   ItemStyle.HorizontalAlign="Center">
  <prop:HeaderTemplate>
    <input type="checkbox" onclick="checkAll(this.checked);"/>
  </prop:HeaderTemplate>
  <prop:ItemTemplate>
    <input type="checkbox" class="cbCol" id="<%=$this->Parent->ItemIndex%>"/>
  </prop:ItemTemplate>
</com:TTemplateColumn>

<com:TBoundColumn
   ID="Name"
   HeaderText="Name"
   DataField="Name"
   />
</com:TDataGrid>

  <com:THiddenField ID="CheckedRows"/>

  <script>
    function checkAll(value)
    {
      $$('.cbCol').each(function(c){c.checked=value;});
    }
    
    function getChecked()
    {
    var checkedRows=$('<%=$this->CheckedRows->ClientID%>');

    checkedRows.value='';

    if ($$('.cbCol').size()>0)
    {
      var chkItems=new Array();
      $$('.cbCol').each(
        function(c)
        {
          if (c.checked)
            {
              chkItems.push(c.id);
            }
        })

    if (chkItems.length > 0)
      {
        checkedRows.value=chkItems.join();

        return true;
      } else
      {
        alert('No rows selected');
        return false;
      }
    }
    else 
    {
      alert('No rows selected');
      return false;
    };
    }
  </script>

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

