configuration file format
=========================

file is an XML with sections corresponding to parsing specific settings of
components using it.
example may look like this:

<?xml version="1.0" encoding="UTF-8"?>
<acarm_ng>
  <persistency>
    <!-- persistency specific options -->
  </persistency>

  <logger>
    <!-- logger specific options -->
  </logger>

  <filters>
    <!-- filters' configuration -->
  </filters>

  <triggers>
    <!-- triggers' configuration -->
  </triggers>

  <preprocessor>
    <!-- alerts' preprocessor -->
  </preprocessor>

  <!-- TODO -->
</acarm_ng>

in the following sections only subtrees specific for given components will
be shown in the example, for sake of document's simplicity.


persistency configuration
=========================

configuration of persistency requires only giving its type to use and options
for it.

following persistencies are supported:
  * PostgreSQL:
<persistency>
  <type>postgres</type>
  <dbname>acarm_ng</dbname>
  <user>john</user>
  <pass>$3cr3t</pass>
  <host>db.server.my.domain.org</host>
  <port>5432</port>
  </persistencytype>
</persistency>



logger configuration
====================

logger's configuration consists of two parts: appenders configuration and
logger nodes configuration.
at this moment only appenders can be assigned to a particullar nodes.
example configuration may look like this:

<logger>
  <appenders>
    <File name="file_app1">
      <output>myoutputfile.log</output>
    </File>

    <console name="stdout"/>

    <multi name="everywhere">
      <appender>file_app1</appender>
      <appender>stdout</appender>
    </multi>
  </appenders>

  <nodes appender="everywhere" threshold="info"> <!-- default is everywhere/info -->
    <someChild appender="console"/> <!-- console/info -->
    <otherChild>                    <!-- console/info -->
      <subChild1 appender="file"/>  <!-- file/info -->
      <subChild2 threshold="warn"/> <!-- console/warn -->
    </otherChild>
  </nodes>
</logger>

TODO: describe appenders.


filter configuration
====================

in filters' configuration subtree there are set of filter entries that are to
be configured. each entry consists of required field 'type' that determines
what type of filter is to be created and optional, filter-specific fields.
example configuration may look like this:

<filters>
  <onetomany>
    <timeout>42</timeout> <!-- timeout for observing entry, in seconds -->
  </onetomany>
</filters>

following filters and paramters are available:

  <!-- correlates multiple atacks originated from single host -->
  <onetomany>
    <timeout>42</timeout> <!-- timeout for observing entry, in seconds -->
  </onetomany>

  <!-- correlates multiple atacks originated from single host -->
  <manytoone>
    <timeout>42</timeout> <!-- timeout for observing entry, in seconds -->
  </manytoone>

  <!-- correlates multiple atacks directed on many host -->
  <manytomany>
    <timeout>42</timeout> <!-- timeout for observing entry, in seconds -->
  </manytomany>

  <!-- performs reverse-DNS mapping on hosts that do not have DNS name set -->
  <dnsresolver>
    <cachetimeout>3600</cachetimeout> <!-- time to keep cached DNS mappings -->
  </dnsresolver>

  <!-- updates severity of given meta-alert if it includes source-host that is blacklisted -->
  <ipblacklist>
    <refresh>36000</refresh> <!-- how often refresh list of black-listed IPs in seconds -->
    <limit>10000</limit>     <!-- limits number of entries to be downloaded -->
    <priorityDelta>0.8</priorityDelta> <!-- amount to update priority by when black-listed IP's found -->
  </ipblacklist>

TODO: describe missing filters.

TODO: describe triggers

TODO: describe inputs


preprocessor configiration
==========================

preprocessor is a tool to allow removing certain alerts just after receiving them.
this is useful when there are known facilities that generates useless alerts that
are not to be correlated. droped alert are not saved in system's data base (only
log is generated about this event).
default behaviour is to accept alert (ex.: when this section is empty).

top level structure looks like this:
<preprocessor>
  <!-- ... -->
  <accept>
    <!-- ... -->
  </accept>
  <!-- ... -->
  <drop>
    <!-- ... -->
  </drop>
  <!-- ... -->
