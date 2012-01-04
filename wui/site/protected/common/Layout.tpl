<?xml version="1.0" encoding="utf-8"?>
<?xml-stylesheet type="text/css" href="style.css"?>

<html xmlns="http://www.w3.org/1999/xhtml" xml:lang="pl">

  
<com:THead>
  <title>ACARM-ng</title>
  <link href="style.css" rel="stylesheet" type="text/css"/>
  <meta http-equiv="Content-Type" content="text/html; charset=UTF-8"/>
</com:THead>


<body bgcolor="#7f7fa0">
<br/><br/><br/><br/>
  <div id="mainDiv">

  <div id="templateDiv">
    <com:MainBarTemplate />
  </div>

    <com:TForm>
      <div id="contentDiv">
        <table id="contentTable" cellpadding="0" border="0" cellspacing="0" bgcolor="#545468" width="100%">
          <tr id="contentTR" align="center" valign="top" height="600">
            <!-- LEFT MENU -->
            <td id="sideMenuPart" bgcolor="#40404f">
              <!-- MENU HEADER -->
              
              <div id="lefthead">
                <br/>
                <a href="http://www.acarm.wcss.wroc.pl" target="new_window_acarmng"><img src="pics/acarm-ng_logo.png" class="FooterLogoImage" alt="ACARM-ng project's page"/></a>      
                <br/><br/><br/>
              </div>

              <!-- MENU ITSELF -->
              <com:MainMenuTemplate ID="mainmenu"/>
              <br/><br/>

              <div id="leftfooter" align="center">
                <table border="0" width="100%">
                  <td align="center">
                    <a href="http://www.plgrid.pl" target="new_window_acarmng"><img src="pics/logo_plgrid.png" class="FooterLogoImage" alt="PlGrid main site"/></a>
                  </td>
                </table>
              </div>
            </td>
            <!-- CONTENT -->
            <td id="mainContentPart">
              <com:TContentPlaceHolder ID="content" />
            </td>
          </tr>
        </table>
      </div>

      <div id="footerDiv">
        <table border="0" cellspacing="0" cellpadding="0" width="100%">
          <tr>
            <td bgcolor="#696982" width="97%" align="right">
              <font face="lucida, verdana, sans-serif" size="2">
                <b>copyright:</b> &copy; 2009-2011 <a class="blacklink" href="http://www.wcss.wroc.pl">WCSS</a>.&nbsp;&nbsp;&nbsp;
              </font>
            </td>
            <td bgcolor="#545468">&nbsp;&nbsp;</td>
            <td bgcolor="#40404f">&nbsp;&nbsp;</td>
            <td bgcolor="#2b2b35">&nbsp;&nbsp;</td>
            <td bgcolor="#16161c">&nbsp;&nbsp;</td>
            <td bgcolor="black">&nbsp;&nbsp;</td>
          </tr>
        </table>


<!--
    <table id="footerBar">
      <tr>
        <td id="w3cValidator">
          <a href="http://validator.w3.org/check?uri=referer"><img
             src="http://www.w3.org/Icons/valid-xhtml11-blue"
             class="FooterLogoImage" alt="Valid XHTML 1.1"/></a>
          <a href="http://jigsaw.w3.org/css-validator/"><img
             class="FooterLogoImage"
             src="http://jigsaw.w3.org/css-validator/images/vcss-blue" alt="Valid CSS!"/></a>
        </td>
        <td id="logos">
          
          <a href="http://www.wcss.wroc.pl"       target="new_window_wcss">   <img src="pics/logo_small.png"    class="FooterLogoImage" alt="Wrocław Centre for Networking and Supercomputing"/></a>
        </td>
      </tr>
    </table>
-->
  <!-- end of FooterDiv -->
      </div>
    </com:TForm>
  </div>
</body>
</html>
