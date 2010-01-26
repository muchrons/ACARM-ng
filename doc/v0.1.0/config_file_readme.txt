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

  <!-- and so on... -->
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