</preprocessor>

section(s) 'drop' and 'accept' contains sets of rules in form of logical extressins
(they are described later on). if given expression is true, alert is accepted or
dropped, respectively. if alert does not match rules specified in given secion
then next section is pocessed. algorithm repeats until first section does match.
if no section match, alert is accepted by default.

logical expression consists of 'and', 'or' and 'not' statements, that can be
nested. 'and' statement is true when all statement(s) listed inside are true.
'or' statement is true when at least one statement(s) listed inside is true. 'not'
statement is true when statement (exactly one!) listed inside is false.
example statement, that can be placed inside 'accept'/'drop' section can look
like this:
<and>
  <A/>
  <B/>
  <not> <C/> </not>
  <or>
    <not> <D/> </not>
  </or>
  <not>
    <or>
      <E>
      <F>
    </or>
  </not>
</and>

assuming A,B,C,D,E,F are rules (rules are described later on in this section)
logical condition is: ( A && B && (!C) && ( !D ) && !( E || F ) ).

there are 3 possible rules: 'true', 'false' and 'rule'. 'true' is term indicating
true. 'false' is term indicating false. 'rule' is rule to be avaluated for a given
alert and may be true or false depending on alert's concent.
'rule' have two parameters: 'path' and comparison mode ('equals' or 'contains' -
they are described later on).
example rules can look like this:
<true>  <!-- always true -->
<false> <!-- always false-->
<rule path="A" equals="B"/>   <!-- true if element A has value B -->
<rule path="A" contains="B"/> <!-- true if element A contains string B -->
'path' consists of string that represents position in alerts' hierarchy. assuming
that all collection types are marked as '[]' in place of element's index the full
tree looks like this:
-------------
alert.name
alert.detected
alert.created
alert.certanity
alert.severity
alert.description
alert.analyzers.[].name
alert.analyzers.[].version
alert.analyzers.[].os
alert.analyzers.[].ip
alert.source.[].ip
alert.source.[].mask
alert.source.[].os
alert.source.[].url
alert.source.[].name
alert.source.[].services.[].name
alert.source.[].services.[].port
alert.source.[].services.[].protocol
alert.source.[].services.[].url
alert.source.[].processes.[].path
alert.source.[].processes.[].name
alert.source.[].processes.[].md5
alert.source.[].processes.[].pid
alert.source.[].processes.[].uid
alert.source.[].processes.[].username
alert.source.[].processes.[].arguments
alert.source.[].processes.[].url
alert.target.[].ip
alert.target.[].mask
alert.target.[].os
alert.target.[].url
alert.target.[].name
alert.target.[].services.[].name
alert.target.[].services.[].port
alert.target.[].services.[].protocol
alert.target.[].services.[].url
alert.target.[].processes.[].path
alert.target.[].processes.[].name
alert.target.[].processes.[].md5
alert.target.[].processes.[].pid
alert.target.[].processes.[].uid
alert.target.[].processes.[].username
alert.target.[].processes.[].arguments
alert.target.[].processes.[].url
-------------
notice that since elements in collections are in undefined order there are no
indexes available. instead two markers are present: '*' and '$'. putting '*' in
place of collection index means 'all elements must be match given rule'.
putting '$' in place of collection index means 'any element must match given
rule'.

example configs along with description goes here:

<preprocessor>
  <accept>
    <and>
      <rule path="alert.name"        contains="ssh"/>
      <rule path="alert.target.$.ip" equals="1.2.3.4"/>
    </and>
  </accept>
  <drop>
    <true/>
  </drop>
</preprocessor>
accepts only alerts that name contains string 'ssh' and at least one target host
is '1.2.3.4'. all other alerts will be droped.

<preprocessor>
  <drop>
    <rule path="alert.source.*.name" equals="my.security.scanner.org"/>
  </drop>
</preprocessor>
drops all alerts that were generated by source host whose name's
"my.security.scanner.org". all other alerts will be accepted. notice
that we use '*' here, so if alert will consist of reports generated
by multiple hosts (including 'my.security.scanner.org') alert may still
be accepted. this is safe aproach in this context.
