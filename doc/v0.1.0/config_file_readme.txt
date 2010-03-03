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
    <!-- filter's configuration -->
  </filters>

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
  <filter>
    <type>onetomany</type>  <!-- filter type name -->
    <timeout>42</timeout>   <!-- timeout for observing entry, in seconds -->
  </filter>
  <filter>
    <type>manytomany</type> <!-- filter type name -->
    <timeout>42</timeout>   <!-- timeout for observing entry, in seconds -->
  </filter>
</filters>

TODO: describe filters.
